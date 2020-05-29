#include "RfidReader.h"

RfidReader::RfidReader() {
  mfrc522 = new MFRC522(SSP, RESETP);
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
}

void RfidReader::set()const {
  Serial.println("OK!");
  SPI.begin();
  mfrc522->PCD_Init();
}

void RfidReader::readCard() {
  short balance = 100;
  if ( !mfrc522->PICC_IsNewCardPresent() ) { // 새로운 카드 접근 검사
    this->resultCode = 50;
    return;
  }  if ( !mfrc522->PICC_ReadCardSerial() ) { // 카드 선택
    this->resultCode = 50;
    return;
  }
  printUID();
  readBlock();
  this->tempBalance = getBalanceFromCard();
  Serial.println(this->tempBalance);

  mfrc522->PICC_HaltA();
  mfrc522->PCD_StopCrypto1();
}

void RfidReader::printUID() {
  Serial.println("* 새로운 카드가 발견되었습니다.");
  Serial.print(" => Card UID\t:");
  printByteArrayInHex(mfrc522->uid.uidByte , mfrc522->uid.size);
  Serial.println(String(" (") + mfrc522->uid.size + " 바이트)");
}

void RfidReader::confirm() {}

void RfidReader::printSectorData() {}

void RfidReader::readBlock() {
  status = (MFRC522::StatusCode) mfrc522->MIFARE_Read(BALANCE_BLOCK, buffer, &size);

  if (status != MFRC522::STATUS_OK) {
    Serial.print(" => 데이터 읽기에 실패했습니다 : ");
    Serial.println(mfrc522->GetStatusCodeName(status));
  }
  else {
    Serial.print(String(" => 블록 ") + BALANCE_BLOCK + "번 데이터 : ");
    printByteArrayInHex(buffer , 16);
    Serial.println();
    writeCard();
  }
}

short RfidReader::getBalanceFromCard() const {
  Serial.print("잔액을 알려드리겠습니다");
  return buffer[5];
}

void RfidReader::writeCard() {
  //기본적인 잔액 차감 부분
  const short price = 50;
  if (tempBalance < price) {
    Serial.println("잔액이 부족합니다");
    this->resultCode = LOW_BALANCE;
    return;
  } else {
    Serial.println("잔액을 차감합니다");
    tempBalance = tempBalance - price;
    buffer[5] =  tempBalance;
    this->resultCode = 1;
  }

  //라이브러리를 활용하여 카드에 데이터를 쓰는 부분
  status = (MFRC522:: StatusCode) mfrc522->MIFARE_Write(BALANCE_BLOCK, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.println("데이터 쓰기에 실패 하였습니다");
    Serial.println(mfrc522->GetStatusCodeName(status));
  } else {
    Serial.println("데이터 쓰기에 성공하였습니다");
  }
}

byte RfidReader::getResult()const {
  return this->resultCode;
}

bool RfidReader::isGetOff() const {
  return this->isGettingOff;
}

void RfidReader::printByteArrayInHex(const byte *buffer , const byte bufferSize) const {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
