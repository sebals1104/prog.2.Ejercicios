#include "historialMedico.hpp"
using namespace std;

// ======= CONSTRUCTORES =======
// ================= CONSTRUCTORES =================
HistorialMedico::HistorialMedico() {
    id = 0;
    pacienteID = 0;
    strcpy(fecha, "");
    strcpy(hora, "");
    strcpy(diagnostico, "");
    strcpy(tratamiento, "");
    strcpy(medicamentos, "");
    doctorID = 0;
    costo = 0.0f;
    siguienteConsultaID = -1;
    eliminado = false;
    fechaRegistro = time(nullptr);
}

HistorialMedico::HistorialMedico(int id_, int pacienteID_, const char* fecha_, const char* hora_, const char* diagnostico_, const char* tratamiento_, const char* medicamentos_, int doctorID_, float costo_, int siguienteConsultaID_){
    id = id_;
    pacienteID = pacienteID_;
    strncpy(fecha, fecha_, sizeof(fecha) - 1); fecha[sizeof(fecha)-1] = '\0';
    strncpy(hora, hora_, sizeof(hora) - 1); hora[sizeof(hora)-1] = '\0';
    strncpy(diagnostico, diagnostico_, sizeof(diagnostico) - 1); diagnostico[sizeof(diagnostico)-1] = '\0';
    strncpy(tratamiento, tratamiento_, sizeof(tratamiento) - 1); tratamiento[sizeof(tratamiento)-1] = '\0';
    strncpy(medicamentos, medicamentos_, sizeof(medicamentos) - 1); medicamentos[sizeof(medicamentos)-1] = '\0';
    doctorID = doctorID_;
    costo = costo_;
    siguienteConsultaID = siguienteConsultaID_;
    eliminado = false;
    fechaRegistro = time(nullptr);
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
bool HistorialMedico::isEliminado() const { return eliminado; }
time_t HistorialMedico::getFechaRegistro() const { return fechaRegistro; }

// ================= SETTERS =================
void HistorialMedico::setId(int valor) { id = valor; }
void HistorialMedico::setPacienteID(int valor) { pacienteID = valor; }
void HistorialMedico::setFecha(const char* valor) { strncpy(fecha, valor, sizeof(fecha)-1); fecha[sizeof(fecha)-1]='\0'; }
void HistorialMedico::setHora(const char* valor) { strncpy(hora, valor, sizeof(hora)-1); hora[sizeof(hora)-1]='\0'; }
void HistorialMedico::setDiagnostico(const char* valor) { strncpy(diagnostico, valor, sizeof(diagnostico)-1); diagnostico[sizeof(diagnostico)-1]='\0'; }
void HistorialMedico::setTratamiento(const char* valor) { strncpy(tratamiento, valor, sizeof(tratamiento)-1); tratamiento[sizeof(tratamiento)-1]='\0'; }
void HistorialMedico::setMedicamentos(const char* valor) { strncpy(medicamentos, valor, sizeof(medicamentos)-1); medicamentos[sizeof(medicamentos)-1]='\0'; }
void HistorialMedico::setDoctorID(int valor) { doctorID = valor; }
void HistorialMedico::setCosto(float valor) { costo = valor; }
void HistorialMedico::setSiguienteConsultaID(int valor) { siguienteConsultaID = valor; }
void HistorialMedico::setEliminado(bool valor) { eliminado = valor; }
void HistorialMedico::setFechaRegistro(time_t valor) { fechaRegistro = valor; }

// ================= MÉTODOS DE PRESENTACIÓN =================
void HistorialMedico::mostrarInformacion() const {
    cout << "ID: " << id << endl;
    cout << "Paciente ID: " << pacienteID << endl;
    cout << "Fecha: " << fecha << " Hora: " << hora << endl;
    cout << "Diagnóstico: " << diagnostico << endl;
    cout << "Tratamiento: " << tratamiento << endl;
    cout << "Medicamentos: " << medicamentos << endl;
    cout << "Doctor ID: " << doctorID << endl;
    cout << "Costo: $" << costo << endl;
    cout << "Siguiente Consulta ID: " << siguienteConsultaID << endl;
    cout << "Eliminado: " << (eliminado ? "Sí" : "No") << endl;
}