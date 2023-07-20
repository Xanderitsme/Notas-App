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

void encabezado(const int&);
void iniciarSesion(vector<Cuenta>&);
void registrarse(vector<Cuenta>&);
void verificarNombre(vector<Cuenta>&, bool&, const string, int&);
void interfazUsuario(Cuenta&);
void interfazListas(Cuenta&, const int);
bool ActualizarBD(vector<Cuenta>&);
bool verificarCarpeta(const string&, string&, string&);
void carpetasValidas();
void eliminarArchivos(const string&);


int main() {
	vector<Cuenta> cuentas; 
	char opcion; 
	const int general = 0, esc = 27; 
	ActualizarBD(cuentas);

	while(true) {
		encabezado(general);
		cout<<"\t[1]: Iniciar sesion \n";
		cout<<"\t[2]: No tiene una cuenta? (registrarse) \n";
		cout<<"\t[X]: Salir \n";
		cout<<"\t-> ";
		opcion = getch();

		if (opcion == '1') {
			iniciarSesion(cuentas);
		} else if (opcion == '2') {
			registrarse(cuentas);
		} else if (opcion == 'X' || opcion == 'x' || opcion == esc) {
			break;
		}
	}

	return 0;
}

void encabezado(const int& interfaz) {
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
	usuario.actualizarListas();

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
		} else if (opcion == "S" || opcion == "s" && usuario.getCantListas() > 0) {
			encabezado(general);
			cout << "\tIngrese el numero de la lista que desee eliminar\n\n";
			usuario.mostrarListas();
			cout << "\t-> ";
			getline(cin, opcion);
			if (convertirString(opcion, indice) && indice >= 0 && indice < usuario.getCantListas()) {
				usuario.eliminarLista(indice);
				encabezado(general);
				cout << "\tLa lista se ha eliminado con exito\n\n\n";
				system("pause");
			} else {
				encabezado(general);
				cout << "\tEl numero de lista u opcion seleccionada no es valida\n\n\n";
				system("pause");
			}
		} else if (opcion == "D" || opcion == "d") {

		} else if (opcion == "X" || opcion == "x") {
			volverCarpetaAnt();
			break;
		} else {
			if (convertirString(opcion, indice) && indice >= 0 && indice < usuario.getCantListas()) {
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
		cout<<"\tEscriba el numero de cualquier tarea para ver mas opciones, o la letra de cualquiera de las opciones\n\n";
		if (usuario.getCantTareas(indice) == 0) {
			cout<<"\tActualmente no tiene ninguna tarea en esta lista \n\n";
		} else {
			usuario.mostrarTareas(indice);
		}
		cout<<"\t[A]: Crear una tarea \n";
		cout<<"\t[X]: Volver atras \n";
		cout<<"\t-> ";
		getline(cin, opcion);
		if (opcion == "A" || opcion == "a") {
			encabezado(general);
			cout<<"\tEscriba el contenido de la tarea: ";
			getline(cin, contenido);
			usuario.crearTarea(indice, contenido);
		} else if (opcion == "X" || opcion == "x") {
			break;
		}
	}
}

// ActualizarUsuario()
// ActualizarLista()
bool ActualizarBD(vector<Cuenta>& cuentas) {
	const string nombre = "Data", cuenta = "Cuenta.txt";
	string carpetaNom;
	bool newUser;
	if (!accederCarpeta(nombre)) {
		crearCarpeta(nombre);
		accederCarpeta(nombre);
		return true;
	}
	eliminarArchivos(cuenta);
	carpetasValidas();

	vector<string> carpetas;
	int carpetasCant = carpetasCont(carpetas);
	string usuario, clave;
	ordenarVector(carpetas);
	for (const auto& carpetaNom : carpetas) {
		if (verificarCarpeta(carpetaNom, usuario, clave)) {
			cuentas.push_back(Cuenta(usuario, clave));
			// newUser = true;
			if (/*newUser &&*/ carpetaNom != to_string(cuentas.size() - 1)) {
				cambiarNombreCarpeta(carpetaNom, to_string(cuentas.size() - 1));					
			}
		}
		
	}
	return true;
}

bool verificarCarpeta(const string& carpetaNom, string& usuario, string& clave) {
	if (!accederCarpeta(carpetaNom)) {
		return false;
	}
	eliminarArchivos("Cuenta.txt");
	if (contarVariables("Cuenta.txt") != 2) {
		volverCarpetaAnt();
		eliminarCarpeta(carpetaNom);
		return false;
	}

	vector<string> datos;
	cargarVariables("Cuenta.txt", datos, 2);
	if (!datoValido(datos[0]) && !datoValido(datos[1])) {
		volverCarpetaAnt();
		eliminarCarpeta(carpetaNom);
		return false;
	}
	usuario = datos[0];
	clave = datos[1];
	volverCarpetaAnt();
	return true;
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