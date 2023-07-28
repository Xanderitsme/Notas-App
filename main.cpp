#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <stdexcept>

#include "Mensajes.h"
#include "Carpetas.h"
#include "Archivos.h"
#include "Cadenas.h"
#include "Cuenta.h"
#include "ControlBD.h"
using namespace std;

void iniciarSesion(vector<Cuenta>&);
int getID(vector<Cuenta>&, const string&);
bool cuentaRegistrada(vector<Cuenta>&, const string&);

void registrarse(vector<Cuenta>&);
bool usuarioValido(vector<Cuenta>&, const string&);
bool usuarioValidoCambio(const string&, const int&);

void interfazCuenta(Cuenta&, const int&);
void crearLista(Cuenta&);
// void combinarListas();
bool configuracionAvanzada(Cuenta&, const int&);
void cambiarUsuario(Cuenta&, const int&);
void cambiarClave(Cuenta&, const int&);
bool eliminarCuenta(Cuenta&, const int&);

void interfazLista(Cuenta&, const int&);
void crearTarea(Cuenta&, const int&);
void renombrarLista(Cuenta&, const int&);
bool eliminarLista(Cuenta&, const int&);

void interfazTarea(Cuenta&, const int&, const int&);
bool eliminarTarea(Cuenta&, const int&, const int&);

int main() {
	system("color 07");
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
		cout << "\t-> ";
		opcion = getch();

		if (opcion == 'A' || opcion == 'a') {
			iniciarSesion(cuentas);

		} else if (opcion == 'S' || opcion == 's') {
			registrarse(cuentas);

		} else if (opcion != 'X' && opcion != 'x' && opcion != esc) {
			opcionInvalida();
		}
	}

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
			encabezado(titulo);
			if (cuentas[ID].credencialesCorrectas(usuario, clave)) {
				cout << "\tHa iniciado sesion como <" << usuario << "> \n";
				accesoConcedido = true;

			} else {
				cout << "\tLa clave es incorrecta, vuelva a intentarlo (" << intentosMax - intentos << " intentos restantes)\n";
				mensajeError(tecla);
			}

		} else {
			encabezado(titulo);
			cout << "\tNo existe una cuenta con el nombre <" << usuario << ">, verifique si esta bien escrito\n";
			mensajeError(tecla);
		}
	}

	if (!accesoConcedido) {
		return;
	}

	cargando();
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
			encabezado("Combinar listas");
			cout << "\tComing soon . . .";
			getch();
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
	cout << "\tIngrese un nombre para la nueva lista: ";
	getline(cin, nombreLista);
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
	string clave, opcion;
	bool accesoConcedido = false, accionConfirmada = false;
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

	const string titulo = "Ver lista";
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
			encabezado("Vaciar lista");
			cout << "\tComing soon . . .";
			getch();
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
	cout << "\tEscriba la descripcion para la nueva tarea:\n\n";
	cout << "\t";
	getline(cin, descripcion);
	if (salir(descripcion)) {
		return;
	}

	encabezado(titulo);
	if (registrarTareaBD(descripcion, tareaID)) {
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
	cout << "\tLista: <" << cuenta.getNombreLista(listaID) << ">\n\n";
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
	const int esc = 27;
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
	if (!eliminarListaBD(listaID)) {
		cout << "\tHubo un error al intentar eliminar la lista...\n";
		return false;
	}

	cout << "\tLa lista ha sido eliminada\n";
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
			encabezado("Marcar como cumplida");
			cout << "\tComing soon . . .";
			getch();
		} else if (opcion == "S" || opcion == "s") {
			encabezado("Editar tarea");
			cout << "\tComing soon . . .";
			getch();
		} else if (opcion == "D" || opcion == "d") {
			if (eliminarTarea(cuenta, listaID, tareaID)) {
				return;
			}
		} else if (opcion == "F" || opcion == "f") {
			encabezado("Transferir tarea");
			cout << "\tComing soon . . .";
			getch();
		} else if (!salir(opcion)) {
			opcionInvalida();
		}
	}
}

bool eliminarTarea(Cuenta& cuenta, const int& listaID, const int& tareaID) {
	const int esc = 27;
	const string titulo = "Eliminar nota";
	string opcion;
	bool accionConfirmada = false;

	while (opcion != "X" && opcion != "x" && !accionConfirmada) {
		encabezado(titulo);
		cout << "\tEsta tarea sera eliminada:\n";
		cout << "\t<" << cuenta.getDescripcionTarea(listaID, tareaID) << ">\n\n";
		cout << "\tEliminar esta tarea?\n\n";
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