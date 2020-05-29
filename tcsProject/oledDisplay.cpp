#include "oledDisplay.h"

oledDisplay::oledDisplay() {
  u8g = new U8GLIB_SSD1306_128X64 (U8G_I2C_OPT_DEV_0 |
                                   U8G_I2C_OPT_NO_ACK |
                                   U8G_I2C_OPT_FAST);
  set();
}

void oledDisplay::set()const {
  u8g->setFont(u8g_font_unifont);
  u8g->setColorIndex(1);
}

void oledDisplay::display(const byte &code) {
  u8g->firstPage();
  do {
    switch (code) {
      case 1:
      case 2:
      case 3:
      default:
    }
    u8g->drawStr(0, 20, strs[code]);
  } while (u8g->nextPage());
}
/*
  void oledDisplay::display(const byte &code , const short &price , const byte &balance) {
  u8g->firstPage();

  do {
    switch (code) {
      case 1: //승차
      drawStr
        break;

      case 2:
        break;

      case 3:
        break;

      case 4:
        break;
    }
  } while (u8g->nextPage();
  }
*/
