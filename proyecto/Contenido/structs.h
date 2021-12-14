

#include <iostream>

using namespace std;
string rpath = "/home/gerardo/Documents";
struct Disk {
    string spath;
    int size;
    int unit;
};
struct Partition {
    char status;
    char type;
    char fit;
    int start;
    int size;
    char name[16] = "";
};

struct MBR {
    int size;
    time_t creationDate;
    int diskSignature;
    char fit;
    Partition partition[4];
};

struct EBR {
    char status;
    char fit;
    int size;
    int start;
    int siguiente;
    string name;
};
struct espacio{
    long int inicio;
    long int final;
    char tip;
    string particion;
};



