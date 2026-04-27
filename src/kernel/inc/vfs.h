#ifndef VFS_H
#define VFS_H
#include <util.h>

#define TYPE_DIR  1
#define TYPE_FILE 2


struct FsNode {
    char             Name[64];
    u8               Type;
    struct FsMnt     *Mnt;
    void*            FsData;
    struct FsHandler *Ops;
    
};

struct FsMnt {
    char          Type[32];
    char          Dev[64];
    char          Path[64];

    /* make lookups less painful */
    struct FsNode *Mnt;

    struct FsNode *Root;

    struct FsNode Inode[256];
};

struct FsFd {
    struct FsNode *Inode;
    struct FsMnt  *Mnt;
    u64 Pos;
    u64 Flags; 
};
struct FsHandler {
    int            (*Open)  (struct FsNode *n, u64 flags);
    int            (*Read)  (struct FsFd *fd, u8* buf, u64 size);
    int            (*Write) (struct FsFd *fd, u8* buf, u64 size);
    void           (*Close) (struct FsFd *fd);
    int            (*MkDir) (struct FsNode *n, char* name);
    struct FsNode* (*Lookup)(struct FsNode *n, char* name);
};
void vfsInit();
int vfsAlloc(char* name, struct FsMnt *mnt, u8 type);
struct FsFd* vfsOpen(char* path, u64 flags);
int vfsWrite(struct FsFd *fd, u8* buf, u64 size);
#endif