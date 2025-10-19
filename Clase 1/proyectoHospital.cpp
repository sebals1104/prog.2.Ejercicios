#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>

using namespace std;

struct Hospital;
struct Doctor;
struct Cita;
struct Paciente;
struct HistorialMedico;


struct Hospital{
    char nombre[100];
    char direccion[200];
    char telefono[15];

    Doctor* doctores;
    int cantidadDoctores;
    int capacidadDoctores;
    
    Cita* citas;
    int cantidadCitas;
    int capacidadCitas;

    Paciente* pacientes;
    int cantidadPacientes;
    int capacidadPacientes;
    
    int siguienteIdPaciente;
    int siguienteIdDoctor;
    int siguienteIdCita;
    int siguienteIdConsulta;
};

struct Paciente{
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

    Doctor* doctores;
    int cantidadDoctores;
    int capacidadDoctores;
    
    Cita* citas;
    int cantidadCitas;
    int capacidadCitas;
    
    int siguienteIdPaciente;
    int siguienteIdDoctor;
    int siguienteIdCita;
    int siguienteIdConsulta;
    bool activo;

    char** alergias;
    int cantidadAlergias;
    int capacidadAlergias;

    char** observaciones;
    int cantidadObservaciones;
    int capacidadObservaciones;
};

struct HistorialMedico {
    int idConsulta;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int idDoctor;
    float costo;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];

    int* pacientesAsignados;
    int cantidadPacientes;
    int capacidadPacientes;
    
    int* citasAgendadas;
    int cantidadCitas;
    int capacidadCitas;
};

struct Cita {
    int id;
    int idPaciente;
    int idDoctor;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
};

int compararCadenas(const char* a, const char* b) {
    if (!a || !b) return -1; // si alguno es nulo
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

void* buscarPorId(Hospital* hospital, const char* tipo, int idBuscado) {
    if (compararCadenas(tipo, "paciente") == 0) {
        for (int i = 0; i < hospital->cantidadPacientes; i++) {
            if (hospital->pacientes[i].id == idBuscado) {
                return &hospital->pacientes[i];
            }
        }
    } 
    else if (compararCadenas(tipo, "doctor") == 0) {
        for (int i = 0; i < hospital->cantidadDoctores; i++) {
            if (hospital->doctores[i].id == idBuscado) {
                return &hospital->doctores[i];
            }
        }
    }

    return nullptr;
}

Hospital* crearHospital(const char* nombre, const char* direccion, const char* telefono){
    Hospital* h = new Hospital;
    strncpy(h->nombre, nombre, sizeof(h->nombre)-1); h->nombre[sizeof(h->nombre)-1] = '\0';
    strncpy(h->direccion, direccion, sizeof(h->direccion)-1); h->direccion[sizeof(h->direccion)-1] = '\0';
    strncpy(h->telefono, telefono, sizeof(h->telefono)-1); h->telefono[sizeof(h->telefono)-1] = '\0';

    // Inicializar arrays dinamicos
    h->capacidadDoctores = 5;
    h->cantidadDoctores = 0;
    h->doctores = new Doctor[h->capacidadDoctores];

    h->capacidadCitas = 10;
    h->cantidadCitas = 0;
    h->citas = new Cita[h->capacidadCitas];

    // Contadores para IDs unicos
    h->siguienteIdPaciente = 1;
    h->siguienteIdDoctor = 1;
    h->siguienteIdCita = 1;
    h->siguienteIdConsulta = 1;

    h->capacidadPacientes = 10;
    h->cantidadPacientes = 0;
    h->pacientes = new Paciente[h->capacidadPacientes];

    return h;
}

Paciente* crearPaciente(Hospital* hospital, const char* nombre,const char* apellido,const char* cedula, int edad, char sexo,const char* tipoSangre,const char* telefono,const char* direccion, const char* email){
    //verificar cedula
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (compararCadenas(hospital->pacientes[i].cedula, cedula) == 0) {
            cout << "Error: La cédula ya existe en el sistema.\n";
            return nullptr;
        }
    }

    //verficiar capacidad
    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        int nuevaCapacidad = hospital->capacidadPacientes * 2;
        Paciente* nuevosPacientes = new Paciente[nuevaCapacidad];
        for (int i = 0; i < hospital->cantidadPacientes; i++) {
            nuevosPacientes[i] = hospital->pacientes[i]; // copia superficial
        }
        delete[] hospital->pacientes;
        hospital->pacientes = nuevosPacientes;
        hospital->capacidadPacientes = nuevaCapacidad;
    }

    //crear paciente
    Paciente& p = hospital->pacientes[hospital->cantidadPacientes];

    p.id = hospital->siguienteIdPaciente++;
    strncpy(p.nombre, nombre, sizeof(p.nombre)-1); p.nombre[sizeof(p.nombre)-1] = '\0';
    strncpy(p.apellido, apellido, sizeof(p.apellido)-1); p.apellido[sizeof(p.apellido)-1] = '\0';
    strncpy(p.cedula, cedula, sizeof(p.cedula)-1); p.cedula[sizeof(p.cedula)-1] = '\0';
    strncpy(p.tipoSangre, tipoSangre, sizeof(p.tipoSangre)-1); p.tipoSangre[sizeof(p.tipoSangre)-1] = '\0';
    strncpy(p.telefono, telefono, sizeof(p.telefono)-1); p.telefono[sizeof(p.telefono)-1] = '\0';
    strncpy(p.direccion, direccion, sizeof(p.direccion)-1); p.direccion[sizeof(p.direccion)-1] = '\0';
    strncpy(p.email, email, sizeof(p.email)-1); p.email[sizeof(p.email)-1] = '\0';
    p.edad = edad;
    p.sexo = sexo;
    p.activo = true;

    //historial medico
    p.capacidadCitas = 5;
    p.cantidadCitas = 0;
    p.citas = new Cita[p.capacidadCitas];

    //Citas
    p.cantidadCitas = 0;
    p.capacidadCitas = 5;
    p.citas = new Cita[p.capacidadCitas];

    //alergias y observaciones
    p.capacidadAlergias = 5;
    p.cantidadAlergias = 0;
    p.alergias = new char*[p.capacidadAlergias];
    for (int i = 0; i < p.capacidadAlergias; i++) {
        p.alergias[i] = new char[50];
        p.alergias[i][0] = '\0';
    }

    p.capacidadObservaciones = 5;
    p.cantidadObservaciones = 0;
    p.observaciones = new char*[p.capacidadObservaciones];
    for (int i = 0; i < p.capacidadObservaciones; i++) {
        p.observaciones[i] = new char[150];
        p.observaciones[i][0] = '\0';
    }

    hospital -> cantidadPacientes++;

    return &p;
};

void mostrarMenu() {
    cout << "==Hospital Central==" << endl;
    cout << "1. Mostrar datos del hospital" << endl;
    cout << "2. Agregar nuevo paciente" << endl;
    //siguente funcion
    cout << "0. Salir" << endl;
}

void mostarDatosHospital(const Hospital* h){
    cout << "Datos del hospital" << endl;
    cout << "Nombre: " << h -> nombre << endl;
    cout << "Direccion: " << h -> direccion << endl;
    cout << "Telefono: " << h -> telefono << endl;
}

int main(){
    Hospital* hospital = crearHospital("Hospital Central", "Av. Siempre Viva 123", "555-1234");
    int opcion;
    
    do{
        mostrarMenu();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch(opcion){
            case 1:
                mostarDatosHospital(hospital);
                cout << "presione enter para continuar...";
                cin.get();
                break;
            case 2:{
                char nombre[50], apellido[50], cedula[20], tipoSangre[5], telefono[15], direccion[100], email[50];
                int edad;
                char sexo;

                cout << "Nombre: "; cin.getline(nombre, 50); while(strlen(nombre) == 0) {
                    cout << "El nombre no puede estar vacío. Ingrese nuevamente: ";
                    cin.getline(nombre, 50);
                }
                cout << "Apellido: "; cin.getline(apellido, 50); while(strlen(apellido) == 0) {
                    cout << "El apellido no puede estar vacío. Ingrese nuevamente: ";
                    cin.getline(apellido, 50);
                }
                cout << "Cédula: "; cin.getline(cedula, 20); while(strlen(cedula) == 0) {
                    cout << "La cédula no puede estar vacía. Ingrese nuevamente: ";
                    cin.getline(cedula, 20);
                }
                cout << "Edad: "; cin >> edad; while(edad <= 0) {
                    cout << "La edad debe ser un número positivo. Ingrese nuevamente: ";
                    cin >> edad;
                }
                cin.ignore();
                cout << "Sexo (M/F): "; cin >> sexo; while(sexo != 'M' && sexo != 'F' && sexo != 'm' && sexo != 'f') {
                    cout << "Sexo inválido. Ingrese 'M' para masculino o 'F' para femenino: ";
                    cin >> sexo;
                }
                cin.ignore();
                cout << "Tipo de sangre: "; cin.getline(tipoSangre, 5);while(strlen(tipoSangre) == 0) {
                    cout << "El tipo de sangre no puede estar vacío. Ingrese nuevamente: ";
                    cin.getline(tipoSangre, 5);
                }
                cout << "Teléfono: "; cin.getline(telefono, 15); while(strlen(telefono) == 0) {
                    cout << "El teléfono no puede estar vacío. Ingrese nuevamente: ";
                    cin.getline(telefono, 15);
                }
                cout << "Dirección: "; cin.getline(direccion, 100); while(strlen(direccion) == 0) {
                    cout << "La dirección no puede estar vacía. Ingrese nuevamente: ";
                    cin.getline(direccion, 100);
                }
                cout << "Email: "; cin.getline(email, 50); while(strlen(email) == 0) {
                    cout << "El email no puede estar vacío. Ingrese nuevamente: ";
                    cin.getline(email, 50);
                }

                Paciente* p = crearPaciente(hospital, nombre, apellido, cedula, edad, sexo, tipoSangre, telefono, direccion, email);
                if (p != nullptr) {
                    cout << "Paciente creado exitosamente. ID asignado: " << p->id << endl;
                } else {
                    cout << "No se pudo crear el paciente." << endl;
                }

                cout << "Presione enter para continuar...";
                cin.get();
                break;
                }
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
            cout << "opcion no valida";
            break;
        }
        
    } while (opcion != 0);

    // Liberar memoria (como antes)
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente& p = hospital->pacientes[i];
        delete[] p.citas;
        for (int j = 0; j < p.capacidadAlergias; j++) delete[] p.alergias[j];
        delete[] p.alergias;
        for (int j = 0; j < p.capacidadObservaciones; j++) delete[] p.observaciones[j];
        delete[] p.observaciones;
    }
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        Doctor& d = hospital->doctores[i];
        delete[] d.pacientesAsignados;
        delete[] d.citasAgendadas;
    }

    delete[] hospital->pacientes;
    delete[] hospital->doctores;
    delete[] hospital->citas;
    delete hospital;
    return 0;
}