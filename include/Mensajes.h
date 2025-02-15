#ifndef Mensajes_h
#define Mensajes_h

#include <iostream>
#include <string>
#include "system_utils.h"

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include "linux_conio.h"
#endif
using namespace std;

// Definición de caracteres para dibujar cuadros
namespace box {
    #ifdef _WIN32
        // Caracteres para Windows (código página 437)
        const char HORIZONTAL = 205;  // ═
        const char VERTICAL = 186;    // ║
        const char TOP_LEFT = 201;    // ╔
        const char TOP_RIGHT = 187;   // ╗
        const char BOTTOM_LEFT = 200; // ╚
        const char BOTTOM_RIGHT = 188;// ╝
        const char LINE = 22;         // ▬
    #else
        // Caracteres Unicode para Linux
        const std::string HORIZONTAL = "═";
        const std::string VERTICAL = "║";
        const std::string TOP_LEFT = "╔";
        const std::string TOP_RIGHT = "╗";
        const std::string BOTTOM_LEFT = "╚";
        const std::string BOTTOM_RIGHT = "╝";
        const std::string LINE = "─";
    #endif
}

void encabezado(const string& titulo) {
    string title = "Notas App", tituloMod = titulo;
    const int styleLenght = 32, bordes = 10;

    if (titulo.size() > styleLenght + title.size() - bordes) {
        tituloMod = titulo.substr(0, styleLenght + title.size() - bordes - 1) + "...";
    } else if (titulo.size() > 0){
        tituloMod += " ";
    }

    sys::clearScreen();
    
    // Primera linea
    std::cout << "\t" << box::TOP_LEFT;
    for (int i = 0; i < (styleLenght + title.size()); i++) {
        std::cout << box::HORIZONTAL;
    }
    std::cout << box::TOP_RIGHT << std::flush;

    std::cout << "\t" << box::VERTICAL << " Escriba X  " << box::VERTICAL << "\n" << std::flush;

    // Segunda linea
    std::cout << "\t" << box::VERTICAL << "  ♦   <  < << ";
    std::cout << "· " << title << " ·";
    std::cout << " >> >  >   ♦  " << box::VERTICAL << std::flush;

    std::cout << "\t" << box::VERTICAL << " para salir " << box::VERTICAL << "\n" << std::flush;

    // Tercera linea
    std::cout << "\t" << box::VERTICAL << "  ♦ ";
    std::cout << tituloMod;
    for (int i = 0; i < 24 + title.size() - tituloMod.size(); i++) {
        std::cout << box::LINE;
    }
    std::cout << " ♦  " << box::VERTICAL << std::flush;

    std::cout << "\t" << box::VERTICAL << " o volver   " << box::VERTICAL << "\n" << std::flush;

    // Cuarta linea
    std::cout << "\t" << box::BOTTOM_LEFT;
    for (int i = 0; i < (styleLenght + title.size()); i++) {
        std::cout << box::HORIZONTAL;
    }
    std::cout << box::BOTTOM_RIGHT << std::flush;

    std::cout << "\t" << box::BOTTOM_LEFT;
    for (int i = 0; i < 12; i++) {
        std::cout << box::HORIZONTAL;
    }
    std::cout << box::BOTTOM_RIGHT << "\n\n" << std::flush;
}

void separador() {
    std::cout << "\n\t";
    #ifdef _WIN32
        for (int i = 0; i < 45; i++) {
            std::cout << box::LINE << " ";
        }
    #else
        for (int i = 0; i < 45; i++) {
            std::cout << box::LINE;
        }
    #endif
    std::cout << "\n\n" << std::flush;
}

void mensajeError(char& tecla) {
    std::cout << "\n\tPresione ESC si desea volver a la interfaz anterior\n";
    std::cout << "\tPresione cualquier tecla para volver a intentarlo\n" << std::flush;
    #ifdef _WIN32
        tecla = _getch();
    #else
        fflush(stdout);
        tecla = getch();
    #endif
}

bool salir(const string& opcion) {
    return (opcion == "X" || opcion == "x");
}

void limpiarBuffer() {
    #ifdef _WIN32
        while (_kbhit()) {
            _getch();
        }
    #else
        fflush(stdout);
        while (getchar() != '\n');
    #endif
}

void opcionInvalida() {
    const int milisegundos = 1500, cantidadPuntos = 3;
    const int frame = milisegundos / (cantidadPuntos + 1);
    
    encabezado("");
    std::cout << "\tLa opcion seleccionada no existe, por favor vuelva a intentarlo" << std::flush;
    
    for (int i = 0; i < cantidadPuntos; i++) {
        Sleep(frame);
        std::cout << " ." << std::flush;
    }
    
    Sleep(frame);  // Pausa final para que el mensaje sea visible
    
    #ifdef _WIN32
        while (_kbhit()) _getch();  // Limpiar cualquier entrada pendiente
    #else
        fflush(stdout);
        tcflush(STDIN_FILENO, TCIFLUSH);  // Limpiar buffer de entrada en Linux
    #endif
}

void cargando() {
    std::cout << "\n\t\tCargando" << std::flush;
    
    const int milisegundos = 1500;
    const int frame = milisegundos / 4;
    
    for (int i = 0; i < 3; i++) {
        Sleep(frame);
        std::cout << " ." << std::flush;
    }
    
    Sleep(frame);  // Pausa final para completar la animación
    std::cout << "\n" << std::flush;
    
    #ifdef _WIN32
        while (_kbhit()) _getch();
    #else
        fflush(stdout);
        tcflush(STDIN_FILENO, TCIFLUSH);
    #endif
}

#endif