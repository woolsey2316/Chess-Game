#ifndef SOUND_H
#define SOUND_H

class Sound
{
  public:
    Sound();
    Sound(std::string filename);
    virtual ~Sound();

  protected:

  private:
    sf::SoundBuffer buffer;
    sf::Sound moveSound;
};

#endif // SOUND_H
