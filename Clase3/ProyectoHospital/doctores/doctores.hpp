#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <ctime>
#include <cstring>
#include <iostream>
using namespace std;

class Doctor {
private:
    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    bool eliminado;
    int cantidadPacientes;
    int pacientesIDs[50];
    int cantidadCitas;
    int citasIDs[30];
    time_t fechaCreacion;
    time_t fechaModificacion;

public:
    // ======= Constructores =======
    Doctor(); // Por defecto
    Doctor(int id, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int experiencia, float costo);

    // ======= Getters =======
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedulaProfesional() const;
    const char* getEspecialidad() const;
    int getAniosExperiencia() const;
    float getCostoConsulta() const;
    const char* getHorarioAtencion() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    bool isDisponible() const;
    bool isEliminado() const;
    int getCantidadPacientes() const;
    int getPacienteID(int index) const;
    int getCantidadCitas() const;
    int getCitaID(int index) const;
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;

    // ======= Setters =======
    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setCedulaProfesional(const char* cedula);
    void setEspecialidad(const char* especialidad);
    void setAniosExperiencia(int experiencia);
    void setCostoConsulta(float costo);
    void setHorarioAtencion(const char* horario);
    void setTelefono(const char* telefono);
    void setEmail(const char* email);
    void setDisponible(bool disponible);
    void setEliminado(bool eliminado);
    void agregarPacienteID(int id);
    void eliminarPacienteID(int id);
    void agregarCitaID(int id);
    void eliminarCitaID(int id);

    // ======= Métodos de presentación =======
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // ======= Métodos auxiliares =======
    static size_t obtenerTamano(); // Para archivos binarios
};

#endif // DOCTOR_HPP