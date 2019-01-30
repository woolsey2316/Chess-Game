#include "Sounds.h"

Sounds::Sounds()
{
  sf::SoundBuffer checkMateSound_buffer;
    if (!checkMateSound_buffer.loadFromFile("sounds/CheckMateSound.wav")) {
        return -1;
    }
    checkMateSound.setBuffer(checkMateSound_buffer);

  sf::SoundBuffer checkSound_buffer;
    if (!checkSound_buffer.loadFromFile("sounds/CheckSound.wav")) {
        return -1;
    }
    checkSound.setBuffer(checkSound_buffer);

  sf::SoundBuffer normalMove_buffer;
    if (!normalMove_buffer.loadFromFile("sounds/NormalMove.wav")) {
        return -1;
    }
    normalMove.setBuffer(normalMove_buffer);

  sf::SoundBuffer wooshSound_buffer;
    if (!wooshSound_buffer.loadFromFile("sounds/Woosh.flac")) {
        return -1;
    }
    wooshSound.setBuffer(wooshSound_buffer);
}

Sounds::playNormalMoveSound() {
  normalMove.play();

}

Sounds::playWooshSound() {
  wooshSound.play();

}

Sounds::playCaptureSound() {
  captureMove.play();

}

Sounds::playCheckSound() {
  checkSound.play();

}

Sounds::playCheckMateSound() {
  checkMateSound.play();

}
