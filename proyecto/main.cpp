#include <iostream>
#include "Analizador/parser.h"
#include "Analizador/scanner.h"
#include <fstream>
using namespace std;
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream;

//**************** FUNCIONES Y METODOS
void analizar(string cadena);



int main()
{
    analizar("Mkdisk -Size~:~3000 -unit~:~K -path~:~/home/user/Disco1.disk");

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

