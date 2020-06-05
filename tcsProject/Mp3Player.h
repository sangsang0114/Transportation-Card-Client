#include "DFPlayer_Mini_Mp3.h"
#define VOL 20

class SoundPlayer {
  public:
    SoundPlayer();
    void playSound(const byte& id) const;
    void set() const;
    void playErrorSound(const byte &code)const;
    void playStop(const byte &code)const;

  private:
    SoftwareSerial* mySerial;
};

SoundPlayer::SoundPlayer() {
  this->mySerial = new SoftwareSerial(4, 5);
}

void SoundPlayer::set()const  {
  mySerial->begin(9600);
  mp3_set_serial((*mySerial));
  delay(1);
  mp3_set_volume(VOL);
}

void SoundPlayer::playErrorSound(const byte &code)const {
  mySerial->listen();
  mp3_play(code);
}

void SoundPlayer::playStop(const byte &code)const {
  mySerial->listen();
  mp3_play(code);
}

void SoundPlayer:: playSound(const byte& id) const {
  mySerial->listen();
  mp3_play(id);
  delay(2500);
}
