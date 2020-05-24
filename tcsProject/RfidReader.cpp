#include "RfidReader.h"

RfidReader :: RfidReader() {
  mfrc = new MFRC522(SSP, RESETP);
  SPI.begin();
  (*mfrc).PCD_Init();
}

void RfidReader ::readCard() {
  if ( !(*mfrc).PICC_IsNewCardPresent() || !(*mfrc).PICC_ReadCardSerial() ) {
    // 태그 접촉이 되지 않았을때 또는 ID가 읽혀지지 않았을때
    delay(500);
    return;
  }

  Serial.println("Card UID");
  for (byte i = 0; i < 4; i++) {               // 태그의 ID출력하는 반복문.태그의 ID사이즈(4)까지
    Serial.print((*mfrc).uid.uidByte[i]);        // mfrc.uid.uidByte[0] ~ mfrc.uid.uidByte[3]까지 출력
    Serial.print(" ");                        // id 사이의 간격 출력
  }
  Serial.println();
}

void RfidReader::writeCard() {
}
