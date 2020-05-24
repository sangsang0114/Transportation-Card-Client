#include <Arduino.h>
#include <SoftwareSerial.h>

#define TX 4
#define RX 5

class BluetoothModule {
  public:
    BluetoothModule();
    void sendData(const short &id , const byte &state , const byte balance);
    void getData();
    void focus() const;

  private:
    const SoftwareSerial* HM10;
};
