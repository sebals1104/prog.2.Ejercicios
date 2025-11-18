#include "doctores.hpp"

// ======= Constructores =======
Doctor::Doctor() {
    id = 0;
    strcpy(nombre, "");
    strcpy(apellido, "");
    strcpy(cedulaProfesional, "");
    strcpy(especialidad, "");
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    strcpy(horarioAtencion, "");
    strcpy(telefono, "");
    strcpy(email, "");
    disponible = true;
    eliminado = false;
    cantidadPacientes = 0;
    cantidadCitas = 0;
    fechaCreacion = time(nullptr);
    fechaModificacion = time(nullptr);
}

Doctor::Doctor(int id_, const char* nombre_, const char* apellido_, const char* cedula_, const char* especialidad_, int experiencia, float costo) {
    id = id_;
    strcpy(nombre, nombre_);
    strcpy(apellido, apellido_);
    strcpy(cedulaProfesional, cedula_);
    strcpy(especialidad, especialidad_);
    aniosExperiencia = experiencia;
    costoConsulta = costo;
    strcpy(horarioAtencion, "");
    strcpy(telefono, "");
    strcpy(email, "");
    disponible = true;
    eliminado = false;
    cantidadPacientes = 0;
    cantidadCitas = 0;
    fechaCreacion = time(nullptr);
    fechaModificacion = time(nullptr);
}

// ======= Getters =======
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
bool Doctor::isDisponible() const { return disponible; }
bool Doctor::isEliminado() const { return eliminado; }
int Doctor::getCantidadPacientes() const { return cantidadPacientes; }
int Doctor::getPacienteID(int index) const { return (index >=0 && index < cantidadPacientes) ? pacientesIDs[index] : -1; }
int Doctor::getCantidadCitas() const { return cantidadCitas; }
int Doctor::getCitaID(int index) const { return (index >=0 && index < cantidadCitas) ? citasIDs[index] : -1; }
time_t Doctor::getFechaCreacion() const { return fechaCreacion; }
time_t Doctor::getFechaModificacion() const { return fechaModificacion; }

// ======= Setters =======
void Doctor::setNombre(const char* n) { strcpy(nombre, n); fechaModificacion = time(nullptr); }
void Doctor::setApellido(const char* a) { strcpy(apellido, a); fechaModificacion = time(nullptr); }
void Doctor::setCedulaProfesional(const char* c) { strcpy(cedulaProfesional, c); fechaModificacion = time(nullptr); }
void Doctor::setEspecialidad(const char* e) { strcpy(especialidad, e); fechaModificacion = time(nullptr); }
void Doctor::setAniosExperiencia(int exp) { aniosExperiencia = exp; fechaModificacion = time(nullptr); }
void Doctor::setCostoConsulta(float c) { costoConsulta = c; fechaModificacion = time(nullptr); }
void Doctor::setHorarioAtencion(const char* h) { strcpy(horarioAtencion, h); fechaModificacion = time(nullptr); }
void Doctor::setTelefono(const char* t) { strcpy(telefono, t); fechaModificacion = time(nullptr); }
void Doctor::setEmail(const char* e) { strcpy(email, e); fechaModificacion = time(nullptr); }
void Doctor::setDisponible(bool d) { disponible = d; fechaModificacion = time(nullptr); }
void Doctor::setEliminado(bool e) { eliminado = e; fechaModificacion = time(nullptr); }

// ======= Gestión de relaciones =======
void Doctor::agregarPacienteID(int id) { 
    if(cantidadPacientes < 50) pacientesIDs[cantidadPacientes++] = id; 
    fechaModificacion = time(nullptr);
}

void Doctor::eliminarPacienteID(int id) {
    for(int i=0;i<cantidadPacientes;i++){
        if(pacientesIDs[i]==id){
            for(int j=i;j<cantidadPacientes-1;j++)
                pacientesIDs[j] = pacientesIDs[j+1];
            cantidadPacientes--;
            break;
        }
    }
    fechaModificacion = time(nullptr);
}

void Doctor::agregarCitaID(int id) {
    if(cantidadCitas < 30) citasIDs[cantidadCitas++] = id;
    fechaModificacion = time(nullptr);
}

void Doctor::eliminarCitaID(int id) {
    for(int i=0;i<cantidadCitas;i++){
        if(citasIDs[i]==id){
            for(int j=i;j<cantidadCitas-1;j++)
                citasIDs[j] = citasIDs[j+1];
            cantidadCitas--;
            break;
        }
    }
    fechaModificacion = time(nullptr);
}

// ======= Métodos de presentación =======
void Doctor::mostrarInformacionBasica() const {
    cout << "ID: " << id << " | " << nombre << " " << apellido 
        << " | Especialidad: " << especialidad
        << " | Disponible: " << (disponible ? "Sí" : "No") << endl;
}

void Doctor::mostrarInformacionCompleta() const {
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << " " << apellido << endl;
    cout << "Cédula Profesional: " << cedulaProfesional << endl;
    cout << "Especialidad: " << especialidad << endl;
    cout << "Años Experiencia: " << aniosExperiencia << endl;
    cout << "Costo Consulta: $" << costoConsulta << endl;
    cout << "Horario Atención: " << horarioAtencion << endl;
    cout << "Teléfono: " << telefono << endl;
    cout << "Email: " << email << endl;
    cout << "Disponible: " << (disponible ? "Sí" : "No") << endl;
    cout << "Cantidad Pacientes: " << cantidadPacientes << endl;
    cout << "Cantidad Citas: " << cantidadCitas << endl;
    cout << "Fecha Creación: " << ctime(&fechaCreacion);
    cout << "Fecha Modificación: " << ctime(&fechaModificacion);
}

// ======= Tamaño para archivos binarios =======
size_t Doctor::obtenerTamano() {
    return sizeof(Doctor);
}