#include "GestionArchivos.hpp"

// Implementación de las funciones NO-template

bool GestionArchivos::crearArchivoSiNoExiste(const string& rutaArchivo) {
    ifstream lectura(rutaArchivo);
    if (lectura.good()) {
        lectura.close();
        return true;
    }
    lectura.close();

    ofstream escritura(rutaArchivo, ios::binary);
    if (!escritura.is_open()) return false;

    ArchivoHeader header; // Constructor por defecto inicia en 0
    escritura.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    escritura.close();
    return true;
}

ArchivoHeader GestionArchivos::leerHeader(const string& rutaArchivo) {
    ArchivoHeader header;
    ifstream archivo(rutaArchivo, ios::binary);
    if (archivo.is_open()) {
        archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
    }
    return header;
}

bool GestionArchivos::actualizarHeader(const string& rutaArchivo, const ArchivoHeader& header) {
    fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}