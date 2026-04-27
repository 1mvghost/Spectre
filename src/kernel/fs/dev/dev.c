#include <alloc.h>
#include <dev.h>
#include <debug.h>
#include <vfs.h>

struct FsNode* dirdebug;
struct FsNode* devLookup(struct FsNode *n, char* name) {
    if(!strcmp(name, "dbg")) {
        return dirdebug;
    }
    return 0;
}

int devOpen(struct FsNode *n, u64 flags) {
    return 1;
}
int devWrite(struct FsFd *fd, u8* buf, u64 size) {
    if(fd->Inode == dirdebug) {
        for(int i = 0; i<size; i++) {
            debugPutc(buf[i]);
        }
        return size;
    }
    return 0;
}

struct FsHandler devHandler = {
    .Lookup = devLookup,
    .Open   = devOpen,
    .Write  = devWrite
};
void devInit(struct FsMnt *mnt) {
    debug("devfs: mnt is %s\n",mnt->Path);

    mnt->Root = &mnt->Inode[vfsAlloc("",mnt,TYPE_DIR)];
    mnt->Root->Ops = &devHandler;

    dirdebug = &mnt->Inode[vfsAlloc("dbg",mnt,TYPE_FILE)];
    dirdebug->Ops = &devHandler;
}