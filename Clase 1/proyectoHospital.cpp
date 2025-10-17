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

    char** alergias;
    int cantidadAlergias;
    int capacidadAlergias;

    char** observaciones;
    int cantidadObservaciones;
    int capacidadObservaciones;

    HistorialMedico* historial;
    int cantidadHistorial;
    int capacidadHistorial;

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

Paciente* crearPaciente(Hospital* hospital){
    char nombre[50]; char apellido[50]; char cedula[20];
    int edad;
    char sexo;

    cout << "\n=== CREAR NUEVO PACIENTE ===\n";
    cout << "Ingrese nombre: ";
    cin.ignore();
    cin.getline(nombre, 50);

    cout << "Ingrese apellido: ";
    cin.getline(apellido, 50);

    cout << "Ingrese cédula: ";
    cin.getline(cedula, 20);

    cout << "Ingrese edad: ";
    cin >> edad;

    cout << "Ingrese sexo (M/F): ";
    cin >> sexo;

    //verificar si la cedula existe
    for(int i = 0; i < hospital -> cantidadPacientes; i++){
        if(strcmp(hospital -> pacientes[i].cedula, cedula) == 0){
            cout << "Error: La cédula ya existe.\n";
            return nullptr;
        }
    }

    //Verificar si hay espacio en el array de pacientes
    if(hospital -> cantidadPacientes >= hospital -> capacidadPacientes){
        //Redimensionar array
        int nuevaCapacidad = hospital -> capacidadPacientes * 2;
        Paciente* nuevoArray = new Paciente[nuevaCapacidad];

        for(int i = 0; i < hospital -> cantidadPacientes; i++){
            nuevoArray[i] = hospital -> pacientes[i];
        }

        delete[] hospital -> pacientes;
        hospital -> pacientes = nuevoArray;
        hospital -> capacidadPacientes = nuevaCapacidad;
    }
    //Crear referencia al nuevo paciente
    Paciente& p = hospital->pacientes[hospital->cantidadPacientes];

    //Asignar datos basicos
    p.id = hospital->siguienteIdPaciente++;
    strncpy(p.nombre, nombre, 49);
    strncpy(p.apellido, apellido, 49);
    strncpy(p.cedula, cedula, 19);
    p.edad = edad;
    p.sexo = sexo;
    p.activo = true;

    //inicializar arrays dinamicos
    p.capacidadAlergias = 5;
    p.cantidadAlergias = 0;
    p.alergias = new char*[p.capacidadAlergias];

    p.capacidadObservaciones = 5;
    p.cantidadObservaciones = 0;
    p.observaciones = new char*[p.capacidadObservaciones];

    p.capacidadHistorial = 5;
    p.cantidadHistorial = 0;
    p.historial = new HistorialMedico[p.capacidadHistorial];

    p.capacidadCitas = 5;
    p.cantidadCitas = 0;
    p.citas = new Cita[p.capacidadCitas];

    //incrementar contador de pacientes en el hospital
    hospital->cantidadPacientes++;
    cout << "Paciente creado con ID: " << p.id << endl;

    return &p;
};

void mostrarMenu() {
    cout << "==Hospital Central==" << endl;
    cout << "1. Mostrar datos del hospital" << endl;
    cout << "2. Crear nuevo paciente" << endl;
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
            case 2:
                crearPaciente(hospital);
                break;
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
        }
        
    } while (opcion != 0);
    
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
    Paciente& p = hospital->pacientes[i];

    // liberar historial médico
    delete[] p.historial;

    // liberar citas del paciente
    delete[] p.citas;

    // liberar alergias (char**)
    for (int j = 0; j < p.cantidadAlergias; ++j) {
        delete[] p.alergias[j];
    }
    delete[] p.alergias;

    // liberar observaciones (char**)
    for (int j = 0; j < p.cantidadObservaciones; ++j) {
        delete[] p.observaciones[j];
    }
    delete[] p.observaciones;
}
    delete[] hospital->doctores;
    delete[] hospital->citas;

    delete[] hospital->pacientes;
    delete hospital;

    return 0;
}