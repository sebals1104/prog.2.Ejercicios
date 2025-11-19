#ifndef OPERACIONES_HISTORIAL_HPP
#define OPERACIONES_HISTORIAL_HPP

#include "../hospital/hospital.hpp"

// Función principal del menú
void menuHistorial(Hospital& hospital);

// Funciones de los reportes
void mostrarEstadisticasRegistros();
void mostrarAlergiasPaciente();
void listarCitasPorPaciente();
void listarCitasAtendidasPorDoctor();

#endif