#ifndef MUSIC_H
#define MUSIC_H

#include <SFML/Audio.hpp>
#include <string>

// Definici�n de un volumen por defecto (puedes ajustarlo seg�n necesites)
static const float DEFAULT_VOLUME = 50.f;

class Music {
private:
    sf::Music music;  // Objeto de SFML para manejar la reproducci�n de audio
public:
    Music();

    // Carga la m�sica desde el archivo indicado. Devuelve true si se carga correctamente.
    bool loadMusic(const std::string& path);

    // Reproduce la m�sica
    void play();

    // Pausa la reproducci�n
    void pause();

    // Detiene la reproducci�n
    void stop();

    // Retorna true si la m�sica se est� reproduciendo actualmente
    bool isPlaying() const;
};

#endif // MUSIC_H