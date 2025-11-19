#include <iostream>

// --- INCLUDES DE ARQUITECTURA ---
#include "../hospital/hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"

// --- INCLUDES DE OPERACIONES (Módulos) ---
#include "../pacientes/operacionesPacientes.hpp" 
#include "../doctores/operacionesDoctores.hpp" 
#include "../citas/operacionesCitas.hpp"
#include "../historialMedico/operacionesHistorial.hpp"
#include "../mantenimiento/operacionesMantenimiento.hpp"

using namespace std;

Hospital hospital; 

void inicializarSistema() {
    // Intentamos cargar la configuración existente
    if (GestionArchivos::cargarHospitalConfig(hospital, Rutas::HOSPITAL)) {
        // Sistema cargado correctamente
    } else {
        cout << ">> Archivo de configuracion no encontrado. Creando nuevo..." << endl;
        
        // Crear hospital por defecto si no existe
        hospital = Hospital("Hospital Central", "Calle Principal 123", "555-0001");
        
        // Crear el archivo físico
        if (!GestionArchivos::guardarHospitalConfig(hospital, Rutas::HOSPITAL)) {
            cerr << "Error critico: No se pudo crear el archivo del hospital." << endl;
            exit(1);
        }
    }
}


void mostrarMenuPrincipal() {
    int opcion = 0;
    do {
        Validaciones::limpiarPantalla();
        cout << "\n==========================================" << endl;
        cout << "    SISTEMA DE GESTION HOSPITALARIA      " << endl;
        cout << "==========================================" << endl;
        cout << "1. Manejo de Pacientes" << endl;
        cout << "2. Manejo de Doctores" << endl;
        cout << "3. Manejo de Citas" << endl;
        cout << "4. Mostrar datos hospital" << endl;
        cout << "5. Historiales" << endl;
        cout << "6. Mantenimiento" << endl;
        cout << "0. Salir" << endl;
        
        opcion = Validaciones::leerEntero("Seleccione una opcion: ", 0, 6);
        
        switch (opcion) {
            case 1:
                menuPacientes(hospital); 
                break;
            case 2:
                menuDoctores(hospital);
                break;
            case 3:
                menuCitas(hospital);
                break;
            case 4:
                Validaciones::limpiarPantalla();
                hospital.mostrarInformacion();
                cout << "\nPresione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            case 5:
                menuHistorial(hospital);
                break;
            case 6:
                menuMantenimiento();
                break;
            case 0:
                GestionArchivos::guardarHospitalConfig(hospital, Rutas::HOSPITAL);
                cout << "Sistema cerrado correctamente." << endl;
                break;
        }
        
    } while (opcion != 0);
}

// ==========================================
// MAIN
// ==========================================
int main() {
    cout << " Iniciando Sistema de Gestion Hospitalaria..." << endl;
    
    // 1. Cargar o crear datos iniciales
    inicializarSistema();
    
    // 2. Lanzar menú
    mostrarMenuPrincipal();
    
    return 0;
}