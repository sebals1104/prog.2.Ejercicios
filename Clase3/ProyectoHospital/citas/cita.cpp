#include "cita.hpp"
#include <iomanip>
#include <iostream>

// ================= CONSTRUCTORES =================

Cita::Cita() {
    id = 0;
    pacienteID = 0;
    doctorID = 0;
    atendida = false;
    eliminado = false;
    consultaID = 0;

    // Limpieza de memoria
    memset(fecha, 0, sizeof(fecha));
    memset(hora, 0, sizeof(hora));
    memset(motivo, 0, sizeof(motivo));
    memset(estado, 0, sizeof(estado));
    memset(observaciones, 0, sizeof(observaciones));

    // Estado inicial por defecto
    strncpy(estado, "Pendiente", sizeof(estado) - 1);

    fechaCreacion = time(NULL);
    fechaModificacion = fechaCreacion;
}

Cita::Cita(int _id, int _pacienteID, int _doctorID, const char* _fecha, const char* _hora, const char* _motivo) 
    : Cita() { // Delegamos al constructor base
    id = _id;
    pacienteID = _pacienteID;
    doctorID = _doctorID;
    setFecha(_fecha);
    setHora(_hora);
    setMotivo(_motivo);
}

// ================= GETTERS =================

int Cita::getId() const { return id; }
int Cita::getPacienteID() const { return pacienteID; }
int Cita::getDoctorID() const { return doctorID; }
const char* Cita::getFecha() const { return fecha; }
const char* Cita::getHora() const { return hora; }
const char* Cita::getMotivo() const { return motivo; }
const char* Cita::getEstado() const { return estado; }
const char* Cita::getObservaciones() const { return observaciones; }
bool Cita::estaAtendida() const { return atendida; }
bool Cita::estaEliminado() const { return eliminado; }
int Cita::getConsultaID() const { return consultaID; }
time_t Cita::getFechaCreacion() const { return fechaCreacion; }
time_t Cita::getFechaModificacion() const { return fechaModificacion; }

// ================= SETTERS =================

void Cita::setId(int _id) { id = _id; }

void Cita::setPacienteID(int _id) {
    pacienteID = _id;
    actualizarFechaModificacion();
}

void Cita::setDoctorID(int _id) {
    doctorID = _id;
    actualizarFechaModificacion();
}

void Cita::setFecha(const char* _fecha) {
    strncpy(fecha, _fecha, sizeof(fecha) - 1);
    fecha[sizeof(fecha) - 1] = '\0';
    actualizarFechaModificacion();
}

void Cita::setHora(const char* _hora) {
    strncpy(hora, _hora, sizeof(hora) - 1);
    hora[sizeof(hora) - 1] = '\0';
    actualizarFechaModificacion();
}

void Cita::setMotivo(const char* _motivo) {
    strncpy(motivo, _motivo, sizeof(motivo) - 1);
    motivo[sizeof(motivo) - 1] = '\0';
    actualizarFechaModificacion();
}

void Cita::setEstado(const char* _estado) {
    strncpy(estado, _estado, sizeof(estado) - 1);
    estado[sizeof(estado) - 1] = '\0';
    actualizarFechaModificacion();
}

void Cita::setObservaciones(const char* _observaciones) {
    strncpy(observaciones, _observaciones, sizeof(observaciones) - 1);
    observaciones[sizeof(observaciones) - 1] = '\0';
    actualizarFechaModificacion();
}

void Cita::setAtendida(bool _atendida) {
    atendida = _atendida;
    if(atendida) setEstado("Atendida");
    actualizarFechaModificacion();
}

void Cita::setEliminado(bool _eliminado) {
    eliminado = _eliminado;
    if(eliminado) setEstado("Cancelada");
    actualizarFechaModificacion();
}

void Cita::setConsultaID(int _id) {
    consultaID = _id;
    actualizarFechaModificacion();
}

// ================= UTILIDADES =================

void Cita::actualizarFechaModificacion() {
    fechaModificacion = time(NULL);
}

size_t Cita::obtenerTamano() {
    return sizeof(Cita);
}

void Cita::mostrarInformacion() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║            DETALLES DE CITA            ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    cout << " ID Cita: " << id << endl;
    cout << " Paciente ID: " << pacienteID << endl;
    cout << " Doctor ID:   " << doctorID << endl;
    cout << " Fecha: " << fecha << " | Hora: " << hora << endl;
    cout << " Estado: " << estado << endl;
    cout << " Motivo: " << motivo << endl;
    
    if (atendida) {
        cout << " ----------------------------------------" << endl;
        cout << " Observaciones: " << observaciones << endl;
        cout << " Consulta vinculada ID: " << consultaID << endl;
    }

    if(eliminado) cout << " [ESTADO: ELIMINADO]" << endl;
}