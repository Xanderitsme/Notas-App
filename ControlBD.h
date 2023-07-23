#ifndef ControlBD_h
#define ControlBD_h

#include <iostream>
#include <conio.h>
#include <vector>
using namespace std;

int carpetasCont(vector<string>&);
bool existeCarpeta(const string&);
bool crearCarpeta(const string &);
bool accederCarpeta(const string&);
bool cambiarNombreCarpeta(const string&, const string&);
void volverCarpetaAnt();

bool existeArchivo(const string&);
int cantidadVariables(const string&);
void cargarVariables(const string&, vector<string>&, const int&);
void restablecerArchivo(const string&);

bool convertirStringInt(const string&, int&);
void ordenarVector(vector<string>&);
bool datoValido(const string&);

class Cuenta;

bool cuentaRegistrada(vector<Cuenta>&, const string&);

void error() {
    cout << "\tHa ocurrido un error al intentar cargar la base de datos\n";
    cout << "\tPruebe a volver a iniciar el programa\n";
    getch();
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

// Le falta verificar si hay cuentas duplicadas para evitar almacenarlas
// Falta eliminar las carpetas que no son validas
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
    int num, carpetasCant = carpetasCont(carpetas);
    for (auto& carpetaNom : carpetas) {
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

    ordenarVector(nombresInt);
    int ID = 0;
    for (const auto& carpetaNom : nombresInt) {
        cambiarNombreCarpeta(carpetaNom, to_string(ID));
        ID++;
    }

    for (const auto& carpetaNom : nombresString) {
        cambiarNombreCarpeta(carpetaNom, to_string(ID));
        ID++;
    }
}

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
// Falta la actualización de nombres de las carpetas
void actualizarCuentas(vector<Cuenta>& cuentas) {
    string usuario, clave;
    vector<string> carpetas;
    int carpetasCant = carpetasCont(carpetas);
    ordenarVector(carpetas);
    for (const auto& carpetaNom : carpetas) {
        if (cargarDatosCuenta(carpetaNom, usuario, clave)) {
            if (!cuentaRegistrada(cuentas, usuario)) {
                cuentas.push_back(Cuenta(usuario, clave));
            }
        } else {
            cout << "no se pudieron cargar los datos de la cuenta " << carpetaNom << "\n";
            error();
        }
    }
}

bool cambiarUsuarioBD(const string& usuario) {
    const string cuenta = "Cuenta.txt";
    string clave;
    vector<string> datos;
    cargarVariables(cuenta, datos, 2);
    clave = datos[1];
    restablecerArchivo(cuenta);

    if (!guardarVariable(cuenta, usuario)) {
		return false;
	}

	if (!guardarVariable(cuenta, clave)) {
		return false;
	}
    
    return true;
}

bool registroGeneral(const string& nombreArchivo, const int& ID) {
	if (!crearCarpeta(to_string(ID))) {
		return false;
	}

	if (!accederCarpeta(to_string(ID))) {
		return false;
	}

	if (!crearArchivo(nombreArchivo)) {
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
		return false;
	}

	if (!guardarVariable(cuenta, clave)) {
		return false;
	}
	
	volverCarpetaAnt();
	return true;
}

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

void actualizarListas(Cuenta& cuenta) {
    cuenta.eliminarListas();
    string nombreLista;
    vector<string> carpetas;
    int carpetasCant = carpetasCont(carpetas);
    ordenarVector(carpetas);
    for (const auto& carpetaNom : carpetas) {
        if (cargarDatosLista(carpetaNom, nombreLista)) {
            cuenta.crearLista(nombreLista);
        } else {
            // error();
        }
    }
}

bool registrarListaBD(const string& nombreLista, const int& ID) {
    const string lista = "Lista.txt";

    if (!registroGeneral(lista, ID)) {
        return false;
    }

	if (!guardarVariable(lista, nombreLista)) {
		return false;
	}

    volverCarpetaAnt();
	return true;
}

bool registrarTareaBD(const string& contenido, const int& ID) {
    return false;
}

#endif