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
    struct FsNode *n = fd->Inode;
    if(n->Ops && n->Ops->Write) {
        return n->Ops->Write(fd,buf,size);
    }
    return 0;
}

int vfsRead(struct FsFd *fd, u8* buf, u64 size) {
    struct FsNode *n = fd->Inode;

    if(n->Ops && n->Ops->Read) {
        return n->Ops->Read(fd,buf,size);
    }
    return 0;
}
bool vfsReadDir(struct FsFd *fd, struct linux_dirent64* buf, u64 size) {
    struct FsNode *n = fd->Inode;
    if(n->Ops && n->Ops->ReadDir) {
        return n->Ops->ReadDir(fd,buf,size);
    }
    return 0;
}