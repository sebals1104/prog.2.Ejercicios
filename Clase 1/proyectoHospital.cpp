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



Hospital* crearHospital(const char* nombre, const char* direccion, const char* telefono) {
    Hospital* h = new Hospital;
    strncpy(h->nombre, nombre, 99);
    strncpy(h->direccion, direccion, 199);
    strncpy(h->telefono, telefono, 14);

    // Inicializar arrays dinámicos
    h->capacidadDoctores = 5;
    h->cantidadDoctores = 0;
    h->doctores = new Doctor[h->capacidadDoctores];

    h->capacidadCitas = 10;
    h->cantidadCitas = 0;
    h->citas = new Cita[h->capacidadCitas];

    // Contadores para IDs únicos
    h->siguienteIdPaciente = 1;
    h->siguienteIdDoctor = 1;
    h->siguienteIdCita = 1;
    h->siguienteIdConsulta = 1;

    h->capacidadPacientes = 10;
    h->cantidadPacientes = 0;
    h->pacientes = new Paciente[h->capacidadPacientes];

    return h;
}

void mostrarMenu() {
    cout << "==Hospital Central==" << endl;
    cout << "1. Mostrar datos del hospital" << endl;
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
        cin.ignore(); // Limpiar el buffer de entrada
    } while (opcion != 0);
    delete[] hospital->doctores;
    delete[] hospital->pacientes;
    delete[] hospital->citas;
    delete hospital;

    return 0;
}