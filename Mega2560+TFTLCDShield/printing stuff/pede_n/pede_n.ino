#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "TouchScreen.h" // only when you want to use touch screen

#define BLACK 0x0000
#define PINK 0xF81F

MCUFRIEND_kbv tft(A3, A2, A1, A0, A4);

void setup() {
  uint16_t ID = tft.readID();
  tft.begin(ID);

  int width = tft.width();
  int height = tft.height();

  tft.fillScreen(PINK);
  
  tft.setRotation(3);
}

void loop() {
  Serial.println("Entering loop()");
  int width = tft.width();
  for (int j = 0; j < 5; j++) {
    tft.setRotation(3);
      for (int i = 0; i < width; i++) {
        tft.vertScroll(0, width, i);
        tft.fillScreen(BLACK);
        tft.setRotation(i % 3);
        tft.fillScreen(PINK);
    }
    tft.fillScreen(PINK);
  }
  loop();
}
