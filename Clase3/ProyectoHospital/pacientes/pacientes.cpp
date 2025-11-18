#include "pacientes.hpp"
#include <cstring>
#include <iostream>

using namespace std;

// =========================
//   CONSTRUCTORES
// =========================

Paciente::Paciente() {
    id = 0;
    nombre[0] = apellido[0] = cedula[0] = tipoSangre[0] = telefono[0] = direccion[0] =
    email[0] = alergias[0] = observaciones[0] = '\0';

    edad = 0;
    sexo = 'N';
    activo = true;
    eliminado = false;

    cantidadConsultas = 0;
    primerConsultaID = -1;

    cantidadCitas = 0;
    for (int i = 0; i < 20; i++)
        citasIDs[i] = -1;

    fechaCreacion = time(nullptr);
    fechaModificacion = fechaCreacion;
}

Paciente::Paciente(int id, const std::string& n, const std::string& a)
    : Paciente()   // Llama al constructor por defecto para inicializar todo
{
    this->id = id;
    setNombre(n);
    setApellido(a);
}

// =========================
//          SETTERS
// =========================

void Paciente::setNombre(const string& n) {
    strncpy(nombre, n.c_str(), sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setApellido(const string& a) {
    strncpy(apellido, a.c_str(), sizeof(apellido) - 1);
    apellido[sizeof(apellido) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setCedula(const string& c) {
    strncpy(cedula, c.c_str(), sizeof(cedula) - 1);
    cedula[sizeof(cedula) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setEdad(int e) {
    edad = e;
    fechaModificacion = time(nullptr);
}

void Paciente::setSexo(char s) {
    sexo = s;
    fechaModificacion = time(nullptr);
}

void Paciente::setTipoSangre(const string& ts) {
    strncpy(tipoSangre, ts.c_str(), sizeof(tipoSangre) - 1);
    tipoSangre[sizeof(tipoSangre) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setTelefono(const string& t) {
    strncpy(telefono, t.c_str(), sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setDireccion(const string& d) {
    strncpy(direccion, d.c_str(), sizeof(d) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setEmail(const string& e) {
    strncpy(email, e.c_str(), sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setAlergias(const string& a) {
    strncpy(alergias, a.c_str(), sizeof(alergias) - 1);
    alergias[sizeof(alergias) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::setObservaciones(const string& ob) {
    strncpy(observaciones, ob.c_str(), sizeof(observaciones) - 1);
    observaciones[sizeof(observaciones) - 1] = '\0';
    fechaModificacion = time(nullptr);
}

void Paciente::marcarEliminado() {
    eliminado = true;
    activo = false;
    fechaModificacion = time(nullptr);
}

void Paciente::activar() {
    activo = true;
    fechaModificacion = time(nullptr);
}

void Paciente::desactivar() {
    activo = false;
    fechaModificacion = time(nullptr);
}

bool Paciente::agregarCita(int citaID) {
    if (cantidadCitas >= 20)
        return false;

    citasIDs[cantidadCitas++] = citaID;
    fechaModificacion = time(nullptr);
    return true;
}

// =========================
//          GETTERS
// =========================

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
const int* Paciente::getCitasIDs() const { return citasIDs; }

time_t Paciente::getFechaCreacion() const { return fechaCreacion; }
time_t Paciente::getFechaModificacion() const { return fechaModificacion; }

// =========================
//    IMPRESIÓN
// =========================

void Paciente::imprimirResumen() const {
    cout << "Paciente ID: " << id << "\n";
    cout << "Nombre: " << nombre << " " << apellido << "\n";
    cout << "Edad: " << edad << "\n";
    cout << "Telefono: " << telefono << "\n";
    cout << "Estado: " << (activo ? "Activo" : "Inactivo") << "\n";
    cout << "Citas registradas: " << cantidadCitas << "\n";
}