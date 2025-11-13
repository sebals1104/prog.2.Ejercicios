#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <limits>
#include <fstream>
#include <direct.h>

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
void listarDoctorePorEspecialidad();

//---------------------------Funciones de citas------------
bool agregarCita();
bool guardarCita(const Cita &c);
bool atenderCita();
Cita buscarCitaPorID(int id);
void eliminarCitaLogico(int id);
void mostrarDetallesCita(const Cita &c);
void buscarYMostrarCita();
void listarCitas();
void listarCitasPendientes();

//---------------FUNCIONES DE HISTORIALES MÉDICOS---------------
void mostrarAlergiasPaciente();
void listarCitasPorPaciente();
void listarCitasAtendidasPorDoctor();

//----------------Mantenimiento-----------------
void hacerRespaldo();
void mostrarEstadisticasArchivos();
bool restaurarDesdeRespaldo(const string& carpeta);

// ---------------VALIDACIONES---------------
void ValidarSoloLetras(string prompt, string &outStr, int minLen, int maxLen);
int ValidarNumeroEntero(string prompt, int minVal, int maxVal);
bool validarEmail(string email);
bool validarTipoSangre(string tipoSangre);
int validacionSyN(char opcion);

// ---------------MENUS---------------
void mostrarMenuPaciente();
void mostrarMenuPrincipal();
void mostrarMenuDoctores();
void mostrarMenuCitas();
void mostrarEstadoDeLosArchivos();
void menuDeHistorialesMedicos();
void MostrarMenuDeMantenimiento();

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
long calcularPosicion(int indice){
    return sizeof(ArchivoHeader) + (indice * sizeof(T));
}

template<typename T>
T buscarPorID(const string& nombreArchivo, int id) {
    T registro{};
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << " Error: No se pudo abrir el archivo: " << nombreArchivo << endl;
        return registro;
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    for (int i = 0; i < header.cantidadRegistros; i++) {
        long pos = calcularPosicion<T>(i);
        archivo.seekg(pos);
        T temp{};
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(T));

        if (temp.id == id && !temp.eliminado) {
            return temp;
        }
    }

    cout << "❌ No se encontró registro con ID: " << id << endl;
    return registro;
}

template<typename T>
bool eliminarLogico(const string& nombreArchivo, int id) {
    // 1. Buscar índice del registro
    ifstream archivoBusqueda(nombreArchivo, ios::binary);
    if (!archivoBusqueda.is_open()) {
        cout << "❌ Error: No se pudo abrir el archivo: " << nombreArchivo << endl;
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
        cout << "❌ No se encontró registro con ID: " << id << endl;
        return false;
    }
    
    // Mostrar información específica del tipo
    cout << "\n Registro a eliminar:" << endl;
    cout << "ID: " << registroEncontrado.id;
    
    if constexpr (is_same_v<T, Paciente>) {
        cout << " - " << registroEncontrado.nombre << " " << registroEncontrado.apellido << " (Paciente)";
    } else if constexpr (is_same_v<T, Doctor>) {
        cout << " - " << registroEncontrado.nombre << " " << registroEncontrado.apellido << " (Doctor)";
        cout << " - Especialidad: " << registroEncontrado.especialidad;
    } else if constexpr (is_same_v<T, Cita>) {
        cout << " - Cita del " << registroEncontrado.fecha << " a las " << registroEncontrado.hora;
        cout << " - Estado: " << registroEncontrado.estado;
        cout << " - Paciente ID: " << registroEncontrado.pacienteID << ", Doctor ID: " << registroEncontrado.doctorID;
    } else if constexpr (is_same_v<T, HistorialMedico>) {
        cout << " - Historial - Paciente ID: " << registroEncontrado.pacienteID;
        cout << " - Fecha: " << registroEncontrado.fecha;
    }
    cout << endl;
    
    cout << "\n¿Está seguro de que desea eliminar este registro? (s/n): ";
    char confirmar;
    cin >> confirmar;
    cin.ignore();
    confirmar = validacionSyN((char)confirmar);
    
    if (confirmar == 's' || confirmar == 'S') {
        // 2. Abrir archivo en modo lectura/escritura
        fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            cout << "❌ Error: No se pudo abrir el archivo para eliminar." << endl;
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
        } else if constexpr (is_same_v<T, Cita>) {
            strncpy(registroEncontrado.estado, "Cancelada", sizeof(registroEncontrado.estado)-1);
            registroEncontrado.atendida = false;
        } else if constexpr (is_same_v<T, HistorialMedico>) {
            // Para historial médico, no hay campos adicionales específicos
        }
        
        // 4. Sobrescribir el registro modificado
        archivo.write(reinterpret_cast<const char*>(&registroEncontrado), sizeof(T));
        
        bool success = archivo.good();
        archivo.close();
        
        if (success) {
            // 5. Actualizar header y estadísticas
            ArchivoHeader headerActualizado = leerHeader(nombreArchivo);
            headerActualizado.registrosActivos--;
            
            // Actualizar estadísticas del hospital según el tipo
            if constexpr (is_same_v<T, Paciente>) {
                hospital.totalPacientesRegistrados = headerActualizado.registrosActivos;
            } else if constexpr (is_same_v<T, Doctor>) {
                hospital.totalDoctoresRegistrados = headerActualizado.registrosActivos;
            } else if constexpr (is_same_v<T, Cita>) {
                hospital.totalCitasAgendadas = headerActualizado.registrosActivos;
            } else if constexpr (is_same_v<T, HistorialMedico>) {
                hospital.totalConsultasRealizadas = headerActualizado.registrosActivos;
            }
            
            if (actualizarHeader(nombreArchivo, headerActualizado)) {
                string tipoRegistro;
                if constexpr (is_same_v<T, Paciente>) tipoRegistro = "Paciente";
                else if constexpr (is_same_v<T, Doctor>) tipoRegistro = "Doctor";
                else if constexpr (is_same_v<T, Cita>) tipoRegistro = "Cita";
                else if constexpr (is_same_v<T, HistorialMedico>) tipoRegistro = "Historial médico";
                
                cout << tipoRegistro << " eliminado exitosamente (eliminación lógica)." << endl;
                return true;
            }
        }
        
        cout << "Error al eliminar registro." << endl;
        return false;
    } else {
        cout << "Eliminación cancelada." << endl;
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

bool esFechaValida(const char fecha[]) {
    int dia, mes, anio;
    if (sscanf(fecha, "%2d/%2d/%4d", &dia, &mes, &anio) != 3) {
        return false;
    }

    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;

    int diasMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    // Año bisiesto
    if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
        diasMes[1] = 29;
    }

    if (dia < 1 || dia > diasMes[mes - 1]) return false;

    return true;
}

void ingresarFechaValida(char fecha[], int tam) {
    do {
        cout << "Ingrese fecha (DD/MM/YYYY): ";
        cin.getline(fecha, tam);
        if (!esFechaValida(fecha)) {
            cout << "❌ Fecha inválida. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
}

bool esHoraValida(const char hora[]) {
    int h, m;
    if (sscanf(hora, "%2d:%2d", &h, &m) != 2) return false;
    if (h < 0 || h > 23) return false;
    if (m < 0 || m > 59) return false;
    return true;
}

void ingresarHoraValida(char hora[], int tam) {
    do {
        cout << "Ingrese hora (HH:MM): ";
        cin.getline(hora, tam);
        if (!esHoraValida(hora)) {
            cout << "❌ Hora inválida. Intente nuevamente.\n";
        } else {
            break;
        }
    } while (true);
}

template<typename T>
bool compactarArchivo(const string& nombreArchivo) {
    ifstream archivoOriginal(nombreArchivo, ios::binary);
    if (!archivoOriginal.is_open()) {
        cout << " Error al abrir el archivo: " << nombreArchivo << endl;
        return false;
    }

    // Leer header
    ArchivoHeader header{};
    archivoOriginal.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    string tempFile = "temp.dat";
    ofstream archivoTemp(tempFile, ios::binary);
    if (!archivoTemp.is_open()) {
        cout << "❌ Error al crear archivo temporal." << endl;
        archivoOriginal.close();
        return false;
    }

    // Escribir header temporal (se actualizará después)
    ArchivoHeader nuevoHeader{};
    archivoTemp.write(reinterpret_cast<char*>(&nuevoHeader), sizeof(ArchivoHeader));

    T registro;
    int registrosActivos = 0;

    while (archivoOriginal.read(reinterpret_cast<char*>(&registro), sizeof(T))) {
        if (!registro.eliminado) {
            archivoTemp.write(reinterpret_cast<char*>(&registro), sizeof(T));
            registrosActivos++;
        }
    }

    archivoOriginal.close();
    archivoTemp.close();

    // Actualizar header
    nuevoHeader.cantidadRegistros = registrosActivos;
    nuevoHeader.registrosActivos = registrosActivos;
    nuevoHeader.proximoID = header.proximoID; // Mantener el próximo ID
    actualizarHeader(tempFile, nuevoHeader);

    // Reemplazar archivo original
    if (remove(nombreArchivo.c_str()) != 0) {
        cout << " Error al eliminar archivo original." << endl;
        return false;
    }
    if (rename(tempFile.c_str(), nombreArchivo.c_str()) != 0) {
        cout << " Error al renombrar archivo temporal." << endl;
        return false;
    }

    cout << " Archivo compactado correctamente: " << nombreArchivo << endl;
    return true;
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
    return buscarPorID<Paciente>(hospital.pacientesFile, id);
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

    ifstream archivo(hospital.pacientesFile, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de pacientes.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

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

    Paciente p;
    int contador = 0;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Paciente))) {
        if (!p.eliminado) {
            cout << left << setw(6) << p.id
                << setw(20) << p.nombre
                << setw(20) << p.apellido
                << setw(10) << p.edad
                << setw(15) << p.telefono << endl;
            contador++;
        }
    }

    archivo.close();

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
    return buscarPorID<Doctor>(hospital.doctoresFile, id);
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

    ifstream archivo(hospital.doctoresFile, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de doctores.\n";
        return;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    if (header.registrosActivos == 0) {
        cout << "No hay doctores registrados." << endl;
        archivo.close();
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

    Doctor d;
    int contador = 0;
    while (archivo.read(reinterpret_cast<char*>(&d), sizeof(Doctor))) {
        if (!d.eliminado) {
            cout << left << setw(6) << d.id
                << setw(20) << d.nombre
                << setw(20) << d.apellido
                << setw(20) << d.especialidad
                << setw(8) << d.aniosExperiencia
                << setw(12) << fixed << setprecision(2) << "$" << d.costoConsulta
                << setw(10) << (d.disponible ? "Activo" : "Inactivo") << endl;
            contador++;
        }
    }

    archivo.close();

    cout << string(96, '-') << endl;
    cout << "Total de doctores: " << contador << endl;
}

void listarDoctorePorEspecialidad() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║     LISTA DE DOCTORES POR ESPECIALIDAD ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    //Lista de especialidades médicas
    const int numEspecialidades = 5;
    string especialidades[numEspecialidades] = {
        "Medicina General",
        "Pediatría",
        "Ginecología",
        "Cardiología",
        "Dermatología"
    };

    cout << "Seleccione la especialidad:\n";
    for(int i = 0; i < numEspecialidades; i++){
        cout << i+1 << ". " << especialidades[i] << endl;
    }
    int opcionEspecialidad = ValidarNumeroEntero("Opción: ", 1, numEspecialidades);
    string especialidadSeleccionada = especialidades[opcionEspecialidad - 1];

    ArchivoHeader header = leerHeader(hospital.doctoresFile);
    bool encontrado = false;
    cout << left << setw(6) << "ID" 
        << setw(20) << "Nombre" 
        << setw(20) << "Apellido"
        << setw(20) << "Especialidad"
        << setw(8) << "Exp"
        << setw(12) << "Costo"
        << setw(10) << "Estado" << endl;
    cout << string(96, '-') << endl;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        Doctor d = buscarDoctorPorID(i + 1);
        if (!d.eliminado && d.especialidad == especialidadSeleccionada) {
            cout << left << setw(6) << d.id
                << setw(20) << d.nombre
                << setw(20) << d.apellido
                << setw(20) << d.especialidad
                << setw(8) << d.aniosExperiencia
                << setw(12) << fixed << setprecision(2) << "$" + to_string(d.costoConsulta).substr(0, 6)
                << setw(10) << (d.disponible ? "Activo" : "Inactivo") << endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron doctores en la especialidad: " << especialidadSeleccionada << endl;
    } else {
        cout << string(96, '-') << endl;
    }
}
//---------------------CITAS---------------------
long calcularPosicionCita(int indice){
    return calcularPosicion<Cita>(indice);
}

bool agregarCita() {
        Cita nueva{};
    nueva.id = hospital.siguienteIDCita;

    // Seleccionar paciente válido
    Paciente paciente{};
    do {
        int idP = ValidarNumeroEntero("Ingrese ID de paciente: ", 1, 10000);
        paciente = buscarPorID<Paciente>(hospital.pacientesFile, idP);
        if (paciente.id == 0 || paciente.eliminado) {
            cout << "❌ Paciente no encontrado, ingrese un paciente válido.\n";
        } else break;
    } while(true);
    nueva.pacienteID = paciente.id;

    // Seleccionar doctor válido
    Doctor doctor{};
    do {
        int idD = ValidarNumeroEntero("Ingrese ID de doctor: ", 1, 10000);
        doctor = buscarPorID<Doctor>(hospital.doctoresFile, idD);
        if (doctor.id == 0 || doctor.eliminado) {
            cout << "❌ Doctor no encontrado, ingrese un doctor válido.\n";
        } else break;
    } while(true);
    nueva.doctorID = doctor.id;

    // Fecha
    ingresarFechaValida(nueva.fecha, sizeof(nueva.fecha));

    // Hora
    ingresarHoraValida(nueva.hora, sizeof(nueva.hora));

    // Motivo
    cout << "Ingrese motivo de la cita: ";
    cin.ignore();
    cin.getline(nueva.motivo, sizeof(nueva.motivo));

    strncpy(nueva.estado, "Pendiente", sizeof(nueva.estado)-1);
    nueva.atendida = false;
    nueva.eliminado = false;
    nueva.consultaID = 0;
    nueva.fechaCreacion = time(NULL);
    nueva.fechaModificacion = time(NULL);

    if (guardarCita(nueva)) {
        cout << "Cita agendada con éxito. ID: " << nueva.id << "\n";
        return true;
    } else {
        cout << "Error al guardar la cita.\n";
        return false;
    }
}

bool atenderCita() {
    // Validar y obtener cita existente
    Cita cita;
    int idCita = ValidarNumeroEntero("Ingrese ID de la cita a atender: ", 1, 10000);
    cita = buscarPorID<Cita>(hospital.citasFile, idCita);
    if (cita.id == 0 || cita.eliminado) {
        cout << "Cita no encontrada o eliminada.\n";
        return false;
    }
    if (cita.atendida) {
        cout << "La cita ya ha sido atendida previamente.\n";
        return false;
    }

    // Validar paciente y doctor asociados
    Paciente paciente = buscarPorID<Paciente>(hospital.pacientesFile, cita.pacienteID);
    Doctor doctor = buscarPorID<Doctor>(hospital.doctoresFile, cita.doctorID);

    if (paciente.id == 0 || paciente.eliminado) {
        cout << "Paciente inválido. No se puede atender la cita.\n";
        return false;
    }
    if (doctor.id == 0 || doctor.eliminado) {
        cout << "Doctor inválido. No se puede atender la cita.\n";
        return false;
    }

    // Mostrar info de la cita
    cout << "Atendiendo cita ID: " << cita.id << "\n";
    cout << "Paciente: " << paciente.nombre << " " << paciente.apellido << "\n";
    cout << "Doctor: " << doctor.nombre << " " << doctor.apellido << "\n";
    cout << "Fecha y hora: " << cita.fecha << " " << cita.hora << "\n";

    // Registrar observaciones
    cout << "Ingrese observaciones de la cita: ";
    cin.ignore();
    cin.getline(cita.observaciones, sizeof(cita.observaciones));

    // Registrar historial médico
    HistorialMedico historial{};
    historial.id = hospital.siguienteIDConsulta;
    historial.pacienteID = paciente.id;
    historial.doctorID = doctor.id;
    strncpy(historial.fecha, cita.fecha, sizeof(historial.fecha) - 1);
    strncpy(historial.hora, cita.hora, sizeof(historial.hora) - 1);

    cout << "Ingrese diagnóstico: ";
    cin.getline(historial.diagnostico, sizeof(historial.diagnostico));
    cout << "Ingrese tratamiento: ";
    cin.getline(historial.tratamiento, sizeof(historial.tratamiento));
    cout << "Ingrese medicamentos: ";
    cin.getline(historial.medicamentos, sizeof(historial.medicamentos));

    historial.costo = doctor.costoConsulta;
    historial.siguienteConsultaID = 0;
    historial.eliminado = false;
    historial.fechaRegistro = time(NULL);

    // Guardar historial en archivo
    ofstream archivoHistorial(hospital.historialFile, ios::binary | ios::app);
    if (!archivoHistorial.is_open()) {
        cout << "Error al abrir archivo de historial médico.\n";
        return false;
    }
    archivoHistorial.write(reinterpret_cast<const char*>(&historial), sizeof(HistorialMedico));
    archivoHistorial.close();

    // Actualizar cita como atendida
    cita.atendida = true;
    strncpy(cita.estado, "Atendida", sizeof(cita.estado) - 1);
    cita.consultaID = historial.id;
    cita.fechaModificacion = time(NULL);

    // Guardar cambios en archivo de citas
    fstream archivoCitas(hospital.citasFile, ios::binary | ios::in | ios::out);
    if (!archivoCitas.is_open()) {
        cout << "❌ Error al abrir archivo de citas.\n";
        return false;
    }

    ArchivoHeader headerCitas = leerHeader(hospital.citasFile);
    for (int i = 0; i < headerCitas.cantidadRegistros; i++) {
        long pos = sizeof(ArchivoHeader) + i * sizeof(Cita);
        archivoCitas.seekg(pos);
        Cita temp;
        archivoCitas.read(reinterpret_cast<char*>(&temp), sizeof(Cita));
        if (temp.id == cita.id) {
            archivoCitas.seekp(pos);
            archivoCitas.write(reinterpret_cast<const char*>(&cita), sizeof(Cita));
            break;
        }
    }
    archivoCitas.close();

    // Actualizar IDs y estadísticas del hospital
    hospital.siguienteIDConsulta++;
    hospital.totalConsultasRealizadas++;

    // Actualizar header del historial médico
    ArchivoHeader headerHist = leerHeader(hospital.historialFile);
    headerHist.cantidadRegistros++;
    headerHist.proximoID = historial.id + 1;
    headerHist.registrosActivos++;
    actualizarHeader(hospital.historialFile, headerHist);

    cout << "Cita atendida y historial médico registrado con ID: " << historial.id << "\n";
    return true;
}

Cita buscarCitaPorID(int id) {
    return buscarPorID<Cita>(hospital.citasFile, id);
}

void eliminaeCitaLogico(int id){
    eliminarLogico<Cita>(hospital.citasFile, id);
}

void mostrarDetallesCita(const Cita &c) {
    if (c.id == 0) {
        cout << "Cita no encontrada o ID inválido." << endl;
        return;
    }
    
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║          DETALLES DE LA CITA          ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    cout << " ID: " << c.id << endl;
    cout << " Paciente ID: " << c.pacienteID << endl;
    cout << " Doctor ID: " << c.doctorID << endl;
    cout << " Fecha: " << c.fecha << endl;
    cout << " Hora: " << c.hora << endl;
    cout << " Motivo: " << c.motivo << endl;
    cout << " Estado: " << c.estado << endl;
    cout << " Atendida: " << (c.atendida ? "Sí" : "No") << endl;
    cout << " Consulta ID: " << c.consultaID << endl;

    // Mostrar fechas formateadas
    char fechaStr[20];
    
    strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", localtime(&c.fechaCreacion));
    cout << " Fecha de creación: " << fechaStr << endl;
    
    if (c.fechaModificacion != c.fechaCreacion) {
        strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d %H:%M:%S", localtime(&c.fechaModificacion));
        cout << " Última modificación: " << fechaStr << endl;
    }
}

void buscarYMostrarCita() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║           BUSCAR CITA                  ║\n";
    cout << "╚════════════════════════════════════════╝\n";

    int id = ValidarNumeroEntero("Ingrese el ID de la cita a buscar: ", 1, 10000);
    Cita cita = buscarCitaPorID(id);

    if (cita.id != 0 && !cita.eliminado) {
        mostrarDetallesCita(cita);
    } else {
        cout << "❌ No se encontró ningún cita con ID: " << id << endl;
    }

    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

void listarCitasPendientes() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║        LISTA DE CITAS PENDIENTES       ║\n";
    cout << "╚════════════════════════════════════════╝\n";

    ifstream archivo(hospital.citasFile, ios::binary);
    if (!archivo.is_open()) {
        cout << " Error al abrir archivo de citas.\n";
        return;
    }

    archivo.seekg(sizeof(ArchivoHeader)); // Saltar header
    Cita c;
    int contador = 0;

    cout << left << setw(6) << "ID"
        << setw(12) << "PacienteID"
        << setw(10) << "DoctorID"
        << setw(12) << "Fecha"
        << setw(6) << "Hora"
        << setw(20) << "Motivo"
        << setw(12) << "Estado" << endl;
    cout << string(80, '-') << endl;

    while (archivo.read(reinterpret_cast<char*>(&c), sizeof(Cita))) {
        if (!c.eliminado && !c.atendida) {
            cout << left << setw(6) << c.id
                << setw(12) << c.pacienteID
                << setw(10) << c.doctorID
                << setw(12) << c.fecha
                << setw(6) << c.hora
                << setw(20) << c.motivo
                << setw(12) << c.estado << endl;
            contador++;
        }
    }

    archivo.close();
    cout << string(80, '-') << endl;
    cout << "Total de citas pendientes: " << contador << endl;
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

void listarCitasPorDoctor() {
system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║        LISTA DE CITAS POR DOCTOR       ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    int doctorID = ValidarNumeroEntero("Ingrese ID del doctor: ", 1, 10000);

    Doctor doctor = buscarDoctorPorID(doctorID);
    if (doctor.id == 0 || doctor.eliminado) {
        cout << "❌ Doctor no encontrado.\n";
        cout << "\nPresione ENTER para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    ifstream archivo(hospital.citasFile, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de citas.\n";
        cout << "\nPresione ENTER para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    archivo.seekg(sizeof(ArchivoHeader));
    Cita c;
    bool encontrado = false;

    cout << left << setw(6) << "ID"
        << setw(20) << "Paciente"
        << setw(20) << "Doctor"
        << setw(12) << "Fecha"
        << setw(6) << "Hora"
        << setw(20) << "Motivo"
        << setw(12) << "Estado" << endl;
    cout << string(96, '-') << endl;

    while (archivo.read(reinterpret_cast<char*>(&c), sizeof(Cita))) {
        if (!c.eliminado && c.doctorID == doctorID) {
            Paciente paciente = buscarPacientePorID(c.pacienteID);

            cout << left << setw(6) << c.id
                << setw(20) << (paciente.id != 0 ? string(paciente.nombre) + " " + paciente.apellido : "Desconocido")
                << setw(20) << string(doctor.nombre) + " " + doctor.apellido
                << setw(12) << c.fecha
                << setw(6) << c.hora
                << setw(20) << c.motivo
                << setw(12) << c.estado << endl;

            encontrado = true;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "No se encontraron citas para el doctor " 
            << doctor.nombre << " " << doctor.apellido << " (ID: " << doctorID << ")" << endl;
    } else {
        cout << string(96, '-') << endl;
    }

    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

//-------------------Historiales-------------------

void mostrarCantidadActivos() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   CANTIDAD DE REGISTROS ACTIVOS        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    int doctoresActivos = 0;
    int pacientesActivos = 0;

    // --- Contar Doctores Activos ---
    ifstream fileDoctores(hospital.doctoresFile, ios::binary);
    if (fileDoctores.is_open()) {
        fileDoctores.seekg(sizeof(ArchivoHeader));
        Doctor d;
        while (fileDoctores.read(reinterpret_cast<char*>(&d), sizeof(Doctor))) {
            if (!d.eliminado) doctoresActivos++;
        }
        fileDoctores.close();
    } else {
        cout << "Error al abrir el archivo de doctores." << endl;
    }

    // --- Contar Pacientes Activos ---
    ifstream filePacientes(hospital.pacientesFile, ios::binary);
    if (filePacientes.is_open()) {
        filePacientes.seekg(sizeof(ArchivoHeader));
        Paciente p;
        while (filePacientes.read(reinterpret_cast<char*>(&p), sizeof(Paciente))) {
            if (!p.eliminado) pacientesActivos++;
        }
        filePacientes.close();
    } else {
        cout << "Error al abrir el archivo de pacientes." << endl;
    }

    // --- Mostrar Resultados ---
    cout << "\n📋 Resumen de registros activos:\n";
    cout << "------------------------------------------\n";
    cout << "👨‍⚕️  Doctores activos : " << doctoresActivos << endl;
    cout << "🧑‍🤝‍🧑 Pacientes activos : " << pacientesActivos << endl;
    cout << "------------------------------------------\n";
}

void mostrarAlergiasPaciente() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║        MOSTRAR ALERGIAS PACIENTE       ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    int id = ValidarNumeroEntero("Ingrese el ID del paciente: ", 1, 10000);

    // Buscar paciente en el archivo
    Paciente p = buscarPacientePorID(id);

    if (p.id == 0 || p.eliminado) {
        cout << "❌ No se encontró un paciente activo con ese ID." << endl;
        return;
    }

    cout << "\nInformación del paciente:\n";
    cout << "Nombre completo: " << p.nombre << " " << p.apellido << endl;
    cout << "Edad: " << p.edad << endl;
    cout << "Teléfono: " << p.telefono << endl;

    cout << "\nAlergias registradas: ";
    if (strlen(p.alergias) > 0) {
        cout << p.alergias << endl;
    } else {
        cout << "Ninguna registrada." << endl;
    }

    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
}

void listarCitasPorPaciente() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║     LISTA DE CITAS POR PACIENTE        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    int pacienteID = ValidarNumeroEntero("Ingrese el ID del paciente: ", 1, 10000);

    // Buscar paciente
    Paciente paciente = buscarPacientePorID(pacienteID);
    if (paciente.id == 0 || paciente.eliminado) {
        cout << "Paciente no encontrado o eliminado." << endl;
        return;
    }

    ifstream archivo(hospital.citasFile, ios::binary);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo de citas." << endl;
        return;
    }

    archivo.seekg(sizeof(ArchivoHeader)); // saltar el encabezado

    Cita c;
    bool encontrado = false;

    cout << "\n Citas registradas para: " << paciente.nombre << " " << paciente.apellido << endl;
    cout << left << setw(6) << "ID"
        << setw(12) << "DoctorID"
        << setw(12) << "Fecha"
        << setw(8) << "Hora"
        << setw(20) << "Motivo"
        << setw(12) << "Estado" << endl;
    cout << string(70, '-') << endl;

    while (archivo.read(reinterpret_cast<char*>(&c), sizeof(Cita))) {
        if (!c.eliminado && c.pacienteID == pacienteID) {
            Doctor d = buscarDoctorPorID(c.doctorID);
            string nombreDoctor = (d.id != 0) ? string(d.nombre) + " " + d.apellido : "Desconocido";

            cout << left << setw(6) << c.id
                << setw(12) << nombreDoctor
                << setw(12) << c.fecha
                << setw(8) << c.hora
                << setw(20) << c.motivo
                << setw(12) << c.estado << endl;

            encontrado = true;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "\nNo se encontraron citas para este paciente." << endl;
    } else {
        cout << string(70, '-') << endl;
    }

    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
}

void listarCitasAtendidasPorDoctor() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║     LISTA DE CITAS POR DOCTOR          ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    int doctorID = ValidarNumeroEntero("Ingrese el ID del doctor: ", 1, 10000);

    // Buscar doctor
    Doctor doctor = buscarDoctorPorID(doctorID);
    if (doctor.id == 0 || doctor.eliminado) {
        cout << " Doctor no encontrado o eliminado." << endl;
        return;
    }

    ifstream archivo(hospital.citasFile, ios::binary);
    if (!archivo.is_open()) {
        cout << " Error: No se pudo abrir el archivo de citas." << endl;
        return;
    }

    archivo.seekg(sizeof(ArchivoHeader)); // Saltar header

    Cita c;
    bool encontrado = false;

    cout << "\n Citas registradas para el doctor: " << doctor.nombre << " " << doctor.apellido << endl;
    cout << left << setw(6) << "ID"
        << setw(20) << "Paciente"
        << setw(12) << "Fecha"
        << setw(8) << "Hora"
        << setw(20) << "Motivo"
        << setw(12) << "Estado" << endl;
    cout << string(80, '-') << endl;

    while (archivo.read(reinterpret_cast<char*>(&c), sizeof(Cita))) {
        if (!c.eliminado && c.doctorID == doctorID) {
            Paciente paciente = buscarPacientePorID(c.pacienteID);
            string nombrePaciente = (paciente.id != 0) ? string(paciente.nombre) + " " + paciente.apellido : "Desconocido";

            cout << left << setw(6) << c.id
                << setw(20) << nombrePaciente
                << setw(12) << c.fecha
                << setw(8) << c.hora
                << setw(20) << c.motivo
                << setw(12) << c.estado << endl;

            encontrado = true;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "\n No se encontraron citas para este doctor." << endl;
    } else {
        cout << string(80, '-') << endl;
    }

    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
}

//-----------------Mantenimientos-------------------

void hacerRespaldo() {
    const char* carpetaRespaldo = "RespaldoHospital";

    // Crear carpeta de respaldo si no existe
    if (_mkdir(carpetaRespaldo) == 0) {
        cout << "Carpeta de respaldo creada: " << carpetaRespaldo << endl;
    } else {
        cout << "Carpeta de respaldo ya existe o error al crearla.\n";
    }

    // Archivos a respaldar
    const char* archivos[] = {
        hospital.pacientesFile,
        hospital.doctoresFile,
        hospital.citasFile,
        hospital.historialFile
    };

    char destino[512];

    for (int i = 0; i < 4; i++) {
        // Obtener el nombre del archivo (sin ruta completa)
        const char* nombreArchivo = strrchr(archivos[i], '\\'); // Para Windows
        if (!nombreArchivo) nombreArchivo = archivos[i];
        else nombreArchivo++; // saltar '\'

        snprintf(destino, sizeof(destino), "%s\\%s", carpetaRespaldo, nombreArchivo);

        ifstream src(archivos[i], ios::binary);
        ofstream dst(destino, ios::binary);

        if (!src.is_open() || !dst.is_open()) {
            cout << "❌ Error al respaldar: " << archivos[i] << endl;
            continue;
        }

        dst << src.rdbuf(); // Copiar contenido
        src.close();
        dst.close();

        cout << "Respaldo exitoso: " << destino << endl;
    }
}

bool restaurarDesdeRespaldo(const string& carpetaRespaldo) {
    // Archivos del hospital que vamos a restaurar
    const string archivos[] = {
        "pacientes.dat",
        "doctores.dat",
        "citas.dat",
        "historial.dat"
    };

    bool exito = true;

    for (const string& archivo : archivos) {
        string rutaRespaldo = carpetaRespaldo + "/" + archivo;
        string rutaDestino = archivo;

        ifstream src(rutaRespaldo, ios::binary);
        if (!src.is_open()) {
            cout << " No se pudo abrir el archivo de respaldo: " << rutaRespaldo << endl;
            exito = false;
            continue;
        }

        ofstream dst(rutaDestino, ios::binary | ios::trunc);
        if (!dst.is_open()) {
            cout << " No se pudo restaurar el archivo: " << rutaDestino << endl;
            exito = false;
            src.close();
            continue;
        }

        // Copiar contenido completo
        char buffer[1024];
        while (!src.eof()) {
            src.read(buffer, sizeof(buffer));
            dst.write(buffer, src.gcount());
        }

        src.close();
        dst.close();
        cout << " Restaurado: " << archivo << endl;
    }

    return exito;
}

void mostrarEstadisticasArchivos() {
    system("cls");
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      ESTADÍSTICAS DE USO DE ARCHIVOS   ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    struct ArchivoInfo {
        string nombre;
        char* ruta;
    } archivos[] = {
        {"Pacientes", hospital.pacientesFile},
        {"Doctores", hospital.doctoresFile},
        {"Citas", hospital.citasFile},
        {"Historial médico", hospital.historialFile}
    };

    for (const auto& archivo : archivos) {
        ArchivoHeader header = leerHeader(archivo.ruta);

        cout << "\nArchivo: " << archivo.nombre << endl;
        cout << "Ruta: " << archivo.ruta << endl;
        cout << "Cantidad de registros: " << header.cantidadRegistros << endl;
        cout << "Registros activos: " << header.registrosActivos << endl;
        cout << "Próximo ID disponible: " << header.proximoID << endl;
        cout << string(50, '-') << endl;
    }

    cout << "\nPresione ENTER para continuar...";
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
        cout << "2. Manejo de doctores"  << endl;
        cout << "3. Manejo de citas" << endl;
        cout << "4. Mostrar datos hospital" << endl;
        cout << "5. Historiales" << endl;
        cout << "6. Mantenimiento" << endl;
        cout << "0. Salir" << endl;
        
        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 6);
        
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
                mostrarMenuCitas();
                break;
                
            }
            case 4:{
                mostrarDatosHospital(hospital);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 5:{
                menuDeHistorialesMedicos();
                break;
            }
            case 6:{
                MostrarMenuDeMantenimiento();
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
            case 6:{
                listarDoctorePorEspecialidad();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
        }
    }while(opcion != 0);
}

void mostrarMenuCitas(){
    int opcion = 0;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║        MENU DE GESTIÓN DE CITAS        ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Agregar Cita" << endl;
        cout << "2. Atender Cita" << endl;
        cout << "3. Eliminar Cita" << endl;
        cout << "4. Buscar Cita por ID" << endl;
        cout << "5. Listar Citas pendientes" << endl;
        cout << "6. Listar Citas por Doctor" << endl;
        cout << "0. Regresar al menú principal" << endl;
        
        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 6);
        
        switch(opcion){
            case 1:{
                agregarCita();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 2:{
                atenderCita();
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 3:{
                int idEliminar = ValidarNumeroEntero("Ingrese ID de la cita a eliminar: ", 1, 10000);
                eliminaeCitaLogico(idEliminar);
                cout << "Presione ENTER para continuar...";
                cin.ignore();
                break;
            }
            case 4:{
                buscarYMostrarCita();
                break;
            }
            case 5:{
                listarCitasPendientes();
                break;
            }
            case 6:{
                listarCitasPorDoctor();
                break;
            }
        }
    }while(opcion != 0);
}

void mostrarEstadoDeLosArchivos(){
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      ESTADO DE LOS ARCHIVOS DEL       ║" << endl;
    cout << "║            SISTEMA HOSPITALARIO        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;

    struct ArchivoEstado {
        string nombre;
        string ruta;
    };

    ArchivoEstado archivos[] = {
        {"pacientes", hospital.pacientesFile},
        {"doctores", hospital.doctoresFile},
        {"citas", hospital.citasFile},
        {"historiales", hospital.historialFile}
    };

    cout << left << setw(25) << "Archivo" << setw(20) << "Estado" << endl;
    cout << string(45, '-') << endl;

    for (const auto& a : archivos) {
        ifstream file(a.ruta, ios::binary);
        if (file.is_open()) {
            cout << left << setw(25) << a.nombre << "Abierto correctamente" << endl;
            file.close();
        } else {
            cout << left << setw(25) << a.nombre << "No se pudo abrir" << endl;
        }
    }

    cout << string(45, '-') << endl;
    cout << "Verificación completada.\n";
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void menuDeHistorialesMedicos(){
    int opcion = 0;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║     MENU DE GESTIÓN DE HISTORIALES     ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Doctores y pacientes registrados" << endl;
        cout << "2. Alergias de un paciente" << endl;
        cout << "3. Citas vitas por un paciente" << endl;
        cout << "4. Citas atendidas de un doctor" << endl;
        cout << "0. Regresar al menú principal" << endl;

        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 4);

        switch(opcion){
            case 1:{
                mostrarCantidadActivos();
                cout << "Presione ENTER para continuar..." << endl;
                cin.ignore();
                break;
            }
            case 2:{
                mostrarAlergiasPaciente();
                break;
            }
            case 3:{
                listarCitasPorPaciente();
                break;
            }
            case 4:{
                listarCitasAtendidasPorDoctor();
                break;
            }
        }
    }while (opcion != 0);
}

void MostrarMenuDeMantenimiento(){
    int opcion = 0;
    do{
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║      MENU DE MANTENIMIENTO DEL         ║" << endl;
        cout << "║        SISTEMA HOSPITALARIO            ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        cout << "1. Verificar integridad de archivos" << endl;
        cout << "2. compactar acrchivos (eliminar registros borrados)" << endl;
        cout << "3. hacer un respaldo de datos" << endl;
        cout << "4. restaurar datos desde un respaldo" << endl;
        cout << "5. estadisticas de uso de archivos" << endl;
        cout << "0. Regresar al menú principal" << endl;

        opcion = ValidarNumeroEntero("Seleccione una opcion: ", 0, 5);

        switch(opcion){
            case 1:{
                mostrarEstadoDeLosArchivos();
                cout << "Presione ENTER para continuar..." << endl;
                cin.ignore();
                break;
            }
            case 2:{
                compactarArchivo<Paciente>(hospital.pacientesFile);
                compactarArchivo<Doctor>(hospital.doctoresFile);
                compactarArchivo<Cita>(hospital.citasFile);
                compactarArchivo<HistorialMedico>(hospital.historialFile);

                cout << "Presione ENTER para continuar..." << endl;
                cin.ignore();
                break;
            }
            case 3:{
                hacerRespaldo();
                cout << "Presione ENTER para continuar..." << endl;
                cin.ignore();
                break;
            }
            case 4:{
                string carpeta = "Respaldo";
                if (restaurarDesdeRespaldo(carpeta)) {
                    cout << "Todos los archivos fueron restaurados correctamente.\n";
                } else {
                    cout << "Hubo errores al restaurar algunos archivos.\n";
                }
                cout << "Presione ENTER para continuar..." << endl;
                cin.ignore();
                break;
            }
            case 5:{
                mostrarEstadisticasArchivos();
                cout << "Presione ENTER para continuar..." << endl;
                cin.ignore();
                break;
            }
        }
    }while (opcion != 0);
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

bool guardarCita(const Cita &c) {
    // Leer header
    ArchivoHeader header = leerHeader(hospital.citasFile);

    fstream archivo(hospital.citasFile, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        // Si no existe el archivo, crear con header
        ofstream crear(hospital.citasFile, ios::binary | ios::out);
        if (!crear.is_open()) return false;
        ArchivoHeader h = {0,1,0,1};
        crear.write(reinterpret_cast<const char*>(&h), sizeof(ArchivoHeader));
        crear.close();
        archivo.open(hospital.citasFile, ios::binary | ios::in | ios::out);
    }

    // Posicionar al final según header
    long pos = sizeof(ArchivoHeader) + header.cantidadRegistros * sizeof(Cita);
    archivo.seekp(pos);
    archivo.write(reinterpret_cast<const char*>(&c), sizeof(Cita));
    archivo.close();

    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID = c.id + 1;
    header.registrosActivos++;
    actualizarHeader(hospital.citasFile, header);

    hospital.siguienteIDCita = header.proximoID;
    hospital.totalCitasAgendadas = header.registrosActivos;

    return true;
}
