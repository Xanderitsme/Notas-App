#ifndef Cuenta_h
#define Cuenta_h
#include <iostream>
#include <string>
#include <vector>
#include "Lista.h"
using namespace std;

// ALT GR + 4 = ~

bool crearCarpeta(const string &);
bool eliminarCarpeta(const string&);
bool accederCarpeta(const string&);
void volverCarpetaAnt();
bool crearArchivo(const string&);
bool guardarVariable(const string&, const string&);

class Cuenta {
	private:
		string usuario, clave;
		vector<Lista> listas;

	public:
		Cuenta(const string&, const string&);
		string getUsuario();
		bool guardarRegistro(const int&);
		
};

Cuenta::Cuenta(const string& usuario, const string& clave) {
	this->usuario = usuario;
	this->clave = clave;
}

string Cuenta::getUsuario() {
	return usuario;
}

bool Cuenta::guardarRegistro(const int& ID) {
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

#endif