#ifndef OPERACIONES_DOCTORES_HPP
#define OPERACIONES_DOCTORES_HPP

#include "../hospital/hospital.hpp"

// Función principal del menú
void menuDoctores(Hospital& hospital);

// Lógica de negocio
void registrarDoctor(Hospital& hospital);
void modificarDoctor();
void eliminarDoctor();
void buscarDoctorPorID();
void listarDoctores();

#endif