#include "Cancion.h"

Cancion::Cancion() : id(0), titulo(""), artista(""), album(""), año(0), duracionSegundos(0), rutaArchivo("") {}

Cancion::Cancion(int id, std::string titulo, std::string artista, std::string album, int año, int duracionSegundos, std::string rutaArchivo)
    : id(id), titulo(titulo), artista(artista), album(album), año(año), duracionSegundos(duracionSegundos), rutaArchivo(rutaArchivo) {}

int Cancion::getId() const { return id; }
std::string Cancion::getTitulo() const { return titulo; }
std::string Cancion::getArtista() const { return artista; }
std::string Cancion::getAlbum() const { return album; }
int Cancion::getAño() const { return año; }
int Cancion::getDuracionSegundos() const { return duracionSegundos; }
std::string Cancion::getRutaArchivo() const { return rutaArchivo; }

std::string Cancion::getStringMostrar() const {
    return titulo + " - " + artista;
}