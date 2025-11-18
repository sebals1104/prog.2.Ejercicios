#ifndef ARCHIVOHEADER_HPP
#define ARCHIVOHEADER_HPP

class ArchivoHeader {
private:
    int cantidadRegistros;   // Total de registros escritos en el archivo
    int proximoID;           // Siguiente ID disponible
    int registrosActivos;    // Cantidad de registros no eliminados
    int version;             // Versión del archivo

public:
    // ===== Constructores =====
    ArchivoHeader();  // Constructor por defecto
    ArchivoHeader(int cantidad, int siguiente, int activos, int ver);

    // ===== Getters =====
    int getCantidadRegistros() const;
    int getProximoID() const;
    int getRegistrosActivos() const;
    int getVersion() const;

    // ===== Setters =====
    void setCantidadRegistros(int cantidad);
    void setProximoID(int siguiente);
    void setRegistrosActivos(int activos);
    void setVersion(int ver);
};

#endif // ARCHIVOHEADER_HPP