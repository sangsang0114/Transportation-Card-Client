#include "BluetoothModule.h"

BluetoothModule::BluetoothModule() {
  this->HM10 = new SoftwareSerial(TX, RX);
  HM10->begin(9600);
}

void BluetoothModule::sendData(const short &id , const byte &state , const byte balance) {
  String data = String(id) + " " + String(balance) + " " + String(state);
  if (HM10->available()) {
    for (int i = 0; i < data.length(); i++) {
      HM10->write(data[i]);
    }
  }
}

void BluetoothModule::focus()const {
  HM10->listen();
}

void BluetoothModule::getData() { //정류장 정보 수신
  String data = "";
  if (HM10->available() > 0) {
    data += HM10->readStringUntil('\n');
  }
}
