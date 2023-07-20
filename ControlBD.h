#ifndef ControlBD_h
#define ControlBD_h

#include <iostream>
#include <vector>
using namespace std;

int archivosCont(vector<string>&);
bool existeCarpeta(const string&);
bool crearCarpeta(const string &);
bool accederCarpeta(const string&);
bool eliminarArchivo(const string&);

void ActualizarBD() {
    const string data = "Data", cuenta = "Cuenta.txt";

    if (!existeCarpeta(data)) {
        crearCarpeta(data);
        return;
    }
    if (!accederCarpeta(data)) {
        return;
    }

    vector<string> archivos;
    int archivosCant = archivosCont(archivos);
    for (const auto& archivoNom : archivos) {
        eliminarArchivo(archivoNom);
    }

    
}

#endif