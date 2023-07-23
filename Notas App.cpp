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

void encabezado();
void mensajeError(char&);
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

int main() {
	system("color 07");
	ActualizarBD();
	
	vector<Cuenta> cuentas; 
	char opcion; 
	const int esc = 27;

	while (true) {
		actualizarCuentas(cuentas);
		encabezado();
		cout << "\t[A]: Iniciar sesion \n";
		cout << "\t[S]: No tiene una cuenta? (registrarse) \n";
		cout << "\t[X]: Salir \n";
		cout << "\t-> ";
		opcion = getch();

		if (opcion == 'A' || opcion == 'a') {
			iniciarSesion(cuentas);

		} else if (opcion == 'S' || opcion == 's') {
			registrarse(cuentas);

		} else if (opcion == 'X' || opcion == 'x' || opcion == esc) {
			break;
		}
	}

	return 0;
}

void encabezado() {
    char upLeft = 201, horizontal = 205, upRigth = 187, vertical = 186, downLeft = 200, downRight = 188, diamond = 4, punto = 250, linea = 22;
    string title = "Notas App";
    const int titleSize = title.size(), styleLenght = 32;

    system("cls");
	// Primera linea
    cout << "\t" << upLeft;
    for (int i = 0; i < (styleLenght + titleSize)  ; i++) {
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
    for (int i = 0; i < 24 + titleSize; i++) {
        cout << linea;
    }
    cout << " " << diamond << "  " << vertical;

    cout << "\t" << vertical << " o volver   " << vertical << "\n";

	// Cuarta linea
    cout << "\t" << downLeft;
    for (int i = 0; i < (styleLenght + titleSize); i++) {
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

void mensajeError(char& tecla) {
	cout << "\n\tPresione ESC si desea volver a la interfaz anterior\n";
	cout << "\tPresione cualquier tecla para volver a intentarlo\n";
	tecla = getch();
}

bool salir(const string& opcion) {
	if (opcion == "X" || opcion == "x") {
		return true;
	}
	return false;
}

void iniciarSesion(vector<Cuenta>& cuentas) {
	const int esc = 27, intentosMax = 3;
	int intentos = 0, ID;
	char tecla = 0;
	string usuario, clave;
	bool accesoConcedido = false;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && intentos < intentosMax && !accesoConcedido) {
		encabezado();
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
			encabezado();
			if (cuentas[ID].credencialesCorrectas(usuario, clave)) {
				cout << "\tHa iniciado sesion como <" << usuario << "> \n";
				getch();
				accesoConcedido = true;

			} else {
				cout << "\tLa clave es incorrecta, vuelva a intentarlo (" << intentosMax - intentos << " intentos restantes)\n";
				mensajeError(tecla);
			}

		} else {
			encabezado();
			cout << "\tNo existe una cuenta con el nombre <" << usuario << ">, verifique si esta bien escrito\n";
			mensajeError(tecla);
		}
	}

	if (!accesoConcedido) {
		return;
	}

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

// Error de llamadas infinitas con la funcion actualizarCuentas
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
	char tecla = 0;
	string usuario, clave;

	while (tecla != esc && tecla != 'X' && tecla != 'x') {
		encabezado();
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
					encabezado();
					cout << "\tLa cuenta <" << usuario << "> se ha registrado con exito!\n";
					cout << "\tAhora puede iniciar sesion\n";
					getch();
					return;
				} else {
					cout << "\tHa ocurrido un error al intentar registrar su cuenta\n";
					mensajeError(tecla);
				}

			} else {
				encabezado();
				cout << "\tLa clave no puede tener menos de 4 caracteres o ser solo espacios en blanco\n";
				mensajeError(tecla);
			}

		} else {
			mensajeError(tecla);
		}
	}
}

bool usuarioValido(vector<Cuenta>& cuentas, const string& usuario) {
	if (!datoValido(usuario)) {
		encabezado();
		cout << "\tEl nombre de usuario no puede tener menos de 4 caracteres o ser solo espacios en blanco\n";
		return false;
	}

	if (cuentaRegistrada(cuentas, usuario)) {
		encabezado();
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
		cout << "Ha ocurrido un error al intentar acceder a su cuenta, vuelva a intentarlo\n";
		getch();
		return;
	}

	string opcion, nombreLista;
	bool temaClaro = false;

	while (opcion != "X" && opcion != "x") {
		actualizarListas(cuenta);
		encabezado();
		cout << "\tBienvenido! <" << cuenta.getUsuario() << ">, es un gusto tenerlo aqui nuevamente.\n";
		cout << "\tEscriba el numero de alguna lista para acceder o la letra de cualquiera de las opciones.\n\n";

		if (cuenta.getCantListas() == 0) {
			cout << "\tActualmente no tiene ninguna lista creada!\n";
		}
		cuenta.mostrarListas();

		cout << "\t[A]: Crear una lista\n";
		cout << "\t[S]: Combinar listas\n";
		cout << "\t[D]: Configuracion avanzada\n";
		cout << "\t[X]: Salir de la cuenta\n";
		cout << "\t-> ";
		getline(cin, opcion);

		if (opcion == "A" || opcion == "a") {
			crearLista(cuenta);
		} else if (opcion == "S" || opcion == "s") {

		} else if (opcion == "D" || opcion == "d") {
			configuracionAvanzada(cuenta, ID);
		}
	}

	volverCarpetaAnt();
}

void crearLista(Cuenta& cuenta) {
	string nombreLista;
	const int ID = cuenta.getCantListas();

	encabezado();
	cout << "\tIngrese un nombre para la nueva lista: ";
	getline(cin, nombreLista);
	if (salir(nombreLista)) {
		return;
	}

	registrarListaBD(nombreLista, ID);
}

void configuracionAvanzada(Cuenta& cuenta, const int& ID) {
	string opcion;

	while (opcion != "X" && opcion != "x") {
		encabezado();
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

		} else if (opcion == "D" || opcion == "d") {
			
		}
	}
}

void cambiarUsuario(Cuenta& cuenta, const int& ID) {
	const int esc = 27;
	char tecla = 0;
	string usuario, clave;
	bool cambioNombre = false;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && !cambioNombre) {
		encabezado();
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

			encabezado();
			if (cuenta.credencialesCorrectas(cuenta.getUsuario() ,clave)) {
				cout << "\tSu nuevo nombre de usuario es <" << usuario << "> \n";
				getch();
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
		cout << "Ha ocurrido un error al intentar cambiar su nombre de usuario, \n";
		mensajeError(tecla);
		return;
	}
	cuenta.cambiarUsuario(usuario);
}