#ifndef ControlBD_h
#define ControlBD_h

#include <iostream>
#include <conio.h>
#include <vector>

#include "Carpetas.h"
#include "Archivos.h"
#include "Cadenas.h"

using namespace std;

class Cuenta;

bool cuentaRegistrada(vector<Cuenta>&, const string&);

void error() {
    system("cls");
    cout << "\tHa ocurrido un error al intentar cargar la base de datos\n";
    cout << "\tPruebe a volver a iniciar el programa\n\n";
    system("pause");
    exit(0);
}

bool carpetaValida(const string& carpetaNom) {
    const string cuenta = "Cuenta.txt";

    if (!accederCarpeta(carpetaNom)) {
        return false;
    }

    if (!existeArchivo(cuenta)) {
        return false;
    }

    if (cantidadVariables(cuenta) != 2) {
        return false;
    }
    
    vector<string> datos;
    cargarVariables(cuenta,datos,2);

    for (const auto& dato : datos) {
        if (!datoValido(dato)) {
            return false;
        }
    }

    volverCarpetaAnt();
    return true;
}

void ActualizarBD() {
    const string data = "Data";

    if (!existeCarpeta(data)) {
        crearCarpeta(data);
        if (!accederCarpeta(data)) {
            error();
        }  
        return;
    }
    if (!accederCarpeta(data)) {
        error();
        return;
    }

    vector<string> carpetas;
    vector<string> nombresInt;
    vector<string> nombresString;
    int num;
    carpetasCont(carpetas);

    for (const auto& carpetaNom : carpetas) {
        if (carpetaValida(carpetaNom)) {
            if (convertirStringInt(carpetaNom, num)) {
                nombresInt.push_back(carpetaNom);
            } else {
                nombresString.push_back(carpetaNom);
            }
        } else {
            volverCarpetaAnt();
            eliminarCarpeta(carpetaNom);
        }
    }

    int ID = 0;

    if (nombresInt.size() != 0) {
        ordenarVector(nombresInt);    
        for (const auto& carpetaNom : nombresInt) {
            cambiarNombreAC(carpetaNom, to_string(ID));
            ID++;
        }
    }

    for (const auto& carpetaNom : nombresString) {
        cambiarNombreAC(carpetaNom, to_string(ID));
        ID++;
    }
}

// Control de la BD para las cuentas

bool cargarDatosCuenta(const string& carpetaNom, string& usuario, string& clave) {
    if (!accederCarpeta(carpetaNom)) {
        return false;
    }

    const string cuenta = "Cuenta.txt";
    vector<string> datos;
    cargarVariables(cuenta, datos, 2);
    usuario = datos[0];
    clave = datos[1];

    volverCarpetaAnt();
    return true;
}
// No se esta llevando un control adecuado de las carpetas con nombre no numerico
void actualizarCuentas(vector<Cuenta>& cuentas) {
    while (cuentas.size() > 0) {
        cuentas.erase(cuentas.begin());
    }

    vector<string> carpetas;
    carpetasCont(carpetas);
    if (carpetas.size() == 0) {
        return;
    }

    string usuario, clave;
    int ID;

    ordenarVector(carpetas);
    for (const auto& carpetaNom : carpetas) {
        ID = cuentas.size();
        if (cargarDatosCuenta(carpetaNom, usuario, clave)) {
            if (!cuentaRegistrada(cuentas, usuario)) {
                cuentas.push_back(Cuenta(usuario, clave));
                if (carpetaNom != to_string(ID)) {
                    cambiarNombreAC(carpetaNom, to_string(ID));
                }

            } else {
                eliminarCarpeta(carpetaNom);
            }

        } else {
            error();
        }
    }
}

bool registroGeneral(const string& nombreArchivo, const int& ID) {
	if (!crearCarpeta(to_string(ID))) {
		return false;
	}

	if (!accederCarpeta(to_string(ID))) {
		return false;
	}

	if (!crearArchivo(nombreArchivo)) {
        volverCarpetaAnt();
		return false;
	}

    return true;
}

bool cambiarDatosCuentaBD(const string& usuario, const string& clave) {
    const string cuenta = "Cuenta.txt";
    
    restablecerArchivo(cuenta);

    if (!guardarVariable(cuenta, usuario)) {
		return false;
	}

	if (!guardarVariable(cuenta, clave)) {
		return false;
	}
    
    return true;
}

bool registrarCuentaBD(const string& usuario, const string& clave, const int& ID) {
	const string cuenta = "Cuenta.txt";

    if (!registroGeneral(cuenta, ID)) {
        return false;
    }

	if (!guardarVariable(cuenta, usuario)) {
        volverCarpetaAnt();
		return false;
	}

	if (!guardarVariable(cuenta, clave)) {
        volverCarpetaAnt();
		return false;
	}
	
	volverCarpetaAnt();
	return true;
}

bool eliminarCuentaBD(const int& ID) {
    volverCarpetaAnt();
    if (!eliminarCarpeta(to_string(ID))) {
        return false;
    }

    return true;
}

// Control de la BD para las listas

bool cargarDatosLista(const string& carpetaNom, string& nombreLista) {
    if (!accederCarpeta(carpetaNom)) {
        return false;
    }

    const string lista = "Lista.txt";
    vector<string> datos;
    cargarVariables(lista, datos, 1);
    nombreLista = datos[0];

    volverCarpetaAnt();
    return true;
}
// No se esta llevando un control adecuado de las carpetas con nombre no numerico
void actualizarListas(Cuenta& cuenta) {
    cuenta.eliminarListas();

    vector<string> carpetas;
    carpetasCont(carpetas);
    if (carpetas.size() == 0) {
        return;
    }

    string nombreLista;
    int listaID;

    ordenarVector(carpetas);
    for (const auto& carpetaNom : carpetas) {
        listaID = cuenta.getCantListas();
        if (cargarDatosLista(carpetaNom, nombreLista)) {
            cuenta.crearLista(nombreLista);
            if (carpetaNom != to_string(listaID)) {
                cambiarNombreAC(carpetaNom, to_string(listaID));
            }
            
        } else {
            error();
        }
    }
}

bool cambiarDatosListaBD(const string& nombreLista, const int& listaID) {
    const string lista = "Lista.txt";

    restablecerArchivo(lista);

    if (!guardarVariable(lista, nombreLista)) {
		return false;
	}

	return true;
}

bool registrarListaBD(const string& nombreLista, const int& listaID) {
    const string lista = "Lista.txt";

    if (!registroGeneral(lista, listaID)) {
        return false;
    }

	if (!guardarVariable(lista, nombreLista)) {
        volverCarpetaAnt();
		return false;
	}

    volverCarpetaAnt();
	return true;
}

bool eliminarListaBD(const int& listaID) {
    volverCarpetaAnt();
    if (!eliminarCarpeta(to_string(listaID))) {
        return false;
    }

    return true;
}

bool vaciarListaBD() {
    const string lista = "Lista.txt";
    vector<string> archivos;
    archivosCont(archivos);

    for (const auto& archivoNom : archivos) {
        if (archivoNom != lista) {
            if (!eliminarArchivo(archivoNom)) {
                return false;
            }
        }
    }

    return true;
}

// Control de la BD para las tareas

bool cargarDatosTarea(const string& archivoNom, string& descripcion) {
    vector<string> datos;
    cargarVariables(archivoNom, datos, 1);
    descripcion = datos[0];

    return true;
}

void estandarizarArchivos(vector<string>& archivos) {
    const string lista = "Lista.txt";
    int indice = 0;

    for (const auto& archivoNom : archivos) {
        if (archivoNom == lista) {
            archivos.erase(archivos.begin() + indice);
            break;
        }

        indice++;
    }

    recortarExtension(archivos);
}
// No se esta llevando un control adecuado de los archivos con nombre no numerico
void actualizarTareas(Cuenta& cuenta, const int& listaID) {
    cuenta.eliminarTareas(listaID);

    vector<string> archivos;
    archivosCont(archivos);
    estandarizarArchivos(archivos);

    if (archivos.size() == 0) {
        return;
    }

    ordenarVector(archivos);
    agregarExtensionTXT(archivos);

    const string extension = ".txt";
    string descripcion;
    int tareaID;

    for (const auto& archivoNom : archivos) {
        tareaID = cuenta.getCantTareas(listaID);
        if (cargarDatosTarea(archivoNom, descripcion)) {
            cuenta.crearTarea(listaID, descripcion);
            if (archivoNom != to_string(tareaID) + extension) {
                cambiarNombreAC(archivoNom, to_string(tareaID) + extension);
            }
        } else {
            error();
        }
    }
}

bool cambiarDatosTareaBD(string& descripcion, const int& tareaID) {
    const string extension = ".txt";

    restablecerArchivo(to_string(tareaID) + extension);

    if (!guardarVariable(to_string(tareaID) + extension, descripcion)) {
        return false;
    }
    
    return true;
}

bool registrarTareaBD(const string& descripcion, const int& tareaID) {
    const string extension = ".txt";

    if (!crearArchivo(to_string(tareaID) + extension)) {
        return false;
    }

    if (!guardarVariable(to_string(tareaID) + extension, descripcion)) {
        return false;
    }
    
    return true;
}

bool eliminarTareaBD(const int& tareaID) {
    const string extension = ".txt";

    if (!eliminarArchivo(to_string(tareaID) + extension)) {
        return false;
    }

    return true;
}

bool transferirTareaBD(const int& listOrigID, const int& listDestID, const int& tareaID) {
    const string extension = ".txt";
    string descripcion;

    if (!cargarDatosTarea(to_string(tareaID) + extension, descripcion)) {
        return false;
    }

    volverCarpetaAnt();
    
    if (!accederCarpeta(to_string(listDestID))) {
        return false;
    }

    vector<string> archivos;
    int tareaTransferidaID = archivosCont(archivos) - 1;

    if (!registrarTareaBD(descripcion, tareaTransferidaID)) {
        return false;
    }

    volverCarpetaAnt();

    if (!accederCarpeta(to_string(listOrigID))) {
        return false;
    }

    if (!eliminarArchivo(to_string(tareaID) + extension)) {
        return false;
    }
    
    return true;
}

// Algunas funciones extras para el control de la BD de las listas

bool copiarLista(const int& listID_origen, const int& listID_destino) {
    if (!accederCarpeta(to_string(listID_origen))) {
        return false;
    }

    vector<string> archivos;
    archivosCont(archivos);
    estandarizarArchivos(archivos);
    ordenarVector(archivos);
    agregarExtensionTXT(archivos);

    vector<string> descripciones;
    string descripcionTemp;

    for (const auto& archivoNom : archivos) {
        if (!cargarDatosTarea(archivoNom, descripcionTemp)) {
            return false;
        }

        descripciones.push_back(descripcionTemp);
    }

    volverCarpetaAnt();

    if (!accederCarpeta(to_string(listID_destino))) {
        return false;
    }

    while (archivos.size() > 0) {
        archivos.erase(archivos.begin());
    }

    int tareaID = archivosCont(archivos) - 1;

    for (const auto& descripcion : descripciones) {
        if (!registrarTareaBD(descripcion, tareaID)) {
            return false;
        }

        tareaID++;
    }

    volverCarpetaAnt();
    return true;
}

bool combinarListasBD(const string& nombreLista, const int& listaID, const vector<int>& listasSeleccionadas) {
    if (!registrarListaBD(nombreLista, listaID)) {
        return false;
    }

    for (const auto& listSelID : listasSeleccionadas) {
        if (!copiarLista(listSelID, listaID)) {
            return false;
        }

        if (!eliminarCarpeta(to_string(listSelID))) {
            return false;
        }
    }

    return true;
}

#endif