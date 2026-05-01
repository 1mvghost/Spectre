#ifndef OPS_H
#define OPS_H

#include <util.h>
#include <vfs.h>

struct FsFd* vfsOpen(char* path, u64 flags);
int vfsWrite(struct FsFd *fd, u8* buf, u64 size);
struct FsFd* vfsFdAlloc(struct FsNode *n, u64 flags);

#endif