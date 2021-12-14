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
IDENTIFICADOR ([a-zA-Z])+
WHITESPACE [\t\r\f" "]
DISK  "disk"
ARCHIVO {IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO})*"."{DISK}
SLASH       "/"
RUTA        {SLASH}({IDENTIFICADOR}({IDENTIFICADOR}|{ENTERO}|{NEGATIVO}|{DECIMAL})*{SLASH})+{ARCHIVO}
RUTARARA    \"{SLASH}({IDENTIFICADOR}|{ENTERO}|{SLASH}|" ")+{ARCHIVO}\"

MKDISK      "mkdisk"
RMDISK      "rmdisk"
FDISK       "fdisk"
MOUNT       "mount"
UNMOUNT     "unmount"
MKFS        "mkfs"

SIZE        "-size"
FIT         "-fit"
UNIT        "-unit"
PATH        "-path"

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

{RUTA} {strcpy(yylval.text, yytext); return(ruta);}
{RUTARARA} {strcpy(yylval.text, yytext); return(rutarara);}
{IGUAL} {return(igual);}
{IDENTIFICADOR} {strcpy(yylval.text, yytext); return(identificador);}
{WHITESPACE}           {}
{COMENTARIO} {strcpy(yylval.text, yytext); return(comentario);}
.               {printf("Caracter %s no admitido\n ", yytext);}

%%