#include "operacionesCitas.hpp"
#include "cita.hpp" 
#include "../pacientes/pacientes.hpp" 
#include "../doctores/doctores.hpp"   
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;


// MENÚ CITAS
void menuCitas(Hospital& hospital) {
    int opcion;
    do {
        Validaciones::limpiarPantalla();
        cout << "\n==========================================" << endl;
        cout << "         MENU DE GESTION DE CITAS         " << endl;
        cout << "==========================================" << endl;
        cout << "1. Agendar Cita" << endl;
        cout << "2. Atender Cita (Marcar como realizada)" << endl;
        cout << "3. Cancelar Cita" << endl;
        cout << "4. Buscar Cita por ID" << endl;
        cout << "5. Listar Todas las Citas" << endl;
        cout << "6. Listar Citas Pendientes" << endl;
        cout << "0. Volver al menu principal" << endl;

        opcion = Validaciones::leerEntero("Seleccione una opcion: ", 0, 6);

        switch(opcion) {
            case 1: agendarCita(hospital); break;
            case 2: atenderCita(); break;
            case 3: cancelarCita(); break;
            case 4: buscarCitaPorID(); break;
            case 5: listarCitas(); break;
            case 6: listarCitasPendientes(); break;
            case 0: break;
        }
    } while(opcion != 0);
}

// 1. AGENDAR CITA
void agendarCita(Hospital& hospital) {
    Validaciones::limpiarPantalla();
    cout << "\n=== AGENDAR NUEVA CITA ===" << endl;

    // --- 1. Validar Paciente ---
    int idPaciente = Validaciones::leerEntero("Ingrese ID del Paciente: ", 1, 99999);
    Paciente p = GestionArchivos::buscarPorID<Paciente>(idPaciente, Rutas::PACIENTES);
    
    if (p.getId() == 0 || p.estaEliminado()) {
        cout << ">> Error: Paciente no encontrado o inactivo." << endl;
        Validaciones::pausar();
        return;
    }
    cout << ">> Paciente seleccionado: " << p.getNombre() << " " << p.getApellido() << endl;

    // --- 2. Validar Doctor ---
    int idDoctor = Validaciones::leerEntero("Ingrese ID del Doctor: ", 1, 99999);
    Doctor d = GestionArchivos::buscarPorID<Doctor>(idDoctor, Rutas::DOCTORES);
    
    if (d.getId() == 0 || d.estaEliminado()) {
        cout << ">> Error: Doctor no encontrado o inactivo." << endl;
        Validaciones::pausar();
        return;
    }
    cout << ">> Doctor seleccionado: " << d.getNombre() << " " << d.getApellido() << " (" << d.getEspecialidad() << ")" << endl;

    // --- 3. Datos de la Cita ---
    int idCita = hospital.getSiguienteIDCita();
    char fecha[20];
    char hora[20];
    char motivo[150];

    cout << "\n--- Detalles ---" << endl;
    Validaciones::leerFecha("Fecha (DD/MM/AAAA): ", fecha);
    Validaciones::leerHoraMilitar("Hora (HH:MM): ", hora);
    
    cout << "Motivo de la consulta: ";
    cin.getline(motivo, 150);

    // --- 4. Crear y Guardar ---
    Cita nuevaCita(idCita, idPaciente, idDoctor, fecha, hora, motivo);

    if (GestionArchivos::guardarRegistro(nuevaCita, Rutas::CITAS)) {
        cout << "\n>> Cita agendada con exito! ID: " << idCita << endl;
        
        // Actualizar contadores
        hospital.actualizarContadoresCita(idCita + 1, hospital.getSiguienteIDCita());
        GestionArchivos::guardarHospitalConfig(hospital, Rutas::HOSPITAL);
        
    } else {
        cout << "\n>> Error al guardar la cita." << endl;
    }
    Validaciones::pausar();
}

// 2. ATENDER CITA
void atenderCita() {
    Validaciones::limpiarPantalla();
    cout << "\n=== ATENDER CITA ===" << endl;

    int id = Validaciones::leerEntero("Ingrese ID de la Cita: ", 1, 99999);
    Cita c = GestionArchivos::buscarPorID<Cita>(id, Rutas::CITAS);

    if (c.getId() == 0 || c.estaEliminado()) {
        cout << ">> Cita no encontrada." << endl;
        Validaciones::pausar();
        return;
    }

    if (c.estaAtendida()) {
        cout << ">> Esta cita ya fue atendida anteriormente." << endl;
        Validaciones::pausar();
        return;
    }

    c.mostrarInformacion();

    cout << "\nConfirmar atencion? (s/n): ";
    char conf; cin >> conf; cin.ignore();

    if (conf == 's' || conf == 'S') {
        char observaciones[200];
        cout << "Ingrese Observaciones/Diagnostico preliminar: ";
        cin.getline(observaciones, 200);
        
        c.setObservaciones(observaciones);
        c.setAtendida(true); // Esto cambia el estado internamente a "Atendida"

        if (GestionArchivos::actualizarRegistro(c, Rutas::CITAS)) {
            cout << ">> Cita marcada como ATENDIDA." << endl;
            // Aquí en el futuro conectaremos con el módulo Historial
        } else {
            cout << ">> Error al actualizar." << endl;
        }
    }
    Validaciones::pausar();
}

// 3. CANCELAR CITA
void cancelarCita() {
    Validaciones::limpiarPantalla();
    cout << "\n=== CANCELAR CITA ===" << endl;
    int id = Validaciones::leerEntero("Ingrese ID a cancelar: ", 1, 99999);

    if (GestionArchivos::eliminarRegistro<Cita>(id, Rutas::CITAS)) {
        cout << ">> Cita cancelada correctamente." << endl;
    } else {
        cout << ">> No se pudo cancelar (ID no existe o ya cancelada)." << endl;
    }
    Validaciones::pausar();
}

// 4. BUSCAR
void buscarCitaPorID() {
    Validaciones::limpiarPantalla();
    cout << "\n=== BUSCAR CITA ===" << endl;
    int id = Validaciones::leerEntero("Ingrese ID: ", 1, 99999);
    
    Cita c = GestionArchivos::buscarPorID<Cita>(id, Rutas::CITAS);
    
    if (c.getId() != 0 && !c.estaEliminado()) {
        c.mostrarInformacion();
    } else {
        cout << ">> Cita no encontrada." << endl;
    }
    Validaciones::pausar();
}

// 5. LISTAR TODAS
void listarCitas() {
    Validaciones::limpiarPantalla();
    cout << "\n=== LISTADO DE CITAS ===" << endl;

    vector<Cita> lista = GestionArchivos::listarTodos<Cita>(Rutas::CITAS);

    if (lista.empty()) {
        cout << ">> No hay citas registradas." << endl;
    } else {
        cout << left << setw(5) << "ID" 
            << setw(10) << "Pac. ID" 
            << setw(10) << "Doc. ID" 
            << setw(12) << "Fecha" 
            << setw(12) << "Estado" << endl;
        cout << string(60, '-') << endl;

        for (const auto& c : lista) {
            cout << left << setw(5) << c.getId()
                << setw(10) << c.getPacienteID()
                << setw(10) << c.getDoctorID()
                << setw(12) << c.getFecha()
                << setw(12) << c.getEstado() << endl;
        }
    }
    Validaciones::pausar();
}

// 6. LISTAR PENDIENTES
void listarCitasPendientes() {
    Validaciones::limpiarPantalla();
    cout << "\n=== CITAS PENDIENTES ===" << endl;

    vector<Cita> lista = GestionArchivos::listarTodos<Cita>(Rutas::CITAS);
    bool hayPendientes = false;

    cout << left << setw(5) << "ID" 
            << setw(12) << "Fecha" 
            << setw(8) << "Hora" 
            << setw(20) << "Motivo" << endl;
    cout << string(50, '-') << endl;

    for (const auto& c : lista) {
        // Verificamos si NO está atendida
        if (!c.estaAtendida()) {
            cout << left << setw(5) << c.getId()
                << setw(12) << c.getFecha()
                << setw(8) << c.getHora()
                << setw(20) << c.getMotivo() << endl;
            hayPendientes = true;
        }
    }

    if (!hayPendientes) cout << ">> No hay citas pendientes." << endl;
    
    Validaciones::pausar();
}