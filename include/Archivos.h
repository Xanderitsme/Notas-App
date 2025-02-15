#ifndef Archivos_h
#define Archivos_h

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#ifdef _WIN32
    #include <codecvt>
    #include <locale>
#endif

using namespace std;
namespace fs = filesystem;

#ifdef _WIN32
    bool crearArchivo(const string& nombreArchivo) {
        wofstream archivo(nombreArchivo);
        archivo.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
        if (archivo.is_open()) {
            archivo.close();
            return true;
        }
        return false;
    }
#else
    bool crearArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            archivo.close();
            return true;
        }
        return false;
    }
#endif

bool guardarVariable(const string& nombreArchivo, const string& contenido) {
    try {
        ofstream archivo(nombreArchivo, ios::app);
        if (!archivo.is_open()) {
            return false;
        }
        archivo << contenido << endl;
        archivo.close();
        return true;
    } catch (const exception& e) {
        return false;
    }
}

void cargarVariables(const string& nombreArchivo, vector<string>& variables, const int& numVariables) {
    ifstream ifs(nombreArchivo);
    if (!ifs.is_open()) {
        return;
    }
    string temp;
    variables.reserve(numVariables);
    for (int i = 0; i < numVariables && getline(ifs, temp); i++) {
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
        if (!fs::exists(nombreArchivo)) {
            return false;
        }
        return fs::remove(nombreArchivo);
    } catch (const fs::filesystem_error& e) {
        return false;
    }
}

bool existeArchivo(const string& nombreArchivo) {
    if (fs::exists(nombreArchivo)) {
        return true;
    }
    return false;
}

#endif