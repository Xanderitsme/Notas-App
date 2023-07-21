#ifndef Archivos_h
#define Archivos_h

#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <string>

using namespace std;

bool crearArchivo(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo.close();
        return true;
    }
    return false;
}

bool guardarVariable(const string& nombreArchivo, const string& contenido) {
    ofstream archivo(nombreArchivo, ios::app);
    if (archivo.is_open()) {
        archivo << contenido << endl;
        archivo.close();
        return true;
    }
    return false;
}

void cargarVariables(const string& nombreArchivo, vector<string>& variables, const int& numVariables) {
    string temp;
    ifstream ifs(nombreArchivo);
    for (int i = 0; i < numVariables; i++){
        getline(ifs, temp);
        variables.push_back(temp);
    }
    ifs.close();
}

void restablecerArchivo(const string& nombreArchivo){
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo << "";
        archivo.close();
    }
}

int cantidadVariables(const string& nombreArchivo){
    int numVariables = 0;
    string linea;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            numVariables++;
        }
        archivo.close();
        return numVariables;
    }
    return 0;
}

bool eliminarArchivo(const string& nombreArchivo) {
    if (remove(nombreArchivo.c_str()) == 0) {
        return true;
    }
    return false;
}

bool existeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    return archivo.good();
}

#endif