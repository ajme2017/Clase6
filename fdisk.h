#ifndef FDISK_H
#define FDISK_H
#include "fdisk.h"
#include <QString>
#include <QStringList>
#include "iostream"
#include  <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <QDebug>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <estructuras.h>
#include <string>
using namespace std;
class fdisk
{
public:
    fdisk();

    int size;
    string type;
    string unit;
    string path;
    string fit;
    string borrar;//este es el delete
    string name;
    void crearparticion(fdisk *disco);
    void imprimirdatosdisco(string path);
};

#endif // FDISK_H
