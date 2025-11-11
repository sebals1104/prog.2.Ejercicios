#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace std;

struct Hospital;
struct Doctor;
struct Cita;
struct Paciente;
struct HistorialMedico;

struct ArchivoHeader {
    int cantidadRegistros;      
    int proximoID;              
    int registrosActivos;       
    int version;                
};

struct HistorialMedico {
    int id;
    int pacienteID;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    int siguienteConsultaID;
    bool eliminado;
    time_t fechaRegistro;
};

struct Cita {
    int id;
    int pacienteID;
    int doctorID;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
    bool eliminado;
    int consultaID;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    bool eliminado;
    int cantidadPacientes;
    int pacientesIDs[50];
    int cantidadCitas;
    int citasIDs[30];
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Paciente {
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
    char alergias[500];
    char observaciones[500];
    bool activo;
    bool eliminado;
    int cantidadConsultas;
    int primerConsultaID;
    int cantidadCitas;
    int citasIDs[20];
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Hospital {
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    // Rutas de archivos
    char doctoresFile[260];
    char pacientesFile[260];
    char citasFile[260];
    char historialFile[260];
    
    // Contadores de ID
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // Estadísticas generales
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;
};

// ---------------FUNCIONES DE ARCHIVOS---------------
bool fileExists(const string& path);
bool inicializarArchivo(const string& nombreArchivo, int version = 1);
ArchivoHeader leerHeader(const string& nombreArchivo);
bool actualizarHeader(const string& nombreArchivo, const ArchivoHeader &header);
bool guardarHospital(const string& path, const Hospital &h);
bool cargarHospital(Hospital &hospital, const string& path);
bool crearHospital(Hospital &hospital, const string& nombre, const string& direccion, const string& telefono);
void mostrarDatosHospital(const Hospital &h);

// ---------------FUNCIONES DE PACIENTES---------------
long calcularPosicion(int indice);
bool guardarPaciente(const Paciente &p);
Paciente leerPacientePorIndice(int indice);
bool actualizarPaciente(Paciente pacienteModificado);
bool eliminarPacienteLogico(int id);
void listarPacientes();
bool agregarPaciente();
Paciente buscarPacientePorID(int id);
void mostrarDetallesPaciente(const Paciente &p);
void buscarYMostrarPaciente();
void modificarPaciente();

// ---------------VALIDACIONES---------------
void ValidarSoloLetras(string prompt, string &outStr, int minLen, int maxLen);
int ValidarNumeroEntero(string prompt, int minVal, int maxVal);
bool validarEmail(string email);
bool validarTipoSangre(string tipoSangre);

// ---------------MENUS---------------
void mostrarMenuPaciente();
void mostrarMenuPrincipal();

// ---------------VARIABLES GLOBALES (solo configuración)---------------
Hospital hospital;  

// ---------------MAIN---------------
int main() {
    cout << " Iniciando Sistema de Gestión Hospitalaria" << endl;
    
    // Cargar o crear hospital
    if (fileExists("hospital.bin")) {
        if (!cargarHospital(hospital, "hospital.bin")) {
            cout << "Error al cargar hospital. Creando nuevo..." << endl;
            crearHospital(hospital, "Hospital Central", "Calle Principal 123", "555-0001");
        }
    } else {
        crearHospital(hospital, "Hospital Central", "Calle Principal 123", "555-0001");
    }
    
    mostrarMenuPrincipal();
    
    // Guardar configuración al salir
    guardarHospital("hospital.bin", hospital);
    cout << "Sistema cerrado correctamente." << endl;
    
    return 0;
}

// ---------------IMPLEMENTACIÓN FUNCIONES ARCHIVOS---------------

bool fileExists(const string& path) {
    ifstream f(path, ios::binary);
    return f.good();
}

bool inicializarArchivo(const string& nombreArchivo, int version) {
    ofstream archivo(nombreArchivo, ios::binary | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    ArchivoHeader header = {0, 1, 0, version};
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool success = archivo.good();
    archivo.close();
    return success;
}

ArchivoHeader leerHeader(const string& nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 0};
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return header;
    }
    
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    
    return header;
}

bool actualizarHeader(const string& nombreArchivo, const ArchivoHeader &header) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool success = archivo.good();
    archivo.close();
    return success;
}

// ---------------FUNCIONES DE HOSPITAL---------------
bool guardarHospital(const string& path, const Hospital &h) {
    ofstream archivo(path, ios::binary | ios::out | ios::trunc);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.write(reinterpret_cast<const char*>(&h), sizeof(Hospital));
    bool success = archivo.good();
    archivo.close();
    return success;
}

bool cargarHospital(Hospital &hospital, const string& path) {
    ifstream archivo(path, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }
    
    archivo.read(reinterpret_cast<char*>(&hospital), sizeof(Hospital));
    bool success = archivo.good();
    archivo.close();
    return success;
}

bool crearHospital(Hospital &hospital, const string& nombre, const string& direccion, const string& telefono) {
    // Inicializar estructura
    memset(&hospital, 0, sizeof(Hospital));
    
    // Usar copy en lugar de strncpy para strings
    nombre.copy(hospital.nombre, sizeof(hospital.nombre)-1);
    hospital.nombre[nombre.length()] = '\0';
    
    direccion.copy(hospital.direccion, sizeof(hospital.direccion)-1);
    hospital.direccion[direccion.length()] = '\0';
    
    telefono.copy(hospital.telefono, sizeof(hospital.telefono)-1);
    hospital.telefono[telefono.length()] = '\0';
    
    // Inicializar nombres de archivos
    string doctoresFile = "doctores.bin";
    string pacientesFile = "pacientes.bin";
    string citasFile = "citas.bin";
    string historialFile = "historiales.bin";
    
    doctoresFile.copy(hospital.doctoresFile, sizeof(hospital.doctoresFile)-1);
    hospital.doctoresFile[doctoresFile.length()] = '\0';
    
    pacientesFile.copy(hospital.pacientesFile, sizeof(hospital.pacientesFile)-1);
    hospital.pacientesFile[pacientesFile.length()] = '\0';
    
    citasFile.copy(hospital.citasFile, sizeof(hospital.citasFile)-1);
    hospital.citasFile[citasFile.length()] = '\0';
    
    historialFile.copy(hospital.historialFile, sizeof(hospital.historialFile)-1);
    hospital.historialFile[historialFile.length()] = '\0';
    
    // Inicializar contadores
    hospital.siguienteIDPaciente = 1;
    hospital.siguienteIDDoctor = 1;
    hospital.siguienteIDCita = 1;
    hospital.siguienteIDConsulta = 1;
    
    // Crear archivos si no existen
    inicializarArchivo(hospital.pacientesFile, 1);
    inicializarArchivo(hospital.doctoresFile, 1);
    inicializarArchivo(hospital.citasFile, 1);
    inicializarArchivo(hospital.historialFile, 1);
    
    return guardarHospital("hospital.bin", hospital);
}

void mostrarDatosHospital(const Hospital &h) {
    cout << "\n=== DATOS DEL HOSPITAL ===" << endl;
    cout << "Nombre: " << h.nombre << endl;
    cout << "Dirección: " << h.direccion << endl;
    cout << "Teléfono: " << h.telefono << endl;
    cout << "==========================" << endl;
}

// ---------------FUNCIONES DE PACIENTES---------------

long calcularPosicion(int indice) {
    return sizeof(ArchivoHeader) + (indice * sizeof(Paciente));
}


Paciente leerPacientePorIndice(int indice) {
    Paciente p;
    memset(&p, 0, sizeof(Paciente));
    
    ifstream archivo(hospital.pacientesFile, ios::binary);
    if (!archivo.is_open()) {
        return p;
    }
    
    long posicion = calcularPosicion(indice);
    archivo.seekg(posicion);
    archivo.read(reinterpret_cast<char*>(&p), sizeof(Paciente));
    archivo.close();
    
    return p;
}

Paciente buscarPacientePorID(int id) {
    Paciente paciente;
    memset(&paciente, 0, sizeof(Paciente));
    
    // 1. Abrir archivo de pacientes
    ifstream archivo(hospital.pacientesFile, ios::binary);
    if (!archivo.is_open()) {
        cout << " Error: No se pudo abrir el archivo de pacientes." << endl;
        return paciente;
    }
    
    // 2. Leer header para saber cantidad de registros
    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    // 3. Buscar paciente por ID
    bool encontrado = false;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Paciente temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Paciente));
        
        if (temp.id == id && !temp.eliminado) {
            paciente = temp;
            encontrado = true;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << " No se encontró paciente con ID: " << id << endl;
    }
    
    return paciente;
}

void mostrarDetallesPaciente(const Paciente &p) {
    if (p.id == 0) {
        cout << "Paciente no encontrado o ID inválido." << endl;
        return;
    }
    
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║          DETALLES DEL PACIENTE         ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << " ID: " << p.id << endl;
    cout << " Nombre: " << p.nombre << " " << p.apellido << endl;
    cout << " Cédula: " << p.cedula << endl;
    cout << " Edad: " << p.edad << " años" << endl;
    cout << " Sexo: " << p.sexo << endl;
    cout << " Tipo de Sangre: " << p.tipoSangre << endl;
    cout << " Teléfono: " << p.telefono << endl;
    cout << " Dirección: " << p.direccion << endl;
    cout << " Email: " << p.email << endl;
    cout << " Alergias: " << p.alergias << endl;
    cout << " Observaciones: " << p.observaciones << endl;
    cout << " Consultas realizadas: " << p.cantidadConsultas << endl;
    cout << " Citas agendadas: " << p.cantidadCitas << endl;
    
    // Mostrar fecha de creación formateada
    char fechaStr[20];
    strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", localtime(&p.fechaCreacion));
    cout << "Fecha de registro: " << fechaStr << endl;
    
    cout << "╔════════════════════════════════════════╗" << endl;
    cout << "║              ESTADO                    ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << " Activo: " << (p.activo ? "Sí" : "No") << endl;
    cout << "  Eliminado: " << (p.eliminado ? "Sí" : "No") << endl;
}

void buscarYMostrarPaciente() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           BUSCAR PACIENTE              ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    int id = ValidarNumeroEntero("Ingrese el ID del paciente a buscar: ", 1, 10000);
    
    Paciente paciente = buscarPacientePorID(id);
    
    if (paciente.id != 0) {
        mostrarDetallesPaciente(paciente);
    } else {
        cout << " No se encontró ningún paciente con ID: " << id << endl;
    }
    
    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
}

bool actualizarPaciente(Paciente pacienteModificado) {
    // 1. Buscar posición del paciente por ID
    ifstream archivoBusqueda(hospital.pacientesFile, ios::binary);
    if (!archivoBusqueda.is_open()) {
        return false;
    }
    
    ArchivoHeader header;
    archivoBusqueda.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    int indice = -1;
    Paciente temp;
    
    // Buscar el índice del paciente
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion(i);
        archivoBusqueda.seekg(posicion);
        archivoBusqueda.read(reinterpret_cast<char*>(&temp), sizeof(Paciente));
        
        if (temp.id == pacienteModificado.id && !temp.eliminado) {
            indice = i;
            break;
        }
    }
    archivoBusqueda.close();
    
    if (indice == -1) {
        return false; // Paciente no encontrado
    }
    
    // 2. Abrir archivo en modo lectura/escritura
    fstream archivo(hospital.pacientesFile, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }
    
    // 3. Posicionarse en la ubicación exacta (seekp)
    long posicion = calcularPosicion(indice);
    archivo.seekp(posicion);
    
    // 4. Actualizar fechaModificacion
    pacienteModificado.fechaModificacion = time(NULL);
    
    // 5. Escribir estructura completa sobre el registro existente
    archivo.write(reinterpret_cast<const char*>(&pacienteModificado), sizeof(Paciente));
    
    // 6. Cerrar archivo
    archivo.close();
    
    // 7. Retornar true si exitoso
    return true;
}

bool eliminarPacienteLogico(int id) {
    // 1. Buscar índice del paciente
    ifstream archivoBusqueda(hospital.pacientesFile, ios::binary);
    if (!archivoBusqueda.is_open()) {
        cout << " Error: No se pudo abrir el archivo de pacientes." << endl;
        return false;
    }
    
    ArchivoHeader header;
    archivoBusqueda.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    int indice = -1;
    Paciente pacienteEncontrado;
    
    // Buscar el paciente y su índice
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion(i);
        archivoBusqueda.seekg(posicion);
        archivoBusqueda.read(reinterpret_cast<char*>(&pacienteEncontrado), sizeof(Paciente));
        
        if (pacienteEncontrado.id == id && !pacienteEncontrado.eliminado) {
            indice = i;
            break;
        }
    }
    archivoBusqueda.close();
    
    if (indice == -1) {
        cout << " No se encontró paciente con ID: " << id << endl;
        return false;
    }
    
    // Mostrar información del paciente
    cout << "\nPaciente a eliminar:" << endl;
    cout << "ID: " << pacienteEncontrado.id << " - " << pacienteEncontrado.nombre << " " << pacienteEncontrado.apellido << endl;
    
    // Confirmar eliminación
    char confirmar;
    cout << "¿Está seguro de que desea eliminar este paciente? (s/n): ";
    cin >> confirmar;
    cin.ignore();
    
    if (confirmar == 's' || confirmar == 'S') {
        // 2. Abrir archivo en modo lectura/escritura
        fstream archivo(hospital.pacientesFile, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            cout << " Error: No se pudo abrir el archivo para eliminar." << endl;
            return false;
        }
        
        // 3. Posicionarse en el registro
        long posicion = calcularPosicion(indice);
        archivo.seekp(posicion);
        
        // 4. Leer el paciente completo (ya lo tenemos en pacienteEncontrado)
        // 5. Marcar: paciente.eliminado = true
        pacienteEncontrado.eliminado = true;
        pacienteEncontrado.activo = false;
        pacienteEncontrado.fechaModificacion = time(NULL);
        
        // 6. Volver a posicionarse (seekp)
        archivo.seekp(posicion);
        
        // 7. Sobrescribir el registro modificado
        archivo.write(reinterpret_cast<const char*>(&pacienteEncontrado), sizeof(Paciente));
        
        bool success = archivo.good();
        
        // 8. Cerrar archivo
        archivo.close();
        
        if (success) {
            // 9. Actualizar header.registrosActivos--
            ArchivoHeader headerActualizado = leerHeader(hospital.pacientesFile);
            headerActualizado.registrosActivos--;
            hospital.totalPacientesRegistrados = headerActualizado.registrosActivos;
            
            if (actualizarHeader(hospital.pacientesFile, headerActualizado)) {
                cout << "  Paciente eliminado exitosamente (eliminación lógica)." << endl;
                cout << "  Nota: El registro se mantiene en el archivo marcado como eliminado." << endl;
                return true;
            }
        }
        
        cout << "  Error al eliminar paciente." << endl;
        return false;
    } else {
        cout << "Eliminación cancelada." << endl;
        return false;
    }
}

void listarPacientes() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           LISTA DE PACIENTES           ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    ArchivoHeader header = leerHeader(hospital.pacientesFile);
    
    if (header.registrosActivos == 0) {
        cout << "No hay pacientes registrados." << endl;
        return;
    }
    
    cout << left << setw(6) << "ID" 
        << setw(20) << "Nombre" 
        << setw(20) << "Apellido"
        << setw(10) << "Edad"
        << setw(15) << "Teléfono" << endl;
    cout << string(70, '-') << endl;
    
    int contador = 0;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Paciente p = leerPacientePorIndice(i);
        if (!p.eliminado) {
            cout << left << setw(6) << p.id
                << setw(20) << p.nombre
                << setw(20) << p.apellido
                << setw(10) << p.edad
                << setw(15) << p.telefono << endl;
            contador++;
        }
    }
    
    cout << string(70, '-') << endl;
    cout << "Total de pacientes: " << contador << endl;
}

bool agregarPaciente() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      REGISTRAR NUEVO PACIENTE          ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    Paciente p;
    string tempStr;
    memset(&p, 0, sizeof(Paciente));

    // Obtener ID del header
    ArchivoHeader header = leerHeader(hospital.pacientesFile);
    p.id = header.proximoID;
    p.fechaCreacion = time(NULL);
    p.fechaModificacion = p.fechaCreacion;
    
    cout << "ID asignado: " << p.id << endl;
    
    // Recolección de datos con validaciones SIN PUNTEROS
    ValidarSoloLetras("Ingrese Nombre: ", tempStr, 2, 49);
    strncpy(p.nombre, tempStr.c_str(), sizeof(p.nombre)-1);
    p.nombre[sizeof(p.nombre)-1] = '\0';
    
    ValidarSoloLetras("Ingrese Apellido: ", tempStr, 2, 49);
    strncpy(p.apellido, tempStr.c_str(), sizeof(p.apellido)-1);
    p.apellido[sizeof(p.apellido)-1] = '\0';

    // Cédula
    while (true) {
        cout << "Ingrese Cédula (máx 19): ";
        getline(cin, tempStr);
        if (tempStr.length() >= 6 && tempStr.length() < 20) {
            strncpy(p.cedula, tempStr.c_str(), sizeof(p.cedula)-1);
            p.cedula[sizeof(p.cedula)-1] = '\0';
            break;
        }
        cout << "La cédula debe tener entre 6 y 19 caracteres.\n";
    }

    // Edad
    p.edad = ValidarNumeroEntero("Ingrese Edad: ", 1, 120);
    
    // Sexo
    while (true) {
        cout << "Ingrese Sexo (M/F): ";
        getline(cin, tempStr);
        if (!tempStr.empty() && (tempStr[0] == 'M' || tempStr[0] == 'm' || tempStr[0] == 'F' || tempStr[0] == 'f')) {
            p.sexo = toupper(tempStr[0]);
            break;
        }
        cout << "Por favor ingrese M o F.\n";
    }

    // Tipo de Sangre
    while (true) {
        cout << "Ingrese Tipo de Sangre (ej: O+, A-, AB+): ";
        getline(cin, tempStr);
        if (validarTipoSangre(tempStr)) {
            // Normalizar y guardar
            string cleaned;
            for (char c : tempStr) {
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
                if (c >= 'a' && c <= 'z') c = toupper(c);
                cleaned += c;
            }
            strncpy(p.tipoSangre, cleaned.c_str(), sizeof(p.tipoSangre)-1);
            p.tipoSangre[sizeof(p.tipoSangre)-1] = '\0';
            break;
        }
        cout << "Tipo de sangre inválido. Use formatos: A+, A-, B+, B-, AB+, AB-, O+, O-\n";
    }

    // Teléfono
    while (true) {
        cout << "Ingrese Teléfono: ";
        getline(cin, tempStr);
        if (!tempStr.empty()) {
            strncpy(p.telefono, tempStr.c_str(), sizeof(p.telefono)-1);
            p.telefono[sizeof(p.telefono)-1] = '\0';
            break;
        }
        cout << "El teléfono es obligatorio.\n";
    }

    // Dirección
    while (true) {
        cout << "Ingrese Dirección: ";
        getline(cin, tempStr);
        if (!tempStr.empty()) {
            strncpy(p.direccion, tempStr.c_str(), sizeof(p.direccion)-1);
            p.direccion[sizeof(p.direccion)-1] = '\0';
            break;
        }
        cout << "La dirección es obligatoria.\n";
    }
    
    // Email
    while (true) {
        cout << "Ingrese Email: ";
        getline(cin, tempStr);
        if (validarEmail(tempStr) && !tempStr.empty()) {
            strncpy(p.email, tempStr.c_str(), sizeof(p.email)-1);
            p.email[sizeof(p.email)-1] = '\0';
            break;
        }
        cout << "Correo electrónico inválido.\n";
    }
    
    // Alergias
    cout << "Ingrese Alergias: ";
    getline(cin, tempStr);
    if (tempStr.empty()) {
        strncpy(p.alergias, "Ninguna registrada", sizeof(p.alergias) - 1);
    } else {
        strncpy(p.alergias, tempStr.c_str(), sizeof(p.alergias)-1);
    }
    p.alergias[sizeof(p.alergias)-1] = '\0';
    
    // Observaciones
    cout << "Ingrese Observaciones [Opcional]: ";
    getline(cin, tempStr);
    if (tempStr.empty()) {
        strncpy(p.observaciones, "N/A", sizeof(p.observaciones) - 1);
    } else {
        strncpy(p.observaciones, tempStr.c_str(), sizeof(p.observaciones)-1);
    }
    p.observaciones[sizeof(p.observaciones)-1] = '\0';

    // Inicializar campos restantes
    p.activo = true;
    p.eliminado = false;
    p.cantidadConsultas = 0;
    p.primerConsultaID = -1;
    p.cantidadCitas = 0;
    for (int i = 0; i < 20; i++) {
        p.citasIDs[i] = -1;
    }

    // Guardar paciente
    if (guardarPaciente(p)) {
        cout << "\n Paciente " << p.id << " (" << p.nombre << " " << p.apellido << ") registrado exitosamente." << endl;
        return true;
    } else {
        cout << "\n Error al guardar el paciente." << endl;
        return false;
    }
}

void modificarPaciente() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           MODIFICAR PACIENTE           ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    int id = ValidarNumeroEntero("Ingrese el ID del paciente a modificar: ", 1, 10000);
    
    // Buscar paciente existente
    Paciente paciente = buscarPacientePorID(id);
    
    if (paciente.id == 0 || paciente.eliminado) {
        cout << "❌ No se puede modificar. Paciente no encontrado o eliminado." << endl;
        cout << "Presione ENTER para continuar...";
        cin.ignore();
        return;
    }
    
    // Mostrar datos actuales
    cout << "\n--- DATOS ACTUALES ---" << endl;
    mostrarDetallesPaciente(paciente);
    cout << "\n--- NUEVOS DATOS ---" << endl;
    
    string tempStr;
    char opcion;
    
    // Modificar campos individualmente
    cout << "¿Desea modificar el nombre? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        ValidarSoloLetras("Nuevo nombre: ", tempStr, 2, 49);
        strncpy(paciente.nombre, tempStr.c_str(), sizeof(paciente.nombre)-1);
        paciente.nombre[sizeof(paciente.nombre)-1] = '\0';
    }
    
    cout << "¿Desea modificar el apellido? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        ValidarSoloLetras("Nuevo apellido: ", tempStr, 2, 49);
        strncpy(paciente.apellido, tempStr.c_str(), sizeof(paciente.apellido)-1);
        paciente.apellido[sizeof(paciente.apellido)-1] = '\0';
    }
    
    cout << "¿Desea modificar la cédula? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nueva cédula (máx 19): ";
            getline(cin, tempStr);
            if (tempStr.length() >= 6 && tempStr.length() < 20) {
                strncpy(paciente.cedula, tempStr.c_str(), sizeof(paciente.cedula)-1);
                paciente.cedula[sizeof(paciente.cedula)-1] = '\0';
                break;
            }
            cout << "La cédula debe tener entre 6 y 19 caracteres.\n";
        }
    }
    
    cout << "¿Desea modificar la edad? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        paciente.edad = ValidarNumeroEntero("Nueva edad: ", 1, 120);
    }
    
    cout << "¿Desea modificar el teléfono? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nuevo teléfono: ";
            getline(cin, tempStr);
            if (!tempStr.empty()) {
                strncpy(paciente.telefono, tempStr.c_str(), sizeof(paciente.telefono)-1);
                paciente.telefono[sizeof(paciente.telefono)-1] = '\0';
                break;
            }
            cout << "El teléfono es obligatorio.\n";
        }
    }
    
    cout << "¿Desea modificar la dirección? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nueva dirección: ";
            getline(cin, tempStr);
            if (!tempStr.empty()) {
                strncpy(paciente.direccion, tempStr.c_str(), sizeof(paciente.direccion)-1);
                paciente.direccion[sizeof(paciente.direccion)-1] = '\0';
                break;
            }
            cout << "La dirección es obligatoria.\n";
        }
    }
    
    cout << "¿Desea modificar el email? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nuevo email: ";
            getline(cin, tempStr);
            if (validarEmail(tempStr) && !tempStr.empty()) {
                strncpy(paciente.email, tempStr.c_str(), sizeof(paciente.email)-1);
                paciente.email[sizeof(paciente.email)-1] = '\0';
                break;
            }
            cout << "Correo electrónico inválido.\n";
        }
    }
    
    cout << "¿Desea modificar las alergias? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        cout << "Nuevas alergias: ";
        getline(cin, tempStr);
        if (tempStr.empty()) {
            strncpy(paciente.alergias, "Ninguna registrada", sizeof(paciente.alergias) - 1);
        } else {
            strncpy(paciente.alergias, tempStr.c_str(), sizeof(paciente.alergias)-1);
        }
        paciente.alergias[sizeof(paciente.alergias)-1] = '\0';
    }
    
    cout << "¿Desea modificar las observaciones? (s/n): ";
    cin >> opcion;
    cin.ignore();
    if (opcion == 's' || opcion == 'S') {
        cout << "Nuevas observaciones: ";
        getline(cin, tempStr);
        if (tempStr.empty()) {
            strncpy(paciente.observaciones, "N/A", sizeof(paciente.observaciones) - 1);
        } else {
            strncpy(paciente.observaciones, tempStr.c_str(), sizeof(paciente.observaciones)-1);
        }
        paciente.observaciones[sizeof(paciente.observaciones)-1] = '\0';
    }
    
    // Actualizar en archivo
    if (actualizarPaciente(paciente)) {
        cout << "\n✅ Modificación completada exitosamente." << endl;
    } else {
        cout << "\n❌ Error al guardar las modificaciones." << endl;
    }
    
    cout << "Presione ENTER para continuar...";
    cin.ignore();
}

// ---------------VALIDACIONES---------------
void ValidarSoloLetras(string prompt, string &outStr, int minLen, int maxLen) {
    string temp;
    while (true) {
        cout << prompt;
        getline(cin, temp);
        
        // Verificar si está vacío
        if (temp.empty()) {
            cout << "La entrada no puede estar vacía.\n";
            continue;
        }
        
        // Convertir a size_t para comparar correctamente
        size_t longitud = temp.length();
        
        // Verificar longitud (usando static_cast para evitar warning)
        if (longitud < static_cast<size_t>(minLen)) {
            cout << "Entrada demasiado corta (mínimo " << minLen << " caracteres).\n";
            continue;
        }
        if (longitud > static_cast<size_t>(maxLen)) {
            cout << "Entrada demasiado larga (máximo " << maxLen << " caracteres).\n";
            continue;
        }
        
        // Verificar que solo contenga letras, espacios, guiones y apóstrofes
        bool valido = true;
        for (char c : temp) {
            if (!isalpha(c) && c != ' ' && c != '-' && c != '\'') {
                valido = false;
                break;
            }
        }
        
        if (!valido) {
            cout << "Solo se permiten letras, espacios, guiones (-) y apóstrofes (').\n";
            continue;
        }
        
        outStr = temp;
        break;
    }
}

int ValidarNumeroEntero(string prompt, int minVal, int maxVal) {
    string temp;
    while (true) {
        cout << prompt;
        getline(cin, temp);
        
        // Verificar si está vacío
        if (temp.empty()) {
            cout << "La entrada no puede estar vacía.\n";
            continue;
        }
        
        // Verificar que todos los caracteres sean dígitos
        bool valido = true;
        for (char c : temp) {
            if (!isdigit(c)) {
                valido = false;
                break;
            }
        }
        
        if (!valido) {
            cout << "Solo se permiten números enteros.\n";
            continue;
        }
        
        // Convertir a número
        try {
            int numero = stoi(temp);
            if (numero < minVal || numero > maxVal) {
                cout << "El número debe estar entre " << minVal << " y " << maxVal << ".\n";
                continue;
            }
            return numero;
        } catch (const std::invalid_argument&) {
            cout << "Número inválido.\n";
            continue;
        } catch (const std::out_of_range&) {
            cout << "Número fuera de rango.\n";
            continue;
        }
    }
}

bool validarEmail(string email) {
    if (email.empty()) return false;
    
    // Buscar @
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return false;
    
    // Buscar punto después del @
    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1) return false;
    
    return true;
}

bool validarTipoSangre(string tipoSangre) {
    // Verificar si está vacío
    if (tipoSangre.empty()) return false;

    // Normalizar: eliminar espacios y convertir a mayúsculas
    string cleaned;
    for (char c : tipoSangre) {
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
        if (c >= 'a' && c <= 'z') c = toupper(c);
        cleaned += c;
    }

    // Verificar longitud
    if (cleaned.length() < 2 || cleaned.length() > 3) return false;

    // Verificar primer carácter (A, B, O)
    char primerChar = cleaned[0];
    if (primerChar != 'A' && primerChar != 'B' && primerChar != 'O') return false;

    // Verificar segundo carácter
    char segundoChar = cleaned[1];
    
    if (primerChar == 'A' || primerChar == 'B') {
        if (segundoChar == '+' || segundoChar == '-') {
            // Caso A+ A- B+ B- (2 caracteres)
            if (cleaned.length() != 2) return false;
        } else if (segundoChar == 'B' && primerChar == 'A') {
            // Caso AB (3 caracteres)
            if (cleaned.length() != 3) return false;
            char tercerChar = cleaned[2];
            if (tercerChar != '+' && tercerChar != '-') return false;
        } else {
            return false;
        }
    } else if (primerChar == 'O') {
        // Caso O+ O- (2 caracteres)
        if (segundoChar != '+' && segundoChar != '-') return false;
        if (cleaned.length() != 2) return false;
    }

    return true;
}

// ---------------MENUS---------------
void mostrarMenuPaciente() {
    int opcion = 0;
    do {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║        MENU DE GESTIÓN DE PACIENTES    ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Agregar Paciente" << endl;
        cout << "2. Modificar Paciente" << endl;
        cout << "3. Eliminar Paciente" << endl;
        cout << "4. Buscar Paciente" << endl;
        cout << "5. Listar Pacientes" << endl;
        cout << "0. Regresar al menú principal" << endl;
        
        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 5);
        
        switch (opcion) {
            case 1:
                agregarPaciente();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            case 2:
                modificarPaciente();
                break;
            case 3:{
                int idEliminar = ValidarNumeroEntero("Ingrese ID del paciente a eliminar: ", 1, 10000);
                eliminarPacienteLogico(idEliminar);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 4:{
                buscarYMostrarPaciente();
                break;
            }
            case 5:
                listarPacientes();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            case 0:
                break;
        }
        
        if (opcion != 0) {
            system("cls");
        }
    } while (opcion != 0);
}

void mostrarMenuPrincipal() {
    int opcion = 0;
    do {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║   SISTEMA DE GESTIÓN HOSPITALARIA      ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Manejo de Pacientes" << endl;
        cout << "2. Mostrar datos hospital" << endl;
        cout << "0. Salir" << endl;
        
        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 2);
        
        switch (opcion) {
            case 1:
                mostrarMenuPaciente();
                break;
            case 2:
                mostrarDatosHospital(hospital);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
        }
        
        if (opcion != 0) {
            system("cls");
        }
    } while (opcion != 0);
}

//---------------Registrar-----------------------
bool guardarPaciente(const Paciente &p) {
    // Leer header actual
    ArchivoHeader header = leerHeader(hospital.pacientesFile);
    
    // Abrir archivo en modo append
    ofstream archivo(hospital.pacientesFile, ios::binary | ios::app);
    if (!archivo.is_open()) {
        return false;
    }
    
    // Escribir paciente
    archivo.write(reinterpret_cast<const char*>(&p), sizeof(Paciente));
    bool success = archivo.good();
    archivo.close();
    
    if (!success) {
        return false;
    }
    
    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID = p.id + 1;
    header.registrosActivos++;
    
    // Actualizar contadores del hospital
    hospital.siguienteIDPaciente = header.proximoID;
    hospital.totalPacientesRegistrados = header.registrosActivos;
    
    return actualizarHeader(hospital.pacientesFile, header);
}