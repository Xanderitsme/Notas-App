#ifndef Carpetas_h
#define Carpetas_h

#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = filesystem;

// Funciones para manipular carpetas y directorios

bool crearCarpeta(const string& nombre) {
    if (fs::create_directory(nombre.c_str())) {
        return true;
    }
    return false;
}

bool accederCarpeta(const std::string& nombre) {
    fs::path DirNuevo = fs::absolute(nombre);
    fs::current_path(DirNuevo);
    fs::path DirActual = fs::current_path();
    if (DirNuevo == DirActual) {
        return true;
    }
    return false;
}

void volverCarpetaAnt() {
    fs::path nuevoDirectorio = fs::current_path();
    fs::current_path(nuevoDirectorio.parent_path());
}
// Modifica los nombres de las carpetas o archivos
// Primer parametro: Nombre actual
// Segundo parametro: Nuevo nombre
bool cambiarNombreAC(const string& nombreActual, const string& nombreNuevo) {
    try {
        fs::rename(nombreActual, nombreNuevo);
    } catch (const fs::filesystem_error& e) {
        return false;
    } 
    return true;
}

int carpetasCont(vector<string>& carpetas) {
    string currentPath = fs::current_path().string();
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        if (fs::is_directory(entry)) {
            carpetas.push_back(entry.path().filename().string());
        }
    }
    return carpetas.size();
}

int archivosCont(vector<string>& archivos) {
    string currentPath = fs::current_path().string();
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        if (fs::is_regular_file(entry)) {
            archivos.push_back(entry.path().filename().string());
        }
    }
    return archivos.size();
}

bool eliminarCarpeta(const string& nombre) {
    if (fs::remove_all(nombre.c_str())) {
        return true;
    }
    return false;
}

bool existeCarpeta(const string& carpetaNom) {
    if (fs::exists(carpetaNom)) {
        return true;
    }
    return false;
}

#endif