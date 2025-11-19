#ifndef FORMATO_HPP
#define FORMATO_HPP

#include <string>
#include <ctime>

using namespace std;

class Formato {
public:
    // Convierte time_t a string "DD/MM/AAAA HH:MM"
    static string fechaHoraToString(time_t tiempo);
    
    // Convierte todo a mayúsculas
    static void aMayusculas(char* texto);
    
    // Centrar un texto para los menús
    static void imprimirCentrado(string texto, int ancho);
};

#endif