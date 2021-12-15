#include <iostream>
#include "Analizador/parser.h"
#include "Analizador/scanner.h"
#include <fstream>


using namespace std;
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream;


void analizar(string cadena);



int main()
{
 analizar("Mkdisk -Size~:~3000 -unit~:~K -path~:~/home/gerardo/Documentos/Disco9.disk");
 analizar("Mkdisk -path~:~/home/gerardo/Documentos/Disco1.disk -unit~:~K  -Size~:~ 3000 ");
// analizar("rmdisk -path ~:~ /home/gerardo/Documentos/Disco1.disk");
    analizar("fdisk -type ~:~ e -path ~:~ /home/gerardo/Documentos/Disco9.disk -unit ~:~ k -name~:~ buenas -size ~:~ 30");

// crearDisco(3000, 1,"/home/gerardo/Documentos/Disco1.disk");
//  eliminarDisco("/home/gerardo/Documentos/Disco1.disk");
//  crearParticion("300", "k", "/home/gerardo/Documentos/Disco8.disk" , "ff" , "p" , "elbiho" );
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

