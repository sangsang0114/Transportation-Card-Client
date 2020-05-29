#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

#define RESETP 9
#define SSP    10

#define BALANCE_SECTOR 1
#define BALANCE_BLOCK 4 //blockAddr
#define TRAILER_BLOCK 7

#define LOW_BALANCE 3

class RfidReader {
  public:
    RfidReader();
    void readCard();
    void writeCard();
    byte getResult()const;
    bool isGetOff() const;
    void process();
    void printUID();
    void confirm();
    short getBalanceFromCard() const;
    void readBlock();
    void printSectorData();
    void set()const;

  private:
    const MFRC522 *mfrc522;
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);
    byte dataBlock[16];

  private:
    byte resultCode;
    bool isGettingOff;
    short tempBalance;

  private:
    void printByteArrayInHex(const byte *buffer , const byte bufferSize) const;
};
