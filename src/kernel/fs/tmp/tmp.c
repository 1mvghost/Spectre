#include <alloc.h>
#include <dev.h>
#include <debug.h>
#include <vfs.h>
#include <util.h>
#include <tmp.h>

struct FsNode* dirdev;

struct FsNode* tmpLookup(struct FsNode *n, char* name) {
    if(!strcmp(name, "dev")) {
        return dirdev;
    }
    return 0;
}

struct FsHandler tmpHandler = {
    .Lookup = tmpLookup,
};
void tmpInit(struct FsMnt *mnt) {
    debug("tmpfs: mnt is %s\n",mnt->Path);

    mnt->Root = &mnt->Inode[vfsAlloc("",mnt,TYPE_DIR)];
    mnt->Root->Ops = &tmpHandler;

    dirdev = &mnt->Inode[vfsAlloc("dev",mnt,TYPE_DIR)];
}