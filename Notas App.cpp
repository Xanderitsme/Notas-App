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
void iniciarSesion(vector<Cuenta>&);
void registrarse(vector<Cuenta>&);
int getID(vector<Cuenta>&, const string&);
bool cuentaRegistrada(vector<Cuenta>&, const string&);
bool usuarioValido(vector<Cuenta>&, const string&);
bool registrarCuentaBD(const string&, const string&, const int&);

void interfazCuenta(Cuenta&);

int main() {
	ActualizarBD();
	
	vector<Cuenta> cuentas; 
	char opcion; 
	const int esc = 27;

	while (true) {
		ActualizarCuentas(cuentas);
		encabezado();
		cout << "\t[1]: Iniciar sesion \n";
		cout << "\t[2]: No tiene una cuenta? (registrarse) \n";
		cout << "\t[X]: Salir \n";
		cout << "\t-> ";
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

void encabezado() {
    char upLeft = 201, horizontal = 205, upRigth = 187, vertical = 186, downLeft = 200, downRight = 188, diamond = 4, line = 250;
    string title = "Notas App";
    int titleSize = title.size(), styleLenght = 32;

    system("cls");
	// Primera linea
    cout << "\t" << upLeft;
    for (int i = 0; i < (styleLenght + titleSize)  ; i++) {
        cout << horizontal;
    }
    cout << upRigth << "\n";
	// Segunda linea
    cout << "\t" << vertical << "  " << diamond << "   <  < << ";
    cout << line << " " << title << " " << line;
    cout << " >> >  >   " << diamond << "  " << vertical << "\n";
	// Tercera linea
    cout << "\t" << downLeft;
    for (int i = 0; i < (styleLenght + titleSize); i++) {
        cout << horizontal;
    }
    cout << downRight << "\n\n";

	// Previsualizacion
	// ╔═════════════════════════════════════════╗
	// ║  ♦   <  < << · Notas App · >> >  >   ♦  ║
	// ╚═════════════════════════════════════════╝
}

void mensajeError(char& tecla) {
	cout << "\n\tPresione ESC si desea volver a la interfaz anterior\n";
	cout << "\tPresione cualquier tecla para volver a intentarlo\n";
	tecla = getch();
}

void iniciarSesion(vector<Cuenta>& cuentas) {
	const int esc = 27, intentosMax = 3;
	int intentos = 0, ID;
	char tecla = 0;
	string usuario, clave;

	while (tecla != esc && tecla != 'X' && tecla != 'x' && intentos < intentosMax) {
		encabezado();
		cout << "\tIngrese su nombre de usuario: ";
		getline(cin, usuario);
		if (cuentaRegistrada(cuentas, usuario)) {
			cout << "\tIngrese su clave: ";
			getline(cin, clave);
			intentos++;
			ID = getID(cuentas, usuario);
			if (cuentas[ID].credencialesCorrectas(usuario, clave)) {
				encabezado();
				cout << "\tHa iniciado sesion como <" << usuario << "> \n";
				getch();
				interfazCuenta(cuentas[ID]);

			} else {
				encabezado();
				cout << "\tLa clave es incorrecta, vuelva a intentarlo (" << intentosMax - intentos << " intento restantes)\n";
				mensajeError(tecla);
			}

		} else {
			encabezado();
			cout << "\tNo existe una cuenta con el nombre <" << usuario << ">, verifique si esta bien escrito\n";
			mensajeError(tecla);
		}
	}
}

void registrarse(vector<Cuenta>& cuentas) {
	const int esc = 27;
	char tecla = 0;
	string usuario, clave;

	while (tecla != esc && tecla != 'X' && tecla != 'x') {
		encabezado();
		cout << "\tCree su nombre de usuario: ";
		getline(cin, usuario);
		if (usuarioValido(cuentas, usuario)) {
			cout << "\tCree su clave: ";
			getline(cin, clave);
			if (datoValido(clave)) {
				if (registrarCuentaBD(usuario, clave, cuentas.size())) {
					encabezado();
					cout << "\tLa cuenta <" << usuario << "> se ha registrado con exito!\n";
					cout << "\tAhora puede acceder a su cuenta\n";
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

void interfazCuenta(Cuenta& cuenta) {
	
}