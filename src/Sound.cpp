#include "Sound.h"

Sound::Sound(std::string filename)
{
  sf::SoundBuffer buffer;
    if (!CheckMateSound_buffer.loadFromFile(filename)) {
        return -1;
    }
    Sound.setBuffer(moveSound);
}

Sounds::play() {
  chessMove.play();

}

Sounds::playCheckSound() {
  CheckSound.play();

}

Sounds::playCheckMateSound() {
  CheckMateSound.play();

}

