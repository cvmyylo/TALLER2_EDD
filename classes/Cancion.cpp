#include "Cancion.h"

Cancion::Cancion() {
    this->id = 0;
    this->titulo = "";
    this->artista = "";
    this->album = "";
    this->ano = 0;
    this->duracionSegundos = 0;
    this->rutaArchivo = "";
    this->cantidadReproducciones = 0; 
}

Cancion::Cancion(int id, std::string titulo, std::string artista, std::string album, int ano, int duracionSegundos, std::string rutaArchivo) {
    this->id = id;
    this->titulo = titulo;
    this->artista = artista;
    this->album = album;
    this->ano = ano;
    this->duracionSegundos = duracionSegundos;
    this->rutaArchivo = rutaArchivo;
    this->cantidadReproducciones = 0; 
}

int Cancion::getId() const { return id; }
std::string Cancion::getTitulo() const { return titulo; }
std::string Cancion::getArtista() const { return artista; }
std::string Cancion::getAlbum() const { return album; }
int Cancion::getAno() const { return ano; }
int Cancion::getDuracionSegundos() const { return duracionSegundos; }
std::string Cancion::getRutaArchivo() const { return rutaArchivo; }

int Cancion::getCantidadReproducciones() const { return cantidadReproducciones; }
void Cancion::incrementarReproduccion() { cantidadReproducciones++; }
void Cancion::setCantidadReproducciones(int cant) { cantidadReproducciones = cant; }
std::string Cancion::getStringMostrar() const {
    return titulo + " - " + artista;
}