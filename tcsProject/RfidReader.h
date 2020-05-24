#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

#define RESETP 9
#define SSP    10

class RfidReader {
  public:
    RfidReader();
    void readCard();
    void writeCard();

  private:
    const MFRC522 *mfrc;
};
