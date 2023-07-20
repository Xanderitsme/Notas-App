#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "Carpetas.h"
#include "Archivos.h"
#include "Cuenta.h"
#include "ControlBD.h"
using namespace std;

void encabezado();
void iniciarSesion(vector<Cuenta>&);
void registrarse(vector<Cuenta>&);

int main() {
	ActualizarBD();
	
	vector<Cuenta> cuentas; 
	char opcion; 
	const int esc = 27;

	while (true) {
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
    cout << "\t" << upLeft;
    for (int i = 0; i < (styleLenght + titleSize)  ; i++) {
        cout << horizontal;
    }
    cout << upRigth << "\n";

    cout << "\t" << vertical << "  " << diamond << "   <  < << ";
    cout << line << " " << title << " " << line;
    cout << " >> >  >   " << diamond << "  " << vertical << "\n";

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

void iniciarSesion(vector<Cuenta>& cuentas) {

}

void registrarse(vector<Cuenta>& cuentas) {
	encabezado();
	string usuario, clave;
	cout << "\tIngrese su nombre de usuario: ";
	getline(cin, usuario);
	cout << "\tIngrese su clave: ";
	getline(cin, clave);

	cuentas.push_back(Cuenta(usuario, clave));
	
	// parece que al crear un objeto este se elimina automáticamente xd
	// bueno eso no es un problema pero las carpetas también se eliminan, lo cual no permite el almacenamiento fuera del programa
	encabezado();
	cout << "\tSe ha creado el usuario <" << usuario << ">\n";
	system("pause");

	encabezado();
	cout << "\tCreando el registro en la BD\n";
	if (cuentas[cuentas.size() - 1].guardarRegistro(cuentas.size() - 1)) {
		cout << "\tLa cuenta se ha registrado con exito\n";	
	} else {
		cout << "\tHubo un error\n";
	}

	system("pause");
}
