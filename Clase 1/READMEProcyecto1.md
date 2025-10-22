
## Proyecto 1

El objetivo del proyecto es crear un sistema de gestión hospitalaria con arias funciones, siendo algunas de las más importantes, agregar pacientes, doctores, agendar citas, entre varias funciones más. 

## Compilacion
cd "C:\Users\sebas\OneDrive\Desktop\prog.2.Ejercicios\Clase 1"
g++ -std=c++11 "proyectoHospital.cpp" -o proyectoHospital.exe

## Structs

### hospital
Es el propietario principal de los arrays dinámicos doctor/paciente/cita.

Sus campos claves serian:
char nombre[], direccion[], telefono[]
Doctor* doctores; int cantidadDoctores, capacidadDoctores
Paciente* pacientes; int cantidadPacientes, capacidadPacientes
Cita* citas; int cantidadCitas, capacidadCitas
contadores: siguienteIdPaciente, siguienteIdDoctor, siguienteIdCita, siguienteIdConsulta

### Paciente
Cada Paciente contiene su propio array historial y su propio array de citas (copias). El programa crea y libera esas estructuras (crearPaciente / eliminarPaciente / destruirHospital).

### doctores

Doctor mantiene arrays de IDs (no libera pacientes). Doctor es administrado por el Hospital; sus arrays int* deben liberarse al eliminar doctor o destruir hospital.

### Citas
Las estructuras Cita viven en hospital->citas (array propietario). Algunas funciones devuelven punteros a esas entradas (&hospital->citas[i]).

### historial medico
Almacena una consulta atendida; vive en paciente->historial (array dinámico propiedad del paciente).

El mas importante de los Structs seria hospital, ya que posee array de doctores, pacientes y citas.

## Funciones principales
Validaciones / utilidades

int compararCadenas(const char* a, const char* b)
Compara dos C-strings; devuelve 0 si son iguales (similar a strcmp con protección nula).

bool verificarCedulaExistente(Hospital* hospital, const char* cedula, bool esDoctor)
Comprueba duplicado de cédula en pacientes o doctores.

template<typename T> void redimensionar(T*& array, int& capacidad)
Dobla la capacidad del array dinámico (new[] / delete[]) y copia elementos.

bool validarFecha(const char* fecha)
Valida formato YYYY-MM-DD y rango de día/mes/año (incluye bisiesto).

bool validarHora(const char* hora)
Valida formato HH:MM y rango de hora/minuto.

int compararFechas(const char* f1, const char* f2)
Wrapper que compara dos fechas (usa compararCadenas).

bool validarCedula(const char* cedula)
Verifica caracteres y longitud permitida.

bool validarEmail(const char* email)
Verifica presencia de '@' y punto tras el '@'.

Hospital (creación / destrucción)

Hospital* crearHospital(const char* nombre, const char* direccion, const char* telefono)
Reserva e inicializa la estructura Hospital y sus arrays principales.

void destruirHospital(Hospital* hospital)
Libera recursivamente toda la memoria asociada (pacientes, doctores, citas y subarrays).

Pacientes

Paciente* crearPaciente(Hospital* hospital, const char* nombre, ...)
Inicializa y añade un paciente al hospital (reserva subarrays: citas, historial, alergias, observaciones).

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula)
Busca y devuelve puntero al paciente por cédula (case-insensitive simplificado).

void buscarPacientePorNombre(Hospital* hospital)
Solicita nombre y muestra coincidencias por substring (case-insensitive).

bool actualizarPaciente(Paciente* p)
Interactiva: actualiza campos del paciente si el usuario ingresa nuevos valores.

bool eliminarPaciente(Hospital* hospital, int id)
Cancela sus citas, lo remueve de doctores, libera sus recursos y compacta el array de pacientes.

void listarPacientes(Hospital* hospital)
Imprime tabla resumida de pacientes.

Historial médico

void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta)
Añade una consulta al historial del paciente, redimensionando si es necesario.

HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad)
Devuelve el puntero al array historial y rellena cantidad.
void mostrarHistorialMedico(Paciente* paciente)
Imprime el historial formateado.

HistorialMedico* obtenerUltimaConsulta(Paciente* paciente)
Devuelve puntero a la última entrada del historial (o nullptr).
Doctores

Doctor* crearDoctor(Hospital* hospital, const char* nombre, ...)
Crea e inicializa un doctor; reserva arrays de pacientesAsignados y citasAgendadas.

Doctor* buscarDoctorPorId(Hospital* hospital, int id)
Busca doctor por id y devuelve puntero o nullptr.

Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad)
Devuelve array dinámico de punteros a Doctor que coinciden; caller debe delete[] el array resultante.

bool asignarPacienteADoctor(Doctor* doctor, int idPaciente)
Añade ID de paciente al doctor (con redimensionado).

bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente)
Remueve ID de paciente del doctor (compacta array).

void listarPacientesDeDoctor(Hospital* hospital, int idDoctor)
Imprime pacientes asignados al doctor.

void listarDoctores(Hospital* hospital)
Lista doctores registrados.

bool eliminarDoctor(Hospital* hospital, int id)
Libera arrays del doctor, lo remueve y compacta el array de doctores.

Citas

Cita* agendarCita(Hospital* hospital, int idPaciente, int idDoctor, const char* fecha, const char* hora, const char* motivo)
Valida paciente/doctor, formato fecha/hora, disponibilidad; crea Cita en hospital->citas (redimensiona si hace falta), añade id a doctor->citasAgendadas y copia la cita en paciente->citas (actualmente copia completa).

bool cancelarCita(Hospital* hospital, int idCita)
Marca la cita como "Cancelada" y atendida=false.

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos)
Verifica estado "Agendada", marca "Atendida", crea HistorialMedico con costo tomado del doctor y lo agrega al historial del paciente.

Cita** obtenerCitas(Hospital* hospital, const char* tipo, int id, int* cantidad)
Devuelve array dinámico de punteros a Cita que coinciden por paciente o doctor; caller debe delete[] el array devuelto. (Cita* dentro apuntan a hospital->citas).

Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad)
Similar a obtenerCitas pero filtra por fecha.
void ListarPacientesPendientes(Hospital* hospital)
Imprime pacientes con citas en estado "Agendada".

bool verificarDisponibilidadDoctor(Hospital* hospital, int idDoctor, const char* fecha, const char* hora)
Retorna true si doctor no tiene otra cita activa en la misma fecha/hora.

Menús / UI

void mostrarDatosHospital(const Hospital* h)
Muestra información básica del hospital.

void mostrarMenuPaciente(Hospital* hospital)
Menú interactivo para gestión de pacientes (usa las funciones de Pacientes/Historial).

void mostrarMenuDoctor(Hospital* hospital)
Menú interactivo para gestión de doctores.

void mostrarMenuDeCitas(Hospital* hospital)
Menú interactivo para gestión de citas.

void mostrarMenuPrincipal(Hospital* hospital)
Menú principal que enlaza a los sub-menús y a la salida.

Main

int main()

Crea Hospital con valores iniciales, llama mostrarMenuPrincipal, luego destruirHospital() antes de terminar.

## Decisiones de diseño

Se utilizaron punteros para un mejor control de la memoria y optimizacion del programas, los array dinamicos tambien perimiten una mejor utilizacion de los datos, tambien permiten redimensionar los arrays para seguir usando ese mismo arreglo.
