#ifndef Cuenta_h
#define Cuenta_h
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include "Lista.h"
using namespace std;

bool crearCarpeta(const string&);
bool accederCarpeta(const string&);
void volverCarpetaAnt();
bool crearArchivo(const string&);
void guardarVariable(const string&, const string&);
bool eliminarCarpeta(const string&);
int carpetasCont(vector<string>& carpetas);
bool convertirString(const string, int&);
bool existeArchivo(const string&);
void ordenarVector(vector<string>&);
void cargarVariables(const string&, vector<string>&, const int&);
bool cambiarNombreCarpeta(const string&, const string&);

class Cuenta {
	private:
		string nombre, clave; 
		int cantListas, tarCump, tarNoCump; 
		vector<Lista> listas; 
	public:
		Cuenta(const string, const string); 
		bool verificarAcceso(const string, const string);
		bool nombreRegistrado(const string);
		string getNombre();
		int getCantListas();
		void crearLista(const string&);
		void mostrarListas();
		void eliminarLista(const int);
		string nombreLista(const int);
		void mostrarTareas(const int);
		int getCantTareas(const int);
		void crearTarea(const int, const string);
		void actualizarListas();
};

Cuenta::Cuenta(const string nombre, const string clave) {
	this->nombre = nombre; 
	this->clave = clave; 
	cantListas = 0; 
	tarCump = 0; 
	tarNoCump = 0; 
}

bool Cuenta::verificarAcceso(const string nombre, const string clave) {
	if (this->nombre == nombre && this->clave == clave) {
		return true;
	}
	return false;
}

bool Cuenta::nombreRegistrado(const string nombre) {
	if (this->nombre == nombre) {
		return true;
	}
	return false;
}

string Cuenta::getNombre() {
	return nombre;
}

int Cuenta::getCantListas() {
	return cantListas;
}

void Cuenta::crearLista(const string& nombreLista) {
	listas.push_back(Lista(nombreLista));
	crearCarpeta(to_string(listas.size()-1));
	if (accederCarpeta(to_string(listas.size()-1))) {
		crearArchivo("Lista.txt");
		guardarVariable("Lista.txt", nombreLista);
		volverCarpetaAnt();
	}
	cantListas++;
}

void Cuenta::mostrarListas(){
	for (int i = 0; i < listas.size(); i++) {
		cout<<"\t["<< i + 1 <<"]: <"<<listas[i].getNombre()<<"> \n";
	}
	cout<<"\n";
}

void Cuenta::eliminarLista(const int indice){
	if (listas.size() != 0){
		listas.erase(listas.begin() + (indice));
		eliminarCarpeta(to_string(indice));
		for (int i = indice; i < listas.size(); i++) {
			cambiarNombreCarpeta(to_string(i+1), to_string(i));
		}
	}	
	cantListas--;
}

string Cuenta::nombreLista(const int indice) {
	return listas[indice].getNombre();
}

void Cuenta::mostrarTareas(const int indice) {
	listas[indice].mostrarTareas();
}

int Cuenta::getCantTareas(const int indice) {
	return listas[indice].getCantTareas();
}

void Cuenta::crearTarea(const int indice, const string contenido){
	listas[indice].crearTarea(contenido);
}

void Cuenta::actualizarListas() {
	vector<string> carpetas;
	int cantCarpetas = carpetasCont(carpetas), indice;

	for (const string& carpetaNom : carpetas) {
		if (!convertirString(carpetaNom, indice)) {
			eliminarCarpeta(carpetaNom);
		}
		if (accederCarpeta(carpetaNom)) {
			if (!existeArchivo("Lista.txt")) {
				volverCarpetaAnt();
				eliminarCarpeta(carpetaNom);
			} else {
				volverCarpetaAnt();
			}
		}
	}

	vector<string> datos;
	bool newList;
	ordenarVector(carpetas);
	for (const auto& carpeta : carpetas) {
		newList = false;
		if (accederCarpeta(carpeta)) {
			cargarVariables("Lista.txt", datos, 1);
			listas.push_back(Lista(datos[0]));
			datos.erase(datos.begin());
			newList = true;
			volverCarpetaAnt();
		}
		if (newList && carpeta != to_string(listas.size() - 1)) {
			cambiarNombreCarpeta(carpeta, to_string(listas.size() - 1));
		}
	}

	cantListas = listas.size();
}

#endif