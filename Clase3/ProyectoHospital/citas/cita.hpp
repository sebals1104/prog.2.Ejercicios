#ifndef CITAS_HPP
#define CITAS_HPP

#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

class Cita {
private:
    // --- Atributos Privados (Idénticos al struct legacy) ---
    int id;
    int pacienteID;
    int doctorID;
    char fecha[11]; 
    char hora[6];   
    char motivo[150];
    char estado[20]; 
    char observaciones[200];
    
    // Estados lógicos
    bool atendida;
    bool eliminado;
    
    // Relación con historial (si ya fue atendida)
    int consultaID; 
    
    // Auditoría
    time_t fechaCreacion;
    time_t fechaModificacion;

public:
    // --- Constructores ---
    Cita();
    // Constructor para crear una cita nueva rápida
    Cita(int _id, int _pacienteID, int _doctorID, const char* _fecha, const char* _hora, const char* _motivo);

    // --- Getters (Lectura) ---
    int getId() const;
    int getPacienteID() const;
    int getDoctorID() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getMotivo() const;
    const char* getEstado() const;
    const char* getObservaciones() const;
    
    bool estaAtendida() const;
    bool estaEliminado() const; // Requerido por GestorArchivos
    int getConsultaID() const;
    
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;

    // --- Setters (Escritura) ---
    void setId(int _id);
    void setPacienteID(int _id);
    void setDoctorID(int _id);
    void setFecha(const char* _fecha);
    void setHora(const char* _hora);
    void setMotivo(const char* _motivo);
    void setEstado(const char* _estado);
    void setObservaciones(const char* _observaciones);
    
    void setAtendida(bool _atendida);
    void setEliminado(bool _eliminado);
    void setConsultaID(int _id);

    // --- Utilidades ---
    void actualizarFechaModificacion();
    void mostrarInformacion() const;
    
    static size_t obtenerTamano();
};

#endif