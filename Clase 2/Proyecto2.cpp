#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;

struct Hospital;
struct Doctor;
struct Cita;
struct Paciente;
struct HistorialMedico;

struct HistorialMedico {
    int idConsulta;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int idDoctor;
    float costo;
};

struct Cita {
    int id;
    int idPaciente;
    int idDoctor;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[21];
    char email[50];
};

struct Paciente {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[21];
    char direccion[100];
    char email[50];
    bool activo;
};

struct Hospital {
    char nombre[100];
    char direccion[200];
    char telefono[21];

    char doctoresFile[260];
    char pacientesFile[260];
    char citasFile[260];
    char historialFile[260];

    int cantidadDoctores;
    int cantidadPacientes;
    int cantidadCitas;
    int siguienteIdPaciente;
    int siguienteIdDoctor;
    int siguienteIdCita;
    int siguienteIdConsulta;
};

//----------------Mandar llamar funciones
bool fileExistsProyecto(const char* path);
bool guardarHospitalHeaderProyecto(const char* path, const Hospital &h);
void crearArchivoVacioSiNoExisteProyecto(const char* path);
Hospital* crearHospitalProyecto(const char* nombre, const char* direccion, const char* telefono);
void mostrarDatosHospitalProyecto(const Hospital* h);
void mostrarMenuPrincipal(Hospital* hospital);
void ValidarSoloLetras(const char* prompt, char* outBuf, int bufSize, int minLen, int maxLen);
int ValidarNumeroEntero(const char* prompt, int minVal, int maxVal);

//----------------Main-----------------

int main() {
    // Ejemplo de creación de hospital
    Hospital* hospital = crearHospitalProyecto("Hospital Central", "Calle Falsa 123", "555-1234");
    mostrarDatosHospitalProyecto(hospital);

    mostrarMenuPrincipal(hospital);


    // Liberar memoria
    free(hospital);
    return 0;
}

//------------------------ Validacones ------------------------

void ValidarSoloLetras(const char* prompt, char* outBuf, int bufSize, int minLen, int maxLen) {
    if (!outBuf || bufSize <= 0) return;
    char tmp[256];
    while (true) {
        cout << prompt;
        if (!cin.getline(tmp, (int)sizeof(tmp))) {
            cin.clear();
            int ch;
            while ((ch = cin.get()) != '\n' && ch != EOF) {}
            cout << "Error de lectura. Intente de nuevo.\n";
            continue;
        }

        // trim izquierda
        int s = 0;
        while (tmp[s] == ' ' || tmp[s] == '\t' || tmp[s] == '\r' || tmp[s] == '\n') ++s;
        // trim derecha
        int e = (int)strlen(tmp) - 1;
        while (e >= s && (tmp[e] == ' ' || tmp[e] == '\t' || tmp[e] == '\r' || tmp[e] == '\n')) --e;
        int len = (e >= s) ? (e - s + 1) : 0;

        if (len < minLen) { cout << "Entrada demasiado corta (min " << minLen << " caracteres).\n"; continue; }
        if (len > maxLen)  { cout << "Entrada demasiado larga (max " << maxLen << " caracteres).\n"; continue; }

        bool ok = true;
        for (int i = s; i <= e; ++i) {
            char c = tmp[i];
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '-' || c == '\'')) {
                ok = false;
                break;
            }
        }
        if (!ok) { cout << "Entrada invalida. Ingrese solo letras, espacios, '-' o '''.\n"; continue; }

        // copiar resultado al buffer de salida (asegurar null-terminator)
        int copyLen = (len < bufSize-1) ? len : bufSize-1;
        for (int i = 0; i < copyLen; ++i) outBuf[i] = tmp[s + i];
        outBuf[copyLen] = '\0';
        return;
    }
}

int ValidarNumeroEntero(const char* prompt, int minVal, int maxVal){
char buf[128];
    while (true) {
        cout << prompt;
        if (!cin.getline(buf, sizeof(buf))) {
            // fallo de lectura (por ej. línea demasiado larga o EOF): limpiar y reintentar
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error de lectura. Intente de nuevo.\n";
            continue;
        }

        // trim izquierda
        int s = 0;
        while (buf[s] == ' ' || buf[s] == '\t' || buf[s] == '\r' || buf[s] == '\n') ++s;
        // trim derecha
        int e = (int)strlen(buf) - 1;
        while (e >= s && (buf[e] == ' ' || buf[e] == '\t' || buf[e] == '\r' || buf[e] == '\n')) --e;
        if (e < s) { cout << "Entrada vacía. Intente de nuevo.\n"; continue; }

        // permitir signo opcional
        int i = s;
        bool neg = false;
        if (buf[i] == '+' || buf[i] == '-') { if (buf[i] == '-') neg = true; ++i; }
        if (i > e) { cout << "Entrada inválida. Ingrese un número entero.\n"; continue; }

        // validar que todos los caracteres restantes sean dígitos
        bool ok = true;
        long long val = 0;
        for (; i <= e; ++i) {
            if (buf[i] < '0' || buf[i] > '9') { ok = false; break; }
            val = val * 10 + (buf[i] - '0');
            if (val > (long long)INT_MAX + 1LL) { ok = false; break; } // overflow
        }
        if (!ok) { cout << "Entrada inválida. Ingrese solo dígitos (y opcionalmente signo).\n"; continue; }

        if (neg) val = -val;
        if (val < minVal || val > maxVal) {
            cout << "Valor fuera de rango (" << minVal << " - " << maxVal << "). Intente de nuevo.\n";
            continue;
        }
        return (int)val;
    }
};

// ---------------- Helpers de fichero y creación del hospital (file-backed) ----------------

bool fileExistsProyecto(const char* path) {
    ifstream f(path, ios::binary);
    return f.good();
}

bool guardarHospitalHeaderProyecto(const char* path, const Hospital &h) {
    ofstream ofs(path, ios::binary | ios::out | ios::trunc);
    if (!ofs) return false;
    ofs.write(reinterpret_cast<const char*>(&h), sizeof(Hospital));
    return ofs.good();
}

void crearArchivoVacioSiNoExisteProyecto(const char* path) {
    if (!fileExistsProyecto(path)) {
        ofstream ofs(path, ios::binary | ios::app);
    }
}


//-------------------- Menus-------------------------

void mostrarMenuPrincipal(Hospital* hospital) {
    int opcion = 0;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║   SISTEMA DE GESTIÓN HOSPITALARIA      ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "4.Mostrar datos hospital" << endl;
        cout << "0.Salir" << endl;
        cout << "Seleccione una opcion: ";
        opcion = ValidarNumeroEntero("Seleccione una opcion: ",0,4);
        switch (opcion)
        {
        case 4:{
            mostrarDatosHospitalProyecto(hospital);
            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
            break;
        }
        default:
            cout << "Opcion invalida. Intente de nuevo.\n";
        }
    }while(opcion != 0);
}

//-------------------- Crear hospital ----------------

Hospital* crearHospitalProyecto(const char* nombre, const char* direccion, const char* telefono) {
    Hospital* h = (Hospital*)malloc(sizeof(Hospital));
    if (!h) return nullptr;
    memset(h, 0, sizeof(Hospital));

    strncpy(h->nombre, nombre, sizeof(h->nombre)-1); h->nombre[sizeof(h->nombre)-1] = '\0';
    strncpy(h->direccion, direccion, sizeof(h->direccion)-1); h->direccion[sizeof(h->direccion)-1] = '\0';
    strncpy(h->telefono, telefono, sizeof(h->telefono)-1); h->telefono[sizeof(h->telefono)-1] = '\0';

    // No reservar arrays: serán accedidos por fichero (persistencia). Mantener punteros null.
    h->cantidadDoctores = 0; h->cantidadPacientes = 0; h->cantidadCitas = 0;
    h->siguienteIdPaciente = 1; h->siguienteIdDoctor = 1; h->siguienteIdCita = 1; h->siguienteIdConsulta = 1;

    // Rutas por defecto
    strncpy(h->doctoresFile, "doctores.bin", sizeof(h->doctoresFile)-1);
    strncpy(h->pacientesFile, "pacientes.bin", sizeof(h->pacientesFile)-1);
    strncpy(h->citasFile, "citas.bin", sizeof(h->citasFile)-1);
    strncpy(h->historialFile, "historial.bin", sizeof(h->historialFile)-1);

    // Guardar header en disk
    const char* headerPath = "hospital.bin";
    if (!guardarHospitalHeaderProyecto(headerPath, *h)) {
        cout << "Aviso: no se pudo escribir 'hospital.bin'\n";
        // no abortamos, devolvemos estructura en memoria
    }

    // Crear archivos vacíos si no existen
    crearArchivoVacioSiNoExisteProyecto(h->doctoresFile);
    crearArchivoVacioSiNoExisteProyecto(h->pacientesFile);
    crearArchivoVacioSiNoExisteProyecto(h->citasFile);
    crearArchivoVacioSiNoExisteProyecto(h->historialFile);

    return h;
}

void mostrarDatosHospitalProyecto(const Hospital* h) {
    if (h) {
        cout << "Hospital creado exitosamente:\n";
        cout << "Nombre: " << h->nombre << "\n";
        cout << "Dirección: " << h->direccion << "\n";
        cout << "Teléfono: " << h->telefono << "\n";
    } else {
        cout << "Error al crear el hospital.\n";
    }
}