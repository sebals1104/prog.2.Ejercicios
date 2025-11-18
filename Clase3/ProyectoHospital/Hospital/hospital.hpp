#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

#include <cstring>
#include <iostream>

class Hospital {
private:
    // Datos básicos del hospital
    char nombre[100];
    char direccion[150];
    char telefono[15];

    // Rutas de archivos
    char doctoresFile[260];
    char pacientesFile[260];
    char citasFile[260];
    char historialFile[260];

    // Contadores de ID
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;

    // Estadísticas generales
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;

public:
    // Constructor por defecto
    Hospital();

    // Constructor parametrizado
    Hospital(const char* nombre, const char* direccion, const char* telefono);

    // Getters
    const char* getNombre() const;
    const char* getDireccion() const;
    const char* getTelefono() const;

    const char* getDoctoresFile() const;
    const char* getPacientesFile() const;
    const char* getCitasFile() const;
    const char* getHistorialFile() const;

    int getSiguienteIDPaciente() const;
    int getSiguienteIDDoctor() const;
    int getSiguienteIDCita() const;
    int getSiguienteIDConsulta() const;

    int getTotalPacientesRegistrados() const;
    int getTotalDoctoresRegistrados() const;
    int getTotalCitasAgendadas() const;
    int getTotalConsultasRealizadas() const;

    // Setters
    void setNombre(const char* nuevoNombre);
    void setDireccion(const char* nuevaDireccion);
    void setTelefono(const char* nuevoTelefono);

    void incrementarIDPaciente();
    void incrementarIDDoctor();
    void incrementarIDCita();
    void incrementarIDConsulta();

    void incrementarTotalPacientes();
    void incrementarTotalDoctores();
    void incrementarTotalCitas();
    void incrementarTotalConsultas();

    // Guardar hospital en archivo binario
    bool guardarHospital(const char* rutaArchivo) const;

// Cargar hospital desde archivo binario
    bool cargarHospital(const char* rutaArchivo);
};

#endif