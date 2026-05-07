#ifndef CANCION_H
#define CANCION_H

#include <string>

class Cancion {
    private:
    int id;
    std::string titulo;
    std::string artista;
    std::string album;
    int año;
    int duracionSegundos;
    std::string rutaArchivo;

    public:
    Cancion();
    Cancion(int id, std::string titulo, std::string artista, std::string album, int año, int duracionSegundos, std::string rutaArchivo);

    int getId() const;
    std::string getTitulo() const;
    std::string getArtista() const;
    std::string getAlbum() const;
    int getAño() const;
    int getDuracionSegundos() const;
    std::string getRutaArchivo() const;
    
    std::string getStringMostrar() const;
};

#endif
