#include <iostream> 
#include <string> 
#include <vector> 
#include <conio.h> 
#include <stdexcept>

#include "Carpetas.h"
#include "Archivos.h"
#include "Cadenas.h"
#include "Tarea.h"
#include "Lista.h"
#include "Cuenta.h"
using namespace std;

void encabezado(const int);
void iniciarSesion(vector<Cuenta>&);
void registrarse(vector<Cuenta>&);
void verificarNombre(vector<Cuenta>&, bool&, const string, int&);
void interfazUsuario(Cuenta&);
void interfazListas(Cuenta&, const int);
void verificarBD(vector<Cuenta>&);
void carpetasValidas();
void eliminarArchivos(const string&);

int main() {
	vector<Cuenta> cuentas; 
	char opcion; 
	const int general = 0, esc = 27; 
	verificarBD(cuentas);

	while(true) {
		encabezado(general);
		cout<<"\t[1]: Iniciar sesion \n";
		cout<<"\t[2]: No tiene una cuenta? (registrarse) \n";
		cout<<"\t[0]: Salir \n";
		cout<<"\t-> ";
		opcion = getch();

		if (opcion == '1') {
			iniciarSesion(cuentas);
		} else if (opcion == '2') {
			registrarse(cuentas);
		} else if (opcion == '0' || opcion == esc) {
			break;
		}
	}

	cout<<"\n\n\n";
	system("pause");
	return 0;
}

void encabezado(const int interfaz) {
	system("cls");
	cout<<"\t\t+---------------------------------------------------+ \n";
	cout<<"\t\t| - - - ->) Notas app (patente pendiente) (<- - - - | \n";
	cout<<"\t\t+---------------------------------------------------+ \n";
	if (interfaz == 1) {
		cout<<"\t\t\t (Inicio de sesion) \n\n";
	} else if (interfaz == 2) {
		cout<<"\t\t\t (Crear una cuenta) \n\n";
	} else {
		cout<<"\n";
	}
}

void iniciarSesion(vector<Cuenta>& cuentas){
	const int esc = 27, intentosMax = 3, inicio = 1;
	int intentos = 0, indice;
	char tecla = 0;
	string nombre, clave;
	bool registrado, Acceso = false;
	while (intentos < intentosMax && tecla != esc && !Acceso) {
		registrado = false;
		encabezado(inicio);
		cout<<"\tIngrese su nombre de usuario: ";
		getline(cin, nombre);
		verificarNombre(cuentas, registrado, nombre, indice);
		if (registrado) {
			cout<<"\tIngrese su clave: ";
			getline(cin, clave);
			intentos++;
			encabezado(inicio);
			if (cuentas[indice].verificarAcceso(nombre, clave)) {
				cout<<"\n\tHa iniciado sesion como: <"<<nombre<<"> \n\n\n";
				accederCarpeta(to_string(indice));
				Acceso = true;
			} else if (intentos == intentosMax) {
				cout<<"\n\tHa fallado demasiadas veces, intentelo mas tarde... \n\n\n";
			} else {
				cout<<"\n\tLa clave es incorrecta, vuelva a intentarlo ("<<intentosMax-intentos<<" intentos restantes) \n\n\n";
			}
			system("pause");
		} else {
			encabezado(inicio);
			cout<<"\n\tNo existe una cuenta con el nombre <"<<nombre<<">, verifique si esta bien escrito \n";
			cout<<"\tPresione esc para salir o cualquier otra tecla para volver a intentarlo \n\n\n";
			tecla = getch();
		}
	}
	if (Acceso == true) {
		interfazUsuario(cuentas[indice]);
	}
}

void registrarse(vector<Cuenta>& cuentas){
	const int esc = 27, registro = 2;
	int indice;
	char tecla = 0;
	string nombre, clave;
	bool registrado, cuentaCreada = false;
	while (!cuentaCreada && tecla != esc) {
		registrado = false;
		encabezado(registro);
		cout<<"\tIngrese su nombre de usuario: ";
		getline(cin, nombre);
		verificarNombre(cuentas, registrado, nombre, indice);
		if (registrado) {
			cout<<"\n\tEl nombre <"<<cuentas[indice].getNombre()<<"> ya esta en uso, pruebe a usar otro \n";
			cout<<"\tPresione esc para salir o cualquier otra tecla para volver a intentarlo \n\n\n";
			tecla = getch();
		} else if (!datoValido(nombre)) {
			cout<<"\n\tEl nombre de no puede tener menos de 4 caracteres o contener solo espacios vacios\n";
			cout<<"\tPresione esc para salir o cualquier otra tecla para volver a intentarlo \n\n\n";
			tecla = getch();
		} else {
			cout<<"\tCree una clave: ";
			getline(cin, clave);

			if (datoValido(clave)) {

				cuentas.push_back(Cuenta(nombre, clave));

				crearCarpeta(to_string(cuentas.size()-1));
				if (accederCarpeta(to_string(cuentas.size()-1))) {
					crearArchivo("Cuenta.txt");
					guardarVariable("Cuenta.txt", nombre);
					guardarVariable("Cuenta.txt", clave);
					volverCarpetaAnt();
					cuentaCreada = true;
					encabezado(registro);
					cout<<"\n\tEl usuario <"<<nombre<<"> se ha creado con exito \n\n\n";
					system("pause");
				}
			} else {
				cout<<"\n\tLa clave de no puede tener menos de 4 caracteres o contener solo espacios vacios\n";
				cout<<"\tPresione esc para salir o cualquier otra tecla para volver a intentarlo \n\n\n";
				tecla = getch();
			}
		}
	}
}

void verificarNombre(vector<Cuenta>& cuentas, bool& registrado, const string nombre, int& indice){
	for (int i = 0; i < cuentas.size();i++) {
		if (cuentas[i].nombreRegistrado(nombre)) {
				registrado = true;
				indice = i;
				break;	
		}
	}
}

void interfazUsuario(Cuenta& usuario){
	const int general = 0, esc = 27;
	int indice = 0;
	char tecla = 0;
	string opcion, nombre;
	while(true){
		encabezado(general);
		cout<<"\tBienvenido!!! <"<<usuario.getNombre()<<"> es un gusto tenerlo aqui nuevamente \n";
		cout<<"\tPuede escribir el numero de la lista a la que desee acceder o la letra de cualquiera de las opciones \n\n";
		if (usuario.getCantListas() == 0){
			cout<<"\tActualmente no tiene ninguna lista creada!!! \n\n";
		} else {
			usuario.mostrarListas();
		}
		cout<<"\t[A]: Crear una lista \n";
		cout<<"\t[S]: Eliminar una lista \n";
		cout<<"\t[D]: Combinar listas \n";
		cout<<"\t[X]: Salir de la cuenta \n";
		cout<<"\t-> ";
		getline(cin, opcion);

		encabezado(general);
		if (opcion == "A" || opcion == "a") {
			cout<<"\tIngrese un nombre para una nueva lista: ";
			getline(cin, nombre);
			usuario.crearLista(nombre);
			encabezado(general);
			cout<<"\tLa lista <"<<nombre<<"> se ha creado con exito \n\n\n";
			system("pause");
		} else if (opcion == "S" || opcion == "s") {
			encabezado(general);
			cout<<"\tIngrese el numero de la lista\n\n\n";
			cout<<"\t-> ";
			cin>>indice;
			indice--;
			usuario.eliminarLista(indice);
			cin.ignore();
		} else if (opcion == "D" || opcion == "d") {

		} else if (opcion == "X" || opcion == "x") {
			volverCarpetaAnt();
			break;
		} else {
			if(convertirString(opcion, indice) && indice >= 0 && indice < usuario.getCantListas()){
				interfazListas(usuario, indice);
			} else {
				cout<<"\tEl numero de lista u opcion seleccionada no es valida \n\n\n";
				system("pause");
			}
		}
	}
}

void interfazListas(Cuenta& usuario, const int indice){
	const int general = 0;
	string opcion, contenido;
	while (true) {
		encabezado(general);
		cout<<"\tMostrando lista: <"<<usuario.nombreLista(indice)<<">\n";
		cout<<"\tEscriba el numero de cualquier nota o tarea para ver mas opciones, o la letra de cualquiera de las opciones\n\n";
		if (usuario.getCantTareas(indice) == 0) {
			cout<<"\tActualmente no tiene ninguna tarea en esta lista \n\n";
		} else {
			usuario.mostrarTareas(indice);
		}
		cout<<"\t[A]: Crear una tarea \n";
		cout<<"\t[S]: Eliminar una tarea \n";
		cout<<"\t[X]: Volver atras \n";
		cout<<"\t-> ";
		getline(cin, opcion);
		if (opcion == "A" || opcion == "a") {
			encabezado(general);
			cout<<"\tEscriba el contenido de la tarea: \n\t";
			getline(cin, contenido);
			usuario.crearTarea(indice, contenido);
		} else if (opcion == "S" || opcion == "s") {

		} else if (opcion == "X" || opcion == "x") {
			break;
		}
	}
}

void verificarBD(vector<Cuenta>& cuentas) {
	const string nombre = "Data", cuenta = "Cuenta.txt";
	string carpetaNom;
	bool newUser;

	if (accederCarpeta(nombre)) {
		eliminarArchivos(cuenta);
		carpetasValidas();

		vector<string> carpetas;
		int carpetasCant = carpetasCont(carpetas);
		ordenarVector(carpetas);
		for (int i = 0; i < carpetasCant; i++) {
			carpetaNom = carpetas[i];
			newUser = false;
			if (accederCarpeta(carpetaNom)) {
//				if (existeArchivo(cuenta)) {

					eliminarArchivos(cuenta);

					if (contarVariables(cuenta) == 2) {
						vector<string> datos;
						cargarVariables(cuenta, datos, 2);
						if (datoValido(datos[0]) && datoValido(datos[1])) {
							cuentas.push_back(Cuenta(datos[0], datos[1]));
							newUser = true;
						} else {
							volverCarpetaAnt();
							eliminarCarpeta(carpetaNom);
						}

					} else {
						volverCarpetaAnt();
						eliminarCarpeta(carpetaNom);
					}
//				} else {
//					volverCarpetaAnt();
//					eliminarCarpeta(carpetaNom);
//				}
				volverCarpetaAnt();
				//Hay fallos en esta función, creo que no puede renombrar correctamente las carpetas, tomar en cuenta que pueden haber carpetas que ya ocupan el nombre deseado
				//por lo cual es posible que no se puedo cambiar el nombre a una carpeta
				if (newUser && carpetaNom != to_string(cuentas.size() - 1)) {
					cambiarNombreCarpeta(carpetaNom, to_string(cuentas.size() - 1));					
				}
			}
		}

	} else {
		crearCarpeta(nombre);
		accederCarpeta(nombre);
	}
}

void carpetasValidas() {
	vector<string> carpetas;
	int carpetasCant = carpetasCont(carpetas), indice;
	for (const string& carpetaNom : carpetas) {
		if (!convertirString(carpetaNom, indice)) {
			eliminarCarpeta(carpetaNom);
		}
		if (accederCarpeta(carpetaNom)) {
			if (!existeArchivo("Cuenta.txt")) {
				volverCarpetaAnt();
				eliminarCarpeta(carpetaNom);
			} else {
				volverCarpetaAnt();
			}
		}
	}
}

void eliminarArchivos(const string& nombreArchivo) {
	vector<string> archivos;
	int archivosCant = archivosCont(archivos);
	for (int i = 0; i < archivosCant; i++) {
		if (archivos[i] != nombreArchivo) {
			eliminarArchivo(archivos[i]);
		}
	}
}