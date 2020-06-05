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
    RfidReader(const short &price);
    void readCard();
    void writeCard();
    byte getResult()const;
    byte isGetOff() const;
    void printUID();
    void confirm();
    short getBalanceFromCard() const;
    void readBlock();
    void set()const;
    short getBalance()const;

  private:
    const MFRC522 *mfrc522;
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;

  private:
    byte buffer[18];
    byte size = sizeof(buffer);

  private:
    byte resultCode;
    short price;
    short tempBalance;

  private:
    void printByteArrayInHex(const byte *buffer , const byte bufferSize) const;
};
