#include "operacionesMantenimiento.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"

// Necesitamos los tipos para el Template de Compactar
#include "../pacientes/pacientes.hpp"
#include "../doctores/doctores.hpp"
#include "../citas/cita.hpp"
#include "../historialMedico/historialMedico.hpp"

#include <iostream>
#include <iomanip>
#include <direct.h> 

using namespace std;

void menuMantenimiento() {
    int opcion;
    do {
        Validaciones::limpiarPantalla();
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║      MENU DE MANTENIMIENTO DEL         ║" << endl;
        cout << "║        SISTEMA HOSPITALARIO            ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Verificar integridad de archivos" << endl;
        cout << "2. Compactar archivos (eliminar registros borrados)" << endl;
        cout << "3. Hacer un respaldo de datos" << endl;
        cout << "4. Restaurar datos desde un respaldo" << endl;
        cout << "5. Estadisticas de uso de archivos" << endl;
        cout << "0. Regresar al menu principal" << endl;

        opcion = Validaciones::leerEntero("Seleccione una opcion: ", 0, 5);

        switch(opcion) {
            case 1: verificarIntegridad(); break;
            case 2: compactarArchivos(); break;
            case 3: hacerRespaldo(); break;
            case 4: restaurarRespaldo(); break;
            case 5: mostrarEstadisticasUso(); break;
            case 0: break;
        }
    } while(opcion != 0);
}

// --- 1. Verificar Integridad ---
void verificarIntegridad() {
    Validaciones::limpiarPantalla();
    cout << "\n=== VERIFICACION DE INTEGRIDAD ===" << endl;
    
    const char* archivos[] = {Rutas::PACIENTES, Rutas::DOCTORES, Rutas::CITAS, Rutas::HISTORIAL, Rutas::HOSPITAL};
    string nombres[] = {"Pacientes", "Doctores", "Citas", "Historial", "Config Hospital"};

    cout << left << setw(20) << "ARCHIVO" << "ESTADO" << endl;
    cout << "----------------------------------------" << endl;

    for(int i=0; i<5; i++) {
        ifstream f(archivos[i]);
        cout << left << setw(20) << nombres[i];
        if(f.good()) {
            cout << "[OK] Encontrado" << endl;
        } else {
            cout << "[ERROR] No existe o dañado" << endl;
        }
        f.close();
    }
    Validaciones::pausar();
}

// --- 2. Compactar Archivos ---
void compactarArchivos() {
    Validaciones::limpiarPantalla();
    cout << "\n=== COMPACTACION DE ARCHIVOS ===" << endl;
    cout << "Esto eliminara fisicamente los registros marcados como borrados." << endl;
    cout << "Desea continuar? (s/n): ";
    char conf; cin >> conf; cin.ignore();

    if(conf == 's' || conf == 'S') {
        cout << "\n>> Compactando Pacientes... ";
        if(GestionArchivos::compactarArchivo<Paciente>(Rutas::PACIENTES)) cout << "[OK]" << endl; else cout << "[ERROR]" << endl;

        cout << ">> Compactando Doctores... ";
        if(GestionArchivos::compactarArchivo<Doctor>(Rutas::DOCTORES)) cout << "[OK]" << endl; else cout << "[ERROR]" << endl;

        cout << ">> Compactando Citas... ";
        if(GestionArchivos::compactarArchivo<Cita>(Rutas::CITAS)) cout << "[OK]" << endl; else cout << "[ERROR]" << endl;

        cout << ">> Compactando Historial... ";
        if(GestionArchivos::compactarArchivo<HistorialMedico>(Rutas::HISTORIAL)) cout << "[OK]" << endl; else cout << "[ERROR]" << endl;
        
        cout << "\nProceso finalizado." << endl;
    }
    Validaciones::pausar();
}

// --- 3. Hacer Respaldo ---
void hacerRespaldo() {
    Validaciones::limpiarPantalla();
    cout << "\n=== RESPALDO DE DATOS ===" << endl;
    
    _mkdir("respaldos"); 

    bool exito = true;
    // Copiamos cada archivo a la carpeta respaldos/
    if(!GestionArchivos::copiarArchivo(Rutas::PACIENTES, "respaldos/pacientes.bak")) exito = false;
    if(!GestionArchivos::copiarArchivo(Rutas::DOCTORES,  "respaldos/doctores.bak")) exito = false;
    if(!GestionArchivos::copiarArchivo(Rutas::CITAS,     "respaldos/citas.bak")) exito = false;
    if(!GestionArchivos::copiarArchivo(Rutas::HISTORIAL, "respaldos/historial.bak")) exito = false;
    if(!GestionArchivos::copiarArchivo(Rutas::HOSPITAL,  "respaldos/hospital.bak")) exito = false;

    if(exito) cout << "\n>> Respaldo creado exitosamente en carpeta 'respaldos'." << endl;
    else cout << "\n>> Hubo errores. Verifique si los archivos de origen existen." << endl;

    Validaciones::pausar();
}

// --- 4. Restaurar Respaldo ---
void restaurarRespaldo() {
    Validaciones::limpiarPantalla();
    cout << "\n=== RESTAURAR DATOS ===" << endl;
    cout << "ADVERTENCIA: Esto sobrescribira los datos actuales con el respaldo." << endl;
    cout << "Esta seguro? (s/n): ";
    char conf; cin >> conf; cin.ignore();

    if(conf == 's' || conf == 'S') {
        bool exito = true;
        // Copiamos DE respaldos/ HACIA binarios/
        if(!GestionArchivos::copiarArchivo("respaldos/pacientes.bak", Rutas::PACIENTES)) exito = false;
        if(!GestionArchivos::copiarArchivo("respaldos/doctores.bak",  Rutas::DOCTORES)) exito = false;
        if(!GestionArchivos::copiarArchivo("respaldos/citas.bak",     Rutas::CITAS)) exito = false;
        if(!GestionArchivos::copiarArchivo("respaldos/historial.bak", Rutas::HISTORIAL)) exito = false;
        if(!GestionArchivos::copiarArchivo("respaldos/hospital.bak",  Rutas::HOSPITAL)) exito = false;

        if(exito) cout << "\n>> Sistema restaurado exitosamente." << endl;
        else cout << "\n>> Error: No se encontro carpeta 'respaldos' o archivos .bak faltantes." << endl;
    }
    Validaciones::pausar();
}

// --- 5. Estadísticas ---
void mostrarEstadisticasUso() {
    Validaciones::limpiarPantalla();
    cout << "\n=== ESTADISTICAS DE USO ===" << endl;
    
    struct InfoArchivo { string nombre; const char* ruta; };
    InfoArchivo lista[] = {
        {"Pacientes", Rutas::PACIENTES},
        {"Doctores", Rutas::DOCTORES},
        {"Citas", Rutas::CITAS},
        {"Historial", Rutas::HISTORIAL}
    };

    cout << left << setw(15) << "MODULO" 
        << setw(15) << "REG. TOTALES" 
        << setw(15) << "REG. ACTIVOS" 
        << "PROXIMO ID" << endl;
    cout << string(60, '-') << endl;

    for(int i=0; i<4; i++) {
        ArchivoHeader h = GestionArchivos::leerHeader(lista[i].ruta);
        cout << left << setw(15) << lista[i].nombre
            << setw(15) << h.getCantidadRegistros()
            << setw(15) << h.getRegistrosActivos()
            << h.getProximoID() << endl;
    }
    Validaciones::pausar();
}