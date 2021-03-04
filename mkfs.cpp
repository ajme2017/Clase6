#include "mkfs.h"
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
mkfs::mkfs()
{

}

void mkfs::ejecutarmkfs(mkfs *disco){
    FILE * arch;
    arch=fopen("/home/alan/Escritorio/intento.dk","rb+"); //lectura y escritura de binario
    if(arch==NULL){
        std::cout << "Error!\n No se puede leer el disco\n"; //si da null es porque no se encontro el archivo
        return;
    }
    fseek(arch, 0, SEEK_SET);
    mbr MBR;// va a contener la informacion del disco

    fread(&MBR,sizeof(mbr),1,arch);//leemos solamente el MBR
    particion auxpart;
    bool existeparticionendisco=false;
    for(int i=0;i<4;i++){
        if(MBR.mbr_partitions[i].part_name=="Particion1"){
            auxpart=MBR.mbr_partitions[i];
            existeparticionendisco=true;
            break;
        }
    }

    //CALCULO NUMERO DE INODOS Y BLOQUES  REVISAR SI ES BLOQUEJOURNAL O JOURNAL
    int n=((auxpart.part_size - sizeof(superbloque))/
           (4+sizeof(JOURNAL)+sizeof(inodo)+3*sizeof(bloque_archivos)));


    //--- PROCEDO A FORMATEAR
    fseek(arch,auxpart.part_start,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION
    if(disco->fs=="full"){//SI ES FULL TENGO QUE BORRAR TODO Y LLENAR DE CEROS

        char buff;
        buff='\0';
        for(int i=0;i<auxpart.part_size;i++) { //lleno de ceros el archivo

            fwrite(&buff,sizeof(buff),1,arch);
            //--PART_STAR ES 100 - 101 -102 -103
            fseek(arch, auxpart.part_start + i, SEEK_SET);// AGREGUE ESTA LINEA
        }

    }

    //------------aqui termina formateo FULL-------------
    superbloque sbloque;
    string fecha="4/03/2020 1:00";
    sbloque.s_filesystem_type=3;
    sbloque.s_inodes_count=n;
    sbloque.s_blocks_count=3*n;
    sbloque.s_free_blocks_count=3*n;//RESTO DESPUES
    sbloque.s_free_inodes_count=n; //RESTO DESPUES
    strcpy(sbloque.s_mtime,fecha.c_str());
    strcpy(sbloque.s_umtime,"");
    sbloque.s_mnt_count=1;//
    sbloque.s_magic=0xEF53;
    sbloque.s_inode_size=sizeof(inodo);
    sbloque.s_block_size=sizeof(bloque_archivos);
    sbloque.s_first_ino=2;//el primer inodo libre va a ser el 2
    sbloque.s_first_blo=2;     //100+64+(10*64)=804 byte
    sbloque.s_bm_inode_start=auxpart.part_start + sizeof(superbloque) + (n*sizeof(JOURNAL)); //revisar esta parte
    sbloque.s_bm_block_start=sbloque.s_bm_inode_start + n ;
    sbloque.s_inode_start=sbloque.s_bm_block_start+3*n;
    sbloque.s_block_start=sbloque.s_inode_start+n*sizeof(inodo);

    fseek(arch,auxpart.part_start, SEEK_SET); //INICIO DE LA PARTICION
    fwrite(&sbloque,sizeof(superbloque),1, arch); //escribimos el superbloque en la particion


    //ahora creo los bitmap de inodos Y BLOQUES
    char bitinodos[n];
    char bibloques[3*n];
    int iniciobitnodos=auxpart.part_start + sizeof(superbloque) + (n*sizeof(JOURNAL));
    for(int i=2;i<n;i++){

        bitinodos[i]='0'; //valor por defecto
    }
    //----EL BIT DE INODO 0 PASA A ESTAR OCUPADO POR LA CARPETA ROOT
    bitinodos[0]='1';
    bitinodos[1]='1';
    fseek(arch,iniciobitnodos, SEEK_SET);
    fwrite(&bitinodos,sizeof(bitinodos),1,arch);//ver si es sizeof(bitinodos)

    //-------escribo mi bitmap de bloques
    int iniciobloques=iniciobitnodos+n;
    for(int i=2;i<3*n;i++){
        bibloques[i]='0';
    }
    bibloques[0]='1';//---1 para direnciar carpeta
    bibloques[1]='2';//--2 para diferenciar archivo

    fseek(arch,iniciobloques, SEEK_SET);
    //fwrite(&bibloques,sizeof(char),sizeof(bibloques),arch);//ver si es sizeof(bibloques)//o si es sizeof(char)
    fwrite(&bibloques,sizeof(bibloques),1,arch);//ver si es sizeof(bibloques)//o si es sizeof(char)


    //-----------ESCRIBO JOURNALING

    int iniciojournal=(auxpart.part_start + sizeof(superbloque));
    JOURNAL journalicial;
    //-------------INGRESO DATOS POR DEFECTO PARA JOURNAL
    journalicial.Journal_Tipo_Operacion[0]='-';
    journalicial.Journal_tipo='-';
    journalicial.Journal_nombre[0]='\0';
    journalicial.Journal_contenido[0]='\0';
    journalicial.Journal_fecha[0]='\0';
    journalicial.Journal_propietario[0] = '\0';
    journalicial.Journal_permisos=-1;

    //ESCRIBO TODOS LOS JOURNAL
    for (int i = 0; i < n; i++) {

        fseek(arch, (iniciojournal + (i * sizeof(JOURNAL))), SEEK_SET);
        fwrite(&journalicial, sizeof(JOURNAL), 1, arch);
    }




    //------------ESCRIBO JOURNAL DE LA CARPETA
    JOURNAL journalicial2;
    strcpy(journalicial2.Journal_Tipo_Operacion,"mkfs");
    journalicial2.Journal_tipo='0';//carpeta
    strcpy(journalicial2.Journal_nombre,"/");
    strcpy(journalicial2.Journal_contenido,"-");
    strcpy(journalicial2.Journal_fecha,"4/03/2020");
    strcpy(journalicial2.Journal_propietario,"1");
    journalicial2.Journal_permisos=664;
    //---ESCRIBO EL JOURNAL DE LA CARPETA EN EL ARCHIVO
    int posicioninical=auxpart.part_start+sizeof(superbloque);
    fseek(arch,posicioninical+0*sizeof(JOURNAL),SEEK_SET); //apunta al journal
    fwrite(&journalicial2,sizeof(JOURNAL),1,arch);


    //------------ESCRIBO JOURNAL DEL ARCHIVO
    JOURNAL journalicial3;
    strcpy(journalicial3.Journal_Tipo_Operacion,"mkfs");
    journalicial3.Journal_tipo='1';//archivo
    strcpy(journalicial3.Journal_nombre,"/users.txt");
    strcpy(journalicial3.Journal_contenido,"1,G,root\n1,U,root,root,123\n");
    strcpy(journalicial3.Journal_fecha,"4/03/2020");
    strcpy(journalicial3.Journal_propietario,"1");
    journalicial3.Journal_permisos=664;
    //---ESCRIBO EL JOURNAL DE LA CARPETA EN EL ARCHIVO

    fseek(arch,posicioninical+1*sizeof(JOURNAL),SEEK_SET); //apunta al journal
    fwrite(&journalicial3,sizeof(JOURNAL),1,arch);


    //AHORA LEO EL SUPERBLOQUE PARA ESCRIBIR EN SUS BLOQUES E INODOS
    superbloque auxsuperbloque;

    fseek(arch,auxpart.part_start,SEEK_SET);
    fread(&auxsuperbloque,sizeof(superbloque),1,arch);


    //UN INODO PARA LA CARPETA RAIZ
    inodo raiz;
    raiz.i_uid=1;
    raiz.i_gid=1;
    raiz.i_size=0;//REVISAR
    string fechacarpeta="04/03/2020";
    string fechacarpeta2="04/03/2020";
    string fechacarpeta3="04/03/2020";
    memset(raiz.i_atime,0,sizeof(raiz.i_atime));
    memset(raiz.i_mtime,0,sizeof(raiz.i_atime));
    memset(raiz.i_ctime,0,sizeof(raiz.i_atime));
    strcpy(raiz.i_atime,fechacarpeta.c_str());
    strcpy(raiz.i_mtime,fechacarpeta2.c_str());
    strcpy(raiz.i_ctime,fechacarpeta3.c_str());
    for(int i=0;i<15;i++){
        raiz.i_block[i]=-1;
    }
    raiz.i_type='0'; //es carpeta
    raiz.i_perm=664;
    raiz.i_block[0]=0;

    //---creo el bloque carpetas
    bloque_carpetas carpetaroot;
    content contenraiz;
    strcpy(contenraiz.b_name, ".");//definimos la carpeta actual
    contenraiz.b_inodo = 0; //apuntador
    carpetaroot.b_content[0] = contenraiz;
    strcpy(contenraiz.b_name, "..");//definimos al padre de la carpeta
    carpetaroot.b_content[1] = contenraiz;

    //content para archivo users.txt
    strcpy(contenraiz.b_name, "users.txt");//agregamos nombre
    contenraiz.b_inodo = 1;//apuntadora hacia inodo asociado
    carpetaroot.b_content[2] = contenraiz;//guardamos el inodo archivo a los apuntadores del inodo de la carpeta raiz
    memset(&contenraiz.b_name, 0, sizeof(contenraiz.b_name));//definimos la carpeta actual
    contenraiz.b_inodo = -1;
    carpetaroot.b_content[3] = contenraiz;
    //------GUARDAMOS EL INODO Y EL BLOQUE DE LA CARPETA EN EL DISCO
    fseek(arch, auxsuperbloque.s_inode_start, SEEK_SET);
    fwrite(&raiz, sizeof(inodo), 1, arch);
    auxsuperbloque.s_free_inodes_count--;//disminuyo en 1 los inodos ocupados
    fseek(arch, auxsuperbloque.s_block_start, SEEK_SET);
    fwrite(&carpetaroot, 64, 1, arch);
    auxsuperbloque.s_free_blocks_count--;//disminuyo en 1 los bloques de archivos ocupados


    //------------------------creo user.txt


    //-------------------AHORA CREO INODO PARA EL ARCHIVO USER----

    char datosarchivo[]="1,G,root\n1,U,root,root,123\n";//cadena a quemar en el archivo
    inodo archivousuarios;
    archivousuarios.i_gid=1;
    archivousuarios.i_size=sizeof(datosarchivo);//el tamaño de la cadena que voy a quemar
    archivousuarios.i_uid=1;

    string fech="04/03/2020";
    string fech2="04/03/2020";
    string fech3="04/03/2020";
    memset(archivousuarios.i_atime,0,sizeof(archivousuarios.i_atime));
    memset(archivousuarios.i_mtime,0,sizeof(archivousuarios.i_atime));
    memset(archivousuarios.i_ctime,0,sizeof(archivousuarios.i_atime));
    //archivousuarios.i_ctime[0]='00';
    strcpy(archivousuarios.i_atime,fech.c_str());
    strcpy(archivousuarios.i_mtime,fech2.c_str());
    strcpy(archivousuarios.i_ctime,fech3.c_str());
    for(int i=0;i<15;i++){
        archivousuarios.i_block[i]=-1;
    }
    archivousuarios.i_perm=664;
    archivousuarios.i_block[0]=1; //su bloque directo va al bloque 1 que es donde va a estar el archivo USERS

    archivousuarios.i_type='1'; //es archivo

    //termino de crear el inodo de archivos usuario

    //------------------------AHORA CREO BLOQUE PARA EL ARCHIVO USER

    bloque_archivos bloquearchivos;
    strcpy(bloquearchivos.b_content,datosarchivo);
    //-------------------------------TERMINAMOS DE CREAR EL BLOQUE DE ARCHIVOS


    //------GUARDAMOS EL INODO Y EL BLOQUE DEL ARCHIVO USR EN EL DISCO
    fseek(arch, auxsuperbloque.s_inode_start+sizeof(inodo), SEEK_SET);
    fwrite(&archivousuarios, sizeof(inodo), 1, arch);
    auxsuperbloque.s_free_inodes_count--;//disminuyo en 1 los inodos ocupados
    fseek(arch, auxsuperbloque.s_block_start+64, SEEK_SET);
    fwrite(&bloquearchivos, 64, 1, arch);
    auxsuperbloque.s_free_blocks_count--;//disminuyo en 1 los bloques ocupados



    //REESCRIBO EL SUPERBLOQUE
    fseek(arch, auxpart.part_start, SEEK_SET);
    fwrite(&auxsuperbloque, sizeof(superbloque), 1, arch);
    fclose(arch); //cierro el archivo
    std::cout << "\nPARTICION FORMATEADA CORRECTAMENTE!!! \n"; //si da null es porque no se encontro el archivo

    //   imprimirdatossuperbloque(auxmont);








}
