#include <Arduino.h>
#include <U8glib.h>

class oledDisplay {
  public:
    oledDisplay();
    void set()const;
    void display(const byte &code);
  private:
    const U8GLIB_SSD1306_128X64 *u8g;
    const char* strs[4] = {"on", "off", "nothing" , "lack"};
};
