#ifndef MUSIC_H
#define MUSIC_H

#include <SFML/Audio.hpp>
#include <string>

// Definición de un volumen por defecto (puedes ajustarlo según necesites)
static const float DEFAULT_VOLUME = 50.f;

class Music {
private:
    sf::Music music;  // Objeto de SFML para manejar la reproducción de audio
public:
    Music();

    // Carga la música desde el archivo indicado. Devuelve true si se carga correctamente.
    bool loadMusic(const std::string& path);

    // Reproduce la música
    void play();

    // Pausa la reproducción
    void pause();

    // Detiene la reproducción
    void stop();

    // Retorna true si la música se está reproduciendo actualmente
    bool isPlaying() const;
};

#endif // MUSIC_H