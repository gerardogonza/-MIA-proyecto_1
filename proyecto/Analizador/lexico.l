%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <iostream>
    #include "parser.h"
    using namespace std;
%}

%option case-insensitive
%option noyywrap

DECIMAL [0-9]+("."[0-9]+)+
ENTERO [0-9]+
MENOS "-"
NEGATIVO {MENOS}({ENTERO}|{DECIMAL})
IDENTIFICADOR ([a-zA-Z])([a-zA-Z0-9_])*
WHITESPACE [\t\r\f" "]
DISK  "disk"
PNG   "png"
JPG    "jpg"
PDF    "pdf"
HMTL   "html"
ARCHIVO {IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO})*"."({DISK}|{PNG}|{JPG}|{PDF}|{HMTL})
SLASH       "/"
RUTA        {SLASH}({IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO}|{NEGATIVO}|{DECIMAL})*{SLASH})+{ARCHIVO}
COMILLA "\""

MKDISK      "mkdisk"
RMDISK      "rmdisk"
FDISK       "fdisk"
MOUNT       "mount"
UNMOUNT     "unmount"
MKFS        "mkfs"
REP         "rep"


SIZE        "-size"
FIT         "-fit"
UNIT        "-unit"
PATH        "-path"
PATH2       "-ruta"
TYPE        "-type"
NAME        "-name"
DELETE      "-delete"
ID          "-id"

IGUAL       "~:~"
COMENTARIO      "#"[^\r\n]*[^\r\n]


%%
{ENTERO} {yylval.number = atoi(yytext); return numero;}
{DECIMAL} {yylval.number = atoi(yytext); return numero;}
{NEGATIVO} {yylval.number = atoi(yytext); return numero;}

{MKDISK} {return(mkdisk);}
{RMDISK} {return(rmdisk);}
{FDISK} {return(fdisk);}
{MOUNT} {return(mount);}
{UNMOUNT} {return(unmount);}
{MKFS} {return(mkfs);}

{SIZE} {return(size);}
{FIT} {return(fit);}
{UNIT} {return(unit);}
{PATH} {return(path);}
{TYPE} {return(type);}
{NAME} {return(name);}
{DELETE} {return(deletep);}
{ID} {return(id);}
{COMILLA} {}
{REP} {return(rep);}
{PATH2} {return(path2);}

{RUTA} {strcpy(yylval.text, yytext); return(ruta);}
{IGUAL} {return(igual);}
{IDENTIFICADOR} {strcpy(yylval.text, yytext); return(identificador);}
{WHITESPACE}           {}
{COMENTARIO} {strcpy(yylval.text, yytext); return(comentario);}
.               {printf("Caracter %s no admitido\n ", yytext);}

%%