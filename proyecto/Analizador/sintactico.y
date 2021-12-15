
%{
#include <stdio.h>
#include <iostream>
#include <array>
#include <list>
#include <cstring>
#include "../Contenido/funciones.h"
#include "../Contenido/fdisk.h"
#define MAX_DIGITS 10

using namespace std;
std::array<std::string, 11> getDatos();
void cleanEs();

int Pause();
int yylex();
static string fitD = "ff", unitD = "k", pathD = "", typeD="p", nameD="";
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
%token <text> identificador
%token <text> deletep
%token <entrance> ruta
%token <entrance> rutarara




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
	FDISK

;

MKDISK: mkdisk CUERPO_MDISK
;
CUERPO_MDISK: CUERPO_MDISK P_MDISK|
	P_MDISK
	;
P_MDISK:
	 size igual numero unit igual identificador path igual ruta {crearDisco($3,$6,$9); }|
	 path igual ruta unit igual identificador size igual numero  {crearDisco($9,$6,$3); }|
	 size igual numero path igual ruta {crearDisco($3,"m",$6); }
	 ;
RMDISK: rmdisk CUERPO_RMDISK
;

CUERPO_RMDISK: CUERPO_RMDISK P_RMDISK|
	P_RMDISK
	;
P_RMDISK: path igual ruta { eliminarDisco($3); }

FDISK: fdisk CUERPO_FDISK {crearParticion(sizeD, unitD, pathD ,fitD ,typeD, nameD);}
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
		deletep igual identificador {eliminarP(nameD,pathD,$3);}|
		name igual identificador {nameD=$3;}

	 ;
%%

