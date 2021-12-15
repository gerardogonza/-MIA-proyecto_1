#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <ctype.h>
#include "structs.h"

using namespace std;

void crearDisco(int size,string unit,string spath)
{


    cout << "Creando disco... " <<spath<< endl;
    string completePath =  spath;
    char diskc[size + 1];
    strcpy(diskc, completePath.c_str());
    FILE *file = NULL;
    file = fopen(diskc, "r");
    if (file != NULL)
    {
        cout << "ERROR EL DISCO QUE QUIERES CREAR YA EXISTE CON ESE NOMBRE PRUEBA CON OTRO NOMBRE" << endl;

        return;
    }
    int size_new;
    if (unit == "k" | unit =="K")
    {
        size_new = size * 1024;
    }
    else
    {
        size_new = size * 1024 * 1024;
    }
    file = fopen(diskc, "wb");
    fwrite("\0", 1, 1, file);
    fseek(file, size_new, SEEK_SET);
    fwrite("\0", 1, 1, file);
    auto end = std::chrono::system_clock::now();
    MBR mbr;
    mbr.size = size_new;
    mbr.diskSignature = rand() % 1000;
    mbr.fit = 'f';
    mbr.creationDate = std::chrono::system_clock::to_time_t(end);
    for (int i = 0; i < 4; i++)
    {
        mbr.partition[i].status = '0';
        mbr.partition[i].size = 0;
        mbr.partition[i].fit = 'f';
        mbr.partition[i].start = size_new;
        strcpy(mbr.partition[i].name,"");
    }
    cout << "disco creado" << endl;
    cout << "fecha de creacion: " << asctime(gmtime(&mbr.creationDate));
    cout << "signature: " << mbr.diskSignature << endl;
    cout << "size_new: " << mbr.size << endl;
    cout << "fit: " << mbr.fit << endl;
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, file);
    fclose(file);

}

void eliminarDisco(string pat){
char ruta[500];
strcpy(ruta,pat.c_str());
    int error;
    string completePath =  pat;
    char diskc[1];
    strcpy(diskc, completePath.c_str());
    cout << diskc << endl;
    FILE *file = NULL;
    file = fopen(diskc, "r");
    if (file != NULL)
    {
        cout << "ERROR EL DISCO QUE QUIERES CREAR YA EXISTE CON ESE NOMBRE PRUEBA CON OTRO NOMBRE" << endl;

      error = 1;
    }

if (error == 1){
string valor = "N";
cout <<"¿Esta seguro que desea borrar el disco "<<pat << " ? [y][n]"<< endl;
cout << "RESPUESTA: " ;
cin >> valor;
if(valor == ("y") ){
cout << "AVISO: Se eliminara el disco" << endl;
remove(pat.c_str());
cout << "DISCO BORRADO CON EXITO" << endl;
}else {
cout << " No se borro el disco " << endl;
}
}
else{
cout << "ERROR: El disco "<< pat<< "no existe o el directorio es incorrecto, verificar" << endl;
}
}


