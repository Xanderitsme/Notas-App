#ifndef SYSTEM_UTILS_H
#define SYSTEM_UTILS_H

#include <iostream>
#include <limits>       // Para std::numeric_limits
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/ioctl.h>    // Para ioctl y TIOCGWINSZ
    #include <termios.h>
    #include <sys/types.h>
    #include <term.h>
#endif

namespace sys {
    // Limpiar la pantalla
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            // Primero intentamos usando el comando clear
            if (system("clear") != 0) {
                // Si falla, usamos secuencias ANSI
                std::cout << "\033[2J\033[1;1H";
            }
        #endif
    }

    // Pausar la ejecución
    void pause() {
        #ifdef _WIN32
            system("pause");
        #else
            std::cout << "Presione ENTER para continuar...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        #endif
    }

    // Establecer color de la consola
    void setConsoleColor(const std::string& color = "07") {
        #ifdef _WIN32
            system(("color " + color).c_str());
        #else
            // Mapeo básico de colores de Windows a códigos ANSI
            if (color == "07") {
                std::cout << "\033[0;37m"; // Texto gris claro
            } else if (color == "0F") {
                std::cout << "\033[1;37m"; // Texto blanco brillante
            } else if (color == "0A") {
                std::cout << "\033[0;32m"; // Texto verde
            } else if (color == "0C") {
                std::cout << "\033[0;31m"; // Texto rojo
            } else {
                std::cout << "\033[0m"; // Restablecer a los colores por defecto
            }
        #endif
    }

    // Restablecer color de la consola
    void resetConsoleColor() {
        #ifdef _WIN32
            system("color 07");
        #else
            std::cout << "\033[0m";
        #endif
    }

    // Obtener el ancho de la consola
    int getConsoleWidth() {
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        #else
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            return w.ws_col;
        #endif
    }

    // Obtener el alto de la consola
    int getConsoleHeight() {
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        #else
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            return w.ws_row;
        #endif
    }

    // Función para leer un carácter sin eco y sin buffer
    char getChar() {
        char ch;
        #ifdef _WIN32
            ch = _getch();
        #else
            fflush(stdout);
            ch = getch();
        #endif
        return ch;
    }

    // Función para mostrar un prompt y obtener una opción
    char getOption(const std::string& prompt = "") {
        if (!prompt.empty()) {
            std::cout << prompt << std::flush;
        }
        return getChar();
    }

    // Función para mostrar texto con animación
    void animateText(const std::string& text, int delayMs = 500, bool newLine = true) {
        std::cout << text << std::flush;
        Sleep(delayMs);
        if (newLine) std::cout << std::endl;
    }

    // Función específica para mostrar una animación de carga
    void showLoading(const std::string& message = "Cargando", 
                    int dots = 3, 
                    int totalTimeMs = 1500) {
        const int frameTime = totalTimeMs / (dots + 1);
        
        // Mostrar mensaje inicial
        std::cout << "\n\t\t" << message << " " << std::flush;
        Sleep(frameTime);
        
        // Mostrar los puntos con delay
        for (int i = 0; i < dots; i++) {
            std::cout << ". " << std::flush;
            Sleep(frameTime);
        }
        
        // Limpiar el buffer
        #ifdef _WIN32
            while (_kbhit()) _getch();
        #else
            fflush(stdout);
        #endif
    }

    class LineEditor {
    private:
        std::string buffer;
        size_t cursorPos = 0;

        // Códigos de teclas especiales
        static const char ESC = 27;
        static const char BACKSPACE = 127;
        
    public:
        std::string getLine() {
            buffer.clear();
            cursorPos = 0;

            #ifdef _WIN32
                // En Windows usamos la entrada estándar
                std::getline(std::cin, buffer);
                return buffer;
            #else
                // Configurar el modo raw en Linux
                struct termios old_tio, new_tio;
                tcgetattr(STDIN_FILENO, &old_tio);
                new_tio = old_tio;
                new_tio.c_lflag &= (~ICANON & ~ECHO);
                tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

                while (true) {
                    char c = getch();

                    if (c == '\n') {
                        std::cout << '\n';
                        break;
                    }
                    else if (c == BACKSPACE || c == 8) {  // Backspace
                        if (!buffer.empty() && cursorPos > 0) {
                            buffer.erase(--cursorPos, 1);
                            std::cout << "\b \b" << std::flush;
                        }
                    }
                    else if (c == ESC) {
                        char next = getch();
                        if (next == '[') {
                            char arrow = getch();
                            switch (arrow) {
                                case 'D':  // Flecha izquierda
                                    if (cursorPos > 0) {
                                        cursorPos--;
                                        std::cout << "\b" << std::flush;
                                    }
                                    break;
                                case 'C':  // Flecha derecha
                                    if (cursorPos < buffer.length()) {
                                        cursorPos++;
                                        std::cout << "\033[1C" << std::flush;
                                    }
                                    break;
                            }
                        }
                    }
                    else if (isprint(c)) {  // Caracteres imprimibles
                        if (cursorPos == buffer.length()) {
                            buffer += c;
                            cursorPos++;
                            std::cout << c << std::flush;
                        }
                        else {
                            buffer.insert(cursorPos++, 1, c);
                            std::cout << c << std::flush;
                        }
                    }
                }

                // Restaurar el modo original de la terminal
                tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
                return buffer;
            #endif
        }
    };

    // Función helper para obtener una línea de texto
    std::string getInputLine(const std::string& prompt = "") {
        if (!prompt.empty()) {
            std::cout << prompt << std::flush;
        }
        LineEditor editor;
        return editor.getLine();
    }
}

#endif // SYSTEM_UTILS_H 