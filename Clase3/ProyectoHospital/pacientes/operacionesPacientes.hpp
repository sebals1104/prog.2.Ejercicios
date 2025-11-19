#ifndef OPERACIONES_PACIENTES_HPP
#define OPERACIONES_PACIENTES_HPP

#include "../hospital/hospital.hpp"

// Función principal del menú
void menuPacientes(Hospital& hospital);

// Funciones internas de lógica de negocio
void registrarPaciente(Hospital& hospital);
void modificarPaciente();
void eliminarPaciente(Hospital& hospital); // Necesita hospital para actualizar estadísticas si quisieras (opcional)
void buscarPaciente();
void listarPacientes();

#endif