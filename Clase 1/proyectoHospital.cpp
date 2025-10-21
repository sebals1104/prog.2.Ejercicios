#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <limits>

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
    
    HistorialMedico* historial;   
    int cantidadConsultas;          
    int capacidadHistorial; 

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

//------------------------Validaciones-----------------------------
int compararCadenas(const char* a, const char* b) {
    if (!a || !b) return -1; // si alguno es nulo
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

bool verificarCedulaExistente(Hospital* hospital, const char* cedula, bool esDoctor) {
    if (!hospital || !cedula) return false;

    if (esDoctor) {
        for (int i = 0; i < hospital->cantidadDoctores; i++) {
            if (compararCadenas(hospital->doctores[i].cedula, cedula) == 0) {
                cout << " Error: La cédula ya existe en el sistema (doctor).\n";
                return true;
            }
        }
    } else {
        for (int i = 0; i < hospital->cantidadPacientes; i++) {
            if (compararCadenas(hospital->pacientes[i].cedula, cedula) == 0) {
                cout << "Error: La cédula ya existe en el sistema (paciente).\n";
                return true;
            }
        }
    }

    return false; // no existe duplicado
}

template <typename T>
void redimensionar(T*& array, int& capacidad) {
    int nuevaCapacidad = (capacidad == 0) ? 1 : capacidad * 2; // si capacidad es 0, inicializar a 1
    T* nuevoArray = new T[nuevaCapacidad];

    // Copiar elementos existentes al nuevo array
    for (int i = 0; i < capacidad; i++) {
        nuevoArray[i] = array[i];
    }

    delete[] array;
    array = nuevoArray;
    capacidad = nuevaCapacidad;
}

void destruirHospital(Hospital* hospital) {
    if (!hospital) return;

    //Para cada paciente: liberar historial, citas, alergias y observaciones
    for (int i = 0; i < hospital->cantidadPacientes; ++i) {
        Paciente& p = hospital->pacientes[i];

        if (p.historial) {
            delete[] p.historial;
            p.historial = nullptr;
            p.cantidadConsultas = 0;
            p.capacidadHistorial = 0;
        }

        if (p.citas) {
            delete[] p.citas;
            p.citas = nullptr;
            p.cantidadCitas = 0;
            p.capacidadCitas = 0;
        }

        if (p.alergias) {
            // liberar cada cadena y luego el array de punteros
            for (int j = 0; j < p.capacidadAlergias; ++j) {
                if (p.alergias[j]) {
                    delete[] p.alergias[j];
                    p.alergias[j] = nullptr;
                }
            }
            delete[] p.alergias;
            p.alergias = nullptr;
            p.cantidadAlergias = 0;
            p.capacidadAlergias = 0;
        }

        if (p.observaciones) {
            for (int j = 0; j < p.capacidadObservaciones; ++j) {
                if (p.observaciones[j]) {
                    delete[] p.observaciones[j];
                    p.observaciones[j] = nullptr;
                }
            }
            delete[] p.observaciones;
            p.observaciones = nullptr;
            p.cantidadObservaciones = 0;
            p.capacidadObservaciones = 0;
        }
    }

    //Liberar array pacientes
    if (hospital->pacientes) {
        delete[] hospital->pacientes;
        hospital->pacientes = nullptr;
        hospital->cantidadPacientes = 0;
        hospital->capacidadPacientes = 0;
    }

    //Para cada doctor: liberar pacientesAsignados y citasAgendadas
    for (int i = 0; i < hospital->cantidadDoctores; ++i) {
        Doctor& d = hospital->doctores[i];
        if (d.pacientesAsignados) {
            delete[] d.pacientesAsignados;
            d.pacientesAsignados = nullptr;
            d.cantidadPacientes = 0;
            d.capacidadPacientes = 0;
        }
        if (d.citasAgendadas) {
            delete[] d.citasAgendadas;
            d.citasAgendadas = nullptr;
            d.cantidadCitas = 0;
            d.capacidadCitas = 0;
        }
    }

    //Liberar array doctores
    if (hospital->doctores) {
        delete[] hospital->doctores;
        hospital->doctores = nullptr;
        hospital->cantidadDoctores = 0;
        hospital->capacidadDoctores = 0;
    }

    //Liberar array citas
    if (hospital->citas) {
        delete[] hospital->citas;
        hospital->citas = nullptr;
        hospital->cantidadCitas = 0;
        hospital->capacidadCitas = 0;
    }

    //Liberar estructura Hospital
    delete hospital;
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') {
        return false;
    }

    int anio = atoi(string(fecha, 4).c_str());
    int mes = atoi(string(fecha + 5, 2).c_str());
    int dia = atoi(string(fecha + 8, 2).c_str());

    if (anio < 1900 || mes < 1 || mes > 12 || dia < 1) {
        return false;
    }

    int diasEnMes[] = {31, (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0)) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dia > diasEnMes[mes - 1]) {
        return false;
    }

    return true;
}

bool validarHora(const char* hora) {
    if (strlen(hora) != 5 || hora[2] != ':') {
        return false;
    }

    int horas = atoi(string(hora, 2).c_str());
    int minutos = atoi(string(hora + 3, 2).c_str());

    if (horas < 0 || horas > 23 || minutos < 0 || minutos > 59) {
        return false;
    }

    return true;
}

int compararFechas(const char* fecha1, const char* fecha2){
    return compararCadenas(fecha1, fecha2);
};

bool validarCedula(const char* cedula){
    int len = strlen(cedula);
    if (len < 7 || len > 20) return false; // longitud valida

    for (int i = 0; i < len; i++) {
        if ((cedula[i] < '0' || cedula[i] > '9') && cedula[i] != '-') {
            return false; // solo digitos y guiones
        }
    }
    return true;
};

bool validarEmail(const char* email){
    const char* atPos = strchr(email, '@');
    if (!atPos) return false; // debe contener '@'

    const char* dotPos = strrchr(atPos, '.');
    if (!dotPos || dotPos == atPos + 1) return false; // debe contener '.' después de '@'

    return true;
};

//---------------------- Funciones principales----------------------

Hospital* crearHospital(const char* nombre, const char* direccion, const char* telefono){
    Hospital* h = new Hospital;
    strncpy(h->nombre, nombre, sizeof(h->nombre)-1); h->nombre[sizeof(h->nombre)-1] = '\0';
    strncpy(h->direccion, direccion, sizeof(h->direccion)-1); h->direccion[sizeof(h->direccion)-1] = '\0';
    strncpy(h->telefono, telefono, sizeof(h->telefono)-1); h->telefono[sizeof(h->telefono)-1] = '\0';

    h->capacidadDoctores = 5;
    h->cantidadDoctores = 0;
    h->doctores = new Doctor[h->capacidadDoctores];

    h->capacidadCitas = 10;
    h->cantidadCitas = 0;
    h->citas = new Cita[h->capacidadCitas];

    h->siguienteIdPaciente = 1;
    h->siguienteIdDoctor = 1;
    h->siguienteIdCita = 1;
    h->siguienteIdConsulta = 1;

    h->capacidadPacientes = 10;
    h->cantidadPacientes = 0;
    h->pacientes = new Paciente[h->capacidadPacientes];

    return h;
}
//------------------------Paceientes-----------------------------
Paciente* crearPaciente(Hospital* hospital, const char* nombre,const char* apellido,const char* cedula, int edad, char sexo,const char* tipoSangre,const char* telefono,const char* direccion, const char* email){
    // Verificar cedula duplicada
    if (verificarCedulaExistente(hospital, cedula, false)) {
        return nullptr;
    }

    // Verificar capacidad
    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        redimensionar(hospital->pacientes, hospital->capacidadPacientes);
    }

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

    //inicializar citas
    p.capacidadCitas = 5;
    p.cantidadCitas = 0;
    p.citas = new Cita[p.capacidadCitas];

    //inicializar alergias
    p.capacidadAlergias = 5;
    p.cantidadAlergias = 0;
    p.alergias = new char*[p.capacidadAlergias];
    for (int i = 0; i < p.capacidadAlergias; i++) {
        p.alergias[i] = new char[50];
        p.alergias[i][0] = '\0';
    }

    //inicializar observaciones
    p.capacidadObservaciones = 5;
    p.cantidadObservaciones = 0;
    p.observaciones = new char*[p.capacidadObservaciones];
    for (int i = 0; i < p.capacidadObservaciones; i++) {
        p.observaciones[i] = new char[150];
        p.observaciones[i][0] = '\0';
    }

    hospital->cantidadPacientes++;
    //retornar a puntero
    return &p;
}

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    if (!hospital || !cedula) return nullptr;

    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        const char* c1 = hospital->pacientes[i].cedula;
        const char* c2 = cedula;
        bool igual = true;
        int j = 0;
        while (c1[j] != '\0' && c2[j] != '\0') {
            char ch1 = c1[j], ch2 = c2[j];
            if (ch1 >= 'A' && ch1 <= 'Z') ch1 += 32;
            if (ch2 >= 'A' && ch2 <= 'Z') ch2 += 32;
            if (ch1 != ch2) { igual = false; break; }
            j++;
        }
        if (c1[j] != '\0' || c2[j] != '\0') igual = false;
        if (igual) return &hospital->pacientes[i];
    }
    return nullptr;
}

void buscarPacientePorNombre(Hospital* hospital) {
    if (hospital->cantidadPacientes == 0) {
        cout << "No hay pacientes registrados.\n";
        return;
    }

    char nombreBuscar[50];
    cout << "Ingrese el nombre del paciente a buscar: ";
    cin.ignore(); // limpiar buffer
    cin.getline(nombreBuscar, 50);

    // Convertir nombreBuscar a minúsculas para comparación
    char nombreMinusculas[50];
    int i = 0;
    while (nombreBuscar[i] != '\0' && i < 49) {
        nombreMinusculas[i] = (nombreBuscar[i] >= 'A' && nombreBuscar[i] <= 'Z') ? nombreBuscar[i]+32 : nombreBuscar[i];
        i++;
    }
    nombreMinusculas[i] = '\0';

    bool encontrado = false;
    for (int j = 0; j < hospital->cantidadPacientes; j++) {
        char nombrePaciente[50];
        int k = 0;
        while (hospital->pacientes[j].nombre[k] != '\0' && k < 49) {
            nombrePaciente[k] = (hospital->pacientes[j].nombre[k] >= 'A' && hospital->pacientes[j].nombre[k] <= 'Z') ? hospital->pacientes[j].nombre[k]+32 : hospital->pacientes[j].nombre[k];
            k++;
        }
        nombrePaciente[k] = '\0';

        if (strstr(nombrePaciente, nombreMinusculas) != nullptr) {
            Paciente& p = hospital->pacientes[j];
            cout << "\nPaciente encontrado:\n";
            cout << "ID: " << p.id << "\n";
            cout << "Nombre: " << p.nombre << " " << p.apellido << "\n";
            cout << "Cédula: " << p.cedula << "\n";
            cout << "Edad: " << p.edad << "\n";
            cout << "Sexo: " << p.sexo << "\n";
            cout << "Tipo de sangre: " << p.tipoSangre << "\n";
            cout << "Teléfono: " << p.telefono << "\n";
            cout << "Dirección: " << p.direccion << "\n";
            cout << "Email: " << p.email << "\n";
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron pacientes con ese nombre.\n";
    }

    cout << "Presione ENTER para continuar...";
    cin.get();
}

bool actualizarPaciente(Paciente* p) {
    if (!p) return false;

    char nombre[50], apellido[50], cedula[20], tipoSangre[5], telefono[15], direccion[100], email[50];
    int edad;
    char sexo;

    cout << "\n--- ACTUALIZAR DATOS DEL PACIENTE ---\n";

    cout << "Nombre actual: " << p->nombre << "\nNuevo nombre: ";
    cin.ignore(); // limpiar buffer
    cin.getline(nombre, 50);
    if (strlen(nombre) > 0) strncpy(p->nombre, nombre, sizeof(p->nombre)-1);

    cout << "Apellido actual: " << p->apellido << "\nNuevo apellido: ";
    cin.getline(apellido, 50);
    if (strlen(apellido) > 0) strncpy(p->apellido, apellido, sizeof(p->apellido)-1);

    cout << "Cédula actual: " << p->cedula << "\nNueva cédula: ";
    cin.getline(cedula, 20);
    if (strlen(cedula) > 0) strncpy(p->cedula, cedula, sizeof(p->cedula)-1);

    cout << "Edad actual: " << p->edad << "\nNueva edad: ";
    cin >> edad; 
    if (edad > 0) p->edad = edad;

    cin.ignore(); // limpiar buffer
    cout << "Sexo actual: " << p->sexo << "\nNuevo sexo (M/F): ";
    cin >> sexo;
    if (sexo == 'M' || sexo == 'F' || sexo == 'm' || sexo == 'f') p->sexo = sexo;
    cin.ignore();

    cout << "Tipo de sangre actual: " << p->tipoSangre << "\nNuevo tipo de sangre: ";
    cin.getline(tipoSangre, 5);
    if (strlen(tipoSangre) > 0) strncpy(p->tipoSangre, tipoSangre, sizeof(p->tipoSangre)-1);

    cout << "Teléfono actual: " << p->telefono << "\nNuevo teléfono: ";
    cin.getline(telefono, 15);
    if (strlen(telefono) > 0) strncpy(p->telefono, telefono, sizeof(p->telefono)-1);

    cout << "Dirección actual: " << p->direccion << "\nNueva dirección: ";
    cin.getline(direccion, 100);
    if (strlen(direccion) > 0) strncpy(p->direccion, direccion, sizeof(p->direccion)-1);

    cout << "Email actual: " << p->email << "\nNuevo email: ";
    cin.getline(email, 50);
    if (strlen(email) > 0) strncpy(p->email, email, sizeof(p->email)-1);

    cout << "\nDatos actualizados correctamente.\n";
    return true;
}

bool eliminarPaciente(Hospital* hospital, int id){
    if (!hospital) return false;

    int index = -1;
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) return false; // paciente no encontrado

    Paciente& p = hospital->pacientes[index];

    // Cancelar todas las citas del paciente
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        Cita& c = hospital->citas[i];
        if (c.idPaciente == p.id) {
            c.atendida = false;
            strncpy(c.estado, "Cancelada", sizeof(c.estado)-1);
            c.estado[sizeof(c.estado)-1] = '\0';
        }
    }

    //Remover paciente de los doctores asignados
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        Doctor& d = hospital->doctores[i];
        int pos = 0;
        for (int j = 0; j < d.cantidadPacientes; j++) {
            if (d.pacientesAsignados[j] != p.id) {
                d.pacientesAsignados[pos++] = d.pacientesAsignados[j];
            }
        }
        d.cantidadPacientes = pos;
    }

    // Liberar memoria de citas del paciente
    delete[] p.citas;

    //Liberar memoria de alergias
    for (int i = 0; i < p.capacidadAlergias; i++) {
        delete[] p.alergias[i];
    }
    delete[] p.alergias;

    // Liberar memoria de observaciones
    for (int i = 0; i < p.capacidadObservaciones; i++) {
        delete[] p.observaciones[i];
    }
    delete[] p.observaciones;

    // Compactar el array de pacientes
    for (int i = index; i < hospital->cantidadPacientes - 1; i++) {
        hospital->pacientes[i] = hospital->pacientes[i+1];
    }
    hospital->cantidadPacientes--;

    return true;
};

void listarPacientes(Hospital* hospital){
    if (hospital->cantidadPacientes == 0) {
        cout << "No hay pacientes registrados.\n";
        return;
    }

    // Cabecera de la tabla
    cout << left << setw(5) << "ID"
        << setw(25) << "Nombre Completo"
        << setw(20) << "Cédula"
        << setw(5) << "Edad"
        << setw(10) << "Consultas" << endl;
    cout << string(65, '=') << endl;

    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente& p = hospital->pacientes[i];

        // Concatenar nombre completo
        string nombreCompleto = string(p.nombre) + " " + string(p.apellido);

        cout << left << setw(5) << p.id
        << setw(25) << nombreCompleto
        << setw(20) << p.cedula
        << setw(5) << p.edad
        << setw(10) << p.cantidadCitas // cantidad de citas como proxy de consultas
        << endl;
    }
};

//-------------------------Historial medico----------------------------
void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta){
    if (!paciente) return; //validar puntero
    // Verificar capacidad
    if(paciente->cantidadConsultas >= paciente->capacidadHistorial){
        int nuevaCapacidad = (paciente->capacidadHistorial > 0) ? paciente->capacidadHistorial * 2 : 2; // si estaba en 0, arranca con 2
        HistorialMedico* nuevoHistorial = new HistorialMedico[nuevaCapacidad];
        for(int i = 0; i < paciente->cantidadConsultas; i++){
            nuevoHistorial[i] = paciente->historial[i];
        }
        //Liberar memoria antigua
        delete[] paciente->historial;
        //Asignar nuevo historial
        paciente->historial = nuevoHistorial;
        paciente->capacidadHistorial = nuevaCapacidad;
    }
    paciente->historial[paciente->cantidadConsultas++] = consulta;
    paciente->siguienteIdConsulta++;
    cout << "Consulta agregada al historial medico del paciente.\n";
};

HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad){
    if (!paciente || !cantidad) return nullptr; //validar punteros
    *cantidad = paciente->cantidadConsultas;
    return paciente->historial;
};

void mostrarHistorialMedico(Paciente* paciente) {
    if (!paciente || paciente->cantidadConsultas == 0) {
        cout << "El paciente no tiene consultas registradas.\n";
        return;
    }

    cout << "\n--- HISTORIAL MÉDICO DEL PACIENTE ---\n";
    cout << left << setw(12) << "Fecha"
        << setw(7) << "Hora"
        << setw(25) << "Diagnóstico"
        << setw(12) << "ID Doctor"
        << setw(8) << "Costo" << endl;
    cout << string(64, '=') << endl;

    for (int i = 0; i < paciente->cantidadConsultas; i++) {
        HistorialMedico& h = paciente->historial[i];
        cout << left << setw(12) << h.fecha
            << setw(7) << h.hora
            << setw(25) << h.diagnostico
            << setw(12) << h.idDoctor
            << "$" << fixed << setprecision(2) << h.costo << endl;
    }
    cout << string(64, '=') << endl;
}

HistorialMedico* obtenerUltimaConsulta(Paciente* paciente){
    if (!paciente || paciente->cantidadConsultas == 0) return nullptr; //validar punteros
    return &paciente->historial[paciente->cantidadConsultas - 1];
};

//-----------------------Doctores----------------------------

Doctor* crearDoctor(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula,const char* especialidad, int aniosExperiencia, float costoConsulta){
    if (!hospital) return nullptr;
    //Verificar cedula duplicada
    if (verificarCedulaExistente(hospital, cedula, true)) {
        return nullptr;
    }

    //verificar capacidad
    if (hospital->cantidadDoctores >= hospital->capacidadDoctores) {
        redimensionar(hospital->doctores, hospital->capacidadDoctores);
    }

    //inicializar doctor
    Doctor& d = hospital->doctores[hospital->cantidadDoctores];
    d.id = hospital->siguienteIdDoctor++;
    strncpy(d.nombre, nombre, sizeof(d.nombre)-1); d.nombre[sizeof(d.nombre)-1] = '\0';
    strncpy(d.apellido, apellido, sizeof(d.apellido)-1); d.apellido[sizeof(d.apellido)-1] = '\0';
    strncpy(d.cedula, cedula, sizeof(d.cedula)-1); d.cedula[sizeof(d.cedula)-1] = '\0';
    strncpy(d.especialidad, especialidad, sizeof(d.especialidad)-1); d.especialidad[sizeof(d.especialidad)-1] = '\0';
    d.aniosExperiencia = aniosExperiencia;
    d.costoConsulta = costoConsulta;

    //inicializar pacientes asignados
    d.capacidadPacientes = 5;
    d.cantidadPacientes = 0;
    d.pacientesAsignados = new int[d.capacidadPacientes];

    //inicializar citas agendadas
    d.capacidadCitas = 5;
    d.cantidadCitas = 0;
    d.citasAgendadas = new int[d.capacidadCitas];

    //incrementar cantidad de doctores en el hospital
    hospital->cantidadDoctores++;
    //retornar puntero al doctor creado
    return &d;
};

Doctor* buscarDoctorPorId(Hospital* hospital, int id) {
    if (!hospital) return nullptr;

    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (hospital->doctores[i].id == id) {
            return &hospital->doctores[i];
        }
    }
    return nullptr; // doctor no encontrado
}

Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad){
    if (hospital == nullptr || especialidad == nullptr || cantidad == nullptr) 
        return nullptr;
    Doctor** resultados = nullptr;

    //contar doctores que coinciden con la especialidad
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (compararCadenas(hospital->doctores[i].especialidad, especialidad) == 0) {
            (*cantidad)++;
        }
    }
    //si no hay coincidencias, retornar nullptr
    if (*cantidad == 0) {
        return nullptr;
    }

    //crear array para resultados
    resultados = new Doctor*[*cantidad];

    //llenar array con punteros a doctores que coinciden
    int j = 0;
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (compararCadenas(hospital->doctores[i].especialidad, especialidad) == 0) {
            resultados[j++] = &hospital->doctores[i];
        }
    }

    //retornar array de punteros a doctores
    return resultados;
};

bool asignarPacienteADoctor(Doctor* doctor, int idPaciente){
    if (!doctor) return false;

    // Verificar si el paciente ya está asignado
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        if (doctor->pacientesAsignados[i] == idPaciente) {
            cout << "El paciente ya está asignado a este doctor.\n";
            return false;
        }
    }

    // Verificar capacidad
    if (doctor->cantidadPacientes >= doctor->capacidadPacientes) {
        redimensionar(doctor->pacientesAsignados, doctor->capacidadPacientes);
    }

    // Asignar paciente
    doctor->pacientesAsignados[doctor->cantidadPacientes++] = idPaciente;
    cout << "Paciente asignado al doctor correctamente.\n";
    return true;
};

bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente){
    if (!doctor) return false;

    int index = -1;
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        if (doctor->pacientesAsignados[i] == idPaciente) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "El paciente no está asignado a este doctor.\n";
        return false; // paciente no encontrado
    }

    // Compactar el array de pacientes asignados
    for (int i = index; i < doctor->cantidadPacientes - 1; i++) {
        doctor->pacientesAsignados[i] = doctor->pacientesAsignados[i+1];
    }
    doctor->cantidadPacientes--;
    cout << "Paciente removido del doctor correctamente.\n";
    return true;
};

void listarPacientesDeDoctor(Hospital* hospital, int idDoctor){
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (!doctor) {
        cout << "Doctor no encontrado.\n";
        return;
    }
    if (doctor->cantidadPacientes == 0) {
        cout << "El doctor no tiene pacientes asignados.\n";
        return;
    }

    cout << "\n--- PACIENTES ASIGNADOS AL DOCTOR " << doctor->nombre << " " << doctor->apellido << " ---\n";
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        int idPaciente = doctor->pacientesAsignados[i];
        Paciente* paciente = nullptr;
        // Buscar paciente por ID
        for (int j = 0; j < hospital->cantidadPacientes; j++) {
            if (hospital->pacientes[j].id == idPaciente) {
                paciente = &hospital->pacientes[j];
                break;
            }
        }
        if (paciente) {
            cout << "ID: " << paciente->id << ", Nombre: " << paciente->nombre << " " << paciente->apellido << ", Cédula: " << paciente->cedula << endl;
        }
    }
};

void listarDoctores(Hospital* hospital){
    if (hospital->cantidadDoctores == 0) {
        cout << "No hay doctores registrados.\n";
        return;
    }

    cout << "\n--- LISTA DE DOCTORES REGISTRADOS ---\n";
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        Doctor& d = hospital->doctores[i];
        cout << "ID: " << d.id << ", Nombre: " << d.nombre << " " << d.apellido << ", Cédula: " << d.cedula
            << ", Especialidad: " << d.especialidad << ", Años de experiencia: " << d.aniosExperiencia
            << ", Costo de consulta: $" << fixed << setprecision(2) << d.costoConsulta << endl;
    }
};

bool eliminarDoctor(Hospital* hospital, int id){
    if (!hospital) return false;

    int index = -1;
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (hospital->doctores[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) return false; // doctor no encontrado

    Doctor& d = hospital->doctores[index];

    // Remover doctor de los pacientes asignados
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente& p = hospital->pacientes[i];
        int pos = 0;
        for (int j = 0; j < p.cantidadDoctores; j++) {
            if (p.doctores[j].id != d.id) {
                p.doctores[pos++] = p.doctores[j];
            }
        }
        p.cantidadDoctores = pos;
    }

    // Liberar memoria de pacientes asignados y citas agendadas
    delete[] d.pacientesAsignados;
    delete[] d.citasAgendadas;

    // Compactar el array de doctores
    for (int i = index; i < hospital->cantidadDoctores - 1; i++) {
        hospital->doctores[i] = hospital->doctores[i+1];
    }
    hospital->cantidadDoctores--;

    return true;
};

//-----------------------Citas----------------------------

Cita* agendarCita(Hospital* hospital, int idPaciente, int idDoctor,const char* fecha, const char* hora, const char* motivo){
    if (!hospital) return nullptr;

    //Verificar que el paciente exista
    Paciente* paciente = nullptr;
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == idPaciente) { paciente = &hospital->pacientes[i]; break; }
    }
    if (!paciente) { cout << "Error: Paciente no encontrado.\n"; return nullptr; }

    //Verificar que el doctor exista
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (!doctor) { cout << "Error: Doctor no encontrado.\n"; return nullptr; }

    //Validar formato de fecha YYYY-MM-DD 
    if (!fecha || strlen(fecha) != 10 || fecha[4] != '-' || fecha[7] != '-') {
        cout << "Error: Formato de fecha invalido. Use YYYY-MM-DD\n"; return nullptr;
    }
    bool ok = true;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (fecha[i] < '0' || fecha[i] > '9') { ok = false; break; }
    }
    if (!ok) { cout << "Error: Formato de fecha invalido. Use YYYY-MM-DD\n"; return nullptr; }

    // parsear año/mes/dia directamente y validar rango completo (incluye bisiesto)
    int year  = (fecha[0]-'0')*1000 + (fecha[1]-'0')*100 + (fecha[2]-'0')*10 + (fecha[3]-'0');
    int month = (fecha[5]-'0')*10 + (fecha[6]-'0');
    int day   = (fecha[8]-'0')*10 + (fecha[9]-'0');

    if (month < 1 || month > 12) { cout << "Error: Mes fuera de rango.\n"; return nullptr; }

    int maxDay = 31;
    if (month == 2) {
        bool leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        maxDay = leap ? 29 : 28;
    } else if (month==4 || month==6 || month==9 || month==11) {
        maxDay = 30;
    }

    if (day < 1 || day > maxDay) {
        cout << "Error: Dia fuera de rango para el mes dado.\n";
        return nullptr;
    }

    // opcional: validar rango de año razonable
    if (year < 1900 || year > 2100) { cout << "Error: Año fuera de rango.\n"; return nullptr; }

    //Verificar disponibilidad del doctor (no debe tener otra cita a esa fecha/hora)
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        Cita& existente = hospital->citas[i];
        if (existente.idDoctor == idDoctor && compararCadenas(existente.fecha, fecha) == 0 && compararCadenas(existente.hora, hora) == 0) {
            // si la cita no está cancelada, se considera ocupada
            if (compararCadenas(existente.estado, "Cancelada") != 0) {
                cout << "Error: El doctor ya tiene una cita en esa fecha y hora.\n";
                return nullptr;
            }
        }
    }

    // Redimensionar array de citas del hospital si está lleno
    if (hospital->cantidadCitas >= hospital->capacidadCitas) {
        redimensionar(hospital->citas, hospital->capacidadCitas);
    }

    // Crear la cita en el array del hospital
    Cita& c = hospital->citas[hospital->cantidadCitas];
    c.id = hospital->siguienteIdCita++;
    c.idPaciente = idPaciente;
    c.idDoctor = idDoctor;
    strncpy(c.fecha, fecha, sizeof(c.fecha)-1); c.fecha[sizeof(c.fecha)-1] = '\0';
    strncpy(c.hora, hora, sizeof(c.hora)-1); c.hora[sizeof(c.hora)-1] = '\0';
    strncpy(c.motivo, motivo ? motivo : "", sizeof(c.motivo)-1); c.motivo[sizeof(c.motivo)-1] = '\0';
    strncpy(c.estado, "Agendada", sizeof(c.estado)-1); c.estado[sizeof(c.estado)-1] = '\0';
    c.atendida = false;

    hospital->cantidadCitas++;

    // Agregar ID de cita al array de citas agendadas del doctor
    if (doctor->cantidadCitas >= doctor->capacidadCitas) {
        redimensionar(doctor->citasAgendadas, doctor->capacidadCitas);
    }
    doctor->citasAgendadas[doctor->cantidadCitas++] = c.id;

    // Agregar la cita al array de citas del paciente (se guarda la estructura)
    if (paciente->cantidadCitas >= paciente->capacidadCitas) {
        redimensionar(paciente->citas, paciente->capacidadCitas);
    }
    paciente->citas[paciente->cantidadCitas++] = c;

    return &c;
};

bool cancelarCita(Hospital* hospital, int idCita){
    if (!hospital) return false;

    Cita* cita = nullptr;
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if (hospital->citas[i].id == idCita) {
            cita = &hospital->citas[i];
            break;
        }
    }
    if (!cita) return false; // cita no encontrada

    // Actualizar estado de la cita
    strncpy(cita->estado, "Cancelada", sizeof(cita->estado)-1);
    cita->estado[sizeof(cita->estado)-1] = '\0';
    cita->atendida = false;

    return true;
};

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos){
    if (!hospital) return false;

    //buscar cita por id
    Cita* cita = nullptr;
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if (hospital->citas[i].id == idCita) { cita = &hospital->citas[i]; break; }
    }
    if (!cita) return false; // cita no encontrada

    //Verificar que esté en estado "Agendada"
    if (compararCadenas(cita->estado, "Agendada") != 0) {
        cout << "La cita no está en estado 'Agendada'.\n";
        return false;
    }

    //Cambiar estado a Atendida y atendida = true
    strncpy(cita->estado, "Atendida", sizeof(cita->estado)-1);
    cita->estado[sizeof(cita->estado)-1] = '\0';
    cita->atendida = true;

    //Crear estructura HistorialMedico
    HistorialMedico h;
    h.idConsulta = hospital->siguienteIdConsulta++; // usar y aumentar contador del hospital
    strncpy(h.fecha, cita->fecha, sizeof(h.fecha)-1); h.fecha[sizeof(h.fecha)-1] = '\0';
    strncpy(h.hora, cita->hora, sizeof(h.hora)-1); h.hora[sizeof(h.hora)-1] = '\0';
    strncpy(h.diagnostico, diagnostico ? diagnostico : "", sizeof(h.diagnostico)-1); h.diagnostico[sizeof(h.diagnostico)-1] = '\0';
    strncpy(h.tratamiento, tratamiento ? tratamiento : "", sizeof(h.tratamiento)-1); h.tratamiento[sizeof(h.tratamiento)-1] = '\0';
    strncpy(h.medicamentos, medicamentos ? medicamentos : "", sizeof(h.medicamentos)-1); h.medicamentos[sizeof(h.medicamentos)-1] = '\0';
    h.idDoctor = cita->idDoctor;

    // Obtener costo desde el doctor (si se encuentra)
    Doctor* doc = buscarDoctorPorId(hospital, cita->idDoctor);
    h.costo = doc ? doc->costoConsulta : 0.0f;

    //Agregar al historial del paciente usando agregarConsultaAlHistorial()
    Paciente* paciente = nullptr;
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == cita->idPaciente) { paciente = &hospital->pacientes[i]; break; }
    }
    if (!paciente) {
        cout << "Paciente asociado a la cita no encontrado. Historial no actualizado.\n";
        return true; // la cita fue atendida pero no se pudo agregar al historial
    }

    agregarConsultaAlHistorial(paciente, h);

    //(contador del hospital ya se incrementó al asignar idConsulta)
    return true;
};

Cita** obtenerCitas(Hospital* hospital, const char* tipo, int id, int* cantidad) {
    if (!hospital || !cantidad || !tipo) return nullptr;

    *cantidad = 0;

    // Contar las coincidencias según el tipo
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if ((compararCadenas(tipo, "paciente") == 0 && hospital->citas[i].idPaciente == id) ||
            (compararCadenas(tipo, "doctor") == 0 && hospital->citas[i].idDoctor == id)) {
            (*cantidad)++;
        }
    }

    if (*cantidad == 0) return nullptr;

    // Crear array dinámico para los resultados
    Cita** resultados = new Cita*[*cantidad];
    int j = 0;

    // Llenar el array según el tipo
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if ((compararCadenas(tipo, "paciente") == 0 && hospital->citas[i].idPaciente == id) ||
            (compararCadenas(tipo, "doctor") == 0 && hospital->citas[i].idDoctor == id)) {
            resultados[j++] = &hospital->citas[i];
        }
    }

    return resultados;
}

Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad){
    if (!hospital || !fecha || !cantidad) return nullptr;
    *cantidad = 0;
    // contar
    for (int i = 0; i < hospital->cantidadCitas; ++i)
        if (compararCadenas(hospital->citas[i].fecha, fecha) == 0) (*cantidad)++;
    if (*cantidad == 0) return nullptr;
    Cita** resultados = new Cita*[*cantidad];
    int j = 0;
    for (int i = 0; i < hospital->cantidadCitas; ++i) {
        if (compararCadenas(hospital->citas[i].fecha, fecha) == 0)
            resultados[j++] = &hospital->citas[i];
    }
    return resultados;
};

void ListarPacientesPendientes(Hospital* hospital){
    bool encontrado = false;
    cout << "\n--- PACIENTES CON CITAS PENDIENTES ---\n";
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        Cita& c = hospital->citas[i];
        if (compararCadenas(c.estado, "Agendada") == 0) {
            Paciente* p = nullptr;
            for (int j = 0; j < hospital->cantidadPacientes; j++) {
                if (hospital->pacientes[j].id == c.idPaciente) {
                    p = &hospital->pacientes[j];
                    break;
                }
            }
            if (p) {
                cout << "ID Paciente: " << p->id << ", Nombre: " << p->nombre << " " << p->apellido
                    << ", Cédula: " << p->cedula << ", Fecha Cita: " << c.fecha << ", Hora Cita: " << c.hora << endl;
                encontrado = true;
            }
        }
    }
    if (!encontrado) {
        cout << "No hay pacientes con citas pendientes.\n";
    }

};

bool verificarDisponibilidadDoctor(Hospital* hospital, int idDoctor, const char* fecha, const char* hora){
    if (!hospital) return false;

    for (int i = 0; i < hospital->cantidadCitas; i++) {
        Cita& c = hospital->citas[i];
        if (c.idDoctor == idDoctor && compararCadenas(c.fecha, fecha) == 0 && compararCadenas(c.hora, hora) == 0) {
            // si la cita no está cancelada, el doctor no está disponible
            if (compararCadenas(c.estado, "Cancelada") != 0) {
                return false;
            }
        }
    }
    return true;
};

//---------------------- Menús ----------------------
void mostrarDatosHospital(const Hospital* h){
    cout << "\nDatos del hospital:" << endl;
    cout << "Nombre: " << h->nombre << endl;
    cout << "Direccion: " << h->direccion << endl;
    cout << "Telefono: " << h->telefono << endl;
}

void mostrarMenuPaciente(Hospital* hospital) {
    int opcion;
    do {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║         GESTION DE PACIENTES           ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Agregar Paciente" << endl;
        cout << "2. Buscar Paciente por Cedula" << endl;
        cout << "3. Buscar paciente por nombre" << endl;
        cout << "4. Actualizar paciente" << endl;
        cout << "5. eliminar paciente" << endl;
        cout << "6. listar pacientes" << endl;
        cout << "7. Ver historial medico completo" << endl;
        cout << "8. Agregar consulta nueva al historial" << endl;
        cout << "9. Obtener ultima consulta de un paciente" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                char nombre[50], apellido[50], cedula[20], tipoSangre[5], telefono[15], direccion[100], email[50];
                int edad;
                char sexo;

                cout << "\n--- REGISTRO DE NUEVO PACIENTE ---\n";
                cout << "Nombre: "; cin.getline(nombre, 50); while(strlen(nombre)==0){cout<<"El nombre no puede estar vacio. Ingrese nuevamente: "; cin.getline(nombre,50);}
                cout << "Apellido: "; cin.getline(apellido, 50); while(strlen(apellido)==0){cout<<"El apellido no puede estar vacio. Ingrese nuevamente: "; cin.getline(apellido,50);}
                // validar cédula
                do {
                    cout << "Cedula: "; cin.getline(cedula, 20);
                    if (strlen(cedula) == 0) { cout << "La cédula no puede estar vacia.\n"; continue; }
                    if (!validarCedula(cedula)) { cout << "Cédula inválida. Debe contener solo dígitos y guiones, longitud 7-20.\n"; continue; }
                    break;
                } while (true);

                cout << "Edad: "; cin >> edad; cin.ignore(); while(edad <= 0){cout<<"La edad debe ser un numero positivo. Ingrese nuevamente: "; cin>>edad; cin.ignore();}
                cout << "Sexo (M/F): "; cin >> sexo; cin.ignore(); while(sexo != 'M' && sexo != 'F' && sexo != 'm' && sexo != 'f'){cout<<"Sexo invalido. Ingrese 'M' para masculino o 'F' para femenino: "; cin>>sexo; cin.ignore();}
                cout << "Tipo de sangre: "; cin.getline(tipoSangre, 5);
                cout << "Telefono: "; cin.getline(telefono, 15); while(strlen(telefono)==0){cout<<"El telefono no puede estar vacio. Ingrese nuevamente: "; cin.getline(telefono,15);}
                cout << "Direccion: "; cin.getline(direccion, 100); while(strlen(direccion)==0){cout<<"La direccion no puede estar vacia. Ingrese nuevamente: "; cin.getline(direccion,100);}

                // validar email
                do {
                    cout << "Email: "; cin.getline(email, 50);
                    if (strlen(email) == 0) { cout << "El email no puede estar vacio. Ingrese nuevamente.\n"; continue; }
                    if (!validarEmail(email)) { cout << "Email inválido. Debe tener formato usuario@dominio.ext\n"; continue; }
                    break;
                } while (true);

                Paciente* p = crearPaciente(hospital, nombre, apellido, cedula, edad, sexo, tipoSangre, telefono, direccion, email);
                if (p != nullptr)
                    cout << "\nPaciente creado exitosamente. ID asignado: " << p->id << endl;
                else{
                    cout << "\nNo se pudo crear el paciente (cedula duplicada o error interno)." << endl;
                }
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }
            case 2: {
                char cedulaBuscar[20];
                cout << "Ingrese la cédula del paciente: ";
                cin.getline(cedulaBuscar, 20);
                Paciente* p = buscarPacientePorCedula(hospital, cedulaBuscar);
                if (p != nullptr) {
                    cout << "\nPaciente encontrado:\n";
                    cout << "ID: " << p->id << "\n";
                    cout << "Nombre: " << p->nombre << " " << p->apellido << "\n";
                    cout << "Cedula: " << p->cedula << "\n";
                    cout << "Edad: " << p->edad << "\n";
                    cout << "Sexo: " << p->sexo << "\n";
                    cout << "Tipo de sangre: " << p->tipoSangre << "\n";
                    cout << "Telefono: " << p->telefono << "\n";
                    cout << "Direccion: " << p->direccion << "\n";
                    cout << "Email: " << p->email << "\n";
                } else {
                    cout << "\nPaciente no encontrado.\n";
                }
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
                break;
            }
            case 3:{
                buscarPacientePorNombre(hospital);
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }
            case 4:{
                char cedula[20];
                cout << "Ingrese la cédula del paciente a actualizar: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // LIMPIAR BUFFER
                cin.getline(cedula, 20);
                Paciente* p = buscarPacientePorCedula(hospital, cedula);
                if (p != nullptr) {
                    actualizarPaciente(p);
                } else {
                cout << "Paciente no encontrado.\n";
            }
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }
            case 5:{
                char cedula[20];
                cout << "Ingrese la cédula del paciente a eliminar: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.getline(cedula, 20);

                Paciente* p = buscarPacientePorCedula(hospital, cedula);
                if (p != nullptr) {
                    if (eliminarPaciente(hospital, p->id)) {
                    cout << "Paciente eliminado correctamente.\n";
                } else {
                    cout << "Error al eliminar paciente.\n";
                }
                } else {
                    cout << "Paciente no encontrado.\n";
                }
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
                break;
            }
            case 6:{
                listarPacientes(hospital);
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
                break;
            }
            case 7:{
                char cedula[20];
                cout << "Ingrese la cédula del paciente para ver historial: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.getline(cedula, 20);

                Paciente* p = buscarPacientePorCedula(hospital, cedula);
            if (p != nullptr) {
                int cantidadConsultas = 0;
                HistorialMedico* historial = obtenerHistorialCompleto(p, &cantidadConsultas);

                if (historial && cantidadConsultas > 0) {
                    cout << "\n--- HISTORIAL MÉDICO DEL PACIENTE ---\n";
                    for (int i = 0; i < cantidadConsultas; i++) {
                        cout << "Consulta #" << historial[i].idConsulta << endl;
                        cout << "Fecha: " << historial[i].fecha << " " << historial[i].hora << endl;
                        cout << "Diagnóstico: " << historial[i].diagnostico << endl;
                        cout << "Tratamiento: " << historial[i].tratamiento << endl;
                        cout << "Medicamentos: " << historial[i].medicamentos << endl;
                        cout << "ID Doctor: " << historial[i].idDoctor << endl;
                        cout << "Costo: $" << fixed << setprecision(2) << historial[i].costo << endl;
                        cout << "--------------------------------------\n";
                    }
                } else {
                    cout << "El paciente no tiene consultas registradas.\n";
                }
            } else {
            cout << "Paciente no encontrado.\n";
        }
        cout << "Presione ENTER para continuar...";
        cin.get();
        system("cls");
        break;
            }
        case 8:{
            char cedula[20];
            cout << "Ingrese la cédula del paciente para agregar consulta: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.getline(cedula, 20);

            Paciente* p = buscarPacientePorCedula(hospital, cedula);
            if (p != nullptr) {
                HistorialMedico nuevaConsulta;
                cout << "\n--- NUEVA CONSULTA ---\n";
                cout << "ID Consulta: "; cin >> nuevaConsulta.idConsulta; cin.ignore();
                cout << "Fecha (YYYY-MM-DD): "; cin.getline(nuevaConsulta.fecha, 11);
                cout << "Hora (HH:MM): "; cin.getline(nuevaConsulta.hora, 6);
                cout << "Diagnóstico: "; cin.getline(nuevaConsulta.diagnostico, 200);
                cout << "Tratamiento: "; cin.getline(nuevaConsulta.tratamiento, 200);
                cout << "Medicamentos: "; cin.getline(nuevaConsulta.medicamentos, 150);
                cout << "ID Doctor: "; cin >> nuevaConsulta.idDoctor; cin.ignore();
                cout << "Costo: "; cin >> nuevaConsulta.costo; cin.ignore();

                agregarConsultaAlHistorial(p, nuevaConsulta);
                cout << "Consulta agregada correctamente.\n";
            } else {
                cout << "Paciente no encontrado.\n";
            }

            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        break;
        }
        case 9:{
            char cedula[20];
            cout << "Ingrese la cedula del paciente para obtener ultima consulta: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.getline(cedula, 20);

            Paciente* p = buscarPacientePorCedula(hospital, cedula);
            if (p != nullptr) {
                HistorialMedico* ultimaConsulta = obtenerUltimaConsulta(p);
                if (ultimaConsulta != nullptr) {
                    cout << "\n--- ÚLTIMA CONSULTA DEL PACIENTE ---\n";
                    cout << "ID Consulta: " << ultimaConsulta->idConsulta << endl;
                    cout << "Fecha: " << ultimaConsulta->fecha << " " << ultimaConsulta->hora << endl;
                    cout << "Diagnóstico: " << ultimaConsulta->diagnostico << endl;
                    cout << "Tratamiento: " << ultimaConsulta->tratamiento << endl;
                    cout << "Medicamentos: " << ultimaConsulta->medicamentos << endl;
                    cout << "ID Doctor: " << ultimaConsulta->idDoctor << endl;
                    cout << "Costo: $" << fixed << setprecision(2) << ultimaConsulta->costo << endl;
                } else {
                    cout << "El paciente no tiene consultas registradas.\n";
                }
            } else {
                cout << "Paciente no encontrado.\n";
            }

            cout << "Presione ENTER para continuar...";
            cin.get();
            system("cls");
        }
            case 0:
                return;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 0);
}

void mostrarMenuDoctor(Hospital* hospital){
    const char* especialidades[] = {
        "Medicina General",
        "Pediatria",
        "Cardiologia",
        "Dermatologia",
        "Neurologia"
    };
    const int numEspecialidades = sizeof(especialidades) / sizeof(especialidades[0]);

    int opcion;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║         GESTION DE DOCTORES            ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Agregar Doctor" << endl;
        cout << "2. Buscar Doctor por id" << endl;
        cout << "3. Buscar Doctores por especialidad" << endl;
        cout << "4. Asignar paciente a doctor" << endl;
        cout << "5. Remover paciente a doctor" << endl;
        cout << "6. Listar pacientes de un doctor" << endl;
        cout << "7. Listar doctores" << endl;
        cout << "8. Eliminar doctor" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        switch (opcion){

            case 1:{//agregar doctor
                char nombre[50], apellido[50], cedula[20];
                int aniosExperiencia;
                float costoConsulta;
                int especialidadElegida;

                cout << "\n--- REGISTRO DE NUEVO DOCTOR ---\n";
                cout << "Nombre: "; cin.getline(nombre, 50); 
                cout << "Apellido: "; cin.getline(apellido, 50); 

                // validar cédula
                do {
                    cout << "Cedula: "; cin.getline(cedula, 20);
                    if (strlen(cedula) == 0) { cout << "La cédula no puede estar vacia.\n"; continue; }
                    if (!validarCedula(cedula)) { cout << "Cédula inválida. Debe contener solo dígitos y guiones, longitud 7-20.\n"; continue; }
                    break;
                } while (true);

                // Mostrar especialidades
                cout << "Seleccione la especialidad:\n";
                for(int i = 0; i < numEspecialidades; i++){
                    cout << i+1 << ". " << especialidades[i] << endl;
                }
                cout << "Opcion: "; cin >> especialidadElegida; cin.ignore();
                while(especialidadElegida < 1 || especialidadElegida > numEspecialidades){
                    cout << "Opcion invalida. Ingrese nuevamente: "; cin >> especialidadElegida; cin.ignore();
                }

                // validar años experiencia
                cout << "Años de experiencia: "; cin >> aniosExperiencia; cin.ignore();
                while (aniosExperiencia < 0) { cout << "Años de experiencia inválidos. Ingrese un valor >= 0: "; cin >> aniosExperiencia; cin.ignore(); }

                // validar costo consulta
                cout << "Costo de consulta: "; cin >> costoConsulta; cin.ignore();
                while (costoConsulta < 0.0f) { cout << "Costo inválido. Ingrese un valor >= 0: "; cin >> costoConsulta; cin.ignore(); }

                Doctor* d = crearDoctor(hospital, nombre, apellido, cedula, especialidades[especialidadElegida-1], aniosExperiencia, costoConsulta);
                if (d != nullptr) {
                    cout << "\nDoctor creado exitosamente. ID asignado: " << d->id << endl;
                } else {
                    cout << "\nNo se pudo crear el doctor (cedula duplicada o error interno)." << endl;
                }
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }

            case 2:{//buscar doctor por id
                int idBuscar;
                cout << "Ingrese el ID del doctor: ";
                cin >> idBuscar;
                Doctor* d = buscarDoctorPorId(hospital, idBuscar);
                if (d != nullptr) {
                    cout << "\nDoctor encontrado:\n";
                    cout << "ID: " << d->id << "\n";
                    cout << "Nombre: " << d->nombre << " " << d->apellido << "\n";
                    cout << "Cedula: " << d->cedula << "\n";
                    cout << "Especialidad: " << d->especialidad << "\n";
                    cout << "Años de experiencia: " << d->aniosExperiencia << "\n";
                    cout << "Costo de consulta: $" << fixed << setprecision(2) << d->costoConsulta << "\n";
                } else {
                    cout << "\nDoctor no encontrado.\n";
                }
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            }

            case 3:{//buscar doctores por especialidad
                cout << "\n--- BUSCAR DOCTORES POR ESPECIALIDAD ---\n";

                // Mostrar las especialidades
                for(int i = 0; i < numEspecialidades; i++){
                    cout << i+1 << ". " << especialidades[i] << endl;
                }

                int opcionEsp;
                cout << "Seleccione una especialidad: ";
                cin >> opcionEsp;
                cin.ignore();

                // Validar opción
                while (opcionEsp < 1 || opcionEsp > numEspecialidades) {
                    cout << "Opcion invalida. Ingrese nuevamente: ";
                    cin >> opcionEsp;
                    cin.ignore();
                }

                const char* especialidadSeleccionada = especialidades[opcionEsp - 1];
                int cantidad = 0;
                Doctor** doctores = buscarDoctoresPorEspecialidad(hospital, especialidadSeleccionada, &cantidad);

                if (doctores != nullptr && cantidad > 0) {
                    cout << "\n--- DOCTORES EN " << especialidadSeleccionada << " ---\n";
                    for (int i = 0; i < cantidad; i++) {
                        cout << "ID: " << doctores[i]->id
                            << " | " << doctores[i]->nombre << " " << doctores[i]->apellido
                            << " | Experiencia: " << doctores[i]->aniosExperiencia << " años"
                            << " | Costo: $" << fixed << setprecision(2)
                            << doctores[i]->costoConsulta << "\n";
                    }
                    delete[] doctores; // Liberamos el array dinámico
                } else {
                    cout << "\n No se encontraron doctores con esa especialidad.\n";
                }

                cout << "\nPresione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
            break;
            }

            case 4:{//asignar paciente a doctor
            int idDoctor, idPaciente;
                cout << "Ingrese el ID del doctor: ";
                cin >> idDoctor;
                cout << "Ingrese el ID del paciente: ";
                cin >> idPaciente;

                Doctor* d = buscarDoctorPorId(hospital, idDoctor);
                if (d == nullptr) {
                    cout << "Doctor no encontrado.\n";
                } else {
                    if (asignarPacienteADoctor(d, idPaciente))
                        cout << "Asignación completada.\n";
                    else
                        cout << "No se pudo asignar el paciente.\n";
                }

                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            }

            case 5:{//quitar paciente de doctor
                int idDoctor, idPaciente;
                cout << "Ingrese el ID del doctor: ";
                cin >> idDoctor;
                cout << "Ingrese el ID del paciente a remover: ";
                cin >> idPaciente;

                Doctor* d = buscarDoctorPorId(hospital, idDoctor);
                if (d == nullptr) {
                    cout << "Doctor no encontrado.\n";
                } else {
                    if (removerPacienteDeDoctor(d, idPaciente))
                        cout << "Remoción completada.\n";
                    else
                        cout << "No se pudo remover el paciente.\n";
                }

                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            }

            case 6:{//listar pacientes de un doctor
                int idDoctor;
                cout << "Ingrese el ID del doctor: ";
                cin >> idDoctor;

                listarPacientesDeDoctor(hospital, idDoctor);

                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            }

            case 7:{//listar doctores
                listarDoctores(hospital);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
                break;
            }

            case 8:{//eliminar doctor
                int idDoctor;
                cout << "Ingrese el ID del doctor a eliminar: ";
                cin >> idDoctor;

                if (eliminarDoctor(hospital, idDoctor)) {
                    cout << "Doctor eliminado correctamente.\n";
                } else {
                    cout << "Error al eliminar doctor (ID no encontrado).\n";
                }

                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
                break;

            }
            case 0:
                return;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while(opcion != 0);
};

void mostrarMenuDeCitas(Hospital* hospital){
    int opcion;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║         GESTION DE CITAS               ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Agendar Cita" << endl;
        cout << "2. Cancelar Cita" << endl;
        cout << "3. Atender cita" << endl;
        cout << "4. Obtener citas de un paciente" << endl;
        cout << "5. Obtener citas de un doctor" << endl;
        cout << "6. Obtener citas por fecha" << endl;
        cout << "7. Listar todas las citas pendientes" << endl;
        cout << "8. Verificar disponibilidad" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion){
            case 1:{
                int idPaciente, idDoctor;
                char fecha[11], hora[6], motivo[100];

                cout << "\n--- AGENDAR NUEVA CITA ---\n";
                cout << "ID Paciente: "; cin >> idPaciente; cin.ignore();
                cout << "ID Doctor: "; cin >> idDoctor; cin.ignore();

                // validar fecha
                do {
                    cout << "Fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                    if (!validarFecha(fecha)) { cout << "Fecha inválida. Use formato YYYY-MM-DD y un día válido.\n"; continue; }
                    break;
                } while (true);

                // validar hora
                do {
                    cout << "Hora (HH:MM): "; cin.getline(hora, 6);
                    if (!validarHora(hora)) { cout << "Hora inválida. Use formato HH:MM (00:00 - 23:59).\n"; continue; }
                    break;
                } while (true);

                cout << "Motivo: "; cin.getline(motivo, 100);

                Cita* c = agendarCita(hospital, idPaciente, idDoctor, fecha, hora, motivo);
                if (c != nullptr) {
                    cout << "\nCita agendada exitosamente. ID asignado: " << c->id << endl;
                } else {
                    cout << "\nNo se pudo agendar la cita (error en datos o disponibilidad)." << endl;
                }
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
            break;
            }
            case 2:{
                int idCita;
                cout << "Ingrese el ID de la cita a cancelar: ";
                cin >> idCita;

                if (cancelarCita(hospital, idCita)) {
                    cout << "Cita cancelada correctamente.\n";
                } else {
                    cout << "Error al cancelar la cita (ID no encontrado).\n";
                }

                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
            break;
            }
            case 3:{
                int idCita;
                char diagnostico[200], tratamiento[200], medicamentos[150];

                cout << "Ingrese el ID de la cita a atender: ";
                cin >> idCita; cin.ignore();
                cout << "Diagnóstico: "; cin.getline(diagnostico, 200);
                cout << "Tratamiento: "; cin.getline(tratamiento, 200);
                cout << "Medicamentos: "; cin.getline(medicamentos, 150);

                if (atenderCita(hospital, idCita, diagnostico, tratamiento, medicamentos)) {
                    cout << "Cita atendida correctamente.\n";
                } else {
                    cout << "Error al atender la cita (ID no encontrado o estado inválido).\n";
                }

                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }

            case 4:{
                int idPaciente;
                cout << "Ingrese el ID del paciente para obtener sus citas: ";
                cin >> idPaciente;
                int cantidadCitas = 0;
                Cita** citas = obtenerCitas(hospital, "paciente", idPaciente, &cantidadCitas);
                if (citas != nullptr && cantidadCitas > 0) {
                    cout << "\n--- CITAS DEL PACIENTE ---\n";
                    for (int i = 0; i < cantidadCitas; i++) {
                        cout << "ID Cita: " << citas[i]->id
                            << " | Fecha: " << citas[i]->fecha
                            << " | Hora: " << citas[i]->hora
                            << " | Estado: " << citas[i]->estado << "\n";
                    }
                    delete[] citas; 
                } else {
                    cout << "\n No se encontraron citas para ese paciente.\n";
                }
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
            break;
            }

            case 5:{
                int idDoctor;
                cout << "Ingrese el Id del doctor" << endl;
                cin >> idDoctor;
                int cantidadCitas = 0;
                Cita** citas = obtenerCitas(hospital, "doctor", idDoctor, &cantidadCitas);
                if(citas != nullptr && cantidadCitas > 0){
                    cout << "\n--- CITAS DEL DOCTOR ---\n";
                    for(int i = 0; i < cantidadCitas; i++){
                        cout << "ID Cita: " << citas[i]->id
                            << " | Fecha: " << citas[i]->fecha
                            << " | Hora: " << citas[i]->hora
                            << " | Estado: " << citas[i]->estado << "\n";
                    }
                    delete[] citas;
                } else {
                    cout << "\n No se encontraron citas para ese doctor.\n";
                };
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                cin.get();
                system("cls");
            break;
            }

            case 6:{
                char fecha[11];
                // validar fecha antes de usar
                do {
                    cout << "Ingrese la fecha (YYYY-MM-DD) para obtener las citas: ";
                    cin.getline(fecha, 11);
                    if (!validarFecha(fecha)) { cout << "Fecha inválida. Use formato YYYY-MM-DD y un día válido.\n"; continue; }
                    break;
                } while (true);

                int cantidadCitas = 0;
                Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidadCitas);
                if(citas != nullptr && cantidadCitas > 0){
                    cout << "\n--- CITAS EN LA FECHA " << fecha << " ---\n";
                    for(int i = 0; i < cantidadCitas; i++){
                        cout << "ID Cita: " << citas[i]->id
                            << " | Paciente ID: " << citas[i]->idPaciente
                            << " | Doctor ID: " << citas[i]->idDoctor
                            << " | Hora: " << citas[i]->hora
                            << " | Estado: " << citas[i]->estado << "\n";
                    }
                    delete[] citas;
                } else {
                    cout << "\n No se encontraron citas para esa fecha.\n";
                };
                cout << "Presione ENTER para continuar...";
                cin.get();
                cin.ignore();
                system("cls");
            break;
            }
            case 7:{
                ListarPacientesPendientes(hospital);
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }
            case 8:{
                int idDoctor;
                char fecha[11], hora[6];

                cout << "Ingrese el ID del doctor: ";
                cin >> idDoctor; cin.ignore();

                // validar fecha y hora
                do {
                    cout << "Ingrese la fecha (YYYY-MM-DD): ";
                    cin.getline(fecha, 11);
                    if (!validarFecha(fecha)) { cout << "Fecha inválida. Use formato YYYY-MM-DD.\n"; continue; }
                    break;
                } while (true);

                do {
                    cout << "Ingrese la hora (HH:MM): ";
                    cin.getline(hora, 6);
                    if (!validarHora(hora)) { cout << "Hora inválida. Use formato HH:MM.\n"; continue; }
                    break;
                } while (true);

                if (verificarDisponibilidadDoctor(hospital, idDoctor, fecha, hora)) {
                    cout << "El doctor está disponible en esa fecha y hora.\n";
                } else {
                    cout << "El doctor NO está disponible en esa fecha y hora.\n";
                }

                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
            break;
            }
            case 0:
                return;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while(opcion != 0);
};

void mostrarMenuPrincipal(Hospital* hospital){
    int opcion;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║   SISTEMA DE GESTIÓN HOSPITALARIA      ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Gestion de Pacientes" << endl;
        cout << "2. Gestion de Doctores" << endl;
        cout << "3. Gestion de Citas" << endl;
        cout << "4. Mostrar datos del hospital" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion){
            case 1:
                mostrarMenuPaciente(hospital);
                system("cls");
                break;
            case 2:
                mostrarMenuDoctor(hospital);
                system("cls");
                break;
            case 3:
                mostrarMenuDeCitas(hospital);
                system("cls");
                break;
            case 4:
                mostrarDatosHospital(hospital);
                cout << "Presione ENTER para continuar...";
                cin.get();
                system("cls");
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida. Por favor, intente de nuevo.\n";
        }
    } while(opcion != 0);
}



//---------------------- Main ----------------------
int main(){
    Hospital* hospital = crearHospital("Hospital Central", "Av. Siempre Viva 123", "555-1234");
    mostrarMenuPrincipal(hospital);

    destruirHospital(hospital);
    return 0;
}