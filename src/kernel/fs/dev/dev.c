#include <alloc.h>
#include <dev.h>
#include <debug.h>
#include <vfs.h>
static FsDir *root;

int devOpen(FsFd *d, u64 flags) {
    return 1;
}
int devRead(FsFd *d, u8* buf, u64 size) {
}
int devWrite(FsFd *d, u8* buf, u64 size) {
    if(d->Inode && !strcmp(d->Inode->FsData,"dbg")) {
        for(int i = 0;i<size;i++) {
            debugPutc(buf[i]);
        }
    }
    return 1;
}
int devMkDir(FsDir *dir, char* name) {
    FsDir  *d = vfsDirAlloc(name);
    FsNode *i = d->Inode;
    d->Mnt    = root->Mnt;
    
    i->Type   = TYPE_FILE;
    i->Ops    = dir->Inode->Ops;
    i->Mnt    = root->Mnt;

    /* small hack */
    i->FsData = malloc(64);
    strcpy(i->FsData,name);

    /* add to root */
    if(!dir->Ch) {
        dir->Ch=d;
        return 1;
    }
    FsDir *c = dir->Ch;
    while(c&&c->Nxt){
        c=c->Nxt;
    }

    c->Nxt = d;
    return 1;

}
FsHandler h = {
    .Open   = devOpen,
    .Read   = devRead,
    .Write  = devWrite,
    .MkDir  = devMkDir 
};
void devInit(FsMnt *mnt) {
    FsDir *r = vfsLookup(mnt->Path);
    FsNode *i = calloc(sizeof(FsNode));
    r->Inode = i;
    r->Mnt   = mnt;
    i->Type  = TYPE_DIR;
    i->Mnt   = mnt;

    i->Ops   = &h;

    root=r;
    mnt->Root = r;

    devMkDir(root,"dbg");
    //devMkDir(root,"testt");

    debug("devfs: mnt is %s\n",mnt->Path);
}