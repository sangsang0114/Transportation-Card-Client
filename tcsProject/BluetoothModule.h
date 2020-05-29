#include <Arduino.h>
#include <SoftwareSerial.h>

#define TX 6
#define RX 7

class BluetoothModule {
  public:
    BluetoothModule();
    void sendData(const short &id , const byte &state , const byte balance);
    void getData();
    void focus() const;

  private:
    const SoftwareSerial* HM10;
};
