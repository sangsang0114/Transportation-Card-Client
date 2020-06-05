#include <Arduino.h>
#include <U8glib.h>

class oledDisplay {
  public:
    oledDisplay();
    void display(const byte &code , const short &balance , const short &price , const byte &valid = true);
    void displayStop(const byte &code);
    void displayError(const byte &code);

  private:
    void set()const;
    const U8GLIB_SSD1306_128X64 *u8g;
    const char* strs[4] = {"", "GET ON", "GET OFF" , "LOW_BALANCE"};
};
