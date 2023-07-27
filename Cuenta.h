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
		bool credencialesCorrectas(const string&, const string&);
		string getUsuario();
		void cambiarUsuario(const string&);
		void cambiarClave(const string&);

		void crearLista(const string&);
		void eliminarListas();
		void mostrarListas();
		int getCantListas();
		string getNombreLista(const int&);
		
		void crearTarea(const int&, const string&);
		void eliminarTareas(const int&);
		void mostrarTareas(const int&);
		int getCantTareas(const int&);
		string getDescripcionTarea(const int&, const int&);
};

Cuenta::Cuenta(const string& usuario, const string& clave) {
	this->usuario = usuario;
	this->clave = clave;
}

bool Cuenta::credencialesCorrectas(const string& usuario, const string& clave) {
	if (usuario == this->usuario && clave == this->clave) {
		return true;
	}
	
	return false;
}

string Cuenta::getUsuario() {
	return usuario;

}

void Cuenta::cambiarUsuario(const string& usuario) {
	this->usuario = usuario;
}

void Cuenta::cambiarClave(const string& clave) {
	this->clave = clave;
}

void Cuenta::crearLista(const string& nombreLista) {
	listas.push_back(Lista(nombreLista));
}

void Cuenta::eliminarListas() {
	while(listas.size() > 0) {
		listas.erase(listas.begin());
	}
}

void Cuenta::mostrarListas() {
	const string fin = "...";
	const int limiteNombre = 70;
	int indice = 1;
	string nombreRecortado;

	for (auto& lista : listas) {
		cout << "\t[" << indice << "]: <";

		if (lista.getNombre().size() > limiteNombre) {
			nombreRecortado = lista.getNombre().substr(0, limiteNombre - fin.size());
			cout << nombreRecortado << fin;
		} else {
			cout << lista.getNombre();
		}

		cout << ">\n";
		indice++;
	}
}

int Cuenta::getCantListas() {
	return listas.size();
}

string Cuenta::getNombreLista(const int& listaID) {
	return listas[listaID].getNombre();
}

void Cuenta::crearTarea(const int& listaID, const string& descripcion) {
	listas[listaID].crearTarea(descripcion);
}

void Cuenta::eliminarTareas(const int& listaID) {
	listas[listaID].eliminarTareas();
}

void Cuenta::mostrarTareas(const int& listaID) {
	listas[listaID].mostrarTareas();
}

int Cuenta::getCantTareas(const int& listaID) {
	return listas[listaID].getCantTareas();
}

string Cuenta::getDescripcionTarea(const int& listaID, const int& tareaID) {
	return listas[listaID].getDescripcionTarea(tareaID);
}

#endif