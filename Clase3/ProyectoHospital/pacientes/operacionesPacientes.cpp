#include "operacionesPacientes.hpp"
#include "pacientes.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../persistencia/constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

//menu pacientes
void menuPacientes(Hospital& hospital) {
    int opcion;
    do {
        Validaciones::limpiarPantalla();
        cout << "\n==========================================" << endl;
        cout << "       MENU DE GESTION DE PACIENTES       " << endl;
        cout << "==========================================" << endl;
        cout << "1. Agregar Paciente" << endl;
        cout << "2. Modificar Paciente" << endl;
        cout << "3. Eliminar Paciente" << endl;
        cout << "4. Buscar Paciente por id" << endl;
        cout << "5. Listar Pacientes" << endl;
        cout << "0. Regresar al menu principal" << endl;
        
        opcion = Validaciones::leerEntero("Seleccione una opcion: ", 0, 5);

        switch(opcion) {
            case 1: registrarPaciente(hospital); break;
            case 2: modificarPaciente(); break;
            case 3: eliminarPaciente(hospital); break;
            case 4: buscarPaciente(); break;
            case 5: listarPacientes(); break;
            case 0: break;
        }
    } while(opcion != 0);
}

// 1. REGISTRAR PACIENTE
void registrarPaciente(Hospital& hospital) {
    Validaciones::limpiarPantalla();
    cout << "\n=== REGISTRO DE NUEVO PACIENTE ===" << endl;

    // 1. ID Automático
    int id = hospital.getSiguienteIDPaciente();
    cout << ">> ID Asignado: " << id << endl;

    // 2. Recolección de Datos con Validaciones Estrictas
    char buffer[500]; // Buffer reutilizable
    
    // --- NOMBRE Y APELLIDO (Solo letras) ---
    Validaciones::leerSoloLetras(" Ingrese Nombre: ", buffer, 50);
    string nombre = buffer;
    
    Validaciones::leerSoloLetras(" Ingrese Apellido: ", buffer, 50);
    string apellido = buffer;
    
    // --- CÉDULA (Solo números o formato específico) ---
    // Usamos leerCadenaNumerica si quieres solo números, o un lector genérico si permites guiones
    Validaciones::leerCadenaNumerica(" Ingrese Cedula (solo numeros): ", buffer, 20);
    string cedula = buffer;

    // --- INSTANCIA ---
    Paciente p(id, nombre.c_str(), apellido.c_str(), cedula.c_str());

    // --- EDAD (Entero rango lógico) ---
    p.setEdad(Validaciones::leerEntero(" Ingrese Edad: ", 0, 120));
    
    // --- SEXO (Carácter específico) ---
    char sexo;
    do {
        cout << " Ingrese Sexo (M/F): ";
        cin >> sexo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiar
        sexo = toupper(sexo);
        if (sexo != 'M' && sexo != 'F') cout << ">> Error: Ingrese M o F.\n";
    } while (sexo != 'M' && sexo != 'F');
    p.setSexo(sexo);

    // --- TIPO SANGRE (Validación médica) ---
    Validaciones::leerTipoSangre(" Ingrese Tipo de Sangre (ej: O+): ", buffer);
    p.setTipoSangre(buffer);

    // --- TELÉFONO (Números) ---
    Validaciones::leerCadenaNumerica(" Ingrese Telefono: ", buffer, 15);
    p.setTelefono(buffer);

    // --- DIRECCIÓN (Texto libre, permitimos números y letras) ---
    // Aquí usamos un getline simple encapsulado o una función leerTextoGenérico
    cout << " Ingrese Direccion: ";
    cin.getline(buffer, 100);
    p.setDireccion(buffer);

    // --- EMAIL (Formato correo) ---
    Validaciones::leerEmail(" Ingrese Email: ", buffer, 50);
    p.setEmail(buffer);

    // --- ALERGIAS Y OBSERVACIONES (Texto libre) ---
    cout << " Ingrese Alergias (Enter para ninguna): ";
    cin.getline(buffer, 500);
    if (strlen(buffer) == 0) strcpy(buffer, "Ninguna");
    p.setAlergias(buffer);
    
    cout << " Observaciones (Enter para ninguna): ";
    cin.getline(buffer, 500);
    if (strlen(buffer) == 0) strcpy(buffer, "Ninguna");
    p.setObservaciones(buffer);

    // 3. Guardado
    if (GestionArchivos::guardarRegistro(p, Rutas::PACIENTES)) {
        cout << "\n>> Paciente registrado exitosamente!" << endl;
        hospital.actualizarContadoresPaciente(id + 1, hospital.getSiguienteIDPaciente()); 
        GestionArchivos::guardarHospitalConfig(hospital, Rutas::HOSPITAL);
    } else {
        cout << "\n>> Error al guardar el paciente." << endl;
    }
    Validaciones::pausar();
}

// 2. MODIFICAR PACIENTE
void modificarPaciente() {
Validaciones::limpiarPantalla();
    cout << "\n=== MODIFICAR PACIENTE ===" << endl;
    
    int id = Validaciones::leerEntero("Ingrese el ID del paciente a modificar: ", 1, 99999);
    
    // Buscar primero
    Paciente p = GestionArchivos::buscarPorID<Paciente>(id, Rutas::PACIENTES);
    
    if (p.getId() == 0 || p.estaEliminado()) {
        cout << ">> Paciente no encontrado o eliminado." << endl;
        Validaciones::pausar();
        return;
    }

    // Mostrar datos actuales
    p.mostrarInformacion();
    
    cout << "\n--- DATOS MODIFICABLES ---" << endl;
    cout << "1. Apellido" << endl;
    cout << "2. Cedula" << endl;
    cout << "3. Edad" << endl;
    cout << "4. Telefono" << endl;
    cout << "5. Direccion" << endl;
    cout << "6. Email" << endl;
    cout << "7. Alergias" << endl;
    cout << "8. Observaciones" << endl;
    cout << "0. Cancelar" << endl;
    
    int opcion = Validaciones::leerEntero("Que dato desea modificar?: ", 0, 8);
    
    if (opcion == 0) return;

    char buffer[500]; // Buffer grande reutilizable

    switch(opcion) {
        case 1:
            // Validación estricta: Solo letras
            Validaciones::leerSoloLetras("Nuevo Apellido: ", buffer, 50);
            p.setApellido(buffer);
            break;
        case 2:
            // Validación estricta: Solo números
            Validaciones::leerCadenaNumerica("Nueva Cedula: ", buffer, 20);
            p.setCedula(buffer);
            break;
        case 3:
            // Validación numérica de rango
            p.setEdad(Validaciones::leerEntero("Nueva Edad: ", 0, 120));
            break;
        case 4:
            // Validación estricta: Solo números
            Validaciones::leerCadenaNumerica("Nuevo Telefono: ", buffer, 15);
            p.setTelefono(buffer);
            break;
        case 5:
            // Dirección permite números y letras (usamos leerTexto generico que usa getline)
            cout << "Nueva Direccion: ";
            cin.getline(buffer, 100);
            p.setDireccion(buffer);
            break;
        case 6:
            // Validación estricta: Formato Email
            Validaciones::leerEmail("Nuevo Email: ", buffer, 50);
            p.setEmail(buffer);
            break;
        case 7:
            // Alergias permite todo
            cout << "Nuevas Alergias: ";
            cin.getline(buffer, 500);
            p.setAlergias(buffer);
            break;
        case 8:
            // Observaciones permite todo
            cout << "Nuevas Observaciones: ";
            cin.getline(buffer, 500);
            p.setObservaciones(buffer);
            break;
    }

    // Guardar cambios (Sobrescribir)
    if (GestionArchivos::actualizarRegistro(p, Rutas::PACIENTES)) {
        cout << "\n>> Modificacion exitosa." << endl;
    } else {
        cout << "\n>> Error al actualizar el registro." << endl;
    }
    Validaciones::pausar();
}


// 3. ELIMINAR PACIENTE
void eliminarPaciente(Hospital& hospital) {
    Validaciones::limpiarPantalla();
    cout << "\n=== ELIMINAR PACIENTE ===" << endl;
    
    int id = Validaciones::leerEntero("Ingrese ID a eliminar: ", 1, 99999);
    
    // Confirmación visual
    Paciente p = GestionArchivos::buscarPorID<Paciente>(id, Rutas::PACIENTES);
    if (p.getId() == 0 || p.estaEliminado()) {
        cout << ">> Paciente no encontrado." << endl;
        Validaciones::pausar();
        return;
    }
    
    cout << "Va a eliminar a: " << p.getNombre() << " " << p.getApellido() << endl;
    cout << "Esta seguro? (s/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (confirm == 's' || confirm == 'S') {
        if (GestionArchivos::eliminarRegistro<Paciente>(id, Rutas::PACIENTES)) {
            cout << ">> Paciente eliminado correctamente." << endl;
            // Nota: Podríamos decrementar contadores del hospital aquí si quisiéramos
        } else {
            cout << ">> Error al eliminar." << endl;
        }
    } else {
        cout << ">> Operacion cancelada." << endl;
    }
    Validaciones::pausar();
}


// 4. BUSCAR PACIENTE
void buscarPaciente() {
    Validaciones::limpiarPantalla();
    cout << "\n=== BUSCAR PACIENTE ===" << endl;
    
    int id = Validaciones::leerEntero("Ingrese ID a buscar: ", 1, 99999);
    
    Paciente p = GestionArchivos::buscarPorID<Paciente>(id, Rutas::PACIENTES);
    
    if (p.getId() != 0 && !p.estaEliminado()) {
        p.mostrarInformacion();
    } else {
        cout << ">> No se encontro ningun paciente con ese ID." << endl;
    }
    Validaciones::pausar();
}


// 5. LISTAR PACIENTES
void listarPacientes() {
    Validaciones::limpiarPantalla();
    cout << "\n=== LISTA DE PACIENTES ===" << endl;
    
    vector<Paciente> lista = GestionArchivos::listarTodos<Paciente>(Rutas::PACIENTES);
    
    if (lista.empty()) {
        cout << "No hay pacientes registrados." << endl;
    } else {
        cout << left << setw(6) << "ID" 
            << setw(20) << "Nombre" 
            << setw(20) << "Apellido" 
            << setw(15) << "Telefono" << endl;
        cout << string(65, '-') << endl;
        
        for (const auto& p : lista) {
            cout << left << setw(6) << p.getId()
                << setw(20) << p.getNombre()
                << setw(20) << p.getApellido()
                << setw(15) << p.getTelefono() << endl;
        }
        cout << string(65, '-') << endl;
        cout << "Total: " << lista.size() << endl;
    }
    Validaciones::pausar();
}