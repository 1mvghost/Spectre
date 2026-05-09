#include <vfs.h>
#include <debug.h>
#include <alloc.h>
#include <vmm.h>
#include <mem.h>
#include <dev.h>
#include <stdio.h>
#include <tmp.h>
#include <ops.h>

static struct FsMnt  mntTable[64];
static struct FsNode *root;
static int mntI = 1;

struct FsNode* vfsAlloc(struct FsMnt *mnt, u8 type) {
    struct FsNode *n = malloc(sizeof(struct FsNode));
    n->Type = type;
    n->Mnt  = mnt;
    if(mnt->Root && mnt->Root->Ops) {
        n->Ops  = mnt->Root->Ops;
    }
    return n;
}
int vfsFindMnt(struct FsNode *n) {
    if(!n) return 0;
    for(int i = 1; i<64; i++) {
        if(!strlen(mntTable[i].Path)) {
            break;
        }
        if(n == mntTable[i].Mnt) {
            return i;
        }
    }
    return 0;
}
struct FsNode* vfsLook(struct FsNode* cur, char* name) {
    struct FsNode *c=0;
    if(!cur) return 0;
    if(cur->Type == TYPE_FILE) return 0;
    
    if(cur->Ops && cur->Ops->Lookup) {
        c = cur->Ops->Lookup(cur,name);
    } else {
        return 0;
    }

    if(!c) return 0;

    /* mnt check */
    int mnt = vfsFindMnt(c);
    if(mnt){
        c = mntTable[mnt].Root;
    }
    return c;
}
struct FsNode* vfsLookup(char* path) {
    if(!path)       return 0;
    if(*path=='\0') return 0;

    struct FsNode *cur=root;

    int mnt = vfsFindMnt(cur);
    if(mnt){
        cur = mntTable[mnt].Root;
    }

    if(!strcmp(path, "/")) {

        return cur;
    }
    char *sp = malloc(64);
    memset(sp,0,64);

    char *p = path;
    int i = 0;
    while(*p) {
        if(*p == '/') {
            if(i > 0) {
                //debug("%s\n",sp);
                struct FsNode *l = vfsLook(cur,sp);
                if(!l) {
                    free(sp);
                    return 0;
                }

                cur = l;
                /* putting a / after a file is not valid ! */
                if(l->Type == TYPE_FILE) {
                    //debug("file\n");
                    free(sp);
                    return 0;
                }
            }
            memset(sp,0,64);
            i = 0;
        } else {
            sp[i] = *p;
            i++;
        }


        ++p;

    }

    /* check if theres anything left to handle */
    if(strlen(sp)) {
        cur = vfsLook(cur,sp);
        if(!cur) {
            free(sp);
            return 0;
        }
    }

    free(sp);
    return cur;
}

struct FsFd* vfsFdAlloc(struct FsNode *n, u64 flags) {
    struct FsFd *fd = malloc(sizeof(struct FsFd));
    fd->Inode = n;
    fd->Pos   = 0;
    fd->Mnt   = n->Mnt;
    fd->Flags = flags;
    return fd;
}

Splock mntSplock = ATOMIC_FLAG_INIT;

void vfsMount(char* path, char* dev, char* type) {
    struct FsNode *l = vfsLookup(path);

    if(!l) {
        debug("vfs: cannot mnt on nonexistent path!!\n");
        return;
    }
    
    mSpinlockAcquire(&mntSplock);

    memset(&mntTable[mntI],0,sizeof(struct FsMnt));

    strcpy(mntTable[mntI].Type,type);
    strcpy(mntTable[mntI].Dev, dev);
    strcpy(mntTable[mntI].Path,path);

    mntTable[mntI].Mnt = l;

    /* todo : find a better way */
    if(type=="dev") {
        devInit(&mntTable[mntI]);
    }
    if(type=="tmp") {
        tmpInit(&mntTable[mntI]);
    }

    mntI++;
    debug("vfs: MOUNTED %s\n",path);

    mSpinlockDrop(&mntSplock);
}

void vfsInit() {
    root = malloc(sizeof(struct FsNode));

    vfsMount("/",   "","tmp");
    vfsMount("/dev/","","dev");
}