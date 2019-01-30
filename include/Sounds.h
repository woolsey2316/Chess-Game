#ifndef SOUNDS_H
#define SOUNDS_H

class Sounds
{
  public:
    Sounds();
    virtual ~Sounds();
    void playNormalMoveSound();
    void playCheckSound();
    void playCheckMateSound();
    void playWooshSound();
    void playCaptureSound();

  private:
    sf::SoundBuffer checkMateSound_buffer;
    sf::SoundBuffer checkSound_buffer;
    sf::SoundBuffer normalMove_buffer;
    sf::SoundBuffer wooshSound_buffer;
    sf::SoundBuffer captureSound_buffer;
    sf::Sound wooshSound;
    sf::Sound checkMateSound;
    sf::Sound checkSound;
    sf::Sound normalMove;
    sf::Sound captureSound;
};

#endif // SOUNDS_H
