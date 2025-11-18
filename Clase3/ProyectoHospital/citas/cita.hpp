#ifndef CITA_HPP
#define CITA_HPP

#include <ctime>
#include <cstring>
#include <iostream>

class Cita {
private:
    int id;
    int pacienteID;
    int doctorID;
    char fecha[11];        // "DD/MM/AAAA"
    char hora[6];          // "HH:MM"
    char motivo[150];
    char estado[20];       // "Pendiente", "Cancelada", etc.
    char observaciones[200];
    bool atendida;
    bool eliminado;
    int consultaID;
    time_t fechaCreacion;
    time_t fechaModificacion;

public:
    // =================== Constructores ===================
    Cita();  // por defecto
    Cita(int id, int pacienteID, int doctorID, const char* fecha, const char* hora, const char* motivo);

    // =================== Getters ===================
    int getId() const;
    int getPacienteID() const;
    int getDoctorID() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getMotivo() const;
    const char* getEstado() const;
    const char* getObservaciones() const;
    bool isAtendida() const;
    bool isEliminado() const;
    int getConsultaID() const;
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;

    // =================== Setters ===================
    void setPacienteID(int pacienteID);
    void setDoctorID(int doctorID);
    void setFecha(const char* fecha);
    void setHora(const char* hora);
    void setMotivo(const char* motivo);
    void setEstado(const char* estado);
    void setObservaciones(const char* obs);
    void setAtendida(bool atendida);
    void setEliminado(bool eliminado);
    void setConsultaID(int consultaID);

    // =================== Métodos de presentación ===================
    void mostrarResumen() const;
    void mostrarDetalle() const;

    // =================== Métodos auxiliares ===================
    bool validarDatos() const;  // valida fecha, hora, IDs
    static size_t obtenerTamano(); // retorna tamaño en bytes del objeto
};

#endif