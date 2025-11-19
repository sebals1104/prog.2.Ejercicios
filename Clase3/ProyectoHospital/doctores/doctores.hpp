#ifndef DOCTORES_HPP
#define DOCTORES_HPP

#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

class Doctor {
private:
    // --- Atributos Privados (Idénticos al struct legacy) ---
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
    
    // Estados
    bool disponible;
    bool eliminado;
    
    // Relaciones (Arrays fijos para compatibilidad binaria)
    int cantidadPacientes;
    int pacientesIDs[50];
    int cantidadCitas;
    int citasIDs[30];
    
    // Auditoría
    time_t fechaCreacion;
    time_t fechaModificacion;

public:
    // --- Constructores ---
    Doctor();
    Doctor(int _id, const char* _nombre, const char* _apellido, const char* _cedulaProf, const char* _especialidad);

    // --- Getters (Lectura) ---
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
    
    bool estaDisponible() const;
    bool estaEliminado() const; 
    
    int getCantidadPacientes() const;
    int getCantidadCitas() const;
    
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;

    // --- Setters (Escritura) ---
    void setId(int _id);
    void setNombre(const char* _nombre);
    void setApellido(const char* _apellido);
    void setCedulaProfesional(const char* _cedula);
    void setEspecialidad(const char* _especialidad);
    void setAniosExperiencia(int _anios);
    void setCostoConsulta(float _costo);
    void setHorarioAtencion(const char* _horario);
    void setTelefono(const char* _telefono);
    void setEmail(const char* _email);
    
    void setDisponible(bool _disponible);
    void setEliminado(bool _eliminado);

    // --- Métodos de Gestión de Relaciones ---
    bool agregarPacienteID(int idPaciente); 
    bool agregarCitaID(int idCita);         

    // --- Utilidades ---
    void actualizarFechaModificacion();
    void mostrarInformacion() const;
    
    static size_t obtenerTamano();
};

#endif