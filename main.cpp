#include <QCoreApplication>
#include <comandomkdisk.h>
#include <mkfs.h>
#include <fdisk.h>
#include <string>
int main(int argc, char *argv[])
{
    comandomkdisk *disco=new comandomkdisk();

    //------NOTA, CAMBIAR LA RUTA DEL PATH A ALGUNA RUTA DE SU COMPUTADORA PARA PODER EJECUTARLO

    disco->path="/home/alan/Escritorio/intento.dk";
    disco->unit="m"; //asigno megas en este caso
    disco->size=100; //tendria que pesar 100 mb, pueden probar con diferentes tamaños

    disco->crearfichero(disco);
    printf("Disco creado en %s con tamaño %i\n\n  ",disco->path.c_str(),disco->size);
    printf("\n");
    //------CREO UNA PARTICION
    fdisk *objetoparticion=new fdisk();
    //--id vda1
    objetoparticion->name="Particion1";
    objetoparticion->path="/home/alan/Escritorio/intento.dk";
    objetoparticion->size=25; // voy a crear una partición de 25 megas recordar que en el mbr lo guardara en bytes
    objetoparticion->unit="mb";
    objetoparticion->fit="F";
    objetoparticion->crearparticion(objetoparticion);
    objetoparticion->imprimirdatosdisco("/home/alan/Escritorio/intento.dk");
    printf("\n");


    //------FORMATEO FULL
    mkfs *prueba=new mkfs();
    prueba->ejecutarmkfs(prueba);

}
