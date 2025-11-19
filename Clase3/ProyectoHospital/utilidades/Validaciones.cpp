#include "Validaciones.hpp"
#include <cstring>
#include <cctype>
#include <cstdio>

// ================= NUMÉRICOS =================

int Validaciones::leerEntero(const string& mensaje, int min, int max) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return valor;
            }
            cout << ">> Error: El valor debe estar entre " << min << " y " << max << ".\n";
        } else {
            cout << ">> Error: Ingrese solo numeros enteros.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

float Validaciones::leerFloat(const string& mensaje, float min, float max) {
    float valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return valor;
            }
            cout << ">> Error: El valor debe estar entre " << min << " y " << max << ".\n";
        } else {
            cout << ">> Error: Ingrese solo numeros.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// ================= TEXTO (Letras y Números) =================

void Validaciones::leerSoloLetras(const string& mensaje, char* buffer, int maxLen) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);
        
        if (entrada.empty()) continue;
        if (entrada.length() >= (size_t)maxLen) {
            cout << ">> Texto muy largo (max " << maxLen - 1 << ").\n";
            continue;
        }

        if (esSoloLetras(entrada)) {
            strcpy(buffer, entrada.c_str());
            return;
        }
        cout << ">> Error: Solo se permiten letras y espacios.\n";
    }
}

void Validaciones::leerCadenaNumerica(const string& mensaje, char* buffer, int maxLen) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);
        
        if (entrada.empty()) continue;
        if (entrada.length() >= (size_t)maxLen) {
            cout << ">> Texto muy largo (max " << maxLen - 1 << ").\n";
            continue;
        }

        if (esSoloNumeros(entrada)) {
            strcpy(buffer, entrada.c_str());
            return;
        }
        cout << ">> Error: Solo se permiten digitos numericos.\n";
    }
}

// ================= FORMATOS ESPECÍFICOS =================

void Validaciones::leerEmail(const string& mensaje, char* buffer, int maxLen) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);
        
        if (entrada.length() >= (size_t)maxLen) {
            cout << ">> Email muy largo.\n"; 
            continue;
        }
        
        if (esEmailValido(entrada)) {
            strcpy(buffer, entrada.c_str());
            return;
        }
        cout << ">> Error: Formato de email invalido (falta @ o punto).\n";
    }
}

void Validaciones::leerTipoSangre(const string& mensaje, char* buffer) {
    string entrada;
    while(true) {
        cout << mensaje;
        getline(cin, entrada);
        
        // Convertir a mayúsculas para normalizar
        for(char &c : entrada) c = toupper(c);
        
        if (esTipoSangreValido(entrada)) {
            strcpy(buffer, entrada.c_str());
            return;
        }
        cout << ">> Error: Tipo de sangre invalido. Use: A+, O-, AB+, etc.\n";
    }
}

void Validaciones::leerFecha(const string& mensaje, char* buffer) {
    char entrada[20];
    while (true) {
        cout << mensaje;
        cin.getline(entrada, 20);
        
        if (esFechaValida(entrada)) {
            strcpy(buffer, entrada);
            return;
        }
        cout << ">> Error: Fecha invalida o formato incorrecto (DD/MM/AAAA).\n";
    }
}


// ================= LÓGICA INTERNA (Privada/Helper) =================

bool Validaciones::esSoloLetras(const string& texto) {
    for (char c : texto) {
        if (!isalpha(c) && !isspace(c)) return false;
    }
    return true;
}

bool Validaciones::esSoloNumeros(const string& texto) {
    for (char c : texto) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool Validaciones::esEmailValido(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos);
}

bool Validaciones::esTipoSangreValido(string tipo) {
    if (tipo == "A+" || tipo == "A-" || 
        tipo == "B+" || tipo == "B-" || 
        tipo == "AB+" || tipo == "AB-" || 
        tipo == "O+" || tipo == "O-") {
        return true;
    }
    return false;
}

bool Validaciones::esFechaValida(const char* fecha) {
    int dia, mes, anio;
    if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) != 3) return false;
    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    
    int diasMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) diasMes[2] = 29;
    
    return dia >= 1 && dia <= diasMes[mes];
}

void Validaciones::leerHorarioTexto(const string& mensaje, char* buffer, int maxLen) {
    string entrada;
    while (true) {
        cout << mensaje;
        getline(cin, entrada);

        // 1. Validar longitud
        if (entrada.empty()) {
            cout << ">> Error: El horario no puede estar vacio.\n";
            continue;
        }
        if (entrada.length() >= (size_t)maxLen) {
            cout << ">> Error: Texto muy largo.\n";
            continue;
        }

        // 2. Validar contenido
        if (esHorarioTextoValido(entrada)) {
            strcpy(buffer, entrada.c_str());
            return;
        }
        cout << ">> Error: Formato invalido. Solo use numeros, guiones y 'am'/'pm'.\n";
    }
}

bool Validaciones::esHorarioTextoValido(const string& texto) {
    bool tieneNumero = false;
    
    for (char c : texto) {
        // Si es un dígito, espacio o símbolo común (- : .), todo bien
        if (isdigit(c) || c == ' ' || c == '-' || c == ':' || c == '.') {
            if (isdigit(c)) tieneNumero = true;
            continue;
        }
        
        // Si es una letra, SOLO permitimos a, p, m (mayus o minus)
        if (isalpha(c)) {
            char letra = tolower(c);
            if (letra != 'a' && letra != 'p' && letra != 'm') {
                return false; // Encontró una letra prohibida (ej: 'h' de horas)
            }
        } else {
            // Si es un símbolo raro (ej: $, %, &), rechazamos
            return false;
        }
    }
    
    // Debe tener al menos un número para ser un horario lógico
    return tieneNumero;
}

void Validaciones::leerHoraMilitar(const string& mensaje, char* buffer) {
    char entrada[50];
    while (true) {
        cout << mensaje; 
        // Usamos cin.getline para limpiar el buffer correctamente
        if (!cin.getline(entrada, 50)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (esHoraMilitarValida(entrada)) {
            strcpy(buffer, entrada);
            return;
        }
        cout << ">> Error: Hora invalida. Use formato militar HH:MM (ej: 14:30).\n";
    }
}

bool Validaciones::esHoraMilitarValida(const char* hora) {
    int h, m;
    
    // sscanf busca dos enteros separados por dos puntos ':'
    // %2d limita la lectura a 2 dígitos por número
    if (sscanf(hora, "%2d:%2d", &h, &m) != 2) {
        return false; // No coincide con el formato
    }
    
    // Validar rangos lógicos de reloj 24h
    if (h < 0 || h > 23) return false;
    if (m < 0 || m > 59) return false;
    
    return true;
}

// ================= SISTEMA =================

void Validaciones::pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Validaciones::limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}