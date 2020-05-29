//#include "BluetoothModule.h"
#include "Mp3Player.h"
#include "RfidReader.h"
#include "oledDisplay.h"

#define OK              0
#define GET_ON          1
#define GET_OFF         2
#define LACK_OF_BALANCE 3
#define FAIL_TO_PROCESS 4
#define NOTHING         50

//const BluetoothModule btModule;
const SoundPlayer mPlayer;
const RfidReader cardReader;
const oledDisplay oDisplay;

byte state;
void setup() {
  Serial.begin(9600);
  cardReader.set();
  mPlayer.set();
}

void loop() {
  cardReader.readCard();

  if (cardReader.getResult() != NOTHING) {
    state = cardReader.getResult();
    if (state == LACK_OF_BALANCE) {
      oDisplay.display(LACK_OF_BALANCE);
      mPlayer.playSound(LACK_OF_BALANCE);
    } else if (state == GET_ON) {
      mPlayer.playSound(GET_ON);
    } else if (state == GET_OFF) {
      mPlayer.playSound(GET_OFF);
    }
  } else {
    oDisplay.display(2);
  }

  delay(1000);
  //  btModule.sendData(1, 2, 3);

  // btModule,getData();
}
