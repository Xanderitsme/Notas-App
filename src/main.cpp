#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#include <conio.h>
#else
#include "linux_conio.h"
#endif

#include "Mensajes.h"
#include "Carpetas.h"
#include "Archivos.h"
#include "Cadenas.h"
#include "Cuenta.h"
#include "ControlBD.h"
#include "system_utils.h"
using namespace std;

void iniciarSesion(vector<Cuenta>&);
int getID(vector<Cuenta>&, const string&);
bool cuentaRegistrada(vector<Cuenta>&, const string&);

void registrarse(vector<Cuenta>&);
bool usuarioValido(vector<Cuenta>&, const string&);
bool usuarioValidoCambio(const string&, const int&);

void interfazCuenta(Cuenta&, const int&);
void crearLista(Cuenta&);

void combinarListas(Cuenta&);
bool crearNombreLista(Cuenta&, const vector<int>&, string&);
void mostrarListasSel(Cuenta&, const vector<int>&);
void seleccionarLista(vector<int>&, const int&);

bool configuracionAvanzada(Cuenta&, const int&);
void cambiarUsuario(Cuenta&, const int&);
void cambiarClave(Cuenta&, const int&);
bool eliminarCuenta(Cuenta&, const int&);

void interfazLista(Cuenta&, const int&);
void crearTarea(Cuenta&, const int&);
void renombrarLista(Cuenta&, const int&);
bool eliminarLista(Cuenta&, const int&);
void vaciarLista(Cuenta&, const int&);

void interfazListaCumplidas(Cuenta&, const int&);
void interfazTareaC(Cuenta&, const int&, const int&);
bool desecharTareasC(Cuenta&, const int&);

void interfazTarea(Cuenta&, const int&, const int&);
bool cambiarEstadoTarea(Cuenta&, const int&, const int&, const bool&);
void editarTarea(Cuenta&, const int&, const int&);
bool eliminarTarea(Cuenta&, const int&, const int&);
bool transferirTarea(Cuenta&, const int&, const int&);
bool confirmarTransferencia(Cuenta&, const int&, const int&, const int&);

int main() {
	#ifndef _WIN32
		// Configurar la localización para soporte Unicode
		std::locale::global(std::locale("en_US.UTF-8"));
		std::cout.imbue(std::locale());
	#endif

	sys::setConsoleColor("07");
	ActualizarBD();
	
	vector<Cuenta> cuentas; 
	char opcion; 
	const int esc = 27;

	while (opcion != 'X' && opcion != 'x' && opcion != esc) {
		actualizarCuentas(cuentas);
		encabezado("");
		cout << "\t[A]: Iniciar sesion \n";
		cout << "\t[S]: No tiene una cuenta? (registrarse) \n";
		cout << "\t[X]: Salir \n";
		
		opcion = sys::getOption("\t-> ");
		
		if (opcion == 'A' || opcion == 'a') {
			iniciarSesion(cuentas);
		} else if (opcion == 'S' || opcion == 's') {
			registrarse(cuentas);
		} else if (opcion != 'X' && opcion != 'x' && opcion != esc) {
			opcionInvalida();
		}
	}

	encabezado("");
	cout << "\tHasta la proxima!\n";
	cargando();

	sys::clearScreen();
	return 0;
}

void iniciarSesion(vector<Cuenta>& cuentas) {
	const int esc = 27, intentosMax = 3;
	const string titulo = "Iniciar sesion";
	string usuario, clave;
	int intentos = 0, ID;
	char tecla = 0;
	bool accesoConcedido = false;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && intentos < intentosMax && !accesoConcedido) {
		encabezado(titulo);
		cout << "\tIngrese su nombre de usuario: ";
		getline(cin, usuario);
		if (salir(usuario)) {
			return;
		}

		if (cuentaRegistrada(cuentas, usuario)) {
			cout << "\tIngrese su clave: ";
			getline(cin, clave);
			if (salir(clave)) {
				return;
			}
			
			intentos++;
			ID = getID(cuentas, usuario);
			sys::clearScreen();
			if (cuentas[ID].credencialesCorrectas(usuario, clave)) {
				cout << "\tHa iniciado sesion como <" << usuario << "> \n";
				accesoConcedido = true;

			} else {
				cout << "\tLa clave es incorrecta, vuelva a intentarlo (" << intentosMax - intentos << " intentos restantes)\n";
				mensajeError(tecla);
			}

		} else {
			sys::clearScreen();
			cout << "\tNo existe una cuenta con el nombre <" << usuario << ">, verifique si esta bien escrito\n";
			mensajeError(tecla);
		}
	}

	if (!accesoConcedido) {
		return;
	}

	cargando();
	limpiarBuffer();
	interfazCuenta(cuentas[ID], ID);
}

int getID(vector<Cuenta>& cuentas, const string& usuario) {
	int ID = 0;
	
	for (auto& cuenta : cuentas) {
		if (usuario == cuenta.getUsuario()) {
			return ID;
		}
		ID++;
	}

	return ID;
}

bool cuentaRegistrada(vector<Cuenta>& cuentas, const string& usuario) {
	for (auto& cuenta : cuentas) {
		if (usuario == cuenta.getUsuario()) {
			return true;
		}
	}

	return false;
}

void registrarse(vector<Cuenta>& cuentas) {
	const int esc = 27;
	const string titulo = "Registrarse";
	string usuario, clave;
	char tecla = 0;

	while (tecla != esc && tecla != 'X' && tecla != 'x') {
		encabezado(titulo);
		cout << "\tCree su nombre de usuario: ";
		getline(cin, usuario);
		if (salir(usuario)) {
			return;
		}

		if (usuarioValido(cuentas, usuario)) {
			cout << "\tCree su clave: ";
			getline(cin, clave);
			if (salir(clave)) {
				return;
			}

			if (datoValido(clave)) {
				if (registrarCuentaBD(usuario, clave, cuentas.size())) {
					encabezado(titulo);
					cout << "\tLa cuenta <" << usuario << "> se ha registrado con exito!\n";
					cout << "\tAhora puede iniciar sesion\n";
					cargando();
					return;
				} else {
					cout << "\tHa ocurrido un error al intentar registrar su cuenta\n";
					mensajeError(tecla);
				}

			} else {
				encabezado(titulo);
				cout << "\tLa clave no puede tener menos de 4 o mas de 30 caracteres y tampoco ser solo espacios en blanco\n";
				mensajeError(tecla);
			}

		} else {
			mensajeError(tecla);
		}
	}
}

bool usuarioValido(vector<Cuenta>& cuentas, const string& usuario) {
	if (!datoValido(usuario)) {
		encabezado("");
		cout << "\tEl nombre de usuario no puede tener menos de 4 o mas de 30 caracteres y tampoco ser solo espacios en blanco\n";
		return false;
	}

	if (cuentaRegistrada(cuentas, usuario)) {
		encabezado("");
		cout << "\tEl nombre de usuario <" << usuario << "> ya esta en uso, pruebe a usar otro\n";
		return false;
	}

	return true;
}

bool usuarioValidoCambio(const string& usuario, const int& ID) {
	volverCarpetaAnt();

	vector<Cuenta> cuentas;
	actualizarCuentas(cuentas);
	if (!usuarioValido(cuentas, usuario)) {
		accederCarpeta(to_string(ID));
		return false;
	}

	accederCarpeta(to_string(ID));
	return true;
}

void interfazCuenta(Cuenta& cuenta, const int& ID) {
	if (!accederCarpeta(to_string(ID))) {
		cout << "\tHa ocurrido un error al intentar acceder a su cuenta, por favor vuelva a intentarlo\n";
		cargando();
		return;
	}

	string opcion;
	int listaID = 0;

	while (opcion != "X" && opcion != "x") {
		actualizarListas(cuenta);
		encabezado(cuenta.getUsuario());
		cout << "\tBienvenido! <" << cuenta.getUsuario() << ">, es un gusto tenerlo aqui nuevamente\n";
		cout << "\tEscriba el numero de alguna lista para acceder a ella o la letra de cualquiera de las opciones\n\n";

		if (cuenta.getCantListas() == 0) {
			cout << "\tActualmente no tiene ninguna lista creada!\n";
		}
		cuenta.mostrarListas();

		separador();
		cout << "\t[A]: Crear una lista\n";
		cout << "\t[S]: Combinar listas\n";
		cout << "\t[D]: Configuracion avanzada\n";
		cout << "\t[X]: Salir de la cuenta\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (convertirStringInt(opcion, listaID) && listaID > 0 && listaID <= cuenta.getCantListas()) {
			interfazLista(cuenta, listaID - 1);

		} else if (opcion == "A" || opcion == "a") {
			crearLista(cuenta);

		} else if (opcion == "S" || opcion == "s") {
			combinarListas(cuenta);

		} else if (opcion == "D" || opcion == "d") {
			if (configuracionAvanzada(cuenta, ID)) {
				return;
			}

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	volverCarpetaAnt();
}

void crearLista(Cuenta& cuenta) {
	const int listaID = cuenta.getCantListas();
	const string titulo = "Crear lista";
	string nombreLista;

	encabezado(titulo);
	cout << "\tEscriba un nombre para la nueva lista: ";
	getline(cin, nombreLista);

	// nombreLista = sys::getInputLine("\tEscriba un nombre para la nueva lista: ");

	if (salir(nombreLista)) {
		return;
	}

	encabezado(titulo);
	if (registrarListaBD(nombreLista, listaID)) {
		cout << "\tLa lista <" << nombreLista << "> se ha creado con exito!\n";
	} else {
		cout << "\tHa ocurrido un error al intentar crear la lista\n";
	}
	
	cargando();
}

void combinarListas(Cuenta& cuenta) {
	const string titulo = "Combinar listas";

	if (cuenta.getCantListas() < 2) {
		encabezado(titulo);
		cout << "\tDebe crear al menos 2 listas para poder usar esta opcion!\n";
		cargando();
		return;		
	}

	vector<int> listasSeleccionadas;
	int listaID;
	bool seleccionConfirmada = false;
	string opcion;

	while (opcion != "X" && opcion != "x" && !seleccionConfirmada) {
		encabezado(titulo);
		cout << "\tEscriba el numero de una lista para seleccionarla\n";
		cout << "\tVuelva a escribir el numero de una lista para deseleccionarla\n";
		if (listasSeleccionadas.size() > 0) {
			cout << "\n\tHa seleccionado las listas: ";
			for (const auto& LiSelID : listasSeleccionadas) {
				cout << "[" << LiSelID + 1 << "]";
			}
			cout << "\n";
		}
		cout << "\n";
		cuenta.mostrarListas();

		separador();
		if (listasSeleccionadas.size() > 1) {
			cout << "\t[A]: Empezar a combinar listas\n";
		}
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if ((opcion == "A" || opcion == "a") && listasSeleccionadas.size() > 1) {
			seleccionConfirmada = true;

		} else if (convertirStringInt(opcion, listaID) && listaID > 0 && listaID <= cuenta.getCantListas()) {
			seleccionarLista(listasSeleccionadas, listaID - 1);

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	if (!seleccionConfirmada) {
		return;
	}

	int listSelID;
	bool accionConfirmada = false;
	string nombreLista;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tLas listas a combinar seran las siguientes:\n\n";
		mostrarListasSel(cuenta, listasSeleccionadas);

		separador();
		cout << "\tAhora debe elgir un nombre\n";
		cout << "\tEscriba el numero de alguna lista si desea usarlo como nombre o elija una opcion\n\n";
		cout << "\t[A]: Crear un nuevo nombre\n";
		cout << "\t[X]: Cancelar\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (convertirStringInt(opcion, listSelID) && listSelID > 0 && listSelID <= listasSeleccionadas.size()) {
			nombreLista = cuenta.getNombreLista(listasSeleccionadas[listSelID - 1]);
			accionConfirmada = true;

		} else if (opcion == "A" || opcion == "a") {
			if (crearNombreLista(cuenta, listasSeleccionadas, nombreLista)) {
				accionConfirmada = true;
			}

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	if (!accionConfirmada) {
		return;
	}

	listaID = cuenta.getCantListas();

	encabezado(titulo);
	if (combinarListasBD(nombreLista, listaID, listasSeleccionadas)) {
		cout << "\tLas listas han sido combinadas con exito!\n";
	} else {
		cout << "\tHa ocurrido un error al intentar combinar las listas\n";
	}
	
	cargando();
}

bool crearNombreLista(Cuenta& cuenta, const vector<int>& listasSeleccionadas, string& nombreLista) {
	const string titulo = "Combinar listas";

	encabezado(titulo);
	cout << "\tLas listas a combinar seran las siguientes:\n\n";
	mostrarListasSel(cuenta, listasSeleccionadas);

	separador();
	cout << "\tEscriba un nombre para la nueva lista: ";
	getline(cin, nombreLista);
	if (salir(nombreLista)) {
		return false;
	}

	return true;
}

void mostrarListasSel(Cuenta& cuenta, const vector<int>& listasSeleccionadas) {
	const string fin = "...";
	const int limiteNombre = 70;
	int indice = 1;
	string nombreRecortado;

	for (const auto& listSelID : listasSeleccionadas) {
		cout << "\t[" << indice << "]: <";

		if (cuenta.getNombreLista(listSelID).size() > limiteNombre) {
			nombreRecortado = cuenta.getNombreLista(listSelID).substr(0, limiteNombre - fin.size());
			cout << nombreRecortado << fin;
		} else {
			cout << cuenta.getNombreLista(listSelID);
		}

		cout << ">\n";
		indice++;
	}
}

void seleccionarLista(vector<int>& listasSeleccionadas, const int& listaID) {
	bool listaSeleccionada = false;
	int indice = 0;

	for (const auto& listSelID : listasSeleccionadas) {
		if (listaID == listSelID) {
			listaSeleccionada = true;
			break;
		}

		indice++;
	}

	if (!listaSeleccionada) {
		listasSeleccionadas.push_back(listaID);
		return;
	}

	listasSeleccionadas.erase(listasSeleccionadas.begin() + indice);
}

bool configuracionAvanzada(Cuenta& cuenta, const int& ID) {
	const string titulo = "Configuracion avanzada";
	string opcion;

	while (opcion != "X" && opcion != "x") {
		encabezado(titulo);
		cout << "\tEscriba la letra de la opcion que desee\n\n";
		cout << "\t[A]: Cambiar nombre de usuario\n";
		cout << "\t[S]: Cambiar clave\n";
		cout << "\t[D]: Eliminar cuenta\n";
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			cambiarUsuario(cuenta, ID);

		} else if (opcion == "S" || opcion == "s") {
			cambiarClave(cuenta, ID); 

		} else if (opcion == "D" || opcion == "d") {
			if (eliminarCuenta(cuenta, ID)) {
				return true;
			}

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	return false;
}

void cambiarUsuario(Cuenta& cuenta, const int& ID) {
	const int esc = 27;
	const string titulo = "Cambiar usuario";
	string usuario, clave;
	bool cambioNombre = false;
	char tecla = 0;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && !cambioNombre) {
		encabezado(titulo);
		cout << "\tIngrese su nuevo nombre de usuario: ";
		getline(cin, usuario);
		if (salir(usuario)) {
			return;
		}

		if (usuarioValidoCambio(usuario, ID)) {
			cout << "\tIngrese su clave para confirmar esta accion: ";
			getline(cin, clave);
			if (salir(clave)) {
				return;
			}

			encabezado(titulo);
			if (cuenta.credencialesCorrectas(cuenta.getUsuario(), clave)) {
				cout << "\tSu nuevo nombre de usuario es <" << usuario << "> \n";
				cargando();
				cambioNombre = true;

			} else {
				cout << "\tLa clave es incorrecta\n";
				mensajeError(tecla);
			}

		} else {
			mensajeError(tecla);
		}
	}

	if (!cambioNombre) {
		return;
	}

	if (!cambiarDatosCuentaBD(usuario, clave)) {
		cout << "\tHa ocurrido un error al intentar cambiar su nombre de usuario\n";
		mensajeError(tecla);
		return;
	}

	cuenta.cambiarUsuario(usuario);
}

void cambiarClave(Cuenta& cuenta, const int& ID) {
	const int esc = 27;
	const string titulo = "Cambiar clave";
	string clave, claveAux;
	bool cambioClave = false;
	char tecla = 0;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && !cambioClave) {
		encabezado(titulo);
		cout << "\tEs necesario confirmar su identidad antes de continuar\n\n";
		cout << "\tIngrese su clave actual: ";
		getline(cin, clave);
		if (salir(clave)) {
			return;
		}

		if (cuenta.credencialesCorrectas(cuenta.getUsuario(), clave)) {
			encabezado(titulo);
			cout << "\tIngrese su nueva clave: ";
			getline(cin, clave);
			if (salir(clave)) {
				return;
			}

			if (datoValido(clave)) {
				cout << "\tVuelva a ingresar su nueva clave: ";
				getline(cin, claveAux);
				if (salir(claveAux)) {
					return;
				}
				
				if (clave == claveAux) {
					encabezado(titulo);
					cout << "\tSu clave ha sido cambiada correctamente!\n";
					cargando();
					cambioClave = true;
					
				} else {
					cout << "\n\tLas claves no coinciden\n";
					mensajeError(tecla);
				}

			} else {
				encabezado(titulo);
				cout << "\tLa clave no puede tener menos de 4 o mas de 30 caracteres y tampoco ser solo espacios en blanco\n";
				mensajeError(tecla);
			}

		} else {
			encabezado(titulo);
			cout << "\tLa clave es incorrecta\n";
			mensajeError(tecla);
		}
	}

	if (!cambioClave) {
		return;
	}

	if (!cambiarDatosCuentaBD(cuenta.getUsuario(), clave)) {
		cout << "\tHa ocurrido un error al intentar cambiar su clave\n";
		mensajeError(tecla);
		return;
	}

	cuenta.cambiarClave(clave);
}

bool eliminarCuenta(Cuenta& cuenta, const int& ID) {
	const int esc = 27;
	const string titulo = "Eliminar cuenta";
	string clave;
	bool accesoConcedido = false;
	char tecla = 0;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && !accesoConcedido) {
		encabezado(titulo);
		cout << "\tAntes de continuar es necesario confirmar su identidad\n\n";
		cout << "\tIngrese su clave: ";
		getline(cin, clave);
		if (salir(clave)) {
			return false;
		}

		if (cuenta.credencialesCorrectas(cuenta.getUsuario(), clave)) {
			accesoConcedido = true;

		} else {
			encabezado(titulo);
			cout << "\tLa clave es incorrecta\n";
			mensajeError(tecla);
		}
	}

	if (!accesoConcedido) {
		return false;
	}

	string opcion;
	bool accionConfirmada = false;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tSu cuenta sera eliminada permanentemente y no podra ser recuperada\n\n";
		cout << "\tQuiere eliminar su cuenta?\n\n";
		cout << "\t[A]: Si, quiero eliminar mi cuenta\n";
		cout << "\t[X]: No, no queria hacer esto\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			accionConfirmada = true;

		} else if (!salir(opcion) && opcion != "") {
			opcionInvalida();
		}
	}

	if (!accionConfirmada) {
		encabezado(titulo);
		cout << "\tLa accion ha sido cancelada\n";
		cargando();
		return false;
	}

	encabezado(titulo);
	if (!eliminarCuentaBD(ID)) {
		cout << "\tHubo un error al intentar eliminar su cuenta...\n";
		return false;
	}

	cout << "\tSu cuenta ha sido eliminada\n";
	cargando();
	return true;
}

void interfazLista(Cuenta& cuenta, const int& listaID) {
	if (!accederCarpeta(to_string(listaID))) {
		cout << "\tHa ocurrido un error al intentar acceder a la lista, por favor vuelva a intentarlo\n";
		cargando();
		return;
	}

	const string titulo = "Ver lista", cumplidas = "Cumplidas";
	string opcion;
	int tareaID = 0;

	while (opcion != "X" && opcion != "x") {
		actualizarTareas(cuenta, listaID);
		encabezado(titulo);
		cout << "\tMostrado la lista <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tEscriba el numero de alguna tarea para ver mas opciones, o la letra de cualquiera de las opciones\n\n";

		if (cuenta.getCantTareas(listaID) == 0) {
			cout << "\tActualmente esta lista esta vacia!\n";
		}
		cuenta.mostrarTareas(listaID);

		separador();
		cout << "\t[A]: Crear una tarea\n";
		cout << "\t[S]: Renombrar lista\n";
		cout << "\t[D]: Eliminar lista\n";
		cout << "\t[F]: Vaciar lista\n";
		cout << "\t[G]: Ver tareas cumplidas\n";
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (convertirStringInt(opcion, tareaID) && tareaID > 0 && tareaID <= cuenta.getCantTareas(listaID)) {
			interfazTarea(cuenta, listaID, tareaID - 1);

		} else if (opcion == "A" || opcion == "a") {
			crearTarea(cuenta, listaID);

		} else if (opcion == "S" || opcion == "s") {
			renombrarLista(cuenta, listaID);

		} else if (opcion == "D" || opcion == "d") {
			if (eliminarLista(cuenta, listaID)) {
				return;
			}

		} else if (opcion == "F" || opcion == "f") {
			vaciarLista(cuenta, listaID);

		} else if (opcion == "G" || opcion == "g") {
			interfazListaCumplidas(cuenta, listaID);

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	volverCarpetaAnt();
}

void crearTarea(Cuenta& cuenta, const int& listaID) {
	const int tareaID = cuenta.getCantTareas(listaID);
	const string titulo = "Crear tarea";
	string descripcion;

	encabezado(titulo);
	cout << "\tSe encuentra en la lista <" << cuenta.getNombreLista(listaID) << ">\n";
	separador();
	cout << "\tEscriba la descripcion para la nueva tarea:\n\n";
	cout << "\t";
	getline(cin, descripcion);
	if (salir(descripcion)) {
		return;
	}

	encabezado(titulo);
	if (registrarTareaBD(descripcion, false, tareaID)) {
		cout << "\tLa tarea se ha creado con exito!\n";
	} else {
		cout << "\tHa ocurrido un error al intentar crear la tarea\n";
	}

	cargando();
}

void renombrarLista(Cuenta& cuenta, const int& listaID) {
	const string titulo = "Renombrar lista";
	string nombreLista;

	encabezado(titulo);
	cout << "\tLista: <" << cuenta.getNombreLista(listaID) << ">\n";
	separador();
	cout << "\tIngrese el nuevo nombre para la lista: ";
	getline(cin, nombreLista);
	if (salir(nombreLista)) {
		return;
	}

	encabezado(titulo);
	if (cambiarDatosListaBD(nombreLista, listaID)) {
		cuenta.renombrarLista(nombreLista, listaID);
		cout << "\tEl nombre de la lista ha sido cambiado con exito!\n";
	} else {
		cout << "\tHa ocurrido un error al intentar cambiar el nombre de la lista\n";
	}

	cargando();
}

bool eliminarLista(Cuenta& cuenta, const int& listaID) {
	const string titulo = "Eliminar lista";
	string opcion;
	bool accionConfirmada = false;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tLa lista <" << cuenta.getNombreLista(listaID) << "> sera eliminada permanentemente\n\n";
		cout << "\tEliminar esta lista?\n\n";
		cout << "\t[A]: Si, quiero eliminar esta lista\n";
		cout << "\t[X]: No, no queria hacer esto\n";
		cout << "\t-> ";		
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			accionConfirmada = true;
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	if (!accionConfirmada) {
		return false;
	}

	encabezado(titulo);
	bool eliminacionExitosa = eliminarListaBD(listaID);

	cout << (eliminacionExitosa 
		? "\tLa lista ha sido eliminada!\n" 
		: "\tHubo un error al intentar eliminar la lista...\n");

	cargando();
	return eliminacionExitosa;

	// if (!eliminarListaBD(listaID)) {
	// 	cout << "\tHubo un error al intentar eliminar la lista...\n";
	// 	return false;
	// }

	// cout << "\tLa lista ha sido eliminada!\n";
	// cargando();
	// return true;
}

void vaciarLista(Cuenta& cuenta, const int& listaID) {
	const string titulo = "Vaciar lista";

	if (cuenta.getCantTareas(listaID) == 0) {
		encabezado(titulo);
		cout << "\tEsta lista ya esta vacia!\n";
		cargando();
		return;
	}
	
	string opcion;
	bool accionConfirmada = false;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tLista: <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tTodas las tareas de esta lista seran eliminadas permanentemente\n";
		separador();
		cout << "\tVaciar esta lista?\n\n";
		cout << "\t[A]: Si, quiero vaciar esta lista\n";
		cout << "\t[X]: No, no queria hacer esto\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			accionConfirmada = true;
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	if (!accionConfirmada) {
		return;
	}

	encabezado(titulo);
	if (!vaciarListaBD()) {
		cout << "\tHubo un error al intentar vaciar la lista...\n";
	} else {
		cout << "\tLas tareas de esta lista han sido eliminadas con exito!\n";
	}

	cargando();
}

void interfazListaCumplidas(Cuenta& cuenta, const int& listaID) {
	const string titulo = "Tareas cumplidas", cumplidas = "Cumplidas";

	if (!accederCarpeta(cumplidas)) {
		cout << "\tHa ocurrido un error al intentar acceder a la lista, por favor vuelva a intentarlo\n";
		cargando();
		return;
	}

	actualizarTareasC(cuenta, listaID);
	if (cuenta.getCantTareasC(listaID) == 0) {
		encabezado(titulo);
		cout << "\tNo tiene ninguna tarea cumplida!\n";
		cargando();
		volverCarpetaAnt();
		return;
	}

	string opcion;
	int tareaCID = 0;

	while (opcion != "X" && opcion != "x") {
		encabezado(titulo);
		actualizarTareasC(cuenta, listaID);
		cout << "\tTareas cumplidas en <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tEscriba el numero de alguna tarea para ver mas opciones, o la letra de cualquiera de las opciones\n\n";

		cuenta.mostrarTareasC(listaID);

		separador();
		cout << "\t[A]: Desechar tareas cumplidas\n";
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (convertirStringInt(opcion, tareaCID) && tareaCID > 0 && tareaCID <= cuenta.getCantTareasC(listaID)) {
			interfazTareaC(cuenta, listaID, tareaCID - 1);

		} else if (opcion == "A" || opcion == "a") {
			if (desecharTareasC(cuenta, listaID)) {
				volverCarpetaAnt();
				return;
			}

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	volverCarpetaAnt();
}

void interfazTareaC(Cuenta& cuenta, const int& listaID, const int& tareaCID) {
	const string titulo = "Mostrando tarea cumplida";
	string opcion;

	while (opcion != "X" && opcion != "x") {
		actualizarTareasC(cuenta, listaID);
		encabezado(titulo);
		cout << "\tSe encuentra en la lista <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tEscriba la letra de la opcion que desee\n\n";
		cout << "\tEstado: Cumplida\n";
		cout << "\t<" << cuenta.getDescripcionTareaC(listaID, tareaCID) << ">\n";

		separador();
		cout << "\t[A]: Marcar como no cumplida\n";
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			if (cambiarEstadoTarea(cuenta, listaID, tareaCID, true)) {
				return;
			}
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}
}

bool desecharTareasC(Cuenta& cuenta, const int& listaID) {
	const string titulo = "Desechar tareas";
	string opcion;
	bool accionConfirmada = false;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tLista: <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tTodas las tareas cumplidas de esta lista seran eliminadas\n";
		separador();
		cout << "\tEliminar tareas cumplidas?\n\n";
		cout << "\t[A]: Si, eliminar tareas\n";
		cout << "\t[X]: No, no queria hacer esto\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			accionConfirmada = true;
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	if (!accionConfirmada) {
		return false;
	}

	encabezado(titulo);
	if (!vaciarListaBD()) {
		cout << "\tHubo un error al intentar eliminar las tareas cumplidas...\n";
		cargando();
		return false;
	}

	cout << "\tLas tareas cumplidas de esta lista han sido eliminadas con exito!\n";
	cargando();
	return true;
}

void interfazTarea(Cuenta& cuenta, const int& listaID, const int& tareaID) {
	const string titulo = "Mostrando tarea";
	string opcion;

	while (opcion != "X" && opcion != "x") {
		actualizarTareas(cuenta, listaID);
		encabezado(titulo);
		cout << "\tSe encuentra en la lista <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tEscriba la letra de la opcion que desee\n\n";
		cout << "\tEstado: No cumplida\n";
		cout << "\t<" << cuenta.getDescripcionTarea(listaID, tareaID) << ">\n";

		separador();
		cout << "\t[A]: Marcar como cumplida\n";
		cout << "\t[S]: Editar tarea\n";
		cout << "\t[D]: Eliminar tarea\n";
		cout << "\t[F]: Transferir tarea a otra lista\n";
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			if (cambiarEstadoTarea(cuenta, listaID, tareaID, false)) {
				return;
			}
		} else if (opcion == "S" || opcion == "s") {
			editarTarea(cuenta, listaID, tareaID);

		} else if (opcion == "D" || opcion == "d") {
			if (eliminarTarea(cuenta, listaID, tareaID)) {
				return;
			}

		} else if (opcion == "F" || opcion == "f") {
			if (transferirTarea(cuenta, listaID, tareaID)) {
				return;
			}
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}
}

bool cambiarEstadoTarea(Cuenta& cuenta, const int& listaID, const int& tareaID, const bool& estado) {
	const string titulo = "Cambiar estado tarea";
	string descripcion;

	if (estado) { // Cuando la tarea actualmente ya ha sido cumplida
		descripcion = cuenta. getDescripcionTareaC(listaID, tareaID);
	} else { //Cuando la tarea actualmente no ha sido cumplida
		descripcion = cuenta.getDescripcionTarea(listaID, tareaID);
		cuenta.crearTareaC(listaID, descripcion);
	}

	encabezado(titulo);
	if (!cambiarDatosTareaBD(descripcion, !estado, tareaID)) {
		cout << "\tHa ocurrido un error al intentar cambiar el estado de esta tarea\n";
		cargando();
		return false;
	}

	cout << "\t<" << descripcion << ">\n";
	separador();
	cout << "\tEl estado de esta tarea ha sido cambiado!\n";
	cargando();
	return true;
}

void editarTarea(Cuenta& cuenta, const int& listaID, const int& tareaID) {
	const string titulo = "Editar tarea";
	string descripcion;

	encabezado(titulo);
	cout << "\tTarea: <" << cuenta.getDescripcionTarea(listaID, tareaID) << ">\n";
	separador();
	cout << "\tIngrese el nuevo contenido para la tarea:\n\n";
	cout << "\t";
	getline(cin, descripcion);
	if (salir(descripcion)) {
		return;
	}

	encabezado(titulo);
	if (cambiarDatosTareaBD(descripcion, false, tareaID)) {
		cuenta.editarTarea(listaID, tareaID, descripcion);
		cout << "\tEl contenido de la tarea ha sido editado con exito!\n";
	} else {
		cout << "\tHa ocurrido un error al intentar editar el contenido de la tarea\n";
	}

	cargando();
}

bool eliminarTarea(Cuenta& cuenta, const int& listaID, const int& tareaID) {
	const int esc = 27;
	const string titulo = "Eliminar tarea";
	string opcion;
	bool accionConfirmada = false;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tEsta tarea sera eliminada:\n";
		cout << "\t<" << cuenta.getDescripcionTarea(listaID, tareaID) << ">\n\n";
		cout << "\tEliminar tarea?\n\n";
		cout << "\t[A]: Si, quiero eliminar esta tarea\n";
		cout << "\t[X]: No, no queria hacer esto\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			accionConfirmada = true;
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}

	if (!accionConfirmada) {
		return false;
	}

	encabezado(titulo);
	if (!eliminarTareaBD(tareaID)) {
		cout << "\tHubo un error al intentar eliminar la tarea...\n";
		return false;
	}

	cout << "\tLa tarea ha sido eliminada\n";
	cargando();
	return true;
}

bool transferirTarea(Cuenta& cuenta, const int& listOrigID, const int& tareaID) {
	const string titulo = "Transferir tarea";

	if (cuenta.getCantListas() < 2) {
		encabezado(titulo);
		cout << "\tDebe crear al menos 2 listas para poder usar esta opcion!\n";
		cargando();
		return false;		
	}

	string opcion;
	int listDestID;
	bool transferenciaConfirmada = false;

	while (opcion != "X" && opcion != "x" && !transferenciaConfirmada) {
		encabezado(titulo);
		cout << "\tTarea:\n";
		cout << "\t<" << cuenta.getDescripcionTarea(listOrigID, tareaID) << ">\n";
		separador();
		cout << "\tEscriba el numero de la lista a la cual transferira esta tarea\n\n";
		cuenta.mostrarListas();

		separador();
		cout << "\t-> ";
		getline(cin, opcion);

		if (convertirStringInt(opcion, listDestID) && listDestID > 0 && listDestID <= cuenta.getCantListas()) {
			listDestID--;
			if (listOrigID == listDestID) {
				encabezado(titulo);
				cout << "\tLas listas de origen y destino no pueden ser iguales!\n";
				cargando();

			} else if (confirmarTransferencia(cuenta, listOrigID, listDestID, tareaID)) {
				transferenciaConfirmada = true;
			}

		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}
	
	if (!transferenciaConfirmada) {
		return false;
	}

	encabezado(titulo);
	if (!transferirTareaBD(listOrigID, listDestID, tareaID)) {
		cout << "\tHubo un error al intentar trasnferir esta tarea...\n";
		cargando();
		return false;
	}

	cout << "\tLa lista ha sido transferida con exito!\n";
	cargando();
	return true;
}

bool confirmarTransferencia(Cuenta& cuenta, const int& listOrigID, const int& listDestID, const int& tareaID) {
	const string titulo = "Transferir tarea";
	string opcion;

	while (opcion != "X" && opcion != "x") {
		encabezado(titulo);
		cout << "\tSe transferira esta tarea: \n";
		cout << "\t<" << cuenta.getDescripcionTarea(listOrigID, tareaID) << ">\n\n";
		cout << "\tDesde: <" << cuenta.getNombreLista(listOrigID) << ">\n";
		cout << "\tHacia: <" << cuenta.getNombreLista(listDestID) << ">\n";
		separador();

		cout << "\tConfirmar esta accion?\n\n";
		cout << "\t[A]: Si, transferir tarea\n";
		cout << "\t[X]: No, volver a elegir destino\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			return true;
		} else if (!salir(opcion)) {
			opcionInvalida();
		}

	}

	return false;
}
