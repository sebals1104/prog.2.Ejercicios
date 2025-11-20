#  Sistema de Gestión Hospitalaria (Versión POO v3.0)

Este proyecto es una evolución de un sistema de gestión hospitalaria previo, migrado completamente a una **Arquitectura Modular y Orientada a Objetos (POO)** en C++.

El sistema permite la administración eficiente de pacientes, doctores, citas médicas e historiales, asegurando la persistencia de datos mediante archivos binarios y garantizando la integridad de la información a través de validaciones estrictas.

---

## Características Principales

### 1. Gestión de Entidades
* **Pacientes:** Registro, modificación, eliminación (lógica) y búsqueda por ID. Incluye validaciones de cédula, correo electrónico y tipos de sangre.
* **Doctores:** Gestión de personal médico con selección de especialidades predefinidas.
* **Citas:** Agendamiento con validación de **integridad referencial** (verifica que el paciente y el doctor existan y estén activos). Uso de **Horario Militar (24h)**.
* **Historial Médico:** Generación de reportes cruzados y estadísticas de atención.

### 2. Persistencia de Datos
* Uso de **Archivos Binarios (.bin)** para almacenamiento eficiente.
* Implementación de una clase `GestionArchivos` genérica (Templates) que maneja operaciones CRUD para cualquier entidad.
* Manejo de **Headers de Archivo** para control de IDs autoincrementales y conteo de registros activos.

### 3. Mantenimiento y Seguridad
* **Validaciones Robustas:** Entradas blindadas contra errores de tipo (letras en campos numéricos, formatos de fecha/hora inválidos).
* **Respaldo y Restauración:** Capacidad de generar copias de seguridad (`.bak`) y restaurar el sistema a un estado anterior.
* **Compactación:** Herramienta para eliminar físicamente los registros marcados como borrados y optimizar el espacio en disco.

---

## Estructura del Proyecto

El código está organizado por módulos de dominio para facilitar el mantenimiento:

```text
ProyectoHospital/
│
├── binarios/                 # Almacenamiento de datos (.bin)
├── respaldos/                # Copias de seguridad (.bak)
│
├── hospital/                 # Configuración global y contadores
├── pacientes/                # Clase Paciente y lógica de negocio
├── doctores/                 # Clase Doctor y lógica de negocio
├── citas/                    # Clase Cita y lógica de negocio
├── historial/                # Operaciones de reportes
├── mantenimiento/            # Herramientas de respaldo y compactación
│
├── persistencia/             # Motor de archivos (GestionArchivos)
├── utilidades/               # Validaciones y formatos
│
├── main/                     # Punto de entrada
    └── main.cpp

# Compilacion

g++ main/main.cpp hospital/hospital.cpp persistencia/GestionArchivos.cpp utilidades/Validaciones.cpp utilidades/formato.cpp pacientes/pacientes.cpp pacientes/operacionesPacientes.cpp doctores/doctores.cpp doctores/operacionesDoctores.cpp citas/cita.cpp citas/operacionesCitas.cpp historialMedico/historialMedico.cpp historialMedico/operacionesHistorial.cpp mantenimiento/operacionesMantenimiento.cpp -I. -o sistema_hospital

despues 

.\sistema_hospital