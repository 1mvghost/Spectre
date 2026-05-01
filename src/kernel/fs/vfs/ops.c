#include <ops.h>

struct FsFd* vfsOpen(char* path, u64 flags) {
    struct FsNode* n = vfsLookup(path);
    if(!n) return 0;
    if(n->Ops && n->Ops->Open) {
        if(n->Ops->Open(n,flags)) {
            struct FsFd *fd = vfsFdAlloc(n,flags);
            if(!fd) return 0;

            return fd;
        } else {
            return 0;
        }

    } else {
        return 0;
    }
}

int vfsWrite(struct FsFd *fd, u8* buf, u64 size) {
    if(!fd)  return 0;
    if(!buf) return 0;

    struct FsNode *n = fd->Inode;
    if(!n) return 0;

    if(n->Ops && n->Ops->Write) {
        int w=n->Ops->Write(fd,buf,size);

        if(!w) return 0;

        return w;
    }
    return 0;
}