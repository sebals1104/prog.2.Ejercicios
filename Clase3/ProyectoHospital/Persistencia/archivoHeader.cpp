#include "archivoHeader.hpp"

// ===== Constructores =====
ArchivoHeader::ArchivoHeader() : cantidadRegistros(0), proximoID(1), registrosActivos(0), version(1) {}

ArchivoHeader::ArchivoHeader(int cantidad, int siguiente, int activos, int ver)
    : cantidadRegistros(cantidad), proximoID(siguiente), registrosActivos(activos), version(ver) {}

// ===== Getters =====
int ArchivoHeader::getCantidadRegistros() const { return cantidadRegistros; }
int ArchivoHeader::getProximoID() const { return proximoID; }
int ArchivoHeader::getRegistrosActivos() const { return registrosActivos; }
int ArchivoHeader::getVersion() const { return version; }

// ===== Setters =====
void ArchivoHeader::setCantidadRegistros(int cantidad) { cantidadRegistros = cantidad; }
void ArchivoHeader::setProximoID(int siguiente) { proximoID = siguiente; }
void ArchivoHeader::setRegistrosActivos(int activos) { registrosActivos = activos; }
void ArchivoHeader::setVersion(int ver) { version = ver; }