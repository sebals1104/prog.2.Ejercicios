#include "hospital.hpp"
#include <iomanip>

// ================= CONSTRUCTORES =================

Hospital::Hospital() {
    // Inicializar cadenas vacías
    memset(nombre, 0, sizeof(nombre));
    memset(direccion, 0, sizeof(direccion));
    memset(telefono, 0, sizeof(telefono));
    
    // Rutas por defecto
    strncpy(doctoresFile, "datos/doctores.bin", sizeof(doctoresFile) - 1);
    strncpy(pacientesFile, "datos/pacientes.bin", sizeof(pacientesFile) - 1);
    strncpy(citasFile, "datos/citas.bin", sizeof(citasFile) - 1);
    strncpy(historialFile, "datos/historial.bin", sizeof(historialFile) - 1);
    
    // Inicializar contadores en 1
    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;
    
    // Inicializar estadísticas en 0
    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

Hospital::Hospital(const char* _nombre, const char* _direccion, const char* _telefono) 
    : Hospital() { // Llamada al constructor por defecto para inicializar rutas y contadores
    setNombre(_nombre);
    setDireccion(_direccion);
    setTelefono(_telefono);
}

// ================= GETTERS =================

const char* Hospital::getNombre() const { return nombre; }
const char* Hospital::getDireccion() const { return direccion; }
const char* Hospital::getTelefono() const { return telefono; }

const char* Hospital::getRutaPacientes() const { return pacientesFile; }
const char* Hospital::getRutaDoctores() const { return doctoresFile; }
const char* Hospital::getRutaCitas() const { return citasFile; }
const char* Hospital::getRutaHistorial() const { return historialFile; }

int Hospital::getSiguienteIDPaciente() const { return siguienteIDPaciente; }
int Hospital::getSiguienteIDDoctor() const { return siguienteIDDoctor; }
int Hospital::getSiguienteIDCita() const { return siguienteIDCita; }
int Hospital::getSiguienteIDConsulta() const { return siguienteIDConsulta; }

// ================= SETTERS =================

void Hospital::setNombre(const char* _nombre) {
    strncpy(nombre, _nombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
}

void Hospital::setDireccion(const char* _direccion) {
    strncpy(direccion, _direccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
}

void Hospital::setTelefono(const char* _telefono) {
    strncpy(telefono, _telefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
}

// ================= LÓGICA DE CONTADORES =================

void Hospital::actualizarContadoresPaciente(int proximoID, int totalActual) {
    siguienteIDPaciente = proximoID;
    totalPacientesRegistrados = totalActual;
}

void Hospital::actualizarContadoresDoctor(int proximoID, int totalActual) {
    siguienteIDDoctor = proximoID;
    totalDoctoresRegistrados = totalActual;
}

void Hospital::actualizarContadoresCita(int proximoID, int totalActual) {
    siguienteIDCita = proximoID;
    totalCitasAgendadas = totalActual;
}

void Hospital::actualizarContadoresConsulta(int proximoID, int totalActual) {
    siguienteIDConsulta = proximoID;
    totalConsultasRealizadas = totalActual;
}

// ================= UTILIDADES =================

size_t Hospital::obtenerTamano() {
    return sizeof(Hospital);
}

void Hospital::mostrarInformacion() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           DATOS DEL HOSPITAL           ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << " Nombre:    " << nombre << endl;
    cout << " Dirección: " << direccion << endl;
    cout << " Teléfono:  " << telefono << endl;
    cout << " ----------------------------------------" << endl;
    cout << " ESTADÍSTICAS GLOBALES:" << endl;
    cout << " Pacientes:   " << totalPacientesRegistrados << endl;
    cout << " Doctores:    " << totalDoctoresRegistrados << endl;
    cout << " Citas:       " << totalCitasAgendadas << endl;
    cout << " Consultas:   " << totalConsultasRealizadas << endl;
}