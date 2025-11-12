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
long calcularPosicionPaciente(int indice);
bool guardarPaciente(const Paciente &p);
bool actualizarPaciente(Paciente pacienteModificado);
bool eliminarPacienteLogico(int id);
void listarPacientes();
bool agregarPaciente();
Paciente buscarPacientePorID(int id);
void mostrarDetallesPaciente(const Paciente &p);
void buscarYMostrarPaciente();
void modificarPaciente();

//---------------------------Funciones de doctores------------
bool crearArchivoDoctores();
bool agregarDoctor();
bool guardarDoctor(const Doctor &d);
Doctor buscarDoctorPorID(int id);
long calcularPosicionDoctor(int indice);
void modificarDoctor();
bool actualizarDoctor(Doctor doctorModificado);
void mostrarDetallesDoctor(const Doctor &d);
void buscarYMostrarDoctor();
bool eliminarDoctor(int id);
void listarDoctores();

// ---------------VALIDACIONES---------------
void ValidarSoloLetras(string prompt, string &outStr, int minLen, int maxLen);
int ValidarNumeroEntero(string prompt, int minVal, int maxVal);
bool validarEmail(string email);
bool validarTipoSangre(string tipoSangre);
int validacionSyN(int opcion);

// ---------------MENUS---------------
void mostrarMenuPaciente();
void mostrarMenuPrincipal();
void mostrarMenuDoctores();

// ---------------VARIABLES GLOBALES---------------
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

//---------------FUNCIONES GLOBALES---------------
template<typename T>
T buscarPorID(const string& nombreArchivo, int id, size_t tamañoEstructura) {
    T registro;
    memset(&registro, 0, tamañoEstructura);
    
    // 1. Abrir archivo
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << "❌ Error: No se pudo abrir el archivo: " << nombreArchivo << endl;
        return registro;
    }
    
    // 2. Leer header para saber cantidad de registros
    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    // 3. Buscar registro por ID
    bool encontrado = false;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        T temp;
        archivo.read(reinterpret_cast<char*>(&temp), tamañoEstructura);
        
        if (temp.id == id && !temp.eliminado) {
            registro = temp;
            encontrado = true;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "❌ No se encontró registro con ID: " << id << endl;
    }
    
    return registro;
}

template<typename T>
long calcularPosicion(int indice){
    return sizeof(ArchivoHeader) + (indice * sizeof(T));
}

template<typename T>
bool eliminarLogico(const string& nombreArchivo, int id) {
    // 1. Buscar índice del registro
    ifstream archivoBusqueda(nombreArchivo, ios::binary);
    if (!archivoBusqueda.is_open()) {
        cout << "Error: No se pudo abrir el archivo: " << nombreArchivo << endl;
        return false;
    }
    
    ArchivoHeader header;
    archivoBusqueda.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    int indice = -1;
    T registroEncontrado;
    memset(&registroEncontrado, 0, sizeof(T));
    
    // Buscar el registro y su índice
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicion<T>(i);
        archivoBusqueda.seekg(posicion);
        archivoBusqueda.read(reinterpret_cast<char*>(&registroEncontrado), sizeof(T));
        
        if (registroEncontrado.id == id && !registroEncontrado.eliminado) {
            indice = i;
            break;
        }
    }
    archivoBusqueda.close();
    
    if (indice == -1) {
        cout << "No se encontró registro con ID: " << id << endl;
        return false;
    }
    
    // Mostrar información específica del tipo
    cout << "\nRegistro a eliminar:" << endl;
    cout << "🔹 ID: " << registroEncontrado.id;
    
    if constexpr (is_same_v<T, Paciente>) {
        cout << " - " << registroEncontrado.nombre << " " << registroEncontrado.apellido << " (Paciente)";
    } else if constexpr (is_same_v<T, Doctor>) {
        cout << " - " << registroEncontrado.nombre << " " << registroEncontrado.apellido << " (Doctor)";
        cout << " - Especialidad: " << registroEncontrado.especialidad;
    }
    cout << endl;
    
    // Confirmar eliminación
    char confirmar;
    cout << "❓ ¿Está seguro de que desea eliminar este registro? (s/n): ";
    cin >> confirmar;
    cin.ignore();
    
    if (confirmar == 's' || confirmar == 'S') {
        // 2. Abrir archivo en modo lectura/escritura
        fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            cout << "Error: No se pudo abrir el archivo para eliminar." << endl;
            return false;
        }
        
        // 3. Posicionarse y actualizar el registro
        long posicion = calcularPosicion<T>(indice);
        archivo.seekp(posicion);
        
        registroEncontrado.eliminado = true;
        registroEncontrado.fechaModificacion = time(NULL);
        
        // Campos específicos según el tipo
        if constexpr (is_same_v<T, Paciente>) {
            registroEncontrado.activo = false;
        } else if constexpr (is_same_v<T, Doctor>) {
            registroEncontrado.disponible = false;
        }
        
        // 4. Sobrescribir el registro modificado
        archivo.write(reinterpret_cast<const char*>(&registroEncontrado), sizeof(T));
        
        bool success = archivo.good();
        archivo.close();
        
        if (success) {
            // 5. Actualizar header y estadísticas
            ArchivoHeader headerActualizado = leerHeader(nombreArchivo);
            headerActualizado.registrosActivos--;
            
            if constexpr (is_same_v<T, Paciente>) {
                hospital.totalPacientesRegistrados = headerActualizado.registrosActivos;
            } else if constexpr (is_same_v<T, Doctor>) {
                hospital.totalDoctoresRegistrados = headerActualizado.registrosActivos;
            }
            
            if (actualizarHeader(nombreArchivo, headerActualizado)) {
                cout << "✅ " << (is_same_v<T, Paciente> ? "Paciente" : "Doctor") 
                    << " eliminado exitosamente (eliminación lógica)." << endl;
                return true;
            }
        }
        
        cout << "❌ Error al eliminar registro." << endl;
        return false;
    } else {
        cout << "🚫 Eliminación cancelada." << endl;
        return false;
    }
}

int validacionSyN(char opcion){
    while(opcion != 's' && opcion != 'S' && opcion != 'n' && opcion != 'N'){
        cout << "Entrada inválida. Por favor ingrese 's' o 'n': ";
        cin >> opcion;
        cin.ignore();
    }
    return opcion;
}

// ---------------IMPLEMENTACIÓN FUNCIONES ARCHIVOS---------------

bool fileExists(const string& path) {
    ifstream archivo(path);
    return archivo.is_open();
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

long calcularPosicionPaciente(int indice){
    return calcularPosicion<Paciente>(indice);
}

Paciente buscarPacientePorID(int id) {
    return buscarPorID<Paciente>(hospital.pacientesFile, id, sizeof(Paciente));
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
        long posicion = calcularPosicionPaciente(i);
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
    long posicion = calcularPosicionPaciente(indice);
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
    return eliminarLogico<Paciente>(hospital.pacientesFile, id);
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
        Paciente p = buscarPacientePorID(i + 1);
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
        cout << "No se puede modificar. Paciente no encontrado o eliminado." << endl;
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
    cout << "¿Desea modificar el apellido? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        ValidarSoloLetras("Nuevo apellido: ", tempStr, 2, 49);
        strncpy(paciente.apellido, tempStr.c_str(), sizeof(paciente.apellido)-1);
        paciente.apellido[sizeof(paciente.apellido)-1] = '\0';
    }
    
    cout << "¿Desea modificar la cédula? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
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
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        paciente.edad = ValidarNumeroEntero("Nueva edad: ", 1, 120);
    }
    
    cout << "¿Desea modificar el teléfono? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
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
    validacionSyN(opcion);
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
    validacionSyN(opcion);
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
    validacionSyN(opcion);
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
    validacionSyN(opcion);
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
        cout << "\nModificación completada exitosamente." << endl;
    } else {
        cout << "\nError al guardar las modificaciones." << endl;
    }
    
    cout << "Presione ENTER para continuar...";
    cin.ignore();
}

//--------------------------Doctores--------------------------
long calcularPosicionDoctor(int indice){
    return calcularPosicion<Doctor>(indice);
}

bool crearArchivoDoctores() {
    if (fileExists(hospital.doctoresFile)) {
        return true;
    }
    
    if (!inicializarArchivo(hospital.doctoresFile, 1)) {
        cout << " Error al crear archivo de doctores." << endl;
        return false;
    }
    
    cout << "✅ Archivo de doctores creado exitosamente." << endl;
    return true;
}

bool agregarDoctor() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║         REGISTRAR NUEVO DOCTOR         ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    Doctor d;
    string tempStr;
    float tempFloat;
    memset(&d, 0, sizeof(Doctor));

    // Lista de especialidades médicas
    const int numEspecialidades = 5;
    string especialidades[numEspecialidades] = {
        "Medicina General",
        "Pediatría",
        "Ginecología",
        "Cardiología",
        "Dermatología"
    };

    // Obtener ID del header
    ArchivoHeader header = leerHeader(hospital.doctoresFile);
    d.id = header.proximoID;
    d.fechaCreacion = time(NULL);
    d.fechaModificacion = d.fechaCreacion;
    
    cout << "ID asignado: " << d.id << endl;
    
    // Recolección de datos con validaciones
    ValidarSoloLetras("Ingrese Nombre: ", tempStr, 2, 49);
    strncpy(d.nombre, tempStr.c_str(), sizeof(d.nombre)-1);
    d.nombre[sizeof(d.nombre)-1] = '\0';
    
    ValidarSoloLetras("Ingrese Apellido: ", tempStr, 2, 49);
    strncpy(d.apellido, tempStr.c_str(), sizeof(d.apellido)-1);
    d.apellido[sizeof(d.apellido)-1] = '\0';

    // Cédula Profesional
    while (true) {
        cout << "Ingrese Cédula Profesional: ";
        getline(cin, tempStr);
        if (tempStr.length() >= 3 && tempStr.length() < 20) {
            strncpy(d.cedulaProfesional, tempStr.c_str(), sizeof(d.cedulaProfesional)-1);
            d.cedulaProfesional[sizeof(d.cedulaProfesional)-1] = '\0';
            break;
        }
        cout << "La cédula debe tener entre 3 y 19 caracteres.\n";
    }

    // Especialidad - Selección desde lista
    cout << "\nSeleccione la especialidad:\n";
    for(int i = 0; i < numEspecialidades; i++){
        cout << i+1 << ". " << especialidades[i] << endl;
    }
    cout << "6. Otra especialidad\n";
    
    int opcionEspecialidad = ValidarNumeroEntero("Opción: ", 1, 6);
    
    if (opcionEspecialidad >= 1 && opcionEspecialidad <= 5) {
        // Especialidad de la lista
        strncpy(d.especialidad, especialidades[opcionEspecialidad-1].c_str(), sizeof(d.especialidad)-1);
        d.especialidad[sizeof(d.especialidad)-1] = '\0';
    } else {
        // Otra especialidad - ingresar manualmente
        while (true) {
            cout << "Ingrese la especialidad: ";
            getline(cin, tempStr);
            if (!tempStr.empty() && tempStr.length() <= 49) {
                strncpy(d.especialidad, tempStr.c_str(), sizeof(d.especialidad)-1);
                d.especialidad[sizeof(d.especialidad)-1] = '\0';
                break;
            }
            cout << "La especialidad es obligatoria (máx 49 caracteres).\n";
        }
    }

    // Años de Experiencia
    d.aniosExperiencia = ValidarNumeroEntero("Ingrese Años de Experiencia: ", 0, 60);

    // Costo de Consulta
    while (true) {
        cout << "Ingrese Costo de Consulta: $";
        cin >> tempFloat;
        cin.ignore();
        
        if (tempFloat >= 0 && tempFloat <= 10000) {
            d.costoConsulta = tempFloat;
            break;
        }
        cout << "Costo inválido. Debe estar entre $0 y $10,000.\n";
    }

    // Horario de Atención
    while (true) {
        cout << "Ingrese Horario de Atención: ";
        getline(cin, tempStr);
        if (!tempStr.empty() && tempStr.length() <= 49) {
            strncpy(d.horarioAtencion, tempStr.c_str(), sizeof(d.horarioAtencion)-1);
            d.horarioAtencion[sizeof(d.horarioAtencion)-1] = '\0';
            break;
        }
        cout << "El horario es obligatorio (máx 49 caracteres).\n";
    }

    // Teléfono
    while (true) {
        cout << "Ingrese Teléfono: ";
        getline(cin, tempStr);
        if (!tempStr.empty()) {
            strncpy(d.telefono, tempStr.c_str(), sizeof(d.telefono)-1);
            d.telefono[sizeof(d.telefono)-1] = '\0';
            break;
        }
        cout << "El teléfono es obligatorio.\n";
    }

    // Email
    while (true) {
        cout << "Ingrese Email: ";
        getline(cin, tempStr);
        if (validarEmail(tempStr) && !tempStr.empty()) {
            strncpy(d.email, tempStr.c_str(), sizeof(d.email)-1);
            d.email[sizeof(d.email)-1] = '\0';
            break;
        }
        cout << "Correo electrónico inválido.\n";
    }

    // Inicializar campos restantes
    d.disponible = true;
    d.eliminado = false;
    d.cantidadPacientes = 0;
    d.cantidadCitas = 0;
    
    // Inicializar arrays
    for (int i = 0; i < 50; i++) {
        d.pacientesIDs[i] = -1;
    }
    for (int i = 0; i < 30; i++) {
        d.citasIDs[i] = -1;
    }

    // Guardar doctor
    if (guardarDoctor(d)) {
        cout << "\n✅ Doctor " << d.id << " (" << d.nombre << " " << d.apellido << ") registrado exitosamente." << endl;
        cout << "Especialidad: " << d.especialidad << endl;
        cout << "Costo consulta: $" << d.costoConsulta << endl;
        return true;
    } else {
        cout << "\n❌ Error al guardar el doctor." << endl;
        return false;
    }
}

Doctor buscarDoctorPorID(int id) {
    return buscarPorID<Doctor>(hospital.doctoresFile, id, sizeof(Doctor));
}

bool actualizarDoctor(Doctor doctorModificado) {
    // 1. Buscar el doctor para verificar que existe y obtener sus datos actuales
    Doctor doctorExistente = buscarDoctorPorID(doctorModificado.id);
    
    if (doctorExistente.id == 0 || doctorExistente.eliminado) {
        cout << "❌ No se puede actualizar. Doctor no encontrado o eliminado." << endl;
        return false;
    }
    
    // 2. Buscar la posición del doctor por ID en el archivo
    ifstream archivoBusqueda(hospital.doctoresFile, ios::binary);
    if (!archivoBusqueda.is_open()) {
        cout << "Error: No se pudo abrir el archivo de doctores." << endl;
        return false;
    }
    
    ArchivoHeader header;
    archivoBusqueda.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    int indice = -1;
    Doctor temp;
    
    // Buscar el índice del doctor usando la función calcularPosicionDoctor
    for (int i = 0; i < header.cantidadRegistros; i++) {
        long posicion = calcularPosicionDoctor(i);
        archivoBusqueda.seekg(posicion);
        archivoBusqueda.read(reinterpret_cast<char*>(&temp), sizeof(Doctor));
        
        if (temp.id == doctorModificado.id && !temp.eliminado) {
            indice = i;
            break;
        }
    }
    archivoBusqueda.close();
    
    if (indice == -1) {
        cout << "❌ No se encontró doctor con ID: " << doctorModificado.id << endl;
        return false;
    }
    
    // 3. Abrir archivo en modo lectura/escritura
    fstream archivo(hospital.doctoresFile, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo para modificación." << endl;
        return false;
    }
    
    // 4. Posicionarse en la ubicación exacta usando seekp
    long posicion = calcularPosicionDoctor(indice);
    archivo.seekp(posicion);
    
    // 5. Actualizar fechaModificacion
    doctorModificado.fechaModificacion = time(NULL);
    
    // 6. Escribir estructura completa sobre el registro existente
    archivo.write(reinterpret_cast<const char*>(&doctorModificado), sizeof(Doctor));
    
    bool success = archivo.good();
    
    // 7. Cerrar archivo
    archivo.close();
    
    // 8. Retornar true si exitoso
    if (success) {
        cout << "Doctor actualizado exitosamente." << endl;
        return true;
    } else {
        cout << "Error al actualizar doctor." << endl;
        return false;
    }
}

void mostrarDetallesDoctor(const Doctor &d) {
    if (d.id == 0) {
        cout << "❌ Doctor no encontrado o ID inválido." << endl;
        return;
    }
    
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║          DETALLES DEL DOCTOR          ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << " ID: " << d.id << endl;
    cout << " Nombre: " << d.nombre << " " << d.apellido << endl;
    cout << " Cédula Profesional: " << d.cedulaProfesional << endl;
    cout << " Especialidad: " << d.especialidad << endl;
    cout << " Años de Experiencia: " << d.aniosExperiencia << endl;
    cout << " Costo Consulta: $" << fixed << setprecision(2) << d.costoConsulta << endl;
    cout << " Horario Atención: " << d.horarioAtencion << endl;
    cout << " Teléfono: " << d.telefono << endl;
    cout << " Email: " << d.email << endl;
    cout << " Pacientes atendidos: " << d.cantidadPacientes << endl;
    cout << " Citas pendientes: " << d.cantidadCitas << endl;
    
    // Mostrar lista de IDs de pacientes si existen
    if (d.cantidadPacientes > 0) {
        cout << " 🔹 IDs de Pacientes: ";
        for (int i = 0; i < d.cantidadPacientes && i < 50; i++) {
            if (d.pacientesIDs[i] != -1) {
                cout << d.pacientesIDs[i];
                if (i < d.cantidadPacientes - 1 && i < 49) cout << ", ";
            }
        }
        cout << endl;
    }
    
    // Mostrar lista de IDs de citas si existen
    if (d.cantidadCitas > 0) {
        cout << "IDs de Citas: ";
        for (int i = 0; i < d.cantidadCitas && i < 30; i++) {
            if (d.citasIDs[i] != -1) {
                cout << d.citasIDs[i];
                if (i < d.cantidadCitas - 1 && i < 29) cout << ", ";
            }
        }
        cout << endl;
    }
    
    // Mostrar fechas formateadas
    char fechaStr[20];
    
    strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", localtime(&d.fechaCreacion));
    cout << " 🔹 Fecha de registro: " << fechaStr << endl;
    
    if (d.fechaModificacion != d.fechaCreacion) {
        strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", localtime(&d.fechaModificacion));
        cout << " 🔹 Última modificación: " << fechaStr << endl;
    }
    
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║              ESTADO                    ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << " Disponible: " << (d.disponible ? "Sí" : "No") << endl;
    cout << " Eliminado: " << (d.eliminado ? "Sí" : "No") << endl;
    
    if (!d.disponible && !d.eliminado) {
        cout << " 💡 Información: Doctor existe pero no está disponible actualmente" << endl;
    }
}

void buscarYMostrarDoctor() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           BUSCAR DOCTOR               ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    int id = ValidarNumeroEntero("Ingrese el ID del doctor a buscar: ", 1, 10000);
    
    Doctor doctor = buscarDoctorPorID(id);
    
    if (doctor.id != 0) {
        mostrarDetallesDoctor(doctor);
    } else {
        cout << " ❌ No se encontró ningún doctor con ID: " << id << endl;
    }
    
    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
}

void modificarDoctor() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           MODIFICAR DOCTOR             ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    int id = ValidarNumeroEntero("Ingrese el ID del doctor a modificar: ", 1, 10000);

    const int numEspecialidades = 5;
    string especialidades[numEspecialidades] = {
        "Medicina General",
        "Pediatría",
        "Ginecología",
        "Cardiología",
        "Dermatología"
    };

    //buscar doctor
    Doctor doctor = buscarDoctorPorID(id);
    if (doctor.id == 0 || doctor.eliminado) {
        cout << "No se puede modificar. Doctor no encontrado o eliminado." << endl;
        cout << "Presione ENTER para continuar...";
        cin.ignore();
        return;
    }

    //mostar datos
    cout << "\n--- DATOS ACTUALES ---" << endl;
    mostrarDetallesDoctor(doctor);
    cout << "\n--- NUEVOS DATOS ---" << endl;
    string tempStr;
    float tempFloat;
    char opcion;

    // Modificar campos individualmente
    cout << "¿Desea modificar el apellido? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        ValidarSoloLetras("Nuevo apellido: ", tempStr, 2, 49);
        strncpy(doctor.apellido, tempStr.c_str(), sizeof(doctor.apellido)-1);
        doctor.apellido[sizeof(doctor.apellido)-1] = '\0';
    }

    cout << "¿Desea modificar la cédula profesional? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nueva cédula profesional: ";
            getline(cin, tempStr);
            if (tempStr.length() >= 3 && tempStr.length() < 20) {
                strncpy(doctor.cedulaProfesional, tempStr.c_str(), sizeof(doctor.cedulaProfesional)-1);
                doctor.cedulaProfesional[sizeof(doctor.cedulaProfesional)-1] = '\0';
                break;
            }
            cout << "La cédula debe tener entre 3 y 19 caracteres.\n";
        }
    }

    cout << "¿Desea modificar la especialidad? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    while(opcion != 's' && opcion != 'S' && opcion != 'n' && opcion != 'N'){
        cout << "Entrada inválida. Por favor ingrese 's' o 'n': ";
        cin >> opcion;
        cin.ignore();
    }
    if(opcion == 's' || opcion == 'S'){
        
    }

    cout << "¿Desea modificar los años de experiencia? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        doctor.aniosExperiencia = ValidarNumeroEntero("Nuevos años de experiencia: ", 0, 60);
    }

    cout << "¿Desea modificar el costo de consulta? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nuevo costo de consulta: $";
            cin >> tempFloat;
            cin.ignore();
            
            if (tempFloat >= 0 && tempFloat <= 10000) {
                doctor.costoConsulta = tempFloat;
                break;
            }
            cout << "Costo inválido. Debe estar entre $0 y $10,000.\n";
        }
    }

    cout << "¿Desea modificar el horario de atención? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nuevo horario de atención: ";
            getline(cin, tempStr);
            if (!tempStr.empty() && tempStr.length() <= 49) {
                strncpy(doctor.horarioAtencion, tempStr.c_str(), sizeof(doctor.horarioAtencion)-1);
                doctor.horarioAtencion[sizeof(doctor.horarioAtencion)-1] = '\0';
                break;
            }
            cout << "El horario es obligatorio (máx 49 caracteres).\n";
        }
    }

    cout << "¿Desea modificar el teléfono? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nuevo teléfono: ";
            getline(cin, tempStr);
            if (!tempStr.empty()) {
                strncpy(doctor.telefono, tempStr.c_str(), sizeof(doctor.telefono)-1);
                doctor.telefono[sizeof(doctor.telefono)-1] = '\0';
                break;
            }
            cout << "El teléfono es obligatorio.\n";
        }
    }

    cout << "¿Desea modificar el email? (s/n): ";
    cin >> opcion;
    cin.ignore();
    validacionSyN(opcion);
    if (opcion == 's' || opcion == 'S') {
        while (true) {
            cout << "Nuevo email: ";
            getline(cin, tempStr);
            if (validarEmail(tempStr) && !tempStr.empty()) {
                strncpy(doctor.email, tempStr.c_str(), sizeof(doctor.email)-1);
                doctor.email[sizeof(doctor.email)-1] = '\0';
                break;
            }
            cout << "Correo electrónico inválido.\n";
        }
    }

    // Actualizar en archivo
    if (actualizarDoctor(doctor)) {
        cout << "\nModificación completada exitosamente." << endl;
    } else {
        cout << "\nError al guardar las modificaciones." << endl;
    }
    cout << "Presione ENTER para continuar...";
    cin.ignore();
}

bool eliminarDoctor(int id) {
    return eliminarLogico<Doctor>(hospital.doctoresFile, id);
}

void listarDoctores() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║           LISTA DE DOCTORES           ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    ArchivoHeader header = leerHeader(hospital.doctoresFile);
    
    if (header.registrosActivos == 0) {
        cout << "No hay doctores registrados." << endl;
        return;
    }
    
    cout << left << setw(6) << "ID" 
        << setw(20) << "Nombre" 
        << setw(20) << "Apellido"
        << setw(20) << "Especialidad"
        << setw(8) << "Exp"
        << setw(12) << "Costo"
        << setw(10) << "Estado" << endl;
    cout << string(96, '-') << endl;
    
    int contador = 0;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Doctor d = buscarDoctorPorID(i + 1);
        if (!d.eliminado) {
            cout << left << setw(6) << d.id
                << setw(20) << d.nombre
                << setw(20) << d.apellido
                << setw(20) << d.especialidad
                << setw(8) << d.aniosExperiencia
                << setw(12) << fixed << setprecision(2) << "$" + to_string(d.costoConsulta).substr(0, 6)
                << setw(10) << (d.disponible ? "Activo" : "Inactivo") << endl;
            contador++;
        }
    }
    
    cout << string(96, '-') << endl;
    cout << "Total de doctores: " << contador << endl;
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
        cout << "2. Manejo de doctores"  << endl;
        cout << "3. Mostrar datos hospital" << endl;
        cout << "0. Salir" << endl;
        
        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 3);
        
        switch (opcion) {
            case 1:{
                mostrarMenuPaciente();
                break;
            }
            case 2:{
                mostrarMenuDoctores();
                break;
            }
            case 3:{
                mostrarDatosHospital(hospital);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
        }
        
        if (opcion != 0) {
            system("cls");
        }
    } while (opcion != 0);
}

void mostrarMenuDoctores(){
    int opcion = 0;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║        MENU DE GESTIÓN DE DOCTORES     ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Agregar Doctor" << endl;
        cout << "2. Modificar Doctor" << endl;
        cout << "3. Eliminar Doctor" << endl;
        cout << "4. Buscar Doctor" << endl;
        cout << "5. Listar Doctores" << endl;
        cout << "6. Listar Doctores por Especialidad" << endl;
        cout << "0. Regresar al menú principal" << endl;
        
        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 6);
        switch(opcion){
            case 1:{
                agregarDoctor();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 2:{
                modificarDoctor();
                break;
            }
            case 3:{
                int idEliminar = ValidarNumeroEntero("Ingrese ID del doctor a eliminar: ", 1, 10000);
                eliminarDoctor(idEliminar);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 4:{
                buscarYMostrarDoctor();
                break;
            }
            case 5:{
                listarDoctores();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
        }
    }while(opcion != 0);
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

bool guardarDoctor(const Doctor &d) {
    // Verificar/crear archivo primero
    if (!crearArchivoDoctores()) {
        return false;
    }
    
    // Leer header actual
    ArchivoHeader header = leerHeader(hospital.doctoresFile);
    
    // Abrir archivo en modo lectura/escritura
    fstream archivo(hospital.doctoresFile, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "❌ Error: No se pudo abrir archivo de doctores." << endl;
        return false;
    }
    
    // Posicionarse al final del archivo usando seekp
    archivo.seekp(0, ios::end);
    long posicion = archivo.tellp();
    
    // Verificar si es el primer registro después del header
    if (posicion == sizeof(ArchivoHeader)) {
        // Primer registro, posición exacta
        archivo.seekp(sizeof(ArchivoHeader));
    }
    // Si no, ya estamos posicionados al final
    
    // Escribir doctor
    archivo.write(reinterpret_cast<const char*>(&d), sizeof(Doctor));
    bool success = archivo.good();
    archivo.close();
    
    if (!success) {
        cout << "❌ Error al escribir doctor en archivo." << endl;
        return false;
    }
    
    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID = d.id + 1;
    header.registrosActivos++;
    
    // Actualizar contadores del hospital
    hospital.siguienteIDDoctor = header.proximoID;
    hospital.totalDoctoresRegistrados = header.registrosActivos;
    
    if (!actualizarHeader(hospital.doctoresFile, header)) {
        cout << "❌ Error al actualizar header de doctores." << endl;
        return false;
    }
    
    return true;
}

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

