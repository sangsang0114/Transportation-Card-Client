#include "DFPlayer_Mini_Mp3.h"
#define VOL 20
class SoundPlayer {
  public:
    SoundPlayer();
    void playSound(const byte& id) const;
    void set();

  private:
    SoftwareSerial* mySerial;
};

SoundPlayer::SoundPlayer() {
  this->mySerial = new SoftwareSerial(4, 5);
}


void SoundPlayer::set() {
  Serial.println("TEST");
  mySerial->begin(9600);
  mp3_set_serial((*mySerial));
  delay(1);
  mp3_set_volume(VOL);
}

void SoundPlayer:: playSound(const byte& id) const {
  mp3_play(id);
  delay(3000);
}
