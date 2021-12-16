#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <ctype.h>
#include <vector>
#include <fstream>



using namespace std;

void crearPrimaria(string pat, char typ, char fi, long int tam, string nam);
void crearExtendida(string pat, char typ, char fi, long tam, string nam);
void eliminarParticion(string nam, string pat, string tipo);
string obtenerRutaDisco(string nombreM);
void mostrarParticiones(string patD, string patR);
void mostrarMBR(string patD, string patR);

void crearParticion(int tam, string uni, string pat, string fi, string typ, string nam) {
    cout << "Particion: " << tam << "," << uni << "," << pat << "," << fi << "," << typ << "," << nam << "." << endl;
    bool banderaError = false;
    long int tama = 0;
    char fit = 'F';
    //----Verificar Unidad
    if(uni=="K"){
        uni="k";
    }
    if(uni=="M"){
        uni="m";
    }
    if(uni=="B"){
        uni="b";
    }
    if((strcmp(uni.c_str(),"m")==0) || (strcmp(uni.c_str(),"k")==0)|| (strcmp(uni.c_str(),"b")==0)){
        if (strcmp(uni.c_str(),"m")==0){
            tama=1024*1024* tam;
        }else if(strcmp(uni.c_str(),"k")==0){
            tama=1024*tam;
        }else if(strcmp(uni.c_str(),"b")==0){
            tama=tam;
        }
    }else{
        cout <<("ERROR: El parametro de unit no es valido.\n");
        cout <<("AVISO: No se pudo crear la particion.\n");
        banderaError = true;
    }
    if(fi=="BF"){
        fi="bf";
    }
    if(fi=="FF"){
        fi="ff";
    }
    if(fi=="WF"){
        fi="wf";
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
    //ver la ruta si existe o no
    char ruta[500];
    string completePath = rpath+ pat;
    strcpy(ruta,completePath.c_str());

    char diskc[1];
    strcpy(diskc, completePath.c_str());
    FILE *file = NULL;
    file = fopen(diskc, "r");
    if (file != NULL)
    {
        cout << "La Ruta es valida" << endl;
        banderaError = false;
    } else{
        cout << "ERROR esa ruta no es valida" << endl;
        banderaError = true;
    }

    //no funciona
    if (strcmp(typ.c_str(),"P")==0| strcmp(typ.c_str(),"p")==0){
        if(banderaError == false){
            cout << "Se va a crear una primaria"<<endl;
            crearPrimaria(completePath,'P',fit,tama,nam);
        }
    }else if(strcmp(typ.c_str(),"E")==0 | strcmp(typ.c_str(),"e")==0){
        if(banderaError == false){
            cout << "Se va a crear una extendida"<<endl;
            crearExtendida(completePath,'E',fit,tama,nam);
        }
    }else{
        cout << "LOGICA NO JALA AUN"<<endl;
    }


}

void crearPrimaria(string pat, char typ, char fi, long int tam, string nam) {
    bool errorCreacion = false;
    char ruta[500];
    string rutacompleta=pat  ;
    strcpy(ruta, rutacompleta.c_str());

    FILE *fileC;
    fileC = fopen(ruta, "rb+");
    rewind(fileC);

    MBR mbrtemp;
    fread(&mbrtemp, sizeof(MBR), 1, fileC);

//******* Verificar Tamaño
    cout << "Part:" << tam << "...Disco:" << mbrtemp.size << endl;
    if (tam >= mbrtemp.size) {
        errorCreacion = true;
        cout << "ERROR: El size de la particion es mayor o igual al size del disco" << endl;
    }
    //******* Ver si hay particiones libres
    int particionLibre = 0;
    if ( mbrtemp.partition[0].status == '0' ){
        particionLibre = 1;
        cout << "Particion 1 libre" << endl;
    }
    else if( mbrtemp.partition[1].status == '0' ){
        particionLibre = 2;
        cout << "Particion 2 libre" << endl;
    }
    else if( mbrtemp.partition[2].status == '0' ){
        particionLibre = 3;
        cout << "Particion 3 libre" << endl;
    }
    else if( mbrtemp.partition[3].status == '0' ){
        particionLibre = 4;
        cout << "Particion 4 libre" << endl;
    }
    else{
        cout <<("ERROR: No hay particiones libres.\n");
        cout <<("AVISO: No se pudo crear la particion primaria.\n");
        errorCreacion = true;
    }
    //******* Ver si aun se pueden crear primarias
    int partPrim = 0;
    for (int i = 0; i < 4; i++){
        if ( (mbrtemp.partition[i].type == 'p' ) && (mbrtemp.partition[i].status == '1') ){
            cout << "P  "<<i<< " es primaria" << endl;
            partPrim++;
        }
    }
    if(partPrim >= 3){
        cout <<("ERROR: Ya existen 3 particiones primarias.\n");
        cout <<("AVISO: No se pudo crear la particion primaria.\n");
        errorCreacion = true;
    }

    if(errorCreacion == false){
        vector <espacio> esp;
        for (int i = 0; i < 4; i++){
            if( mbrtemp.partition[i].status == '1' )
            {
                espacio nuevo;
                nuevo.inicio = mbrtemp.partition[i].start;
                nuevo.final = mbrtemp.partition[i].status + mbrtemp.partition[i].size;
                nuevo.particion = mbrtemp.partition[i].name;
                esp.push_back(nuevo);
            }
            }
        if(1+1 == 2){
            espacio nu;
            nu.inicio = 0;
            nu.final = sizeof (MBR) + 1;
            nu.particion = "MBR";
            esp.push_back(nu);
        }
        espacio temporal;
        for (int i = 0; i < esp.size(); i++){
            for (int j = 0; j< esp.size()-1; j++){
                if (esp[j].inicio > esp[j+1].inicio){ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
                    temporal = esp[j];
                    esp[j] = esp[j+1];
                    esp[j+1] = temporal;
                }
            }
        }
        //--- Mostrar como quedo ordenado
        for (int i =0; i < esp.size(); i++)
            cout<< esp[i].inicio <<"-"<< esp[i].final<<" "<<esp[i].particion << endl;
        if(esp.size() == 1){
            //--Insertar despues del mbr
            mbrtemp.partition[0].status='1';
            mbrtemp.partition[0].type=typ;
            mbrtemp.partition[0].fit =fi;
            mbrtemp.partition[0].start=( (sizeof (MBR)) + 1);
            mbrtemp.partition[0].size= tam;
            strcpy(mbrtemp.partition[0].name,nam.c_str());

            rewind(fileC);
            fwrite(&mbrtemp,sizeof (MBR),1,fileC);
            printf("AVISO: Particion primaria creada correctamente. Se almaceno en la P1. \n");
        }
        else{
            //---Insertar con el FF
            if( mbrtemp.fit == 'F' ){
                //----Buscar espacio
                espacio nTemp;
                bool hayEspacio = false;
                long int restaFI = 0;

                for (int i =0; i < esp.size()-1; i++){
                    restaFI =  esp[i+1].inicio - esp[i].final;

                    if(restaFI >= tam){
                        nTemp.inicio = esp[i].final + 1;
                        nTemp.final = esp[i+1].inicio - 1;
                        nTemp.particion = esp[i].particion;
                        hayEspacio = true;
                        break;
                    }

                    if(i == (esp.size()-2) ){
                        restaFI =  mbrtemp.size - esp[i+1].final;
                        //cout << "--**" << restaFI <<endl;
                        if(restaFI >= tam){
                            nTemp.inicio = esp[i+1].final + 1;
                            nTemp.particion = esp[i+1].particion;
                            hayEspacio = true;
                            break;
                        }
                    }
                }

                if(hayEspacio == true){
                    cout << "Insertar despues de: "<< nTemp.particion << ", IN: " << nTemp.inicio << endl;
                    int BcreoP = 0;
                    for (int i = 0; i < 4; i++) {
                        if( (mbrtemp.partition[i].status=='0') && BcreoP==0){
                            mbrtemp.partition[i].status='1';
                            mbrtemp.partition[i].type=typ;
                            mbrtemp.partition[i].fit =fi;
                            mbrtemp.partition[i].start=nTemp.inicio;
                            mbrtemp.partition[i].size= tam;
                            strcpy(mbrtemp.partition[i].name,nam.c_str());

                            rewind(fileC);
                            fwrite(&mbrtemp,sizeof (MBR),1,fileC);
                            cout<< "AVISO: Particion primaria creada correctamente. Se almaceno en la P"<<i << endl;

                            BcreoP=1;
                        }
                    }
                }
                else {
                    cout << "ERROR: No existe espacio suficiente para almacenar la particion" <<endl;
                    cout << "AVISO: No se creara la particion primaria" << endl;
                }


            }
                //---Insertar con el BF
            else if ( mbrtemp.fit == 'B' ) {
                //----Buscar espacio
                espacio nTemp;
                bool hayEspacio = false;
                long int restaFI = 0, restaTemp = 0, entrarF = 0;

                for (int i =0; i < esp.size()-1; i++){
                    restaFI =  esp[i+1].inicio - esp[i].final;
                    //cout << "--*" << restaFI << endl; //<< "," << restaTemp <<endl;
                    if(restaFI >= tam){
                        if(entrarF == 0 ){
                            entrarF++;
                            restaTemp = restaFI;
                            nTemp.inicio = esp[i].final + 1;
                            nTemp.final = esp[i+1].inicio - 1;
                            nTemp.particion = esp[i].particion;
                            hayEspacio = true;
                        }else {
                            if(restaTemp > restaFI){
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i].final + 1;
                                nTemp.final = esp[i+1].inicio - 1;
                                nTemp.particion = esp[i].particion;
                                hayEspacio = true;
                            }
                        }
                    }

                    if(i == (esp.size()-2) ){
                        restaFI =  mbrtemp.size - esp[i+1].final;
                        //cout << "--**" << restaFI << endl; //<< "," << restaTemp <<endl;
                        if(restaFI >= tam){
                            if(entrarF == 0 ){
                                entrarF++;
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i+1].final + 1;
                                nTemp.particion = esp[i+1].particion;
                                hayEspacio = true;
                            }else {
                                if(restaTemp > restaFI){
                                    restaTemp = restaFI;
                                    nTemp.inicio = esp[i+1].final + 1;
                                    nTemp.particion = esp[i+1].particion;
                                    hayEspacio = true;
                                }
                            }
                        }
                    }

                }

                if(hayEspacio == true){
                    cout << "Insertar despues de: "<< nTemp.particion << ", IN: " << nTemp.inicio << endl;

                    int BcreoP = 0;
                    for (int i = 0; i < 4; i++) {
                        if( (mbrtemp.partition[i].status=='0') && BcreoP==0){
                            mbrtemp.partition[i].status='1';
                            mbrtemp.partition[i].type=typ;
                            mbrtemp.partition[i].size =fi;
                            mbrtemp.partition[i].start=nTemp.inicio;
                            mbrtemp.partition[i].size= tam;
                            strcpy(mbrtemp.partition[i].name,nam.c_str());

                            rewind(fileC);
                            fwrite(&mbrtemp,sizeof (MBR),1,fileC);
                            cout<< "AVISO: Particion primaria creada correctamente. Se almaceno en la P"<<i << endl;
                            BcreoP=1;
                        }
                    }

                }
                else {
                    cout << "ERROR: No existe espacio suficiente para almacenar la particion" <<endl;
                    cout << "AVISO: No se creara la particion primaria" << endl;
                }

            }
                //---Insertar con el WF
            else{
                //----Buscar espacio
                espacio nTemp;
                bool hayEspacio = false;
                long int restaFI = 0, restaTemp = 0, entrarF = 0;

                for (int i =0; i < esp.size()-1; i++){
                    restaFI =  esp[i+1].inicio - esp[i].final;
                    //cout << "--*" << restaFI << endl; //<< "," << restaTemp <<endl;
                    if(restaFI >= tam){
                        if(entrarF == 0 ){
                            entrarF++;
                            restaTemp = restaFI;
                            nTemp.inicio = esp[i].final + 1;
                            nTemp.final = esp[i+1].inicio - 1;
                            nTemp.particion = esp[i].particion;
                            hayEspacio = true;
                        }else {
                            if(restaTemp < restaFI){
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i].final + 1;
                                nTemp.final = esp[i+1].inicio - 1;
                                nTemp.particion = esp[i].particion;
                                hayEspacio = true;
                            }
                        }
                    }

                    if(i == (esp.size()-2) ){
                        restaFI =  mbrtemp.size - esp[i+1].final;
                        //cout << "--**" << restaFI << endl; //<< "," << restaTemp <<endl;
                        if(restaFI >= tam){
                            if(entrarF == 0 ){
                                entrarF++;
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i+1].final + 1;
                                nTemp.particion = esp[i+1].particion;
                                hayEspacio = true;
                            }else {
                                if(restaTemp < restaFI){
                                    restaTemp = restaFI;
                                    nTemp.inicio = esp[i+1].final + 1;
                                    nTemp.particion = esp[i+1].particion;
                                    hayEspacio = true;
                                }
                            }
                        }
                    }

                }

                if(hayEspacio == true){
                    cout << "Insertar despues de: "<< nTemp.particion << ", IN: " << nTemp.inicio << endl;

                    int BcreoP = 0;
                     for (int i = 0; i < 4; i++) {
                         if( (mbrtemp.partition[i].status=='0') && BcreoP==0){
                             mbrtemp.partition[i].status='1';
                             mbrtemp.partition[i].type=typ;
                             mbrtemp.partition[i].fit =fi;
                             mbrtemp.partition[i].start=nTemp.inicio;
                             mbrtemp.partition[i].size= tam;
                             strcpy(mbrtemp.partition[i].name,nam.c_str());

                             rewind(fileC);
                             fwrite(&mbrtemp,sizeof (MBR),1,fileC);
                             cout<< "AVISO: Particion primaria creada correctamente. Se almaceno en la P"<<i << endl;
                             BcreoP=1;
                         }
                     }

                }
                else {
                    cout << "ERROR: No existe espacio suficiente para almacenar la particion" <<endl;
                    cout << "AVISO: No se creara la particion primaria" << endl;
                }

            }
        }

    }
    fclose(fileC);

    }
// ************************************************************EXTENDIDA*************************************************************************************

void crearExtendida(string pat, char typ, char fi, long tam, string nam){
    bool errorCreacion = false;
    char ruta[500];
    string rutacompleta=pat;
    strcpy(ruta, rutacompleta.c_str());

    FILE *fileC;
    fileC= fopen(ruta,"rb+");
    rewind(fileC);

    MBR mbrtemp;
    fread(&mbrtemp,sizeof(MBR),1,fileC);

    //******* Verificar Tamaño
    cout << tam <<",..." << mbrtemp.size << endl;
    if(tam >= mbrtemp.size){
        errorCreacion = true;
        cout << "ERROR: El tamanio de la particion es mayor o igual al tamanio del disco" << endl;
        cout << "AVISO: No se creara la particion extendida"<<endl;
    }

    //******* Ver si hay particiones libres
    int particionLibre = 0;

    if ( mbrtemp.partition[0].status == '0' ){
        particionLibre = 1;
        cout << "Particion 1 libre" << endl;
    }
    else if( mbrtemp.partition[1].status == '0' ){
        particionLibre = 2;
        cout << "Particion 2 libre" << endl;
    }
    else if( mbrtemp.partition[2].status == '0' ){
        particionLibre = 3;
        cout << "Particion 3 libre" << endl;
    }
    else if( mbrtemp.partition[3].status == '0' ){
        particionLibre = 4;
        cout << "Particion 4 libre" << endl;
    }
    else{
        cout <<("ERROR: No hay particiones libres.\n");
        cout <<("AVISO: No se pudo crear la particion extendida.\n");
        errorCreacion = true;
    }

    //******* Ver si aun se pueden crear primarias
    int partPrim = 0;
    if ( (mbrtemp.partition[0].type == 'E') && (mbrtemp.partition[0].status == '1') ){
        cout << "P1 es Extendida" << endl;
        partPrim++;}
    if ( (mbrtemp.partition[1].type == 'E') && (mbrtemp.partition[1].status == '1') ){
        cout << "P2 es Extendida" << endl;
        partPrim++;}
    if ( (mbrtemp.partition[2].type == 'E') && (mbrtemp.partition[2].status == '1') ){
        cout << "P3 es Extendida" << endl;
        partPrim++;}
    if ( (mbrtemp.partition[3].type == 'E') && (mbrtemp.partition[3].status == '1') ){
        cout << "P4 es Extendida" << endl;
        partPrim++;}

    if(partPrim >= 1){
        cout <<("ERROR: Ya existe un particion extendida.\n");
        cout <<("AVISO: No se pudo crear la particion extendida.\n");
        errorCreacion = true;
    }

    //******* Verificar si cabe en algun espacio
    if(errorCreacion == false){
        vector<espacio> esp;

        //--- Llenar el vector con los espacios usados
        for (int i = 0; i < 4; i++) {
            if (mbrtemp.partition[i].status == '1') {
                espacio nuevo;
                nuevo.inicio = mbrtemp.partition[i].start;
                nuevo.final = mbrtemp.partition[i].start + mbrtemp.partition[i].size;
                nuevo.particion = mbrtemp.partition[i].name;
                esp.push_back(nuevo);
            }
        }
        //--- Insertar tamañao del ebr
        if(1+1 == 2){
            espacio nu;
            nu.inicio = 0;
            nu.final = sizeof (MBR) + 1;
            nu.particion = "MBR";
            esp.push_back(nu);
        }

        //--- Ordenar de menor a mayor
        espacio temporal;
        for (int i = 0; i < esp.size(); i++){
            for (int j = 0; j< esp.size()-1; j++){
                if (esp[j].inicio > esp[j+1].inicio){ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
                    temporal = esp[j];
                    esp[j] = esp[j+1];
                    esp[j+1] = temporal;
                }
            }
        }

        //--- Mostrar como quedo ordenado
        for (int i =0; i < esp.size(); i++)
            cout<< esp[i].inicio <<"-"<< esp[i].final<<" "<<esp[i].particion << endl;

        //---- Ver cual es el espacio ideal segun el fit
        if(esp.size() == 1){
            //--Insertar despues del mbr
            mbrtemp.partition[0].status='1';
            mbrtemp.partition[0].type='E';
            mbrtemp.partition[0].fit =fi;
            mbrtemp.partition[0].start=( (sizeof (MBR)) + 1);
            mbrtemp.partition[0].size= tam;
            strcpy(mbrtemp.partition[0].name,nam.c_str());
            //cout << "Tamaño de la pa " << tam << "," << mbrtemp.mbr_partition_1.part_start << "," << mbrtemp.mbr_partition_1.part_size<< endl;

            //fseek(fileC,0,SEEK_SET);
            rewind(fileC);
            fwrite(&mbrtemp,sizeof (MBR),1,fileC);
            printf("AVISO: Particion extendida creada correctamente. Se almaceno en la P1. \n");
        }
        else{
            //---Insertar con el FF
            if( mbrtemp.fit == 'F' ){
                //----Buscar espacio
                espacio nTemp;
                bool hayEspacio = false;
                long int restaFI = 0;

                for (int i =0; i < esp.size()-1; i++){
                    restaFI =  esp[i+1].inicio - esp[i].final;
                    //cout << "--*" << restaFI <<endl;
                    if(restaFI >= tam){
                        nTemp.inicio = esp[i].final + 1;
                        nTemp.final = esp[i+1].inicio - 1;
                        nTemp.particion = esp[i].particion;
                        hayEspacio = true;
                        break;
                    }

                    if(i == (esp.size()-2) ){
                        restaFI =  mbrtemp.size - esp[i+1].final;
                        //cout << "--**" << restaFI <<endl;
                        if(restaFI >= tam){
                            nTemp.inicio = esp[i+1].final + 1;
                            nTemp.particion = esp[i+1].particion;
                            hayEspacio = true;
                            break;
                        }
                    }
                }

                if(hayEspacio == true) {
                    cout << "Insertar despues de: " << nTemp.particion << ", IN: " << nTemp.inicio << endl;

                    int BcreoP = 0;
                    for (int i = 0; i < esp.size(); i++) {
                        if ((mbrtemp.partition[i].status == '0') && BcreoP == 0) {
                            mbrtemp.partition[i].status = '1';
                            mbrtemp.partition[i].type = 'E';
                            mbrtemp.partition[i].fit = fi;
                            mbrtemp.partition[i].start = nTemp.inicio;
                            mbrtemp.partition[i].size = tam;
                            strcpy(mbrtemp.partition[i].name, nam.c_str());

                            rewind(fileC);
                            fwrite(&mbrtemp, sizeof(MBR), 1, fileC);

                            cout << "AVISO: Particion extendida creada correctamente. Se almaceno en la P" <<i<<endl;
                            BcreoP = 1;
                        }
                    }
                }
                else {
                    cout << "ERROR: No existe espacio suficiente para almacenar la particion" <<endl;
                    cout << "AVISO: No se creara la particion extendida" << endl;
                }


            }
                //---Insertar con el BF
            else if ( mbrtemp.fit == 'B' ) {
                //----Buscar espacio
                espacio nTemp;
                bool hayEspacio = false;
                long int restaFI = 0, restaTemp = 0, entrarF = 0;

                for (int i =0; i < esp.size()-1; i++){
                    restaFI =  esp[i+1].inicio - esp[i].final;
                    //cout << "--*" << restaFI << endl; //<< "," << restaTemp <<endl;
                    if(restaFI >= tam){
                        if(entrarF == 0 ){
                            entrarF++;
                            restaTemp = restaFI;
                            nTemp.inicio = esp[i].final + 1;
                            nTemp.final = esp[i+1].inicio - 1;
                            nTemp.particion = esp[i].particion;
                            hayEspacio = true;
                        }else {
                            if(restaTemp > restaFI){
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i].final + 1;
                                nTemp.final = esp[i+1].inicio - 1;
                                nTemp.particion = esp[i].particion;
                                hayEspacio = true;
                            }
                        }
                    }

                    if(i == (esp.size()-2) ){
                        restaFI =  mbrtemp.size - esp[i+1].final;
                        //cout << "--**" << restaFI << endl; //<< "," << restaTemp <<endl;
                        if(restaFI >= tam){
                            if(entrarF == 0 ){
                                entrarF++;
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i+1].final + 1;
                                nTemp.particion = esp[i+1].particion;
                                hayEspacio = true;
                            }else {
                                if(restaTemp > restaFI){
                                    restaTemp = restaFI;
                                    nTemp.inicio = esp[i+1].final + 1;
                                    nTemp.particion = esp[i+1].particion;
                                    hayEspacio = true;
                                }
                            }
                        }
                    }

                }

                if(hayEspacio == true){
                    cout << "Insertar despues de: "<< nTemp.particion << ", IN: " << nTemp.inicio << endl;

                    int BcreoP = 0;
                    for (int i = 0; i < 4; i++) {
                        if ((mbrtemp.partition[i].status == '0') && BcreoP == 0) {
                            mbrtemp.partition[i].status = '1';
                            mbrtemp.partition[i].type = 'E';
                            mbrtemp.partition[i].fit = fi;
                            mbrtemp.partition[i].start = nTemp.inicio;
                            mbrtemp.partition[i].size = tam;
                            strcpy(mbrtemp.partition[i].name, nam.c_str());

                            rewind(fileC);
                            fwrite(&mbrtemp, sizeof(MBR), 1, fileC);
                            cout << "AVISO: Particion extendida creada correctamente. Se almaceno en la P" <<i<<endl;
                            BcreoP = 1;
                        }
                    }

                }
                else {
                    cout << "ERROR: No existe espacio suficiente para almacenar la particion" <<endl;
                    cout << "AVISO: No se creara la particion extendida" << endl;
                }

            }
                //---Insertar con el WF
            else{
                //----Buscar espacio
                espacio nTemp;
                bool hayEspacio = false;
                long int restaFI = 0, restaTemp = 0, entrarF = 0;

                for (int i =0; i < esp.size()-1; i++){
                    restaFI =  esp[i+1].inicio - esp[i].final;
                    //cout << "--*" << restaFI << endl; //<< "," << restaTemp <<endl;
                    if(restaFI >= tam){
                        if(entrarF == 0 ){
                            entrarF++;
                            restaTemp = restaFI;
                            nTemp.inicio = esp[i].final + 1;
                            nTemp.final = esp[i+1].inicio - 1;
                            nTemp.particion = esp[i].particion;
                            hayEspacio = true;
                        }else {
                            if(restaTemp < restaFI){
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i].final + 1;
                                nTemp.final = esp[i+1].inicio - 1;
                                nTemp.particion = esp[i].particion;
                                hayEspacio = true;
                            }
                        }
                    }

                    if(i == (esp.size()-2) ){
                        restaFI =  mbrtemp.size - esp[i+1].final;
                        //cout << "--**" << restaFI << endl; //<< "," << restaTemp <<endl;
                        if(restaFI >= tam){
                            if(entrarF == 0 ){
                                entrarF++;
                                restaTemp = restaFI;
                                nTemp.inicio = esp[i+1].final + 1;
                                nTemp.particion = esp[i+1].particion;
                                hayEspacio = true;
                            }else {
                                if(restaTemp < restaFI){
                                    restaTemp = restaFI;
                                    nTemp.inicio = esp[i+1].final + 1;
                                    nTemp.particion = esp[i+1].particion;
                                    hayEspacio = true;
                                }
                            }
                        }
                    }

                }

                if(hayEspacio == true){
                    cout << "Insertar despues de: "<< nTemp.particion << ", IN: " << nTemp.inicio << endl;

                    int BcreoP = 0;
                    for (int i = 0; i < 4; i++) {
                        if ((mbrtemp.partition[i].status == '0') && BcreoP == 0) {
                            mbrtemp.partition[i].status = '1';
                            mbrtemp.partition[i].type = 'E';
                            mbrtemp.partition[i].fit = fi;
                            mbrtemp.partition[i].start = nTemp.inicio;
                            mbrtemp.partition[i].size = tam;
                            strcpy(mbrtemp.partition[i].name, nam.c_str());

                            rewind(fileC);
                            fwrite(&mbrtemp, sizeof(MBR), 1, fileC);
                            cout << "AVISO: Particion extendida creada correctamente. Se almaceno en la P" <<i<<endl;
                            BcreoP = 1;
                        }
                    }
                }
                else {
                    cout << "ERROR: No existe espacio suficiente para almacenar la particion" <<endl;
                    cout << "AVISO: No se creara la particion extendida" << endl;
                }

            }
        }

    }

    //---- Cerrar el archivo
    fclose(fileC);
}

void eliminarP(string nam, string pat, string tipo){
    cout << "Part. delete: " << nam << "," << tipo << "," << pat << endl;
    int largo;
    largo = tipo.size();
    string rutacompleta=rpath+pat;
    for (int letra = 0; letra < largo; ++letra)
    {
        tipo[letra] = tolower(tipo[letra]);
    }
    if(strcmp(tipo.c_str(),"fast")){
        eliminarParticion(nam,rutacompleta,tipo);
    }else if(strcmp(tipo.c_str(),"full")){
        eliminarParticion(nam,rutacompleta,tipo);
    }else{
        printf("ERROR: El parametro de delete no es valido.\n");
        printf("AVISO: No se pudo eliminar la particion.\n");
    }
}

void eliminarParticion(string nam, string pat, string tipo){

    int BborroP = 0;
    char ruta[500];
    char nom[16];
    strcpy(nom,nam.c_str());
    string rutacompleta=pat;
    strcpy(ruta,rutacompleta.c_str());

    FILE *fileC;

    fileC= fopen(ruta,"rb+");
    rewind(fileC);
    MBR mbrtemp;
    fread(&mbrtemp,sizeof(MBR),1,fileC);
    cout << "Particion Encontrada"<< endl;

    if(fileC!=nullptr){
            for (int i = 0; i < 4; i++) {
                if ((mbrtemp.partition[i].status == '1') && (BborroP == 0)) {

                    if (strcmp(mbrtemp.partition[i].name, nam.c_str()) == 0) {

                        if (tipo == "fast") {
                            mbrtemp.partition[i].status = '0';

                            rewind(fileC);
                            fwrite(&mbrtemp, sizeof(MBR), 1, fileC); // aca escribo de nuevo el MBR

                            cout << "AVISO: Particion eliminada correctamente. en la P " <<i<<" FAST"<<endl;
                            BborroP = 1;
                        } else {
                            mbrtemp.partition[i].status = '0';
                            mbrtemp.partition[i].type = '\0';
                            mbrtemp.partition[i].fit = '\0';
                            mbrtemp.partition[i].start = 0;
                            mbrtemp.partition[i].size = 0;
                            strcpy(mbrtemp.partition[i].name, "\0");
                            //fseek(archivo,0,SEEK_SET);
                            rewind(fileC);
                            fwrite(&mbrtemp, sizeof(MBR), 1, fileC); // aca escribo de nuevo el MBR

                            cout << "AVISO: Particion eliminada correctamente.  en la P " <<i<<" FULL"<<endl;
                            BborroP = 1;
                        }

                    }

                }
            }
            if(BborroP == 0){
                printf("ERROR: No se encontro la particion.\n");
                cout << "AVISO: No se pudo eliminar la particion " << nam <<".\n";
            }
        }
        else{
            printf("ERROR: No se pudo acceder al disco.\n");
            printf("AVISO: No se pudo eliminar la particion.\n");
        }

        fclose(fileC);




}


void reportes(string path, string tipo, string nombre){

    if(tipo == "disk"){
        string ruta = "";
        ruta = obtenerRutaDisco(nombre);
        mostrarParticiones(ruta,path);
    }
    else{
        string ruta = "";
        ruta = obtenerRutaDisco(nombre);
        mostrarMBR(ruta,path);
        //cout << "NO LOGRE IMPLEMENTAR EL OTRO TIPO DE REPORTE" << endl;
    }
}
//******************** MOSTRAR PARTICIONES
void mostrarParticiones(string patD, string patR){

    cout << "REPORTE DEL DISCO disco" << endl;

    char ruta[500];
    strcpy(ruta, patD.c_str());
    cout << ruta << endl;
    FILE *fileC;
    fileC= fopen(ruta,"rb+");
    rewind(fileC);

    MBR mbrtemp;
    fread(&mbrtemp,sizeof(MBR),1,fileC);

    //****
    vector<espacio> esp;
    string nodos ="";
    int contadorNodos = 0;
    //--- Llenar el vector con los espacios usados

    for (int i = 0; i < 4; i++) {
        if (mbrtemp.partition[i].status == '1') {
            espacio nuevo;
            nuevo.inicio = mbrtemp.partition[i].start;
            nuevo.final = mbrtemp.partition[i].start + mbrtemp.partition[i].size;
            nuevo.particion = mbrtemp.partition[i].name;
            nuevo.tip = mbrtemp.partition[i].type;
            esp.push_back(nuevo);
        }
    }

    //--- Insertar tamañao del ebr
    if(1+1 == 2){
        espacio nu;
        nu.inicio = 0;
        nu.final = sizeof (MBR) + 1;
        nu.particion = "MBR";
        nu.tip = 'M';
        esp.push_back(nu);
    }

    //--- Ordenar de menor a mayor
    espacio temporal;
    for (int i = 0; i < esp.size(); i++){
        for (int j = 0; j< esp.size()-1; j++){
            if (esp[j].inicio > esp[j+1].inicio){ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
                temporal = esp[j];
                esp[j] = esp[j+1];
                esp[j+1] = temporal;
            }
        }
    }

    //--- Mostrar como quedo ordenado
    cout << "===========" << endl;
    for (int i =0; i < esp.size(); i++){
        cout<< esp[i].inicio <<"-"<< esp[i].final<<" "<<esp[i].particion << endl;
    }
    cout << "===========" << endl;
    double p1 = 0, p2 = 0;
    int p3 = 0;
    float restita = 0;
    int contadorV = 300;
    int entrada = 0;
    for (int i = esp.size()-1; i >= 0; i--) {
        //---Ver si quedo espacio al final
        if(entrada == 0){
            entrada ++;
            //cout << mbrtemp.mbr_tamano << "-" << esp[i].final << endl;
            restita = mbrtemp.size - esp[i].final;
            if(restita > 5){
                //cout << "Existe un espacio en blanco entre " << esp[i].particion << " y " << esp[i-1].particion << endl;
                nodos += "  nd_";
                nodos += to_string(contadorV);
                contadorV++;
                nodos += "[label = \" { LIBRE | ";
                p1 = restita *100;
                p2 = p1 / mbrtemp.size;
                nodos += to_string(p2);
                nodos += "\\n % del disco}\"];\n";
            }
        }

        if( esp[i].tip == 'P'){
            nodos += "  nd_";
            nodos += to_string(contadorNodos);
            contadorNodos++;
            nodos += "       [label = \"{";
            nodos += (esp[i].particion);
            //nodos += " | Tam: ";
            p3 = (esp[i].final - esp[i].inicio);
            //nodos += to_string(p3);
            nodos += "|";
            p1 = ( p3 * 100 );
            p2 = ( p1 / mbrtemp.size ) ;
            nodos += to_string(p2) ;
            nodos += "\\n% del disco }\"];\n";
        }
        else if(esp[i].tip == 'M'){
            nodos += "  nd_mbr       [label = \"MBR\"];\n";
        }
        else{
            nodos += "  nd_";
            nodos += to_string(contadorNodos);
            contadorNodos++;
            nodos +="      [ shape=record label=\"";
            nodos += "{ ";
            nodos += esp[i].particion;
            nodos += " | ";
            p3 = (esp[i].final - esp[i].inicio);
            p1 = ( p3 * 100 );
            p2 = ( p1 / mbrtemp.size ) ;
            nodos += to_string(p2) ;
            nodos += "\\n % del disco | { logica|logica|logica} }\"];\n";
        }

        //cout<< esp[i].inicio <<"-"<< esp[i].final<<" "<<esp[i].particion << endl;
        //---Pequeña resta para ver si hay un vacio con el anterior

        //---Ver si hay espacio entre la primera y el mbr
        if((i-1) == 0){
            //cout << esp[i-1].inicio << "====" <<sizeof (MBR) << endl;
            restita = esp[i].inicio - sizeof (MBR);
            if(restita > 5){
                //cout << "Existe un espacio en blanco entre " << esp[i].particion << " y " << esp[i-1].particion << endl;
                nodos += "  nd_";
                nodos += to_string(contadorV);
                contadorV++;
                nodos += "[label = \" { LIBRE | ";
                p1 = restita *100;
                p2 = p1 / mbrtemp.size;
                nodos += to_string(p2);
                nodos += "\\n % del disco}\"];\n";
            }
        }
            //---Ver si hay espacio entre particiones
        else {
            restita = esp[i].inicio - esp[i-1].final;
            if(restita > 5){
                //cout << "Existe un espacio en blanco entre " << esp[i].particion << " y " << esp[i-1].particion << endl;
                nodos += "  nd_";
                nodos += to_string(contadorV);
                contadorV++;
                nodos += "[label = \" { LIBRE | ";
                p1 = restita *100;
                p2 = p1 / mbrtemp.size;
                nodos += to_string(p2);
                nodos += "\\n % del disco}\"];\n";
            }
        }
    }

    //****

    fclose(fileC);

    string grafo = "digraph Q { \n";
    grafo += "  node [shape=record];\n";
    grafo += nodos;
    grafo += "subgraph cluster_R {\n";
    grafo += "  label = \"Disco ";
    grafo += to_string(mbrtemp.diskSignature);
    grafo += "\";\n";
    grafo += "  rank=same nd_mbr";
    for(int i = 0; i < contadorNodos; i++){
        grafo += " nd_";
        grafo += to_string(i);
    }
    for (int i = 300; i < contadorV; i++) {
        grafo += " nd_";
        grafo += to_string(i);
    }

    string cc = "dot -Tpng /home/gerardo/Documentos/archivoDot.dot -o " ;
    cc += patR;
    //cout << cc << endl;
    grafo += "\n}\n";
    grafo += "}\n";
    //cout <<grafo <<endl;

    char ru[500];
    strcpy(ru, cc.c_str());
    cout << "ru: " << ru << endl;

    ofstream file;
    file.open("/home/gerardo/Documentos/archivoDot.dot");
    file << grafo;
    file.close();

    system(ru);

    string ccc ="nohup display ";
    ccc+= patR;
    ccc+= " &";

    char rut[500];
    strcpy(rut, ccc.c_str());
    cout << ccc << endl;

    system(rut);
    getchar();


}



void mostrarMBR(string patD, string patR){
    cout << "REPORTE DEL DISCO mbr" << endl;



    string grafo_final = "";

    char ruta[500];
    strcpy(ruta, patD.c_str());
    cout << ruta << endl;
    FILE *fileC;
    fileC= fopen(ruta,"rb+");
    rewind(fileC);

    MBR mbrtemp;
    fread(&mbrtemp,sizeof(MBR),1,fileC);

    //****
    vector<espacio> esp;
    string nodos ="";
    int contadorNodos = 0;
    //--- Llenar el vector con los espacios usados
    grafo_final += " mbr_tam_disco :  " ;
    grafo_final +=  to_string(mbrtemp.size);
    grafo_final += "  | " ;

    grafo_final += " diskr_fecha_creacion :  " ;
    grafo_final +=  mbrtemp.creationDate;
    grafo_final += "  | " ;

    grafo_final += " disk_signature :  " ;
    grafo_final +=  to_string(mbrtemp.diskSignature);
    grafo_final += "  | " ;

    grafo_final += " disk_fit :  " ;
    grafo_final +=  to_string(mbrtemp.fit);
    grafo_final += "  |" ;
    for (int i = 0; i < 4; i++) {
        if (mbrtemp.partition[0].status == 'V' || mbrtemp.partition[0].status == '0') {
            grafo_final += " part_1 status: ";
            grafo_final += to_string(mbrtemp.partition[i].status);
            grafo_final += " |";

            grafo_final += " part_1 type:  ";
            grafo_final += to_string(mbrtemp.partition[i].type);
            grafo_final += " |";

            grafo_final += " part_1 fit:  ";
            grafo_final += to_string(mbrtemp.partition[i].fit);
            grafo_final += " |";

            grafo_final += " part_1 start: ";
            grafo_final += to_string(mbrtemp.partition[i].start);
            grafo_final += " |";

            grafo_final += " part_1 size:  ";
            grafo_final += to_string(mbrtemp.partition[i].size);
            grafo_final += " |";

            grafo_final += " part_1 fit:  ";
            grafo_final += mbrtemp.partition[i].name;
            grafo_final += " |";

        }
    }


    //***************

    fclose(fileC);

    string grafo = "digraph Q { \n";
    grafo += "  node [shape=record];\n";
    grafo += nodos;
    grafo += "subgraph cluster_R {\n";
    grafo += "  label = \"Disco ";
    grafo += to_string(mbrtemp.diskSignature);
    grafo += "\";\n";
    grafo += "  rank=same ";
    grafo += "nodo_MBR [ label = \" {";
    grafo += grafo_final;
    grafo += "}\" ] ; \n";


    grafo += "\n}\n";
    grafo += "}\n";
    //cout << grafo << endl;
    string cc = "dot -Tpng /home/gerardo/Documentos/archivoDot1.dot -o " ;
    cc += patR;
    //cout << cc << endl;

    //cout <<grafo <<endl;

    char ru[500];
    strcpy(ru, cc.c_str());
    cout << "ru: " << ru << endl;

    ofstream file;
    file.open("/home/gerardo/Documentos/archivoDot1.dot");
    file << grafo;
    file.close();
    //system("dot -Tpng archivoDot.dot -o grafoMBR.png");
    system(ru);

    string ccc ="nohup display ";
    ccc+= patR;
    ccc+= " &";

    char rut[500];
    strcpy(rut, ccc.c_str());
    cout << ccc << endl;
    //system("nohup display grafoMBR.png &" );
    system(rut);
    getchar();


}



