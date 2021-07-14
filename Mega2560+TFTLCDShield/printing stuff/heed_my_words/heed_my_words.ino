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
  tft.setCursor(width/2 - 100,height/2 - 100);

  tft.setTextColor(BLACK);

  tft.setTextSize(3);

  tft.println("Send noods!");
}

void loop() {

}
