#ifndef OPERACIONES_CITAS_HPP
#define OPERACIONES_CITAS_HPP

#include "../hospital/hospital.hpp"

// Menú principal
void menuCitas(Hospital& hospital);

// Lógica de negocio
void agendarCita(Hospital& hospital);
void atenderCita();
void cancelarCita();
void buscarCitaPorID();
void listarCitas();
void listarCitasPendientes();

#endif