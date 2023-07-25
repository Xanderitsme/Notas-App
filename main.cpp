#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "Carpetas.h"
#include "Archivos.h"
#include "Cadenas.h"
#include "Cuenta.h"
#include "ControlBD.h"
using namespace std;

void encabezado(const string&);
void separador();
void mensajeError(char&);
void opcionInvalida();
void cargando();
bool salir(const string&);

void iniciarSesion(vector<Cuenta>&);
int getID(vector<Cuenta>&, const string&);
bool cuentaRegistrada(vector<Cuenta>&, const string&);

void registrarse(vector<Cuenta>&);
bool usuarioValido(vector<Cuenta>&, const string&);
bool usuarioValidoCambio(const string&, const int&);

void interfazCuenta(Cuenta&, const int&);
void crearLista(Cuenta&);
void configuracionAvanzada(Cuenta&, const int&);
void cambiarUsuario(Cuenta&, const int&);
void cambiarClave(Cuenta&, const int&);

void interfazLista(Cuenta&, const int&);

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

void encabezado(const string& titulo) {
    char upLeft = 201, horizontal = 205, upRigth = 187, vertical = 186, downLeft = 200, downRight = 188, diamond = 4, punto = 250, linea = 22;
    string title = "Notas App", tituloMod = titulo;
    const int styleLenght = 32, bordes = 10;

	if (titulo.size() > styleLenght + title.size() - bordes) {
		tituloMod = titulo.substr(0, styleLenght + title.size() - bordes - 1) + "...";
	} else if (titulo.size() > 0){
		tituloMod+= " ";
	}

    system("cls");
	// Primera linea
    cout << "\t" << upLeft;
    for (int i = 0; i < (styleLenght + title.size()); i++) {
        cout << horizontal;
    }
    cout << upRigth;

    cout << "\t" << vertical << " Escriba X  " << vertical <<"\n";

	// Segunda linea
    cout << "\t" << vertical << "  " << diamond << "   <  < << ";
    cout << punto << " " << title << " " << punto;
    cout << " >> >  >   " << diamond << "  " << vertical;

    cout << "\t" << vertical << " para salir " << vertical << "\n";

    // Tercera linea
    cout << "\t" << vertical << "  " << diamond << " ";

	cout << tituloMod;
	for (int i = 0; i < 24 + title.size() - tituloMod.size(); i++) {
		cout << linea;
	}

    cout << " " << diamond << "  " << vertical;

    cout << "\t" << vertical << " o volver   " << vertical << "\n";

	// Cuarta linea
    cout << "\t" << downLeft;
    for (int i = 0; i < (styleLenght + title.size()); i++) {
        cout << horizontal;
    }
    cout << downRight;

    cout << "\t" << downLeft;
    for (int i = 0; i < 12; i++) {
        cout << horizontal;
    }
    cout << downRight << "\n\n";
    
	// Previsualizacion
	//   ╔═════════════════════════════════════════╗    ║ Escriba X  ║
	//   ║  ♦   <  < << · Notas App · >> >  >   ♦  ║    ║ para salir ║
    //   ║  ♦ ▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬ ♦  ║    ║ o volver   ║
	//   ╚═════════════════════════════════════════╝    ╚════════════╝

}

void separador() {
	const char linea = 22;
	cout << "\n\t";
	for (int i = 0; i < 45; i++) {
		cout << linea << " ";
	}
	cout << "\n\n";
}

void mensajeError(char& tecla) {
	cout << "\n\tPresione ESC si desea volver a la interfaz anterior\n";
	cout << "\tPresione cualquier tecla para volver a intentarlo\n";
	tecla = getch();
}

void opcionInvalida() {
	encabezado("");
	cout << "\tLa opcion seleccionada no existe, por favor vuelva a intentarlo\n\n";
	cout << "\tPresione cualquier tecla para continuar . . .";
	getch();
}

void cargando() {
	cout << "\n\t\tCargando ";
	Sleep(400);
	cout << ". ";
	Sleep(400);
	cout << ". ";
	Sleep(400);
	cout << ". ";
	Sleep(400);
}

bool salir(const string& opcion) {
	if (opcion == "X" || opcion == "x") {
		return true;
	}
	return false;
}

void iniciarSesion(vector<Cuenta>& cuentas) {
	const int esc = 27, intentosMax = 3;
	const string titulo = "Iniciar sesion";
	int intentos = 0, ID;
	char tecla = 0;
	string usuario, clave;
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
		cout << "Ha ocurrido un error al intentar acceder a su cuenta, por favor vuelva a intentarlo\n";
		cargando();
		return;
	}

	string opcion;
	int listaID = 0;

	while (opcion != "X" && opcion != "x") {
		actualizarListas(cuenta);
		encabezado(cuenta.getUsuario());
		cout << "\tBienvenido! <" << cuenta.getUsuario() << ">, es un gusto tenerlo aqui nuevamente.\n";
		cout << "\tEscriba el numero de alguna lista para acceder o la letra de cualquiera de las opciones.\n\n";

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

		} else if (opcion == "D" || opcion == "d") {
			configuracionAvanzada(cuenta, ID);
		} else if (!salir(opcion) && opcion != "") {
			opcionInvalida();
		}
	}

	volverCarpetaAnt();
}

void crearLista(Cuenta& cuenta) {
	const int ID = cuenta.getCantListas();
	const string titulo;
	string nombreLista;

	encabezado(titulo);
	cout << "\tIngrese un nombre para la nueva lista: ";
	getline(cin, nombreLista);
	if (salir(nombreLista)) {
		return;
	}

	encabezado(titulo);
	cout << "\tLa lista <" << nombreLista << "> se ha creado con exito!\n";
	cargando();
	registrarListaBD(nombreLista, ID);
}

void configuracionAvanzada(Cuenta& cuenta, const int& ID) {
	const string titulo = "Configuracion avanzada";
	string opcion;

	while (opcion != "X" && opcion != "x") {
		encabezado(titulo);
		cout << "\tEscriba la letra de la opcion que desee. \n\n";
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
			
		} else if (!salir(opcion) && opcion != "") {
			opcionInvalida();
		}
	}
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

	if (!cambiarUsuarioBD(usuario)) {
		cout << "Ha ocurrido un error al intentar cambiar su nombre de usuario\n";
		mensajeError(tecla);
		return;
	}
	cuenta.cambiarUsuario(usuario);
}

void cambiarClave(Cuenta& cuenta, const int& ID) {
	const int esc = 27;
	const string titulo = "Cambiar clave";
	string usuario, clave, nuevaClave1, nuevaClave2;
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
			getline(cin, nuevaClave1);
			if (salir(clave)) {
				return;
			}

			if (datoValido(clave)) {
				cout << "\tVuelva a ingresar su nueva clave: ";
				getline(cin, nuevaClave2);
				if (salir(nuevaClave2)) {
					return;
				}
				
				if (nuevaClave1 == nuevaClave2) {
					encabezado(titulo);
					cout << "\tSu clave ha sido cambiada correctamente!\n";
					cargando();
					cambioClave = true;
					
				} else {
					cout << "\tLas claves no son iguales\n";
					mensajeError(tecla);
				}

			} else {
				cout << "\tLa clave no puede tener menos de 4 o mas de 30 caracteres y tampoco ser solo espacios en blanco\n";
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

	if (!cambiarClaveBD(nuevaClave1)) {
		cout << "Ha ocurrido un error al intentar cambiar su clave\n";
		mensajeError(tecla);
		return;
	}
	cuenta.cambiarClave(nuevaClave1);
}

void interfazLista(Cuenta& cuenta, const int& listaID) {
	if (!accederCarpeta(to_string(listaID))) {
		cout << "Ha ocurrido un error al intentar acceder a la lista, por favor vuelva a intentarlo\n";
		cargando();
		return;
	}

	const string titulo = "Ver lista";
	string opcion;

	while (opcion != "X" && opcion != "x") {
		// actualizarTareas();
		encabezado(titulo);
		cout << "\tMostrado la lista <" << cuenta.getNombreLista(listaID) << ">\n";
		cout << "\tEscriba el numero de alguna tarea para ver mas opciones, o la letra de cualquiera de las opciones\n";

		// 

		separador();
		cout << "\t[A]: Cambiar nombre\n";
		cout << "\t[S]: Vaciar lista\n";
		cout << "\t[D]: Eliminar lista\n";
		cout << "\t[X]: Volver\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {

		} else if (opcion == "S" || opcion == "s") {

		} else if (opcion == "D" || opcion == "d") {

		} else if (!salir(opcion) && opcion != "") {
			opcionInvalida();
		}
	}

	volverCarpetaAnt();
}