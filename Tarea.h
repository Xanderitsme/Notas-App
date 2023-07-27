#ifndef Tarea_h
#define Tarea_h
#include <iostream>
#include <chrono>
using namespace std;

class Tarea {
	private:
		string descripcion;
		bool cumplida;

	public:
		Tarea(const string&);
		string getDescripcion();
};

Tarea::Tarea(const string& descripcion) {
	this->descripcion = descripcion;
	cumplida = false;
}

string Tarea::getDescripcion(){
	return descripcion;
}

#endif