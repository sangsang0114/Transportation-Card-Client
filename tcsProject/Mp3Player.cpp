#include "DFPlayer_Mini_Mp3.h"
#include "Mp3Player.h"

SoundPlayer::SoundPlayer() {
  
}

void SoundPlayer:: playSound(const byte& id) const {
  mp3_play(id);
  delay(2000);
}
