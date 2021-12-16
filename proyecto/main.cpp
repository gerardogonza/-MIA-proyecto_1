#include <iostream>
#include "Analizador/parser.h"
#include "Analizador/scanner.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>


using namespace std;
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream;

using std::cin;
using std::string;
using std::cout; using std::cin;
using std::endl; using std::vector;
using std::copy;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;
void analizar(string cadena);



int main()
{
    cout << "*Bienvenido a mi Proyecto 1 de MIA*"<<endl;
    cout << "* Gerardo Steve Munoz Contreras  *"<<endl;
    cout << "*           201900853            *"<<endl;
    cout << "                                 "<<endl;

    cout << "1  - Para Ingresar Comando por Comando" <<endl;
    cout << "2  - Para leer un archivo .sh con Comandos"<<endl;
    cout << "Ingrese Opcion solo numeros enteros: " ;
    int opcion = 0;
    cin >> opcion;

    if (opcion==1){
        string p;
        cout << "escribe tu comando:" ;
        cin >> p;
        while (p != "s") {
            analizar(p);
            cout << "escribe tu comando (si desesas salir escribe \"s\"):" ;
            cin >> p;
        }
    }
    if (opcion==2){
        string p;
        cout << "INGRESA RUTA DEL ARCHIVO DE ENTRADA" ;
//        cin >> p;
        ifstream archivo_entrada("/home/gerardo/Documentos/archivo.txt");
        string linea;

        while(getline(archivo_entrada, linea)) {
            if(linea !="pause"){

                analizar(linea);
            }else{
                string p;
                cout << "ingresa cuaquier letra pra continuar" ;
                cin >> p;
            }
        }

    }






// analizar("Mkdisk -Size~:~3000 -unit~:~K -path~:~/home/gerardo/Documentos/Disco9.disk");
// analizar("Mkdisk -path~:~/home/gerardo/Documentos/Disco1.disk -unit~:~K  -Size~:~ 3000 ");
// analizar("rmdisk -path ~:~ /home/gerardo/Documentos/Disco1.disk");
//    analizar("fdisk -type ~:~ e -path ~:~ /home/gerardo/Documentos/Disco9.disk -unit ~:~ k -name~:~ buenas -size ~:~ 30");
//    analizar("fdisk -delete ~:~ fast -name ~:~ buenas -path~:~/home/gerardo/Documentos/Disco9.disk");
  // analizar("mount -path~:~/home/gerardo/Documentos/Disco9.disk -name ~:~ mamadas" );
  //  analizar("unmount -id~:~vda1" );
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
        cout<<"Hay uno error"<< endl;
    }
}

