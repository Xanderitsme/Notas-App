#ifndef Archivos_h
#define Archivos_h

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
namespace fs = filesystem;

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

void restablecerArchivo(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        archivo << "";
        archivo.close();
    }
}

int cantidadVariables(const string& nombreArchivo) {
    int numVariables = 0;
    string linea;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            numVariables++;
        }
        archivo.close();
    }
    return numVariables;
}

bool eliminarArchivo(const string& nombreArchivo) {
    try {
        fs::remove(nombreArchivo);
    } catch (const std::filesystem::filesystem_error& e) {
        return false;
    }
    return true;
}

bool existeArchivo(const string& nombreArchivo) {
    if (fs::exists(nombreArchivo)) {
        return true;
    }
    return false;
}

#endif