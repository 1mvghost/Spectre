#include <vfs.h>

typedef struct {
    char Name[32];
    u32  Flags;
    u64  FileLen;

    u64 (*Open) (char *path, int flags);
    u64 (*Close)(u64 fileDesc);
    u64 (*Read) (u64 fileDesc, u8* buf, u64 count);
    u64 (*Write)(u64 fileDesc, u8* buf, u64 count);    
} FsNode;

void vfsInit() {

}