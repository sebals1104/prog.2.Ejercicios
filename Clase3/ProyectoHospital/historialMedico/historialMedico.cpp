#include "historialMedico.hpp"
#include <iomanip>
#include <iostream>

// ================= CONSTRUCTORES =================

HistorialMedico::HistorialMedico() {
    id = 0;
    pacienteID = 0;
    doctorID = 0;
    costo = 0.0f;
    siguienteConsultaID = -1; // -1 indica fin de la lista o sin vínculo
    eliminado = false;

    // Limpieza de buffers
    memset(fecha, 0, sizeof(fecha));
    memset(hora, 0, sizeof(hora));
    memset(diagnostico, 0, sizeof(diagnostico));
    memset(tratamiento, 0, sizeof(tratamiento));
    memset(medicamentos, 0, sizeof(medicamentos));

    fechaRegistro = time(NULL);
}

HistorialMedico::HistorialMedico(int _id, int _pacienteID, int _doctorID, const char* _fecha, const char* _hora) 
    : HistorialMedico() { // Delegación
    id = _id;
    pacienteID = _pacienteID;
    doctorID = _doctorID;
    setFecha(_fecha);
    setHora(_hora);
}

// ================= GETTERS =================

int HistorialMedico::getId() const { return id; }
int HistorialMedico::getPacienteID() const { return pacienteID; }
const char* HistorialMedico::getFecha() const { return fecha; }
const char* HistorialMedico::getHora() const { return hora; }
const char* HistorialMedico::getDiagnostico() const { return diagnostico; }
const char* HistorialMedico::getTratamiento() const { return tratamiento; }
const char* HistorialMedico::getMedicamentos() const { return medicamentos; }
int HistorialMedico::getDoctorID() const { return doctorID; }
float HistorialMedico::getCosto() const { return costo; }
int HistorialMedico::getSiguienteConsultaID() const { return siguienteConsultaID; }
bool HistorialMedico::estaEliminado() const { return eliminado; }
time_t HistorialMedico::getFechaRegistro() const { return fechaRegistro; }

// ================= SETTERS =================

void HistorialMedico::setId(int _id) { id = _id; }
void HistorialMedico::setPacienteID(int _id) { pacienteID = _id; }

void HistorialMedico::setFecha(const char* _fecha) {
    strncpy(fecha, _fecha, sizeof(fecha) - 1);
    fecha[sizeof(fecha) - 1] = '\0';
}

void HistorialMedico::setHora(const char* _hora) {
    strncpy(hora, _hora, sizeof(hora) - 1);
    hora[sizeof(hora) - 1] = '\0';
}

void HistorialMedico::setDiagnostico(const char* _diagnostico) {
    strncpy(diagnostico, _diagnostico, sizeof(diagnostico) - 1);
    diagnostico[sizeof(diagnostico) - 1] = '\0';
}

void HistorialMedico::setTratamiento(const char* _tratamiento) {
    strncpy(tratamiento, _tratamiento, sizeof(tratamiento) - 1);
    tratamiento[sizeof(tratamiento) - 1] = '\0';
}

void HistorialMedico::setMedicamentos(const char* _medicamentos) {
    strncpy(medicamentos, _medicamentos, sizeof(medicamentos) - 1);
    medicamentos[sizeof(medicamentos) - 1] = '\0';
}

void HistorialMedico::setDoctorID(int _id) { doctorID = _id; }

void HistorialMedico::setCosto(float _costo) {
    costo = (_costo < 0) ? 0.0f : _costo;
}

void HistorialMedico::setSiguienteConsultaID(int _id) {
    siguienteConsultaID = _id;
}

void HistorialMedico::setEliminado(bool _eliminado) {
    eliminado = _eliminado;
}

// ================= UTILIDADES =================

size_t HistorialMedico::obtenerTamano() {
    return sizeof(HistorialMedico);
}

void HistorialMedico::mostrarInformacion() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           CONSULTA MÉDICA              ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    cout << " ID Consulta: " << id << endl;
    cout << " Fecha: " << fecha << " | Hora: " << hora << endl;
    cout << " Paciente ID: " << pacienteID << endl;
    cout << " Doctor ID:   " << doctorID << endl;
    cout << " Costo:       $" << fixed << setprecision(2) << costo << endl;
    cout << " ----------------------------------------" << endl;
    cout << " Diagnóstico: " << diagnostico << endl;
    cout << " Tratamiento: " << tratamiento << endl;
    cout << " Receta:      " << medicamentos << endl;
    
    if (siguienteConsultaID != -1 && siguienteConsultaID != 0) {
        cout << " [Tiene seguimiento -> Consulta ID: " << siguienteConsultaID << "]" << endl;
    }

    if(eliminado) cout << " [ESTADO: ELIMINADO]" << endl;
}