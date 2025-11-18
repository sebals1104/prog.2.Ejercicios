#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include <string>
#include <ctime>

class Paciente {
    private:
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
    bool activo;
    bool eliminado;
    int cantidadConsultas;
    int primerConsultaID;
    int cantidadCitas;
    int citasIDs[20];
    time_t fechaCreacion;
    time_t fechaModificacion;
    private:
        // ===== Constructores =====
    Paciente();
    Paciente(int id, const std::string& nombre, const std::string& apellido);

    // ===== Setters =====
    void setNombre(const std::string& n);
    void setApellido(const std::string& a);
    void setCedula(const std::string& c);
    void setEdad(int e);
    void setSexo(char s);
    void setTipoSangre(const std::string& ts);
    void setTelefono(const std::string& t);
    void setDireccion(const std::string& d);
    void setEmail(const std::string& e);
    void setAlergias(const std::string& a);
    void setObservaciones(const std::string& ob);

    void marcarEliminado();
    void activar();
    void desactivar();

    bool agregarCita(int citaID);

    // ===== Getters =====
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
    const int* getCitasIDs() const;

    time_t getFechaCreacion() const;
    time_t getFechaModificacion() const;

    void imprimirResumen() const;
};

#endif // PACIENTE_HPP