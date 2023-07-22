#ifndef ControlBD_h
#define ControlBD_h

#include <iostream>
#include <vector>
using namespace std;

int carpetasCont(vector<string>&);
bool existeCarpeta(const string&);
bool crearCarpeta(const string &);
bool accederCarpeta(const string&);
bool cambiarNombreCarpeta(const string&, const string&);

bool existeArchivo(const string&);

bool convertirStringInt(const string&, int&);
void ordenarVector(vector<string>&);

bool cuentaRegistrada(vector<Cuenta>&, const string&);

class Cuenta;

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
void ActualizarBD() {
    const string data = "Data";

    if (!existeCarpeta(data)) {
        crearCarpeta(data);
        return;
    }
    if (!accederCarpeta(data)) {
        return;
    }

    vector<string> carpetas;
    vector<string> nombresInt;
    vector<string> nombresString;
    int num, carpetasCant = carpetasCont(carpetas);
    for (const auto& carpetaNom : carpetas) {
        if (carpetaValida(carpetaNom)) {
            if (convertirStringInt(carpetaNom, num)) {
                nombresInt.push_back(carpetaNom);
            } else {
                nombresString.push_back(carpetaNom);
            }
        } else {
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

void ActualizarCuentas(vector<Cuenta>& cuentas) {
    string usuario, clave;
    vector<string> carpetas;
    int carpetasCant = carpetasCont(carpetas);
    for (const auto& carpetaNom : carpetas) {
        if (cargarDatosCuenta(carpetaNom, usuario, clave)) {
            if (!cuentaRegistrada(cuentas, usuario)) {
                cuentas.push_back(Cuenta(usuario, clave));
            }
        } else {
            // pendiente
        }
    }
}

bool registrarCuentaBD(const string& usuario, const string& clave, const int& ID) {
	const string cuenta = "Cuenta.txt";

	if (!crearCarpeta(to_string(ID))) {
		return false;
	}

	if (!accederCarpeta(to_string(ID))) {
		return false;
	}

	if (!crearArchivo(cuenta)) {
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

bool registrarListaBD(const string& nombreLista, const int& ID) {
    return false;
}

#endif