#ifndef Mensajes_H
#define Mensajes_H

#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

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

#endif