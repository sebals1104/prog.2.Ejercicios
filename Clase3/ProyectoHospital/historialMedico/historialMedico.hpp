#ifndef HISTORIAL_MEDICO_HPP
#define HISTORIAL_MEDICO_HPP

#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

class HistorialMedico {
private:
    // --- Atributos Privados (Compatibilidad binaria estricta) ---
    int id;
    int pacienteID;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    int siguienteConsultaID; // Para lista enlazada de historias
    bool eliminado;
    time_t fechaRegistro;

public:
    // --- Constructores ---
    HistorialMedico();
    HistorialMedico(int _id, int _pacienteID, int _doctorID, const char* _fecha, const char* _hora);

    // --- Getters (Lectura) ---
    int getId() const;
    int getPacienteID() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getDiagnostico() const;
    const char* getTratamiento() const;
    const char* getMedicamentos() const;
    int getDoctorID() const;
    float getCosto() const;
    int getSiguienteConsultaID() const;
    bool estaEliminado() const;
    time_t getFechaRegistro() const;

    // --- Setters (Escritura) ---
    void setId(int _id);
    void setPacienteID(int _id);
    void setFecha(const char* _fecha);
    void setHora(const char* _hora);
    void setDiagnostico(const char* _diagnostico);
    void setTratamiento(const char* _tratamiento);
    void setMedicamentos(const char* _medicamentos);
    void setDoctorID(int _id);
    void setCosto(float _costo);
    void setSiguienteConsultaID(int _id);
    void setEliminado(bool _eliminado);
    // No hay setFechaRegistro pública, se define en el constructor

    // --- Utilidades ---
    void mostrarInformacion() const;
    static size_t obtenerTamano();
};

#endif