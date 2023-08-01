#ifndef Lista_h
#define Lista_h
#include <iostream>
#include <chrono>
#include <vector>
#include "Tarea.h"
using namespace std;

class Lista {
	private:
		string nombreLista;
		vector<Tarea> tareas, tareasC;

	public:
		Lista(const string&);
		string getNombre();
		void renombrarLista(const string&);

		void crearTarea(const string&);
		void eliminarTareas();
		void mostrarTareas();
		int getCantTareas();
		string getDescripcionTarea(const int&);
		void mostrarTareasC();
		void editarTarea(const int&, const string&);

		void crearTareaC(const string&);
		void eliminarTareasC();
		int getCantTareasC();
		string getDescripcionTareaC(const int&);
		
};

Lista::Lista(const string& nombreLista) {
	this->nombreLista = nombreLista;
}

string Lista::getNombre() {
	return nombreLista;
}

void Lista::renombrarLista(const string& nombreLista){
	this->nombreLista = nombreLista;
}

void Lista::crearTarea(const string& descripcion) {
	tareas.push_back(Tarea(descripcion));
}

void Lista::eliminarTareas() {
	while (tareas.size() > 0) {
		tareas.erase(tareas.begin());
	}
}

void Lista::mostrarTareas() {
	const string fin = "...";
	const int limiteDescripcion = 70;
	int indice = 1;
	string descRecortada;

	for (auto& tarea : tareas) {
		cout << "\t[" << indice << "]: <";

		if (tarea.getDescripcion().size() > limiteDescripcion) {
			descRecortada = tarea.getDescripcion().substr(0, limiteDescripcion - fin.size());
			cout << descRecortada << fin;
		} else {
			cout << tarea.getDescripcion();
		}

		cout << ">\n";
		indice++;
	}
}

int Lista::getCantTareas() {
	return tareas.size();
}

string Lista::getDescripcionTarea(const int& tareaID) {
	return tareas[tareaID].getDescripcion();
}

void Lista::editarTarea(const int& tareaID, const string& descripcion) {
	tareas[tareaID].editarDescripcion(descripcion);
}

void Lista::crearTareaC(const string& descripcion) {
	tareasC.push_back(Tarea(descripcion));
	tareasC[tareasC.size() - 1].cambiarEstado();
}

void Lista::mostrarTareasC() {
	const string fin = "...";
	const int limiteDescripcion = 70;
	int indice = 1;
	string descRecortada;

	for (auto& tareaC : tareasC) {
		cout << "\t[" << indice << "]: <";

		if (tareaC.getDescripcion().size() > limiteDescripcion) {
			descRecortada = tareaC.getDescripcion().substr(0, limiteDescripcion - fin.size());
			cout << descRecortada << fin;
		} else {
			cout << tareaC.getDescripcion();
		}

		cout << ">\n";
		indice++;
	}
}

void Lista::eliminarTareasC() {
	while (tareasC.size() > 0) {
		tareasC.erase(tareasC.begin());
	}
}

int Lista::getCantTareasC() {
	return tareasC.size();
}

string Lista::getDescripcionTareaC(const int& tareaCID) {
	return tareasC[tareaCID].getDescripcion();
}

#endif