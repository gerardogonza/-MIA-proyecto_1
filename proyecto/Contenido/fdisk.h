#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <ctype.h>


using namespace std;



void crearParticion(string tam, string uni, string pat, string fi, string typ, string nam) {
    cout << "Particion: " << tam << "," << uni << "," << pat << "," << fi << "," << typ << "," << nam << "." << endl;
    bool banderaError = false;
    long int tama = 0;
    char fit = 'F';
    //----Verificar Unidad
    if((strcmp(uni.c_str(),"m")==0) || (strcmp(uni.c_str(),"k")==0)|| (strcmp(uni.c_str(),"b")==0)){
        if (strcmp(uni.c_str(),"m")==0){

            tama=1024*1024* atoi(tam.c_str());
        }else if(strcmp(uni.c_str(),"k")==0){
            tama=1024*atoi(tam.c_str());
        }else if(strcmp(uni.c_str(),"b")==0){
            tama=atoi(tam.c_str());
        }
    }else{
        cout <<("ERROR: El parametro de unit no es valido.\n");
        cout <<("AVISO: No se pudo crear la particion.\n");
        banderaError = true;
    }
    if((strcmp(fi.c_str(),"bf")==0) || (strcmp(fi.c_str(),"ff")==0)|| (strcmp(fi.c_str(),"wf")==0)){
        if (strcmp(uni.c_str(),"bf")==0){
            fit  = 'B';
        }else if(strcmp(uni.c_str(),"ff")==0){
            fit  = 'F';
        }else{
            fit  = 'W';
        }
    }else{
        cout << ("ERROR: El parametro de fit no es valido.\n");
        cout << ("AVISO: No se puede crear la particion.\n");
        banderaError = true;
    }
    //ahorita andamos viendo

}


