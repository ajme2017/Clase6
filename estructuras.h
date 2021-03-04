#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include "time.h"
typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
      int part_next;
    char part_name[16];
}ebr;

typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
}particion;


typedef struct{
    int mbr_tamano;
    char mbr_fecha_creacion[16];
    int mbr_disk_signature;
    char disk_fit[1];
    particion mbr_partitions[4];
}mbr;

typedef struct{
   char Journal_Tipo_Operacion[10] ; //mkfs,mkfile,...etc
   char Journal_tipo; //Carpeta(0), Archivo(1).
   char Journal_nombre [200]; //..../home....../home/chepe........../home/chepe/datos.txt
   char Journal_contenido [200]; //....este es el contenido de mi archivo de texto.....-
    char Journal_fecha[16]; // 18/06/2019 10:30
    char Journal_propietario [10]; //Quien creó el archivo o carpeta
    int Journal_permisos; //777...664
   }JOURNAL;



typedef struct {
   int s_filesystem_type;
   int s_inodes_count;
   int s_blocks_count;
   int s_free_blocks_count;
   int s_free_inodes_count;
   char s_mtime [16];
   char s_umtime [16];
   int s_mnt_count;
   int s_magic;
   int s_inode_size;
   int s_block_size;
   int s_first_ino;
   int s_first_blo;
   int s_bm_inode_start;
   int s_bm_block_start;
   int s_inode_start;
   int s_block_start;

}superbloque;

typedef struct {
    int i_uid;
    int i_gid;
    int i_size;
    char i_atime [16];
    char i_ctime[16];
    char i_mtime [16];
    int i_block[15];
    char i_type;
    int i_perm;


}inodo;

typedef struct {
    char b_name [12];
    int b_inodo;

}content;

typedef struct {
    content b_content [4];

}bloque_carpetas;

typedef struct {
    char b_content[64];

}bloque_archivos;



class estructuras
{
public:
    estructuras();



};

#endif // ESTRUCTURAS_H
