#include "operacionesDoctores.hpp"
#include "doctores.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// ==========================================
// LISTA DE ESPECIALIDADES (Configuración)
// ==========================================
const int CANT_ESPECIALIDADES = 5;
const char* LISTA_ESPECIALIDADES[CANT_ESPECIALIDADES] = {
    "Medicina General",
    "Pediatria",
    "Ginecologia",
    "Cardiologia",
    "Dermatologia"
};

// MENÚ PRINCIPAL DE DOCTORES
void menuDoctores(Hospital& hospital) {
    int opcion;
    do {
        Validaciones::limpiarPantalla();
        cout << "\n==========================================" << endl;
        cout << "       MENU DE GESTION DE DOCTORES        " << endl;
        cout << "==========================================" << endl;
        cout << "1. Registrar Nuevo Doctor" << endl;
        cout << "2. Modificar Doctor" << endl;
        cout << "3. Eliminar Doctor" << endl;
        cout << "4. Buscar Doctor por ID" << endl;
        cout << "5. Listar Todos los Doctores" << endl;
        cout << "0. Volver al menu principal" << endl;

        opcion = Validaciones::leerEntero("Seleccione una opcion: ", 0, 5);

        switch(opcion) {
            case 1: registrarDoctor(hospital); break;
            case 2: modificarDoctor(); break;
            case 3: eliminarDoctor(); break;
            case 4: buscarDoctorPorID(); break;
            case 5: listarDoctores(); break;
            case 0: break;
        }
    } while(opcion != 0);
}

// 1. REGISTRAR DOCTOR 
void registrarDoctor(Hospital& hospital) {
Validaciones::limpiarPantalla();
    cout << "\n=== REGISTRAR NUEVO DOCTOR ===" << endl;

    int id = hospital.getSiguienteIDDoctor();
    cout << ">> ID Asignado: " << id << endl;

    char buffer[100];

    // --- Datos Personales ---
    Validaciones::leerSoloLetras("Nombre: ", buffer, 50);
    string nombre = buffer;

    Validaciones::leerSoloLetras("Apellido: ", buffer, 50);
    string apellido = buffer;

    cout << "Cedula Profesional: ";
    cin.getline(buffer, 20);
    string cedula = buffer;

    // --- Selección de Especialidad ---
    cout << "\n--- Seleccione Especialidad ---" << endl;
    for (int i = 0; i < CANT_ESPECIALIDADES; i++) {
        cout << (i + 1) << ". " << LISTA_ESPECIALIDADES[i] << endl;
    }
    
    int opEsp = Validaciones::leerEntero("Opcion: ", 1, CANT_ESPECIALIDADES);
    string especialidad = LISTA_ESPECIALIDADES[opEsp - 1];

    Doctor d(id, nombre.c_str(), apellido.c_str(), cedula.c_str(), especialidad.c_str());

    // --- Datos Profesionales ---
    d.setAniosExperiencia(Validaciones::leerEntero("Anios de Experiencia: ", 0, 60));
    d.setCostoConsulta(Validaciones::leerFloat("Costo de Consulta ($): ", 0, 10000));

    d.setHorarioAtencion("Por Cita");

    // --- Contacto ---
    Validaciones::leerCadenaNumerica("Telefono: ", buffer, 15);
    d.setTelefono(buffer);

    Validaciones::leerEmail("Email: ", buffer, 50);
    d.setEmail(buffer);

    // Guardar
    if (GestionArchivos::guardarRegistro(d, Rutas::DOCTORES)) {
        cout << "\n>> Doctor registrado exitosamente!" << endl;
        hospital.actualizarContadoresDoctor(id + 1, hospital.getSiguienteIDDoctor());
        GestionArchivos::guardarHospitalConfig(hospital, Rutas::HOSPITAL);
    } else {
        cout << "\n>> Error critico al guardar el doctor." << endl;
    }
    Validaciones::pausar();
}

// 2. MODIFICAR DOCTOR 
void modificarDoctor() {
    Validaciones::limpiarPantalla();
    cout << "\n=== MODIFICAR DOCTOR ===" << endl;

    int id = Validaciones::leerEntero("Ingrese ID del doctor: ", 1, 99999);
    Doctor d = GestionArchivos::buscarPorID<Doctor>(id, Rutas::DOCTORES);

    if (d.getId() == 0 || d.estaEliminado()) {
        cout << ">> Doctor no encontrado." << endl;
        Validaciones::pausar();
        return;
    }

    d.mostrarInformacion();

    cout << "\n--- DATOS MODIFICABLES ---" << endl;
    cout << "1. Apellido" << endl;
    cout << "2. Especialidad" << endl; // Ahora usa el menú optimizado
    cout << "3. Costo Consulta" << endl;
    cout << "4. Horario" << endl;
    cout << "5. Telefono" << endl;
    cout << "6. Email" << endl;
    cout << "7. Disponibilidad" << endl;
    cout << "0. Cancelar" << endl;

    int opcion = Validaciones::leerEntero("Opcion: ", 0, 7);
    if (opcion == 0) return;

    char buffer[100];

    switch(opcion) {
        case 1:
            Validaciones::leerSoloLetras("Nuevo Apellido: ", buffer, 50);
            d.setApellido(buffer);
            break;
        case 2: {
            cout << "\n--- Nueva Especialidad ---" << endl;
            for (int i = 0; i < CANT_ESPECIALIDADES; i++) {
                cout << (i + 1) << ". " << LISTA_ESPECIALIDADES[i] << endl;
            }
            cout << (CANT_ESPECIALIDADES + 1) << ". Otra" << endl;
            
            int opEsp = Validaciones::leerEntero("Opcion: ", 1, CANT_ESPECIALIDADES + 1);
            if (opEsp <= CANT_ESPECIALIDADES) {
                d.setEspecialidad(LISTA_ESPECIALIDADES[opEsp - 1]);
            } else {
                Validaciones::leerSoloLetras("Especifique: ", buffer, 50);
                d.setEspecialidad(buffer);
            }
            break;
        }
        case 3:
            d.setCostoConsulta(Validaciones::leerFloat("Nuevo Costo: ", 0, 10000));
            break;
        case 4:
            Validaciones::leerHorarioTexto("Nuevo Horario (ej: 8am - 5pm): ", buffer, 50);
            d.setHorarioAtencion(buffer);;
            break;
        case 5:
            Validaciones::leerCadenaNumerica("Nuevo Telefono: ", buffer, 15);
            d.setTelefono(buffer);
            break;
        case 6:
            Validaciones::leerEmail("Nuevo Email: ", buffer, 50);
            d.setEmail(buffer);
            break;
        case 7:
            cout << "Esta disponible? (1=Si, 0=No): ";
            int disp; cin >> disp;
            d.setDisponible(disp == 1);
            break;
    }

    if (GestionArchivos::actualizarRegistro(d, Rutas::DOCTORES)) {
        cout << "\n>> Modificacion exitosa." << endl;
    } else {
        cout << "\n>> Error al actualizar." << endl;
    }
    Validaciones::pausar();
}

// 3. ELIMINAR DOCTOR
void eliminarDoctor() {
    Validaciones::limpiarPantalla();
    cout << "\n=== ELIMINAR DOCTOR ===" << endl;
    int id = Validaciones::leerEntero("Ingrese ID a eliminar: ", 1, 99999);

    Doctor d = GestionArchivos::buscarPorID<Doctor>(id, Rutas::DOCTORES);
    if (d.getId() == 0 || d.estaEliminado()) {
        cout << ">> Doctor no encontrado." << endl;
        Validaciones::pausar();
        return;
    }

    cout << "Va a eliminar a: Dr. " << d.getApellido() << endl;
    cout << "Esta seguro? (s/n): ";
    char conf; cin >> conf;
    cin.ignore();

    if (conf == 's' || conf == 'S') {
        if (GestionArchivos::eliminarRegistro<Doctor>(id, Rutas::DOCTORES)) {
            cout << ">> Doctor eliminado correctamente." << endl;
        }
    }
    Validaciones::pausar();
}

// 4. BUSCAR DOCTOR
void buscarDoctorPorID() {
    Validaciones::limpiarPantalla();
    cout << "\n=== BUSCAR DOCTOR ===" << endl;
    int id = Validaciones::leerEntero("Ingrese ID: ", 1, 99999);
    
    Doctor d = GestionArchivos::buscarPorID<Doctor>(id, Rutas::DOCTORES);
    
    if (d.getId() != 0 && !d.estaEliminado()) {
        d.mostrarInformacion();
    } else {
        cout << ">> No se encontro el doctor." << endl;
    }
    Validaciones::pausar();
}

// 5. LISTAR DOCTORES
void listarDoctores() {
    Validaciones::limpiarPantalla();
    cout << "\n=== LISTA DE DOCTORES ===" << endl;

    vector<Doctor> lista = GestionArchivos::listarTodos<Doctor>(Rutas::DOCTORES);

    if (lista.empty()) {
        cout << ">> No hay doctores registrados." << endl;
    } else {
        cout << left << setw(5) << "ID" 
            << setw(20) << "Apellido" 
            << setw(20) << "Especialidad" 
            << setw(10) << "Costo" << endl;
        cout << string(60, '-') << endl;

        for (const auto& d : lista) {
            cout << left << setw(5) << d.getId()
                << setw(20) << d.getApellido()
                << setw(20) << d.getEspecialidad()
                << "$" << fixed << setprecision(2) << d.getCostoConsulta() << endl;
        }
        cout << string(60, '-') << endl;
        cout << "Total: " << lista.size() << endl;
    }
    Validaciones::pausar();
}