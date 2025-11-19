#include "pacientes.hpp" // Importante: coincide con el nombre del archivo
#include <iomanip>
#include <iostream>

// ================= CONSTRUCTORES =================

Paciente::Paciente() {
    id = 0;
    edad = 0;
    sexo = ' ';
    activo = true;
    eliminado = false;
    cantidadConsultas = 0;
    primerConsultaID = -1;
    cantidadCitas = 0;
    
    // Limpieza de memoria
    memset(nombre, 0, sizeof(nombre));
    memset(apellido, 0, sizeof(apellido));
    memset(cedula, 0, sizeof(cedula));
    memset(tipoSangre, 0, sizeof(tipoSangre));
    memset(telefono, 0, sizeof(telefono));
    memset(direccion, 0, sizeof(direccion));
    memset(email, 0, sizeof(email));
    memset(alergias, 0, sizeof(alergias));
    memset(observaciones, 0, sizeof(observaciones));
    
    for(int i = 0; i < 20; i++) citasIDs[i] = -1;
    
    fechaCreacion = time(NULL);
    fechaModificacion = fechaCreacion;
}

Paciente::Paciente(int _id, const char* _nombre, const char* _apellido, const char* _cedula) : Paciente() {
    id = _id;
    setNombre(_nombre);
    setApellido(_apellido);
    setCedula(_cedula);
}

// ================= GETTERS =================

int Paciente::getId() const { return id; }
const char* Paciente::getNombre() const { return nombre; }
const char* Paciente::getApellido() const { return apellido; }
const char* Paciente::getCedula() const { return cedula; }
int Paciente::getEdad() const { return edad; }
char Paciente::getSexo() const { return sexo; }
const char* Paciente::getTipoSangre() const { return tipoSangre; }
const char* Paciente::getTelefono() const { return telefono; }
const char* Paciente::getDireccion() const { return direccion; }
const char* Paciente::getEmail() const { return email; }
const char* Paciente::getAlergias() const { return alergias; }
const char* Paciente::getObservaciones() const { return observaciones; }
bool Paciente::estaActivo() const { return activo; }
bool Paciente::estaEliminado() const { return eliminado; }
int Paciente::getCantidadCitas() const { return cantidadCitas; }
time_t Paciente::getFechaCreacion() const { return fechaCreacion; }
time_t Paciente::getFechaModificacion() const { return fechaModificacion; }

// ================= SETTERS =================

void Paciente::setId(int _id) { id = _id; }

void Paciente::setNombre(const char* _nombre) {
    strncpy(nombre, _nombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setApellido(const char* _apellido) {
    strncpy(apellido, _apellido, sizeof(apellido) - 1);
    apellido[sizeof(apellido) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setCedula(const char* _cedula) {
    strncpy(cedula, _cedula, sizeof(cedula) - 1);
    cedula[sizeof(cedula) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setEdad(int _edad) {
    edad = (_edad < 0 || _edad > 120) ? 0 : _edad;
    actualizarFechaModificacion();
}

void Paciente::setSexo(char _sexo) {
    sexo = toupper(_sexo);
    actualizarFechaModificacion();
}

void Paciente::setTipoSangre(const char* _tipoSangre) {
    strncpy(tipoSangre, _tipoSangre, sizeof(tipoSangre) - 1);
    tipoSangre[sizeof(tipoSangre) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setTelefono(const char* _telefono) {
    strncpy(telefono, _telefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setDireccion(const char* _direccion) {
    strncpy(direccion, _direccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setEmail(const char* _email) {
    strncpy(email, _email, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setAlergias(const char* _alergias) {
    strncpy(alergias, _alergias, sizeof(alergias) - 1);
    alergias[sizeof(alergias) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setObservaciones(const char* _observaciones) {
    strncpy(observaciones, _observaciones, sizeof(observaciones) - 1);
    observaciones[sizeof(observaciones) - 1] = '\0';
    actualizarFechaModificacion();
}

void Paciente::setActivo(bool _activo) {
    activo = _activo;
    actualizarFechaModificacion();
}

void Paciente::setEliminado(bool _eliminado) {
    eliminado = _eliminado;
    actualizarFechaModificacion();
}

// ================= OTROS MÉTODOS =================

bool Paciente::agregarCitaID(int idCita) {
    if (cantidadCitas >= 20) return false;
    citasIDs[cantidadCitas++] = idCita;
    actualizarFechaModificacion();
    return true;
}

void Paciente::actualizarFechaModificacion() {
    fechaModificacion = time(NULL);
}

size_t Paciente::obtenerTamano() {
    return sizeof(Paciente);
}

void Paciente::mostrarInformacion() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║          DETALLES DEL PACIENTE         ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << " ID: " << id << endl;
    cout << " Nombre: " << nombre << " " << apellido << endl;
    cout << " Cédula: " << cedula << endl;
    cout << " Edad: " << edad << " años" << endl;
    cout << " Sexo: " << sexo << endl;
    cout << " Tipo Sangre: " << tipoSangre << endl;
    cout << " Teléfono: " << telefono << endl;
    cout << " Email: " << email << endl;
    cout << " ----------------------------------------" << endl;
    
    // Usando strftime como en tu código original (más seguro y standard)
    char fechaStr[30];
    
    struct tm* timeinfo = localtime(&fechaCreacion);
    if (timeinfo != nullptr) {
        strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        cout << " Registrado: " << fechaStr << endl;
    }
    
    if(eliminado) cout << " [ESTADO: ELIMINADO]" << endl;
}