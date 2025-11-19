#ifndef GESTION_ARCHIVOS_HPP
#define GESTION_ARCHIVOS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


// CLASE 1: ARCHIVO HEADER


class GestionArchivos; 

class ArchivoHeader {
    friend class GestionArchivos;

private:
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;

public:
    ArchivoHeader() : cantidadRegistros(0), proximoID(1), registrosActivos(0), version(1) {}
    
    int getProximoID() const { return proximoID; }
    int getRegistrosActivos() const { return registrosActivos; }
};

// ==========================================
// CLASE 2: GESTOR DE ARCHIVOS
// ==========================================

class GestionArchivos {
public:
    // --- Métodos Genéricos ---
    
    template <typename T>
    static bool guardarRegistro(const T& registro, const string& rutaArchivo) {
        crearArchivoSiNoExiste(rutaArchivo);
        ArchivoHeader header = leerHeader(rutaArchivo);
        
        fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) return false;

        long posicion = sizeof(ArchivoHeader) + (header.cantidadRegistros * sizeof(T));
        archivo.seekp(posicion, ios::beg);
        archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));

        header.cantidadRegistros++;
        header.proximoID++;
        header.registrosActivos++;

        archivo.seekp(0, ios::beg);
        archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
        return true;
    }

    template <typename T>
    static T buscarPorID(int id, const string& rutaArchivo) {
        T registroVacio = {}; // Inicializa en 0
        ifstream archivo(rutaArchivo, ios::binary);
        if (!archivo.is_open()) return registroVacio;

        ArchivoHeader header;
        archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

        T temp;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            archivo.read(reinterpret_cast<char*>(&temp), sizeof(T));
            if (temp.getId() == id) {
                archivo.close();
                return temp;
            }
        }
        archivo.close();
        return registroVacio;
    }
    
    template <typename T>
    static vector<T> listarTodos(const string& rutaArchivo) {
        vector<T> lista;
        ifstream archivo(rutaArchivo, ios::binary);
        if (!archivo.is_open()) return lista;

        ArchivoHeader header;
        archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

        T temp;
        for (int i = 0; i < header.cantidadRegistros; i++) {
            archivo.read(reinterpret_cast<char*>(&temp), sizeof(T));
            if (!temp.estaEliminado()) {
                lista.push_back(temp);
            }
        }
        archivo.close();
        return lista;
    }
    
    template <typename T>
    static bool actualizarRegistro(const T& registro, const string& rutaArchivo) {
        fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) return false;

        ArchivoHeader header;
        archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

        T temp;
        bool encontrado = false;
        int indice = -1;

        for (int i = 0; i < header.cantidadRegistros; i++) {
            archivo.read(reinterpret_cast<char*>(&temp), sizeof(T));
            if (temp.getId() == registro.getId()) {
                indice = i;
                encontrado = true;
                break;
            }
        }

        if (encontrado) {
            long posicion = sizeof(ArchivoHeader) + (indice * sizeof(T));
            archivo.seekp(posicion, ios::beg);
            archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
        }
        archivo.close();
        return encontrado;
    }

    template <typename T>
    static bool eliminarRegistro(int id, const string& rutaArchivo) {
        T registro = buscarPorID<T>(id, rutaArchivo);
        if (registro.getId() == 0) return false;

        registro.setEliminado(true); // Eliminado lógico
        
        if (actualizarRegistro<T>(registro, rutaArchivo)) {
            ArchivoHeader header = leerHeader(rutaArchivo);
            header.registrosActivos--;
            actualizarHeader(rutaArchivo, header);
            return true;
        }
        return false;
    }

    template <typename T>
    static bool guardarHospitalConfig(const T& hospital, const string& rutaArchivo) {
        // trunc: Borra el contenido anterior para sobrescribir la configuración
        ofstream archivo(rutaArchivo, ios::binary | ios::trunc);
        if (!archivo.is_open()) return false;
        
        archivo.write(reinterpret_cast<const char*>(&hospital), sizeof(T));
        return archivo.good();
    }

    template <typename T>
    static bool cargarHospitalConfig(T& hospital, const string& rutaArchivo) {
        ifstream archivo(rutaArchivo, ios::binary);
        if (!archivo.is_open()) return false;
        
        archivo.read(reinterpret_cast<char*>(&hospital), sizeof(T));
        return archivo.good();
    }

    // --- Métodos Auxiliares (Implementados en .cpp) ---
    static bool crearArchivoSiNoExiste(const string& rutaArchivo);
    static ArchivoHeader leerHeader(const string& rutaArchivo);
    static bool actualizarHeader(const string& rutaArchivo, const ArchivoHeader& header);
};

#endif