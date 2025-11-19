#include "operacionesHistorial.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"

#include "../pacientes/pacientes.hpp"
#include "../doctores/doctores.hpp"
#include "../citas/cita.hpp"

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// MENÚ DE HISTORIALES
void menuHistorial(Hospital& hospital) {
    int opcion;
    do {
        Validaciones::limpiarPantalla();
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║      MENU DE GESTION DE HISTORIALES    ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Doctores y pacientes registrados" << endl;
        cout << "2. Alergias de un paciente" << endl;
        cout << "3. Citas vistas por un paciente" << endl;
        cout << "4. Citas atendidas de un doctor" << endl;
        cout << "0. Regresar al menu principal" << endl;

        opcion = Validaciones::leerEntero("Seleccione una opcion: ", 0, 4);

        switch(opcion) {
            case 1: mostrarEstadisticasRegistros(); break;
            case 2: mostrarAlergiasPaciente(); break;
            case 3: listarCitasPorPaciente(); break;
            case 4: listarCitasAtendidasPorDoctor(); break;
            case 0: break;
        }
    } while(opcion != 0);
}

// 1. ESTADÍSTICAS
void mostrarEstadisticasRegistros() {
    Validaciones::limpiarPantalla();
    cout << "\n=== REGISTROS ACTIVOS EN SISTEMA ===" << endl;

    // Leemos los headers de los archivos para saber cuántos activos hay realmente
    ArchivoHeader hPacientes = GestionArchivos::leerHeader(Rutas::PACIENTES);
    ArchivoHeader hDoctores = GestionArchivos::leerHeader(Rutas::DOCTORES);

    cout << "\n----------------------------------------" << endl;
    cout << " Pacientes Activos: " << hPacientes.getRegistrosActivos() << endl;
    cout << " Doctores Activos:  " << hDoctores.getRegistrosActivos() << endl;
    cout << "----------------------------------------" << endl;

    Validaciones::pausar();
}

// ALERGIAS DE UN PACIENTE
void mostrarAlergiasPaciente() {
    Validaciones::limpiarPantalla();
    cout << "\n=== CONSULTAR ALERGIAS ===" << endl;

    int id = Validaciones::leerEntero("Ingrese ID del Paciente: ", 1, 99999);
    Paciente p = GestionArchivos::buscarPorID<Paciente>(id, Rutas::PACIENTES);

    if (p.getId() == 0 || p.estaEliminado()) {
        cout << ">> Paciente no encontrado." << endl;
    } else {
        cout << "\nPaciente: " << p.getNombre() << " " << p.getApellido() << endl;
        cout << "----------------------------------------" << endl;
        cout << "ALERGIAS REGISTRADAS:" << endl;
        // Verificamos si el campo está vacío o tiene datos
        if (strlen(p.getAlergias()) > 0) {
            cout << ">> " << p.getAlergias() << endl;
        } else {
            cout << ">> No registra alergias." << endl;
        }
    }
    Validaciones::pausar();
}

//  CITAS DE UN PACIENTE
void listarCitasPorPaciente() {
    Validaciones::limpiarPantalla();
    cout << "\n=== HISTORIAL DE CITAS POR PACIENTE ===" << endl;

    // 1. Validar Paciente
    int id = Validaciones::leerEntero("Ingrese ID del Paciente: ", 1, 99999);
    Paciente p = GestionArchivos::buscarPorID<Paciente>(id, Rutas::PACIENTES);

    if (p.getId() == 0 || p.estaEliminado()) {
        cout << ">> Paciente no encontrado." << endl;
        Validaciones::pausar();
        return;
    }

    cout << "\nHistorial de: " << p.getNombre() << " " << p.getApellido() << endl;

    // 2. Traer todas las citas y filtrar
    vector<Cita> lista = GestionArchivos::listarTodos<Cita>(Rutas::CITAS);
    bool encontro = false;

    cout << left << setw(5) << "ID" 
            << setw(12) << "Fecha" 
             << setw(20) << "Doctor ID" // Podríamos buscar el nombre del doc, pero por simplicidad ID
            << setw(12) << "Estado" << endl;
    cout << string(60, '-') << endl;

    for (const auto& c : lista) {
        if (c.getPacienteID() == id) {
            cout << left << setw(5) << c.getId()
                << setw(12) << c.getFecha()
                << setw(20) << c.getDoctorID()
                << setw(12) << c.getEstado() << endl;
            encontro = true;
        }
    }

    if (!encontro) cout << ">> No se encontraron citas para este paciente." << endl;

    Validaciones::pausar();
}

// CITAS ATENDIDAS POR DOCTOR
void listarCitasAtendidasPorDoctor() {
    Validaciones::limpiarPantalla();
    cout << "\n=== CITAS ATENDIDAS POR DOCTOR ===" << endl;

    // 1. Validar Doctor
    int id = Validaciones::leerEntero("Ingrese ID del Doctor: ", 1, 99999);
    Doctor d = GestionArchivos::buscarPorID<Doctor>(id, Rutas::DOCTORES);

    if (d.getId() == 0 || d.estaEliminado()) {
        cout << ">> Doctor no encontrado." << endl;
        Validaciones::pausar();
        return;
    }

    cout << "\nReporte del Dr. " << d.getApellido() << " (" << d.getEspecialidad() << ")" << endl;

    // 2. Traer todas y filtrar
    vector<Cita> lista = GestionArchivos::listarTodos<Cita>(Rutas::CITAS);
    bool encontro = false;

    cout << left << setw(5) << "ID" 
            << setw(12) << "Fecha" 
            << setw(25) << "Observaciones" << endl;
    cout << string(50, '-') << endl;

    for (const auto& c : lista) {
        if (c.getDoctorID() == id && c.estaAtendida()) {
            cout << left << setw(5) << c.getId()
                << setw(12) << c.getFecha()
                << setw(25) << c.getObservaciones() << endl;
            encontro = true;
        }
    }

    if (!encontro) cout << ">> No se encontraron citas atendidas por este doctor." << endl;

    Validaciones::pausar();
}