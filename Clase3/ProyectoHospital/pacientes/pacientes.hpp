#ifndef PACIENTES_HPP  // Ajustado al nombre del archivo
#define PACIENTES_HPP

#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

class Paciente {
private:
    // Atributos privados (Idénticos al struct original para compatibilidad binaria)
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    char alergias[500];
    char observaciones[500];
    
    // Estados
    bool activo;
    bool eliminado;
    
    // Relaciones y contadores
    int cantidadConsultas;
    int primerConsultaID;
    int cantidadCitas;
    int citasIDs[20]; // Array fijo
    
    // Auditoría
    time_t fechaCreacion;
    time_t fechaModificacion;

public:
    // --- Constructores ---
    Paciente(); 
    Paciente(int _id, const char* _nombre, const char* _apellido, const char* _cedula);
    
    // --- Getters (Lectura) ---
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedula() const;
    int getEdad() const;
    char getSexo() const;
    const char* getTipoSangre() const;
    const char* getTelefono() const;
    const char* getDireccion() const;
    const char* getEmail() const;
    const char* getAlergias() const;
    const char* getObservaciones() const;
    
    bool estaActivo() const;
    bool estaEliminado() const; 
    int getCantidadCitas() const;
    
    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;

    // --- Setters (Escritura) ---
    void setId(int _id);
    void setNombre(const char* _nombre);
    void setApellido(const char* _apellido);
    void setCedula(const char* _cedula);
    void setEdad(int _edad);
    void setSexo(char _sexo);
    void setTipoSangre(const char* _tipoSangre);
    void setTelefono(const char* _telefono);
    void setDireccion(const char* _direccion);
    void setEmail(const char* _email);
    void setAlergias(const char* _alergias);
    void setObservaciones(const char* _observaciones);
    
    void setActivo(bool _activo);
    void setEliminado(bool _eliminado);
    
    // --- Métodos de Gestión ---
    bool agregarCitaID(int idCita); 
    void actualizarFechaModificacion(); 
    void mostrarInformacion() const;    
    
    static size_t obtenerTamano();
};

#endif