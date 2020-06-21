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

  setCardUID();
  confirm();
  readBlock();

  mfrc522->PICC_HaltA();
  mfrc522->PCD_StopCrypto1();
}

void RfidReader::setCardUID() {
  this->UID = mfrc522->uid.uidByte;
}

const byte* RfidReader::getCardUID() const {
  return this->UID;
}

void RfidReader::confirm() {
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

int RfidReader::byteToInt(byte *bf) {
  int balance = 0;
  balance = balance + 10000 * bf[0];
  balance = balance + 1000 * bf[1];
  balance = balance + 100 * bf[2];
  balance = balance + 10 * bf[3];
  balance = balance + 1 * bf[4];
  Serial.println(balance);
  return balance;
}

void RfidReader::IntToByte(const int &n) {
  Serial.println(n);
  buffer[0] = n / 10000; //만의 자리수
  buffer[1] = (n % 10000) / 1000; //천의 자리수
  buffer[2] = (n % 1000) / 100; // 백의 자리 수
  buffer[3] = (n % 100) / 10; // 십의 자리 수
  buffer[4] = (n % 10); // 일의 자리수
}

int RfidReader::getBalanceFromCard() const {
  int bal = byteToInt(buffer);
  return bal;
}

void RfidReader::writeCard() {
  if (isGetOff() == 0) { // 승차
    //if (false) {
    if (tempBalance < price) { // 잔액이 부족할 경우
      this->resultCode = LOW_BALANCE;
      return;
    }
    else { //잔액이 여유 있는 경우
      tempBalance = tempBalance - price;
      /*
        buffer[0] = 1;
        buffer[1] = 5;
        buffer[2] = 2;
        buffer[3] = 5;
        buffer[4] = 0 ;
      */
      IntToByte(tempBalance);
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

void RfidReader::setPrice(const int &price) {
  this->price = price;
}

int RfidReader::getPrice()const {
  return this->price;
}

byte RfidReader::isGetOff() const { //승하차 기록 확인
  return buffer[10];
}

byte RfidReader::getResult()const {
  return this->resultCode;
}

int RfidReader::getBalance()const {
  return this->tempBalance;
}
