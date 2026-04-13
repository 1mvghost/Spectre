#include <vfs.h>
#include <debug.h>
#include <alloc.h>
#include <vmm.h>
#include <mem.h>
#include <dev.h>
#include <stdio.h>
#include <tmp.h>

static FsMnt   mnt[64];
static int     mntI=0;
static FsDir   *root;

FsDir* vfsFind(FsDir* ch, char* name) {
    FsDir *c = ch;
    while(c) {
        if(!strcmp(c->Name,name)) {
            return c;
        }
        c=c->Nxt;
    }
    return 0;
}
FsFd* vfsFdAlloc(FsDir *d) {
    if(!d) return 0;
    FsFd *fd = malloc(sizeof(FsFd));
    if(!fd) return 0;
    fd->Inode = d->Inode;
    return fd;
} 
FsDir* vfsLookup(char* path) {
    if(!path)     return 0;
    if(path=='/') return root;

    FsDir *cur = root;
    char* sp   = malloc(64);
    memset(sp,0,64);
    char* p    = path;

    u64 i=0;
    while(*p) {
        if(*p=='/') {
            if(i > 0) {
                i = 0;
                FsDir* d = vfsFind(cur->Ch,sp);

                if(!d){
                    free(sp);
                    return 0;
                }

                cur=d;

                /* if its a file, return 0 because putting / after a file is not valid ! */
                if(cur->Inode && cur->Inode->Type == TYPE_FILE){
                    free(sp);
                    return 0;
                }

                memset(sp,0,64);
            }
        } else {
            sp[i] = *p;
            i++;
        }

        p++;
    }

    /* check if theres anything left to handle */
    if(strlen(sp)) {
        FsDir *d = vfsFind(cur->Ch,sp);
        if(!d){
            free(sp);
            return 0;
        }
        cur=d;
    }
    free(sp);
    return cur;
}

FsFd* vfsOpen(char* path, u64 flags) {
    if(!path) return 0;
    FsDir *d = vfsLookup(path);

    if(d&&d->Inode) {
        FsFd *fd = vfsFdAlloc(d);
        if(d->Inode->Ops && d->Inode->Ops->Open) {

            if(!d->Inode->Ops->Open(fd,flags)) {
                free(fd);
                return 0;
            }

        } else {
            free(fd);
            return 0;
        }
        return fd;
    }
    return 0;
}
int vfsWrite(FsFd *fd, u8* buf, u64 size) {
    if(!fd) return 0;
    
    if(fd->Inode->Ops && fd->Inode->Ops->Write) {
        return fd->Inode->Ops->Write(fd,buf,size);
    }
    return 0;
}

Splock mntSplock = ATOMIC_FLAG_INIT;

void vfsMount(char* path, char* dev, char* type) {
    if(!vfsLookup(path)){
        debug("vfs: cannot create mnt on nonexistent file!!!!\n");
        return;
    }
    mSpinlockAcquire(&mntSplock);

    memset(&mnt[mntI],0,sizeof(FsMnt));
    
    strcpy(mnt[mntI].Type,type);
    strcpy(mnt[mntI].Dev, dev);
    strcpy(mnt[mntI].Path,path);

    /* todo : find a better way */
    if(type=="dev") {
        devInit(&mnt[mntI]);
    }
    if(type=="tmp") {
        tmpInit(&mnt[mntI]);
    }

    mntI++;
    debug("vfs: MOUNTED %s\n",path);
    mSpinlockDrop(&mntSplock);
}

FsDir* vfsDirAlloc(char* name) {
    FsDir *d  = malloc(sizeof(FsDir));
    FsNode *i = malloc(sizeof(FsNode));
    if(!d) return 0; 
    if(!i) return 0;
    strcpy(d->Name,name);
    d->Inode = i;
    return d;
}
void vfsInit() {
    root=calloc(sizeof(FsDir));

    vfsMount("/",   "","tmp");
    vfsMount("/dev","","dev");
}

