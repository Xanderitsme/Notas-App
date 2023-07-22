#ifndef Lista_h
#define Lista_h
#include <iostream>
#include <chrono>
#include <vector>
#include "Tarea.h"
using namespace std;

class Lista {
	private:
		string nombre;
		vector<Tarea> tareas;

	public:
		Lista(const string);
		string getNombre();
};

Lista::Lista(const string nombre) {
	this->nombre = nombre;
}

string Lista::getNombre() {
	return nombre;
}

#endif