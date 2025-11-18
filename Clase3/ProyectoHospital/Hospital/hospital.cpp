#include "hospital.hpp"

using namespace std;

// Constructor por defecto
Hospital::Hospital() {
    strcpy(nombre, "Sin nombre");
    strcpy(direccion, "Sin direccion");
    strcpy(telefono, "0000000000");

    // Rutas de archivos en la carpeta binarios
    strcpy(doctoresFile, "Clase 3/ProyectoHospital/binarios/doctores.dat");
    strcpy(pacientesFile, "Clase 3/ProyectoHospital/binarios/pacientes.dat");
    strcpy(citasFile,     "Clase 3/ProyectoHospital/binarios/citas.dat");
    strcpy(historialFile, "Clase 3/ProyectoHospital/binarios/historial.dat");

    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;

    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

// Constructor parametrizado
Hospital::Hospital(const char* nom, const char* dir, const char* tel) {
    setNombre(nom);
    setDireccion(dir);
    setTelefono(tel);

    // Rutas de archivos en la carpeta binarios
    strcpy(doctoresFile, "Clase 3/ProyectoHospital/binarios/doctores.dat");
    strcpy(pacientesFile, "Clase 3/ProyectoHospital/binarios/pacientes.dat");
    strcpy(citasFile,     "Clase 3/ProyectoHospital/binarios/citas.dat");
    strcpy(historialFile, "Clase 3/ProyectoHospital/binarios/historial.dat");

    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;

    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

// Getters
const char* Hospital::getNombre() const { return nombre; }
const char* Hospital::getDireccion() const { return direccion; }
const char* Hospital::getTelefono() const { return telefono; }

const char* Hospital::getDoctoresFile() const { return doctoresFile; }
const char* Hospital::getPacientesFile() const { return pacientesFile; }
const char* Hospital::getCitasFile() const { return citasFile; }
const char* Hospital::getHistorialFile() const { return historialFile; }

int Hospital::getSiguienteIDPaciente() const { return siguienteIDPaciente; }
int Hospital::getSiguienteIDDoctor() const { return siguienteIDDoctor; }
int Hospital::getSiguienteIDCita() const { return siguienteIDCita; }
int Hospital::getSiguienteIDConsulta() const { return siguienteIDConsulta; }

int Hospital::getTotalPacientesRegistrados() const { return totalPacientesRegistrados; }
int Hospital::getTotalDoctoresRegistrados() const { return totalDoctoresRegistrados; }
int Hospital::getTotalCitasAgendadas() const { return totalCitasAgendadas; }
int Hospital::getTotalConsultasRealizadas() const { return totalConsultasRealizadas; }

// Setters
void Hospital::setNombre(const char* nuevoNombre) {
    strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
}

void Hospital::setDireccion(const char* nuevaDireccion) {
    strncpy(direccion, nuevaDireccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
}

void Hospital::setTelefono(const char* nuevoTelefono) {
    strncpy(telefono, nuevoTelefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
}

bool Hospital::guardarHospital(const char* rutaArchivo) const {
    FILE* archivo = fopen(rutaArchivo, "wb");
    if (!archivo) {
        return false;
    }
    fwrite(this, sizeof(Hospital), 1, archivo);
    fclose(archivo);
    return true;
}

bool Hospital::cargarHospital(const char* rutaArchivo) {
    FILE* archivo = fopen(rutaArchivo, "rb");
    if (!archivo) {
        return false;
    }
    fread(this, sizeof(Hospital), 1, archivo);
    fclose(archivo);
    return true;
}

// Incrementadores de IDs
void Hospital::incrementarIDPaciente() { siguienteIDPaciente++; }
void Hospital::incrementarIDDoctor() { siguienteIDDoctor++; }
void Hospital::incrementarIDCita() { siguienteIDCita++; }
void Hospital::incrementarIDConsulta() { siguienteIDConsulta++; }

// Incrementadores de estadísticas
void Hospital::incrementarTotalPacientes() { totalPacientesRegistrados++; }
void Hospital::incrementarTotalDoctores() { totalDoctoresRegistrados++; }
void Hospital::incrementarTotalCitas() { totalCitasAgendadas++; }
void Hospital::incrementarTotalConsultas() { totalConsultasRealizadas++; }