#include "formato.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

string Formato::fechaHoraToString(time_t tiempo) {
    char buffer[30];
    struct tm* timeinfo = localtime(&tiempo);
    if (timeinfo) {
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", timeinfo);
        return string(buffer);
    }
    return "Fecha invalida";
}

void Formato::aMayusculas(char* texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper(texto[i]);
    }
}

void Formato::imprimirCentrado(string texto, int ancho) {
    int len = texto.length();
    int padding = (ancho - len) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << texto << endl;
}