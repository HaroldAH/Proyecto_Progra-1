#include "Music.h"
#include <iostream>

Music::Music() {
    music.setVolume(DEFAULT_VOLUME);
}

bool Music::loadMusic(const std::string& path) {
    if (!music.openFromFile(path)) {
        std::cout << "Error al cargar la música" << std::endl;
        return false;
    }
    music.setLoop(true);
    return true;
}

void Music::play() {
    music.play();
}

void Music::pause() {
    music.pause();
}

void Music::stop() {
    music.stop();
}

bool Music::isPlaying() const {
    return music.getStatus() == sf::Music::Playing;
}