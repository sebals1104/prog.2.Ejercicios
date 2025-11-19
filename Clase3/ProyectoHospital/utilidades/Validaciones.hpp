#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <string>
#include <iostream>
#include <limits>

using namespace std;

class Validaciones {
public:
    // --- Validaciones Numéricas ---
    static int leerEntero(const string& mensaje, int min, int max);
    static float leerFloat(const string& mensaje, float min, float max);

    // --- Validaciones de Texto ---
    // Pide input hasta que solo contenga letras (y espacios)
    static void leerSoloLetras(const string& mensaje, char* buffer, int maxLen);
    
    // Pide input hasta que solo contenga números (para cédula o teléfono como string)
    static void leerCadenaNumerica(const string& mensaje, char* buffer, int maxLen);

    // --- Validaciones de Formato ---
    // Pide input hasta que tenga formato de email (x@y.z)
    static void leerEmail(const string& mensaje, char* buffer, int maxLen);
    
    // Pide input hasta que sea un tipo de sangre válido (A+, O-, etc.)
    static void leerTipoSangre(const string& mensaje, char* buffer);
    
    // Pide fecha hasta que sea válida (DD/MM/AAAA)
    static void leerFecha(const string& mensaje, char* buffer);

    // --- Auxiliares Internos (Lógica pura) ---
    static bool esFechaValida(const char* fecha);
    static bool esEmailValido(const string& email);
    static bool esSoloLetras(const string& texto);
    static bool esSoloNumeros(const string& texto);
    static bool esTipoSangreValido(string tipo);

    // --- Utilidades de Sistema ---
    static void pausar();
    static void limpiarPantalla();
};

#endif