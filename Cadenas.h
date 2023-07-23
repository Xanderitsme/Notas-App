#ifndef Cadenas_h
#define Cadenas_h

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;

bool datoValido(const string& cadena) {
    if (cadena.length() < 4) {
        return false;
    }
    bool soloEspacios = all_of(cadena.begin(), cadena.end(), [](char c) {
        return isspace(static_cast<unsigned char>(c));
    });
    if (soloEspacios) {
        return false;
    }
    return true;
}

bool convertirStringInt(const string& cadena, int& numero) {
    for (char caracter : cadena) {
        if (!isdigit(caracter)) {
            return false;
        }
    }
    
    numero = stoi(cadena);
    return true;
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