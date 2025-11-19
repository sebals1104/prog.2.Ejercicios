#include <iostream>

// --- INCLUDES DE ARQUITECTURA ---
#include "../hospital/hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"

// --- INCLUDES DE OPERACIONES (Módulos) ---
#include "../pacientes/operacionesPacientes.hpp" 
// #include "doctores/operacionesDoctores.hpp" // Aún pendiente
// #include "citas/operacionesCitas.hpp"       // Aún pendiente
// #include "historial/operacionesHistorial.hpp" // Aún pendiente

using namespace std;

// ==========================================
// VARIABLE GLOBAL DEL SISTEMA
// ==========================================
Hospital hospital; 

// ==========================================
// LÓGICA DE INICIALIZACIÓN
// ==========================================
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

// ==========================================
// MENÚS
// ==========================================

void mostrarMenuMantenimiento() {
    Validaciones::limpiarPantalla();
    cout << "\n==========================================" << endl;
    cout << "       MENU DE MANTENIMIENTO DEL          " << endl;
    cout << "         SISTEMA HOSPITALARIO             " << endl;
    cout << "==========================================" << endl;
    cout << "1. Verificar integridad de archivos" << endl;
    cout << "2. Compactar archivos (eliminar registros borrados)" << endl;
    cout << "3. Hacer un respaldo de datos" << endl;
    cout << "4. Restaurar datos desde un respaldo" << endl;
    cout << "5. Estadisticas de uso de archivos" << endl;
    cout << "0. Regresar al menu principal" << endl;
    
    cout << "\n>> [Opcion en construccion]" << endl;
    Validaciones::pausar();
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
                // ✅ AQUI ESTA LA CONEXION: Llama al menu de pacientes
                menuPacientes(hospital); 
                break;
            case 2:
                cout << ">> Modulo Doctores en construccion..." << endl;
                Validaciones::pausar();
                break;
            case 3:
                cout << ">> Modulo Citas en construccion..." << endl;
                Validaciones::pausar();
                break;
            case 4:
                Validaciones::limpiarPantalla();
                hospital.mostrarInformacion();
                cout << "\nPresione ENTER para continuar...";
                cin.ignore();
                cin.get();
                break;
            case 5:
                cout << ">> Modulo Historiales en construccion..." << endl;
                Validaciones::pausar();
                break;
            case 6:
                mostrarMenuMantenimiento();
                break;
            case 0:
                // Guardado final de seguridad
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