#include <alloc.h>
#include <dev.h>
#include <debug.h>
#include <vfs.h>

static FsDir *root;

void tmpInit(FsMnt *mnt) {
    
    FsDir *d   = vfsLookup(mnt->Path);
    FsNode *i  = malloc(sizeof(FsNode));
    d->Mnt     = mnt;
    d->Inode   = i;

    FsDir *dev = vfsDirAlloc("dev");
    dev->Mnt = d->Mnt;
    FsNode *devI = dev->Inode;
    devI->Mnt    = mnt;
    devI->Type   = TYPE_DIR;

    d->Ch=dev;

    root=d;

    debug("tmpfs: mnt is %s\n",mnt->Path);
}