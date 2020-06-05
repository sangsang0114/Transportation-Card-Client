#include "BluetoothModule.h"
#include "Mp3Player.h"
#include "RfidReader.h"
#include "oledDisplay.h"

#define OK              0
#define GET_ON          1
#define GET_OFF         2
#define LACK_OF_BALANCE 3
#define FAIL_TO_PROCESS 4
#define TRANSFER        11
#define ERROR           20
#define NOTHING         50

#define PRICE 15

const BluetoothModule btModule;
const SoundPlayer mPlayer;
const RfidReader cardReader(PRICE);
const oledDisplay oDisplay;

byte state;
short tempBalance;
String thisStop;
short stopCode;

void setup() {
  Serial.begin(9600);
  cardReader.set();
  mPlayer.set();
  thisStop = "사당역";
}

void loop() {
  cardReader.readCard();

  if (cardReader.getResult() != NOTHING) {
    state = cardReader.getResult();
    tempBalance = cardReader.getBalance();
    switch (state) {
      case LACK_OF_BALANCE:
        oDisplay.display(LACK_OF_BALANCE , PRICE , tempBalance , false);
        mPlayer.playSound(LACK_OF_BALANCE);
        break;

      case GET_ON:
        oDisplay.display(GET_ON, PRICE , tempBalance);
        mPlayer.playSound(GET_ON);
        break;

      case GET_OFF:
        oDisplay.display(GET_OFF, 0, tempBalance);
        mPlayer.playSound(GET_OFF);
        break;

      case ERROR:
        oDisplay.displayError(ERROR);
        mPlayer.playErrorSound(ERROR);
        delay(3000);
        break;
    }
    String message = makeMsg(tempBalance, state, thisStop);
    btModule.sendMsg(message);
  } else {
    oDisplay.display(NOTHING , 0, 0, false);
  }

  //블루투스로부터 정보 수신 : 정류장 정보
  byte existsAnyData = btModule.getData();
  if (existsAnyData) {
    thisStop = btModule.getStop();
    Serial.println(thisStop);

    if (thisStop.equals("사당역")) {
      oDisplay.displayStop(11);
      mPlayer.playStop(11);
      delay(5000);
    } else if (thisStop.equals("정릉동")) {
      oDisplay.displayStop(12);
      mPlayer.playStop(12);
      delay(5000);
    }
  }
}
String makeMsg(const short & balance , const byte & state , const String & thisStop) {
  String msg = "";
  String msg2;
  short cardID = random(1000, 2000);

  switch (state) {
    case LACK_OF_BALANCE:
      msg2 = "잔액부족 ";
      break;

    case GET_ON:
      msg2 = "승차 ";
      break;

    case GET_OFF:
      msg2 = "하차 ";
      break;

    case ERROR:
      break;
  }

  msg = String("카드번호 : " ) + cardID + String(",잔액 : ") + balance + String(",처리결과 : ") + msg2 + String(",정류장 : ") + thisStop + String("\n");
  return msg;
}
