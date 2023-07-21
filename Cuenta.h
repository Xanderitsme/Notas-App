#ifndef Cuenta_h
#define Cuenta_h
#include <iostream>
#include <string>
#include <vector>
#include "Lista.h"
using namespace std;

class Cuenta {
	private:
		string usuario, clave;
		vector<Lista> listas;

	public:
		Cuenta(const string&, const string&);
		string getUsuario();
		bool credencialesCorrectas(const string&, const string&);
		
};

Cuenta::Cuenta(const string& usuario, const string& clave) {
	this->usuario = usuario;
	this->clave = clave;
}

string Cuenta::getUsuario() {
	return usuario;
}

bool Cuenta::credencialesCorrectas(const string& usuario, const string& clave) {
	if (usuario == this->usuario && clave == this->clave) {
		return true;
	}
	
	return false;
}

#endif