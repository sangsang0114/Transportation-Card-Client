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

void oledDisplay::display(const byte &code , const short &price , const short &balance, const byte &valid) {
  u8g->firstPage();
  char buf[9];
  do {
    if (valid == true) {
      u8g->setFont(u8g_font_unifont);
      sprintf(buf, "%d", price);
      u8g->drawStr(0, 20, "OK-->");
      u8g->drawStr(20, 20, strs[code]);
      u8g->drawStr(0, 40, "price : " );
      u8g->drawStr(80, 40, buf);

      sprintf(buf, "%d", balance);
      u8g->drawStr(0, 60, "balance : " );
      u8g->drawStr(80, 60, buf);
    } else {
      if (code == 50) {
        u8g->setFont(u8g_font_courB18);
        u8g->drawStr(0, 30, "TAG HERE");
      } else {
        u8g->setFont(u8g_font_unifont);
        u8g->drawStr(0, 20, "X ");
        u8g->drawStr(0, 40, strs[code]);
        sprintf(buf, "%d", balance);
        u8g->drawStr(0, 60, "balance : " );
        u8g->drawStr(80, 60, buf);
      }
    }
  } while (u8g->nextPage());
}

void oledDisplay::displayStop(const byte &code) {
  u8g->firstPage();
  u8g->setFont(u8g_font_unifont);
  do {
    if (code == 11) {
      u8g->drawStr(0, 20, "This Stop is");
      u8g->drawStr(0, 40, "Sadang Station");
    } else if (code == 12) {
      u8g->drawStr(0, 20, "This Stop is");
      u8g->drawStr(0, 40, "Jeongnenug-Dong");
    }
  } while (u8g->nextPage());
}

void oledDisplay::displayError(const byte &code) {
  u8g->firstPage();
  u8g->setFont(u8g_font_unifont);
  do {
    if (code == 20) {
      u8g->drawStr(0, 20, "Give me ");
      u8g->drawStr(0, 40, "your card");
      u8g->drawStr(0, 60, "again.");
    }
  } while (u8g->nextPage());
}
