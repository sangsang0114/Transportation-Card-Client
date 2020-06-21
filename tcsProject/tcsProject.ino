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

#define PRICE 1250

const BluetoothModule btModule;
const SoundPlayer mPlayer;
const RfidReader cardReader(PRICE);
const oledDisplay oDisplay;

byte state;
int tempBalance;
String thisStop;
short stopCode = 11;
byte *cardUID;
char cardUIDbuffer[32];
const byte size = 4;

void setup() {
  Serial.begin(9600);
  cardReader.set();
  mPlayer.set();
  thisStop = "사당역";
}

void loop() {
  int price = cardReader.getPrice();
  cardReader.readCard();
  if (cardReader.getResult() != NOTHING) {
    state = cardReader.getResult();

    cardUID = cardReader.getCardUID();
    array_to_string(cardUID , 4 , cardUIDbuffer);

    tempBalance = cardReader.getBalance();
    switch (state) {
      case LACK_OF_BALANCE:
        oDisplay.display(LACK_OF_BALANCE , price , tempBalance , false);
        mPlayer.playSound(LACK_OF_BALANCE);
        break;

      case GET_ON:
        oDisplay.display(GET_ON, price, tempBalance);
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
    if (state != ERROR) {
      String message = makeMsg(cardUIDbuffer, tempBalance, state, thisStop);
      btModule.sendMsg(message);
    }
  } else {
    oDisplay.display(NOTHING , 0, 0, false);
  }

  //블루투스로부터 정보 수신 : 정류장 정보
  byte existsAnyData = btModule.getData();
  bool isStop = false;
  String stopBackup;
  if (existsAnyData) {
    stopBackup = thisStop;
    thisStop = btModule.getStop();
    if (thisStop.equals("사당역")) {
      stopCode = 11;
      isStop = true;
    } else if (thisStop.equals("정릉동")) {
      stopCode = 12;
      isStop = true;
    } else if (thisStop.equals("혜화동")) {
      stopCode = 13;
      isStop = true;
    } else if (thisStop.equals("신금호역")) {
      stopCode = 14;
      isStop = true;
    } else if (thisStop.equals("연서시장")) {
      stopCode = 15;
      isStop = true;
    } else if (thisStop.equals("이촌동")) {
      stopCode = 16;
      isStop = true;
    } else if (thisStop.equals("서정마을")) {
      stopCode = 17;
      isStop = true;
    } else if (thisStop.equals("구파발")) {
      stopCode = 18;
      isStop = true;
    } else if (thisStop.equals("상암동")) {
      stopCode = 19;
      isStop = true;
    }

    else if (thisStop.equals("RED_BUS")) {
      cardReader.setPrice(2800);
      thisStop = stopBackup;
    } else if (thisStop.equals("BLUE_BUS")) {
      cardReader.setPrice(2200);
      thisStop = stopBackup;
    } else if (thisStop.equals("GREEN_BUS")) {
      cardReader.setPrice(1250);
      thisStop = stopBackup;
    }

    if (isStop) {
      oDisplay.displayStop(stopCode);
      mPlayer.playStop(stopCode);
      delay(5500);
    }
  }
}

String makeMsg(char* UID , const int &balance , const byte &state , const String &thisStop) {
  String msg = "";
  String msg2;
  String cardID = String(UID);

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

  msg =  cardID + String(",") + balance + String(",") + msg2 + String(",") + thisStop + String("!");
  return msg;
}

void array_to_string(byte array[], const short &len, char buffer[])
{
  for (short i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
