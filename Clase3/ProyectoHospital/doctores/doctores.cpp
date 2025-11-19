#include "doctores.hpp"
#include <iomanip>
#include <iostream>

// ================= CONSTRUCTORES =================

Doctor::Doctor() {
    id = 0;
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    disponible = true;
    eliminado = false;
    cantidadPacientes = 0;
    cantidadCitas = 0;

    // Limpieza de memoria (cadenas)
    memset(nombre, 0, sizeof(nombre));
    memset(apellido, 0, sizeof(apellido));
    memset(cedulaProfesional, 0, sizeof(cedulaProfesional));
    memset(especialidad, 0, sizeof(especialidad));
    memset(horarioAtencion, 0, sizeof(horarioAtencion));
    memset(telefono, 0, sizeof(telefono));
    memset(email, 0, sizeof(email));

    // Inicializar arrays de IDs en -1
    for(int i = 0; i < 50; i++) pacientesIDs[i] = -1;
    for(int i = 0; i < 30; i++) citasIDs[i] = -1;

    fechaCreacion = time(NULL);
    fechaModificacion = fechaCreacion;
}

Doctor::Doctor(int _id, const char* _nombre, const char* _apellido, const char* _cedulaProf, const char* _especialidad) 
    : Doctor() { // Delegar al constructor por defecto primero
    id = _id;
    setNombre(_nombre);
    setApellido(_apellido);
    setCedulaProfesional(_cedulaProf);
    setEspecialidad(_especialidad);
}

// ================= GETTERS =================

int Doctor::getId() const { return id; }
const char* Doctor::getNombre() const { return nombre; }
const char* Doctor::getApellido() const { return apellido; }
const char* Doctor::getCedulaProfesional() const { return cedulaProfesional; }
const char* Doctor::getEspecialidad() const { return especialidad; }
int Doctor::getAniosExperiencia() const { return aniosExperiencia; }
float Doctor::getCostoConsulta() const { return costoConsulta; }
const char* Doctor::getHorarioAtencion() const { return horarioAtencion; }
const char* Doctor::getTelefono() const { return telefono; }
const char* Doctor::getEmail() const { return email; }
bool Doctor::estaDisponible() const { return disponible; }
bool Doctor::estaEliminado() const { return eliminado; }
int Doctor::getCantidadPacientes() const { return cantidadPacientes; }
int Doctor::getCantidadCitas() const { return cantidadCitas; }
time_t Doctor::getFechaCreacion() const { return fechaCreacion; }
time_t Doctor::getFechaModificacion() const { return fechaModificacion; }

// ================= SETTERS =================

void Doctor::setId(int _id) { id = _id; }

void Doctor::setNombre(const char* _nombre) {
    strncpy(nombre, _nombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setApellido(const char* _apellido) {
    strncpy(apellido, _apellido, sizeof(apellido) - 1);
    apellido[sizeof(apellido) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setCedulaProfesional(const char* _cedula) {
    strncpy(cedulaProfesional, _cedula, sizeof(cedulaProfesional) - 1);
    cedulaProfesional[sizeof(cedulaProfesional) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setEspecialidad(const char* _especialidad) {
    strncpy(especialidad, _especialidad, sizeof(especialidad) - 1);
    especialidad[sizeof(especialidad) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setAniosExperiencia(int _anios) {
    aniosExperiencia = (_anios < 0) ? 0 : _anios;
    actualizarFechaModificacion();
}

void Doctor::setCostoConsulta(float _costo) {
    costoConsulta = (_costo < 0) ? 0.0f : _costo;
    actualizarFechaModificacion();
}

void Doctor::setHorarioAtencion(const char* _horario) {
    strncpy(horarioAtencion, _horario, sizeof(horarioAtencion) - 1);
    horarioAtencion[sizeof(horarioAtencion) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setTelefono(const char* _telefono) {
    strncpy(telefono, _telefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setEmail(const char* _email) {
    strncpy(email, _email, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    actualizarFechaModificacion();
}

void Doctor::setDisponible(bool _disponible) {
    disponible = _disponible;
    actualizarFechaModificacion();
}

void Doctor::setEliminado(bool _eliminado) {
    eliminado = _eliminado;
    actualizarFechaModificacion();
}

// ================= MÉTODOS DE GESTIÓN =================

bool Doctor::agregarPacienteID(int idPaciente) {
    if (cantidadPacientes >= 50) return false;
    pacientesIDs[cantidadPacientes++] = idPaciente;
    actualizarFechaModificacion();
    return true;
}

bool Doctor::agregarCitaID(int idCita) {
    if (cantidadCitas >= 30) return false;
    citasIDs[cantidadCitas++] = idCita;
    actualizarFechaModificacion();
    return true;
}

void Doctor::actualizarFechaModificacion() {
    fechaModificacion = time(NULL);
}

size_t Doctor::obtenerTamano() {
    return sizeof(Doctor);
}

void Doctor::mostrarInformacion() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           DETALLES DEL DOCTOR          ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    cout << " ID: " << id << endl;
    cout << " Nombre: " << nombre << " " << apellido << endl;
    cout << " Especialidad: " << especialidad << endl;
    cout << " Cédula Prof: " << cedulaProfesional << endl;
    cout << " Experiencia: " << aniosExperiencia << " años" << endl;
    cout << " Costo: $" << fixed << setprecision(2) << costoConsulta << endl;
    cout << " Horario: " << horarioAtencion << endl;
    cout << " Teléfono: " << telefono << endl;
    cout << " Email: " << email << endl;
    cout << " ----------------------------------------" << endl;
    cout << " Pacientes asignados: " << cantidadPacientes << endl;
    cout << " Citas agendadas: " << cantidadCitas << endl;
    cout << " Disponible: " << (disponible ? "Sí" : "No") << endl;

    if(eliminado) cout << " [ESTADO: ELIMINADO]" << endl;
}