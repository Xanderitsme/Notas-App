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
		void editarDescripcion(const string&);
		bool tareaCumplida();
		void cambiarEstado();
};

Tarea::Tarea(const string& descripcion) {
	this->descripcion = descripcion;
	cumplida = false;
}

string Tarea::getDescripcion(){
	return descripcion;
}

void Tarea::editarDescripcion(const string& descripcion) {
	this->descripcion = descripcion;
}

bool Tarea::tareaCumplida() {
	return cumplida;
}

void Tarea::cambiarEstado() {
	this->cumplida = !cumplida;
}

#endif