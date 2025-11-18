#ifndef HISTORIALMEDICO_HPP
#define HISTORIALMEDICO_HPP

#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

class HistorialMedico {
private:
    int id;
    int pacienteID;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    int siguienteConsultaID;
    bool eliminado;
    time_t fechaRegistro;

public:
    // ======= CONSTRUCTORES =======
    HistorialMedico();
    HistorialMedico(int id, int pacienteID, const char* fecha, const char* hora, const char* diagnostico, const char* tratamiento, const char* medicamentos, int doctorID, float costo, int siguienteConsultaID);

    // ======= GETTERS =======
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
    bool isEliminado() const;
    time_t getFechaRegistro() const;

    // ======= SETTERS =======
    void setId(int valor);
    void setPacienteID(int valor);
    void setFecha(const char* valor);
    void setHora(const char* valor);
    void setDiagnostico(const char* valor);
    void setTratamiento(const char* valor);
    void setMedicamentos(const char* valor);
    void setDoctorID(int valor);
    void setCosto(float valor);
    void setSiguienteConsultaID(int valor);
    void setEliminado(bool valor);
    void setFechaRegistro(time_t valor);

    // ======= MÉTODOS =======
    void mostrarInformacion() const;
};

#endif