
%{
#include <stdio.h>
#include <iostream>
#include <array>
#include <list>
#include <cstring>
#include "../Contenido/funciones.h"
#include "../Contenido/fdisk.h"
#include "../Contenido/mount.h"
#define MAX_DIGITS 10

using namespace std;
std::array<std::string, 11> getDatos();
void cleanEs();

int Pause();
int yylex();
static string fitD = "ff", unitD = "k", pathD = "", typeD="p", nameD="", idMountD="",bandera="",deleteD="";
static int sizeD = 0;

int yyerror(const char* msg){
        cout<< "ERROR SINTACTICO" << msg << endl;
        return 0;
}

%}

%define parse.error verbose

%token <text> comentario

%token <text> mkdisk
%token <text> rmdisk
%token <text> fdisk
%token <text> mount
%token <text> unmount
%token <text> mkfs
%token <text> size
%token <text> fit
%token <text> unit
%token <text> path
%token <text> igual
%token <text> type
%token <text> name
%token <text> comillas
%token <text> identificador
%token <text> deletep
%token <text> id
%token <entrance> ruta
%token <text> rep
%token <text> path2




%token <number>numero


%start START
%union{
    int number;
    char text[200];
    char entrance[12];
    int other;
}

%%

START: START CUERPO
        |CUERPO
;

CUERPO: MKDISK|
	RMDISK|
	FDISK|
	MOUNT|
	REP

;

MKDISK: mkdisk CUERPO_MDISK {crearDisco(sizeD,unitD,pathD); }
;
CUERPO_MDISK: CUERPO_MDISK P_MDISK|
	P_MDISK
	;
P_MDISK:
	 size igual numero {sizeD=$3;}|
	 unit igual identificador {unitD=$3;}|
	 path igual ruta {pathD=$3;}
	 ;
RMDISK: rmdisk CUERPO_RMDISK
;

CUERPO_RMDISK: CUERPO_RMDISK P_RMDISK|
	P_RMDISK
	;
P_RMDISK: path igual ruta { eliminarDisco($3); }

FDISK: fdisk CUERPO_FDISK {
		  if(bandera != "jeje"){
			crearParticion(sizeD, unitD, pathD ,fitD ,typeD, nameD);
		  }else{
		  	eliminarP(nameD,pathD,deleteD);
		  }
			}

;
CUERPO_FDISK: CUERPO_FDISK P_FDISK |
	P_FDISK
	;
P_FDISK:
		size igual numero {sizeD=$3;}|
		unit igual identificador {unitD=$3;}|
		path igual ruta {pathD=$3;}|
		type igual identificador {typeD=$3;}|
		fit igual identificador {fitD=$3;}|
		deletep igual identificador {bandera="jeje"; deleteD=$3 ;}|
		name igual identificador {nameD=$3;}

	 ;
MOUNT: mount CUERPO_MOUNT {montarParticion(pathD,nameD);}|
	unmount CUERPO_MOUNT {desmontarParticion(idMountD);}
;
CUERPO_MOUNT: CUERPO_MOUNT P_MOUNT |
	P_MOUNT
	;
P_MOUNT:
		path igual ruta {pathD=$3;}|
		name igual identificador {nameD=$3;}|
		id igual identificador  { idMountD=$3; }
	 ;
REP: rep CUERPO_REP {reportes(pathD, nameD, idMountD);}
;
CUERPO_REP: CUERPO_REP P_REP |
	P_REP
	;
P_REP:
		path igual ruta {pathD=$3;}|
		id igual identificador  { idMountD=$3; }|
		name igual identificador {nameD=$3;}|
		path2 igual ruta {pathD=$3;}
	 ;

%%

