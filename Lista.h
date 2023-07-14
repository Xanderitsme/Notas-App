#ifndef Lista_h
#define Lista_h
#include <iostream>
#include <chrono>
#include <vector>
#include "Nota.h"
using namespace std;

class Lista {
	private:
		string nombre; 
		vector<Nota> notas; 
		int cantNotas;
	public:
		Lista(const string); 
		string getNombre();
		void crearNota(const string);
		void mostrarNotas();
		int getCantNotas();
};

Lista::Lista(const string nombre) {
	this->nombre = nombre; 
	cantNotas = 0;
}

string Lista::getNombre(){
	return nombre;
}

void Lista::crearNota(const string contenido) {
	notas.push_back(Nota(contenido));
	cantNotas++;
}

void Lista::mostrarNotas(){
	int indice = 1;
	for (auto& nota : notas){
		cout<<"\t["<< indice <<"]: <"<<nota.getDescripcion()<<"> \n";
		indice++;
	}
	cout<<"\n";
}

int Lista::getCantNotas(){
	return cantNotas;
}

#endif