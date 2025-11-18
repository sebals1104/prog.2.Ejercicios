#include "cita.hpp"
#include <iomanip>

using namespace std;

// =================== Constructores ===================
Cita::Cita() {
    id = 0;
    pacienteID = 0;
    doctorID = 0;
    strcpy(fecha, "");
    strcpy(hora, "");
    strcpy(motivo, "");
    strcpy(estado, "Pendiente");
    strcpy(observaciones, "");
    atendida = false;
    eliminado = false;
    consultaID = 0;
    fechaCreacion = time(nullptr);
    fechaModificacion = time(nullptr);
}

Cita::Cita(int id_, int pacienteID_, int doctorID_, const char* fecha_, const char* hora_, const char* motivo_) {
    id = id_;
    pacienteID = pacienteID_;
    doctorID = doctorID_;
    strncpy(fecha, fecha_, sizeof(fecha) - 1);
    fecha[sizeof(fecha)-1] = '\0';
    strncpy(hora, hora_, sizeof(hora) - 1);
    hora[sizeof(hora)-1] = '\0';
    strncpy(motivo, motivo_, sizeof(motivo) - 1);
    motivo[sizeof(motivo)-1] = '\0';
    strcpy(estado, "Pendiente");
    strcpy(observaciones, "");
    atendida = false;
    eliminado = false;
    consultaID = 0;
    fechaCreacion = time(nullptr);
    fechaModificacion = time(nullptr);
}

// =================== Getters ===================
int Cita::getId() const { return id; }
int Cita::getPacienteID() const { return pacienteID; }
int Cita::getDoctorID() const { return doctorID; }
const char* Cita::getFecha() const { return fecha; }
const char* Cita::getHora() const { return hora; }
const char* Cita::getMotivo() const { return motivo; }
const char* Cita::getEstado() const { return estado; }
const char* Cita::getObservaciones() const { return observaciones; }
bool Cita::isAtendida() const { return atendida; }
bool Cita::isEliminado() const { return eliminado; }
int Cita::getConsultaID() const { return consultaID; }
time_t Cita::getFechaCreacion() const { return fechaCreacion; }
time_t Cita::getFechaModificacion() const { return fechaModificacion; }

// =================== Setters ===================
void Cita::setPacienteID(int pid) { pacienteID = pid; fechaModificacion = time(nullptr); }
void Cita::setDoctorID(int did) { doctorID = did; fechaModificacion = time(nullptr); }
void Cita::setFecha(const char* f) { strncpy(fecha, f, sizeof(fecha)-1); fecha[sizeof(fecha)-1]='\0'; fechaModificacion=time(nullptr);}
void Cita::setHora(const char* h) { strncpy(hora, h, sizeof(hora)-1); hora[sizeof(hora)-1]='\0'; fechaModificacion=time(nullptr);}
void Cita::setMotivo(const char* m) { strncpy(motivo, m, sizeof(motivo)-1); motivo[sizeof(motivo)-1]='\0'; fechaModificacion=time(nullptr);}
void Cita::setEstado(const char* e) { strncpy(estado, e, sizeof(estado)-1); estado[sizeof(estado)-1]='\0'; fechaModificacion=time(nullptr);}
void Cita::setObservaciones(const char* o) { strncpy(observaciones, o, sizeof(observaciones)-1); observaciones[sizeof(observaciones)-1]='\0'; fechaModificacion=time(nullptr);}
void Cita::setAtendida(bool a) { atendida = a; fechaModificacion=time(nullptr);}
void Cita::setEliminado(bool e) { eliminado = e; fechaModificacion=time(nullptr);}
void Cita::setConsultaID(int cid) { consultaID = cid; fechaModificacion=time(nullptr); }

// =================== Métodos de presentación ===================
void Cita::mostrarResumen() const {
        cout << "ID: " << id 
            << " | PacienteID: " << pacienteID 
            << " | DoctorID: " << doctorID 
            << " | Fecha: " << fecha 
            << " | Hora: " << hora 
            << " | Estado: " << estado 
            << endl;
}

void Cita::mostrarDetalle() const {
        cout << "ID: " << id << "\nPacienteID: " << pacienteID 
            << "\nDoctorID: " << doctorID 
            << "\nFecha: " << fecha << "\nHora: " << hora 
            << "\nMotivo: " << motivo 
            << "\nEstado: " << estado 
            << "\nObservaciones: " << observaciones 
            << "\nAtendida: " << (atendida ? "Sí" : "No")
            << "\nEliminada: " << (eliminado ? "Sí" : "No")
            << "\nConsultaID: " << consultaID 
            << "\nFechaCreacion: " << ctime(&fechaCreacion)
            << "\nFechaModificacion: " << ctime(&fechaModificacion)
            << endl;
}

// =================== Métodos auxiliares ===================
bool Cita::validarDatos() const {
    // Aquí se pueden validar formatos de fecha/hora y IDs positivos
    if (pacienteID <= 0 || doctorID <= 0) return false;
    if (strlen(fecha) != 10 || strlen(hora) != 5) return false;
    return true;
}

size_t Cita::obtenerTamano() {
    return sizeof(Cita);
}