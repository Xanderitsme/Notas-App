#ifndef Nota_h
#define Nota_h
#include <iostream>
#include <chrono>
using namespace std;

class Nota {
	private:
		string descripcion; 
		bool cumplida; 
		chrono::system_clock::time_point inicio; 
		chrono::system_clock::time_point limite; 
	public:
		Nota(const string);
		string getDescripcion();
};

Nota::Nota(const string descripcion) {
	this->descripcion = descripcion; 
	cumplida = false; 
	chrono::system_clock::time_point inicio = std::chrono::system_clock::now();
}

string Nota::getDescripcion(){
	return descripcion;
}
#endif