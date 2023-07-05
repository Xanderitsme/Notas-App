#ifndef Tarea_h
#define Tarea_h
#include <iostream>
#include <chrono>
using namespace std;

class Tarea {
	private:
		string descripcion; 
		bool cumplida; 
		chrono::system_clock::time_point inicio; 
		chrono::system_clock::time_point limite; 
	public:
		Tarea(const string);
		string getDescripcion();
};

Tarea::Tarea(const string descripcion) {
	this->descripcion = descripcion; 
	cumplida = false; 
	chrono::system_clock::time_point inicio = std::chrono::system_clock::now();
}

string Tarea::getDescripcion(){
	return descripcion;
}
#endif