#ifndef MKFS_H
#define MKFS_H
#include "string"
#include "estructuras.h"
using namespace std;


class mkfs
{
public:
    mkfs();
    string id;
    string type;
    string fs;
    bool hayid;
    bool haytype;
    bool hayfs;
    void ejecutarmkfs(mkfs *disco);

};

#endif // MKFS_H
