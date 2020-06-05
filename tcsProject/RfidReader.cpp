#include "RfidReader.h"

RfidReader::RfidReader(const short &price) {
  this->price = price;
  mfrc522 = new MFRC522(SSP, RESETP);
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;
}

void RfidReader::set()const {
  SPI.begin();
  mfrc522->PCD_Init();
}

void RfidReader::readCard() {
  if ( !mfrc522->PICC_IsNewCardPresent() ) { // 새로운 카드 접근 검사
    this->resultCode = 50;
    return;
  }  if ( !mfrc522->PICC_ReadCardSerial() ) { // 카드 선택
    this->resultCode = 50;
    return;
  }

  printUID();
  confirm();
  readBlock();

  mfrc522->PICC_HaltA();
  mfrc522->PCD_StopCrypto1();
}

void RfidReader::printUID() {
  Serial.print(" => Card UID\t:");
  printByteArrayInHex(mfrc522->uid.uidByte , mfrc522->uid.size);
  Serial.println(String(" (") + mfrc522->uid.size + " 바이트)");
}

void RfidReader::confirm() {
  //  Serial.println("* Key A를 사용하여 카드 인증을 진행합니다.");
  status = (MFRC522::StatusCode) mfrc522->PCD_Authenticate(
             MFRC522::PICC_CMD_MF_AUTH_KEY_A, TRAILER_BLOCK, &key, &(mfrc522->uid));
  if (status != MFRC522::STATUS_OK) {
    return;
  }
}

void RfidReader::readBlock() {
  status = (MFRC522::StatusCode) mfrc522->MIFARE_Read(BALANCE_BLOCK, buffer, &size);

  if (status != MFRC522::STATUS_OK) {
    this->resultCode = 20;
  }
  else {
    this->tempBalance = getBalanceFromCard();
    writeCard();
  }
}

short RfidReader::getBalanceFromCard() const {
  return (short)buffer[5];
}

void RfidReader::writeCard() {
  if (isGetOff() == 0) { // 승차
    if (false) { // 잔액이 부족할 경우
      this->resultCode = LOW_BALANCE;
      return;
    }
    else { //잔액이 여유 있는 경우
      tempBalance = tempBalance - price;
      buffer[5] = tempBalance;
      buffer[10] = 1;
      this->resultCode = 1;
    }
  }
  else { //하차
    this->resultCode = 2;
    buffer[10] = 0;
  }

  //라이브러리를 활용하여 카드에 데이터를 쓰는 부분
  status = (MFRC522:: StatusCode) mfrc522->MIFARE_Write(BALANCE_BLOCK, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    this->resultCode = 20;
  }
}

byte RfidReader::isGetOff() const { //승하차 기록 확인
  return buffer[10];
}

byte RfidReader::getResult()const {
  return this->resultCode;
}

short RfidReader::getBalance()const {
  return this->tempBalance;
}

void RfidReader::printByteArrayInHex(const byte *buffer , const byte bufferSize) const {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
