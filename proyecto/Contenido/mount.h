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
int verificarNombre(string pa, string nombre);
listaM discoMontado(string pat);
string obtenerNombreDisco(string pat);
void agregarDisco(string nombreD, string pat);
int agregarParticion(string nomPar, listaM disco);
int particionMontada(string nombre, listaM disco);
void mostrarMontados();
static char letras[26]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
static vector<listaM> listaMontados;
static int contadorM = 0, contadorA = 0;
void montarParticion(string pat, string nombre) {
    cout << "AVISO: Comprobando Montar Particion en: " << pat << " nombre:  " << nombre << endl;
    int BmontoP = 0;
    //RUTA
    char ruta[500];
    strcpy(ruta, pat.c_str());
    //NOMBRE DE LA PARTICION
    char nom[100];
    strcpy(nom, nombre.c_str());
    string n = obtenerNombreDisco(pat);
    //NOMBRE DEL DISCO
    char nombreDisco[500];
    strcpy(nombreDisco, n.c_str());


    if (verificarNombre(pat, nombre) == 1) {
        char ruta[500];
        strcpy(ruta, pat.c_str());
        string completePath = pat;
        char diskc[1];
        strcpy(diskc, completePath.c_str());
        FILE *file = NULL;
        file = fopen(diskc, "r");
        if (file != NULL) {
            FILE *fileC;
            fileC = fopen(ruta, "rb+");
            rewind(fileC);
            MBR mbrtemp;
            fread(&mbrtemp, sizeof(MBR), 1, fileC);
            if (fileC != nullptr) {

                for (int i = 0; i < 4; i++) {
                    if (mbrtemp.partition[i].status == '1' && BmontoP == 0) {

                        if (strcmp(mbrtemp.partition[i].name, nombre.c_str()) == 0) {

                            listaM disco;
                            disco = discoMontado(pat);

                            if (disco.id == 500) {
                                agregarDisco(nombreDisco, pat);
                                disco = discoMontado(pat);
                                //cout <<"Se encontro el " << disco.nombreDisco << "," << " se montara la particion." << endl;

                                if (agregarParticion(nombre, disco) == 1) {
                                    cout << "AVISO: Particion: " << nombre << " montada correctamente.\n";
                                } else {
                                    printf("ERROR: La particion no se pudo montar.\n");
                                    printf("AVISO: La particion ya habia sido montada.\n");
                                }
                                BmontoP = 1;
                            } else {
                                //cout  << "bandera xx" << endl;
                                if (particionMontada(nom, disco) == 1) {
                                    printf("AVISO: La particion ya se encuentra montada.\n");
                                    BmontoP = 1;
                                } else {
                                    if (agregarParticion(nombre, disco) == 1) {
                                        cout << "AVISO: Particion: " << nombre << " montada correctamente.\n";
                                    }
                                    BmontoP = 1;
                                }
                            }

                        }

                    }

                }
                if (BmontoP == 0) {
                    printf("AVISO: No se pudo montar la particion.\n");
                } else {
                    mostrarMontados();
                }
                fclose(fileC);
            } else {
                printf("ERROR: La ruta ingresada no referencia ningun disco.\n");
                printf("AVISO: No se pudo montar la particion.\n");
            }
        } else {
            printf("ERROR: No existe niguna particion con el nombre introducido.\n");
            printf("AVISO: No se pudo montar la particion.\n");
        }
    }
}


int verificarNombre(string pa, string nombre){
    char ruta[500];
    char nom[16];
    strcpy(nom,nombre.c_str());
    strcpy(ruta,pa.c_str());

        FILE *fileC;

        fileC= fopen(ruta,"rb+");
        rewind(fileC);
        MBR mbrtemp;
    fread(&mbrtemp,sizeof(MBR),1,fileC);


        if(fileC!=nullptr){

            for (int i = 0; i < 4; i++) {
                if (mbrtemp.partition[i].status == '1') {

                    if (strcmp(mbrtemp.partition[i].name, nom) == 0) {
                        return 1;
                    }
                }
            }

        fclose(fileC);
    }
    return 0;
}

listaM discoMontado(string pat){
    string n = obtenerNombreDisco(pat);
    if(listaMontados.empty() == false){
        for (int i = 0; i < listaMontados.size(); i++) {
            if(listaMontados[i].nombreDisco == n){
                cout << "El disco " << n << " ya esta montado " << endl;
                return listaMontados[i];
            }
        }
    }
    listaM tmp;
    tmp.id = 500;
    cout << "El disco " << n << " aun no esta montado " << endl;
    return tmp;
}

string obtenerNombreDisco(string pat){
    char nombre[500];
    strcpy(nombre,pat.c_str());

    char separador = '/';
    int contador = 0;
    string vector[40];
    string sentencia= pat;

    for(size_t p=0, q=0; p!=sentencia.npos; p=q){
        vector[contador] = sentencia.substr(p+(p!=0),(q=sentencia.find(separador, p+1))-p-(p!=0));
        contador++;
    }

    sentencia = vector[contador-1];
    separador = '.';
    int contador1 = 0;
    string vec[10];
    for(size_t p=0, q=0; p!=sentencia.npos; p=q){
        vec[contador1] = sentencia.substr(p+(p!=0),(q=sentencia.find(separador, p+1))-p-(p!=0));
        contador1++;
    }
    //cout<<"." << vec[0] <<"."<< endl;
    return  vec[0];
}

void agregarDisco(string nombreD, string pat){
    listaM nuevo;
    nuevo.id = contadorA;
    nuevo.ruta = pat;
    nuevo.letra = letras[contadorA];
    contadorA++;
    nuevo.nombreDisco = nombreD;
    nuevo.listaParticionesMontadas.clear();

    listaMontados.push_back(nuevo);
    cout << "Se agrego el disco " << nuevo.nombreDisco << " a la lista de montados" << nuevo.ruta << endl;
}

int agregarParticion(string nomPar, listaM disco){
    //---Buscar el disco
    for (int i = 0; i < listaMontados.size(); i++) {
        if(listaMontados[i].nombreDisco == disco.nombreDisco){
            //--- Si se encuentra ver las particiones montadas
            if(listaMontados[i].listaParticionesMontadas.size() == 0){
                particionM nuevo;
                nuevo.numeroM = 1;
                nuevo.nombreParticion = nomPar;
                listaMontados[i].listaParticionesMontadas.push_back(nuevo);
                //cout << "El disco " << disco.nombreDisco << " no tiene particiones montadas " << endl;
                return  1;
            }
            else{
                particionM nuevo;
                nuevo.numeroM = listaMontados[i].listaParticionesMontadas.size() + 1;
                nuevo.nombreParticion = nomPar;
                listaMontados[i].listaParticionesMontadas.push_back(nuevo);
                return  1;
            }
        }
    }
    return 0;
}




int particionMontada(string nombre, listaM disco){
    //---Buscar el disco
    for (int i = 0; i < listaMontados.size(); i++) {
        if(listaMontados[i].nombreDisco == disco.nombreDisco){
            //--- Si se encuentra ver las particiones montadas
            if(listaMontados[i].listaParticionesMontadas.size() != 0){
                for (int j = 0; j < listaMontados[i].listaParticionesMontadas.size(); j++) {
                    if(listaMontados[i].listaParticionesMontadas[j].nombreParticion == nombre){
                        return 1;
                    }
                }
                //return  1;
            }
        }
    }
    return 0;
}

    void mostrarMontados(){
        //---Buscar el disco
        for (int i = 0; i < listaMontados.size(); i++) {
            //--- Si se encuentra ver las particiones montadas
            for (int j = 0; j < listaMontados[i].listaParticionesMontadas.size(); j++) {
                cout << "D: " << listaMontados[i].nombreDisco << " vd" << listaMontados[i].letra << listaMontados[i].listaParticionesMontadas[j].numeroM << " "
                     << listaMontados[i].ruta << endl;
            }

        }
    }
void desmontarParticion(string idMount){
    cout << "Se va a desmontar : " << idMount << endl;
    bool bandera = false;
    //---- Recorrer Discos
    for (int i = 0; i < listaMontados.size(); i++) {
        //---- Recorrer partciones
        for (int j = 0; j < listaMontados[i].listaParticionesMontadas.size(); j++) {
            string idM = "vd";
            idM += listaMontados[i].letra;
            idM += to_string( listaMontados[i].listaParticionesMontadas[j].numeroM);
            //cout << j << endl;
            if(idM == idMount){
                cout << idM << "," << to_string( listaMontados[i].listaParticionesMontadas[j].numeroM) <<endl;
                //cout << "d" <<  <<endl;
                listaMontados[i].listaParticionesMontadas.erase (listaMontados[i].listaParticionesMontadas.begin()+j);

                bandera = true;
                break;
            }
        }
    }
    if(bandera == true){
        printf("AVISO: Se desmonto correctamente la particion.\n");
        mostrarMontados();

    }else{
        printf("ERROR: No se encontro la particion solicitada.\n");
        cout <<"AVISO: No se pudo desmontar " << idMount <<" la particion.\n";
    }

}