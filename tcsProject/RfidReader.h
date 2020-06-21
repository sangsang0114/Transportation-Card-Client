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
    void setPrice(const int &price);
    RfidReader(const short &price);
    void readCard();
    void writeCard();
    byte getResult()const;
    byte isGetOff() const;
    void confirm();
    int getPrice()const;
    int getBalanceFromCard() const;
    void readBlock();
    void set()const;
    int getBalance()const;
    int byteToInt(byte *bf);
    void IntToByte(const int &n);

    const byte* getCardUID()const;
    void setCardUID();
    byte* UID;

  private:
    const MFRC522 *mfrc522;
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;

  private:
    byte buffer[18];
    byte size = sizeof(buffer);
    byte resultCode;
    int price;
    int tempBalance;
};
