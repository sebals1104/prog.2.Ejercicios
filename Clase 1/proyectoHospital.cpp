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

//---------------------- Funciones principales----------------------

int compararCadenas(const char* a, const char* b) {
    if (!a || !b) return -1; // si alguno es nulo
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

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

Paciente* crearPaciente(Hospital* hospital, const char* nombre,const char* apellido,const char* cedula, int edad, char sexo,const char* tipoSangre,const char* telefono,const char* direccion, const char* email){
    // Verificar cedula duplicada
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (compararCadenas(hospital->pacientes[i].cedula, cedula) == 0) {
            cout << "Error: La cédula ya existe en el sistema.\n";
            return nullptr;
        }
    }

    // Verificar capacidad
    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        int nuevaCapacidad = hospital->capacidadPacientes * 2;
        Paciente* nuevosPacientes = new Paciente[nuevaCapacidad];
        for (int i = 0; i < hospital->cantidadPacientes; i++)
            nuevosPacientes[i] = hospital->pacientes[i];
        delete[] hospital->pacientes;
        hospital->pacientes = nuevosPacientes;
        hospital->capacidadPacientes = nuevaCapacidad;
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

        if (strstr(nombrePaciente, nombreMinusculas) != nullptr) { // coincidencia parcial
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
        cout << "2. Buscar Paciente por Cédula" << endl;
        cout << "3. Buscar paciente por nombre" << endl;
        cout << "4. Actualizar paciente" << endl;
        cout << "5. eliminar paciente" << endl;
        cout << "6. listar pacientes" << endl;
        cout << "0. Volver al Menú Principal" << endl;
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
                cout << "Cedula: "; cin.getline(cedula, 20); while(strlen(cedula)==0){cout<<"La cédula no puede estar vacia. Ingrese nuevamente: "; cin.getline(cedula,20);}
                cout << "Edad: "; cin >> edad; cin.ignore(); while(edad <= 0){cout<<"La edad debe ser un numero positivo. Ingrese nuevamente: "; cin>>edad; cin.ignore();}
                cout << "Sexo (M/F): "; cin >> sexo; cin.ignore(); while(sexo != 'M' && sexo != 'F' && sexo != 'm' && sexo != 'f'){cout<<"Sexo invalido. Ingrese 'M' para masculino o 'F' para femenino: "; cin>>sexo; cin.ignore();}
                cout << "Tipo de sangre: "; cin.getline(tipoSangre, 5);
                cout << "Telefono: "; cin.getline(telefono, 15); while(strlen(telefono)==0){cout<<"El telefono no puede estar vacio. Ingrese nuevamente: "; cin.getline(telefono,15);}
                cout << "Direccion: "; cin.getline(direccion, 100); while(strlen(direccion)==0){cout<<"La direccion no puede estar vacia. Ingrese nuevamente: "; cin.getline(direccion,100);}
                cout << "Email: "; cin.getline(email, 50);while(strlen(email)==0){cout<<"El email no puede estar vacio. Ingrese nuevamente: "; cin.getline(email,50);}

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
            case 0:
                return;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 0);
}

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
        cout << "5. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion){
            case 1:
                mostrarMenuPaciente(hospital);
                system("cls");
                break;
            case 2:
                cout << "Menu de doctores no implementado aun.\n";
                break;
            case 3:
                cout << "Menu de citas no implementado aun.\n";
                break;
            case 4:
                mostrarDatosHospital(hospital);
                system("cls");
                break;
            case 5:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida. Por favor, intente de nuevo.\n";
        }
    } while(opcion != 5);
}



//---------------------- Main ----------------------
int main(){
    Hospital* hospital = crearHospital("Hospital Central", "Av. Siempre Viva 123", "555-1234");
    mostrarMenuPrincipal(hospital);

    // Liberar memoria de pacientes
    for(int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente& p = hospital->pacientes[i];
        delete[] p.citas;
        for(int j = 0; j < p.capacidadAlergias; j++) delete[] p.alergias[j];
        delete[] p.alergias;
        for(int j = 0; j < p.capacidadObservaciones; j++) delete[] p.observaciones[j];
        delete[] p.observaciones;
    }

    // Liberar memoria de doctores
    for(int i = 0; i < hospital->cantidadDoctores; i++) {
        Doctor& d = hospital->doctores[i];
        delete[] d.pacientesAsignados;
        delete[] d.citasAgendadas;
    }

    // Liberar arrays principales
    delete[] hospital->pacientes;
    delete[] hospital->doctores;
    delete[] hospital->citas;
    delete hospital;

    return 0;
}