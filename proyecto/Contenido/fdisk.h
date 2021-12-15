#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <ctype.h>
#include <vector>


using namespace std;

void crearPrimaria(string pat, char typ, char fi, long int tam, string nam);
void crearExtendida(string pat, char typ, char fi, long tam, string nam);

void crearParticion(int tam, string uni, string pat, string fi, string typ, string nam) {
    cout << "Particion: " << tam << "," << uni << "," << pat << "," << fi << "," << typ << "," << nam << "." << endl;
    bool banderaError = false;
    long int tama = 0;
    char fit = 'F';
    //----Verificar Unidad
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
    char ruta[500];
    strcpy(ruta,pat.c_str());

    string completePath =  pat;
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
            crearPrimaria(pat,'P',fit,tama,nam);
        }
    }else if(strcmp(typ.c_str(),"E")==0 | strcmp(typ.c_str(),"e")==0){
        if(banderaError == false){
            cout << "Se va a crear una extendida"<<endl;
            crearExtendida(pat,'E',fit,tama,nam);
        }
    }else{
        cout << "LOGICA NO JALA AUN"<<endl;
    }


}

void crearPrimaria(string pat, char typ, char fi, long int tam, string nam) {
    bool errorCreacion = false;
    char ruta[500];
    strcpy(ruta, pat.c_str());

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
    strcpy(ruta, pat.c_str());

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






