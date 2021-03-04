#ifndef COMANDOMKDISK_H
#define COMANDOMKDISK_H
#include "string"
using namespace std;
class comandomkdisk
{
public:
    comandomkdisk();
    int size;
    string fechacreacion;
    string unit;
    string path;
    string fit;
    void crearfichero(comandomkdisk *disco);
};

#endif // COMANDOMKDISK_H
