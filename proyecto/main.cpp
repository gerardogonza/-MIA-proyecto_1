#include <iostream>
#include "Analizador/parser.h"
#include "Analizador/scanner.h"
#include <fstream>
#include "Contenido/funciones.h"
#include "Contenido/fdisk.h"
using namespace std;
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream;


void analizar(string cadena);



int main()
{
 analizar("Mkdisk -Size~:~3000 -unit~:~K -path~:~/home/gerardo/Documentos/Disco1.disk");
//  crearDisco(3000, 1,"/home/gerardo/Documentos/Disco1.disk");
//  eliminarDisco("/home/gerardo/Documentos/Disco1.disk");
  crearParticion("3000", "k", "/home/gerardo/Documentos/Disco1.disk" , "ff" , "p" , "elbicho" );
    return 0;

}

void analizar(string cadena){
    YY_BUFFER_STATE buffer = yy_scan_string(cadena.c_str());

   if(yyparse()==0){

        cout<<"siu"<< endl;
    } else{
        cout<<"nouu"<< endl;
    }
}

