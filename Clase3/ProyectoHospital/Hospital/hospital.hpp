#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

#include <cstring>
#include <iostream>

using namespace std;

class Hospital {
private:
    // --- Atributos de Configuración ---
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    // --- Rutas de archivos (Persistencia de configuración) ---
    char doctoresFile[260];
    char pacientesFile[260];
    char citasFile[260];
    char historialFile[260];
    
    // --- Contadores de IDs (Secuencias) ---
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // --- Estadísticas Generales ---
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;

public:
    // --- Constructores ---
    Hospital();
    Hospital(const char* _nombre, const char* _direccion, const char* _telefono);

    // --- Getters (Información General) ---
    const char* getNombre() const;
    const char* getDireccion() const;
    const char* getTelefono() const;
    
    // --- Getters (Rutas) ---
    const char* getRutaPacientes() const;
    const char* getRutaDoctores() const;
    const char* getRutaCitas() const;
    const char* getRutaHistorial() const;
    
    // --- Getters (Contadores - Para asignar nuevos IDs) ---
    int getSiguienteIDPaciente() const;
    int getSiguienteIDDoctor() const;
    int getSiguienteIDCita() const;
    int getSiguienteIDConsulta() const;
    
    // --- Setters (Configuración) ---
    void setNombre(const char* _nombre);
    void setDireccion(const char* _direccion);
    void setTelefono(const char* _telefono);
    
    // --- Métodos para Actualizar Contadores (Lógica de Negocio) ---
    // Estos métodos se llaman después de guardar exitosamente un registro
    void actualizarContadoresPaciente(int proximoID, int totalActual);
    void actualizarContadoresDoctor(int proximoID, int totalActual);
    void actualizarContadoresCita(int proximoID, int totalActual);
    void actualizarContadoresConsulta(int proximoID, int totalActual);
    
    // --- Utilidad ---
    void mostrarInformacion() const;
    static size_t obtenerTamano();
};

#endif