#ifndef Cadenas_h
#define Cadenas_h

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <stdexcept>
using namespace std;

bool datoValido(const string& cadena) {
    if (cadena.size() < 4 || cadena.size() > 30) {
        return false;
    }

    bool soloEspacios = true;
    for (char caracter : cadena) {
        if (caracter != ' ') {
            soloEspacios = false;
            break;
        }
    }

    if (soloEspacios) {
        return false;
    }

    return true;
}

bool convertirStringInt(const string& cadena, int& numero) {
    if (cadena.size() > 9) {
        return false;
    }

    for (char caracter : cadena) {
        if (!isdigit(caracter)) {
            return false;
        }
    }

    try {
        numero = stoi(cadena);	
    } catch (const exception& e) {
		return false;
    }
    
    return true;
}

void recortarExtension(vector<string>& cadenas) {
    int indice;
    bool recortar;

    for (auto& cadena : cadenas) {
        indice = 0;
        recortar = false;
        for (char caracter : cadena) {
            if (caracter == '.') {
                recortar = true;
                break;
            }
            indice++;
        }

        if (recortar) {
            cadena = cadena.substr(0, indice);
        }
    }
}

void agregarExtensionTXT(vector<string>& cadenas) {
    const string extension = ".txt";
    for (auto& cadena : cadenas) {
        cadena+= extension;
    }
}

void ordenarVector(vector<string>& cadenas) {
    vector<int> numeros;
    int aux;
    for (const auto& cadena : cadenas) {
        numeros.push_back(stoi(cadena));
    }

    for (int i = 1; i < numeros.size(); i++) {
        for (int j = i; j > 0; j--) {
                if (numeros[j] < numeros[j-1]) {
                aux = numeros[j];
                numeros[j] = numeros[j-1];
                numeros[j-1] = aux;
            }
        }   
    }

    for (int i = 0; i < numeros.size(); i++) {
        cadenas[i] = to_string(numeros[i]);
    }
}

#endif