
%{
#include <stdio.h>
#include <iostream>
#include <array>
#include <list>
#include <cstring>
#define MAX_DIGITS 10

using namespace std;
std::array<std::string, 11> getDatos();
void cleanEs();
int Pause();
int yylex();
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
%token <text> identificador
%token <text> ruta
%token <text> rutarara




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

CUERPO: MKDISK

;

MKDISK: mkdisk size igual numero unit igual identificador path igual ruta{ cout<< "QUE DICEEEEE" << endl;}
;



%%

