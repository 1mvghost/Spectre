#ifndef VFS_H
#define VFS_H
#include <util.h>

#define TYPE_DIR  0
#define TYPE_FILE 1
typedef struct {
    u8               Type;
    u64              Size;
    struct FsMnt*    Mnt;
    void*            FsData;
    struct FsHandler *Ops;
} FsNode;

typedef struct {
    FsNode *Inode;
    u64    Pos;
    u64    Flags;
} FsFd;

typedef struct FsDir {
    char         Name[64];
    FsNode       *Inode;
    struct FsMnt *Mnt;

    struct FsDir *Par;
    struct FsDir *Nxt;
    struct FsDir *Ch;
} FsDir;

typedef struct FsHandler {
    int           (*Open)   (FsFd *d, u64 flags);
    int           (*Read)   (FsFd *d, u8* buf, u64 size);
    int           (*Write)  (FsFd *d, u8* buf, u64 size);
    void          (*Close)  (FsFd *d);
    int           (*MkDir)  (FsDir *dir, char *name);
} FsHandler;
typedef struct FsMnt {
    char      Type[32];
    char      Dev[64];
    char      Path[64];
    
    FsDir    *Root;
} FsMnt;

FsDir* vfsLookup(char* path);
FsFd*  vfsOpen(char* path, u64 flags);
int    vfsWrite(FsFd *fd, u8* buf, u64 size);
FsDir* vfsDirAlloc(char* name);
void   vfsInit();
#endif