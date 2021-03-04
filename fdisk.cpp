#include "fdisk.h"

fdisk::fdisk()
{

}

void fdisk::crearparticion(fdisk *disco){
    FILE *arch;

    int tamanoparticion;
    arch =fopen(disco->path.c_str(),"rb+"); //abro el archivo del disco en modo lectura para ver si existe el disco y poder crear la particion

    if(arch==NULL){
        std::cout << "Error!\n El disco no existe\n"; //si da null es porque no se encontro el archivo
    return;
    }

      fseek(arch, 0, SEEK_SET); //me posiciono en el inicio del archivo
       mbr MBR;//mbr temporal que agarra el mbr que ya esta en el disco
       fread(&MBR,sizeof(mbr),1,arch);//MBR es donde voy a guardar, sizeof es el tamaño del elemento a leer, 1 es la cantidad de elementos que voy a leer y arch el archivo que estoy leyendo


       if(disco->unit=="pk"||disco->unit.empty()==true){ //si viene vacia  o viene en kb
                      tamanoparticion=disco->size*1024; //aqui es size porque si ya tenemos la particion creada lo que puede venir es EL ADD NO SIZE
                  }else if(disco->unit=="mb"){
                      tamanoparticion=disco->size*1024*1024;
                  }else if(disco->unit=="b"){
                      tamanoparticion=disco->size;
                  }


       if (disco->fit=="B"){
                              MBR.mbr_partitions[0].part_fit='B';
                         }else if(disco->fit=="F"){
                          MBR.mbr_partitions[0].part_fit='F';
                         }else if(disco->fit=="W"||disco->fit.empty()==true){
                          MBR.mbr_partitions[0].part_fit='W';
                         }

       //----asigno nombre a la partición
       strcpy(MBR.mbr_partitions[0].part_name,disco->name.c_str());

     MBR.mbr_partitions[0].part_size=tamanoparticion;

     MBR.mbr_partitions[0].part_start=sizeof(mbr);//--aqui depende de donde del ajuste que calcularon
     MBR.mbr_partitions[0].part_status='1';



     if(disco->type=="P" ||disco->type.empty()==true) {
  MBR.mbr_partitions[0].part_type='P';
     }else if(disco ->type=="E") {
  MBR.mbr_partitions[0].part_type='E';
  //ACABA DE AGREGAR ESTO DEL EBR
  ebr auxiliar; //PRIMER EBR
  auxiliar.part_fit='-';
  auxiliar.part_name[0]='\0';
  auxiliar.part_next=-1;
  auxiliar.part_size=-1;
  auxiliar.part_start=-1;
  auxiliar.part_status='0';
  fseek(arch,MBR.mbr_partitions[0].part_start,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION PARA METER EL EBR
  fwrite(&auxiliar,sizeof(ebr),1,arch);

     }

     fseek(arch, 0, SEEK_SET);//QUEREMOS modificar en donde estaba el MBR original
     fwrite(&MBR, sizeof(mbr), 1, arch);
  fclose(arch);




}


void fdisk::imprimirdatosdisco(string path){
  //  std::cout << "SI LLAME A IMPRIMIR DATOS DISCO\n";
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    if(arch==NULL){
        //return 0;
    }
    mbr MBR;
    fseek(arch, 0, SEEK_SET);
    fread(&MBR,sizeof(mbr),1,arch);
    fclose(arch);
    std::cout << "\n----------DATOS DEL DISCO-----\n";
    std::cout << "\nMBR NAME: "<< MBR.mbr_disk_signature;
    std::cout << "\nMBR SIZE: "<< MBR.mbr_tamano;


    for(int i=0;i<4;i++){
      //  if(MBR.mbr_partitions[i]!=NULL){
        if(MBR.mbr_partitions[i].part_status=='1')  {
        std::cout << "\n\nPARTICION : "<<i;
 std::cout << "\nPARTICION STATUS : "<<MBR.mbr_partitions[i].part_status;
  std::cout << "\nPARTICION TYPE : "<<MBR.mbr_partitions[i].part_type;
   std::cout << "\nPARTICION FIT : "<<MBR.mbr_partitions[i].part_fit;
    std::cout << "\nPARTICION START : "<<MBR.mbr_partitions[i].part_start;
     std::cout << "\nPARTICION SIZE : "<<MBR.mbr_partitions[i].part_size;
     std::cout << "\nPARTICION NAME : "<<MBR.mbr_partitions[i].part_name;
   // }

    }
}

}
