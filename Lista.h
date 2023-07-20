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
		int cantTareas;
	public:
		Lista(const string); 
		string getNombre();
		void crearTarea(const string);
		void mostrarTareas();
		int getCantTareas();
};

Lista::Lista(const string nombre) {
	this->nombre = nombre; 
	cantTareas = 0;
}

string Lista::getNombre(){
	return nombre;
}

void Lista::crearTarea(const string contenido) {
	tareas.push_back(Tarea(contenido));
	cantTareas++;
}

void Lista::mostrarTareas(){
	int indice = 1;
	for (auto& tarea : tareas){
		cout<<"\t["<< indice <<"]: <"<<tarea.getDescripcion()<<"> \n";
		indice++;
	}
	cout<<"\n";
}

int Lista::getCantTareas(){
	return cantTareas;
}

#endif