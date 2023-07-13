#ifndef Carpetas_h
#define Carpetas_h

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <filesystem>

#include <vector>
#include <string>


#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#define rmdir(path) _rmdir(path)

using namespace std;
namespace fs = filesystem;

//FUNCIONES PARA MANIPULAR CARPETAS Y DIRECTORIOS

bool crearCarpeta(const string& nombre) {
    if (mkdir(nombre.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        return true;
    }
    return false;
}

bool accederCarpeta(const string& nombre) {
    if (chdir(nombre.c_str()) == 0) {
        return true;
    }
    return false;    
}

void volverCarpetaAnt() {
    chdir("..");
}

bool cambiarNombreCarpeta(const string& nombreActual, const string& nombreNuevo) {
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
    if (rmdir(nombre.c_str()) == 0) {
        return true;
    }
    if (accederCarpeta(nombre)) {
        vector<string> archivos;
        int archivosCant = archivosCont(archivos);
        for (const auto& archivoNom : archivos) {
            remove(archivoNom.c_str());
        }

        vector<string> carpetas;
        int carpetasCant = carpetasCont(carpetas);
        carpetasCant = carpetasCont(carpetas);
        for (const auto& carpetaNom : carpetas) {
            eliminarCarpeta(carpetaNom);
        }

        volverCarpetaAnt();
    }

    if (rmdir(nombre.c_str()) == 0) {
        return true;
    }
    return false;
}

#endif