#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 200
#define MAXPRESSURE 1000
const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button bot_btn, bot2_btn, bot3_btn, bot4_btn, bot5_btn;

uint16_t color = 0xFFFF;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

/////////////////////////////////////////////////////////////

const int size = 50;

const int width = tft.width(), height = tft.height();

int pos_x = width / 2 + size;
int pos_y = height / 2 + size;

void setup(void) {
////////////////////////////////////////////////////////////////////////////////////////
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);
////////////////////////////////////////////////////////////////////////////////////////

  bot_btn.initButton   (&tft, size + 20, size - 20, size, size, MAGENTA, BLACK, WHITE, "M", 1);
  bot_btn.drawButton(false);

  bot2_btn.initButton   (&tft, size * 2 + 20, size - 20, size, size, MAGENTA, BLACK, WHITE, "R", 1);
  bot2_btn.drawButton(false);

  bot3_btn.initButton   (&tft, size * 3 + 20, size - 20, size, size, MAGENTA, BLACK, WHITE, "G", 1);
  bot3_btn.drawButton(false);

  bot4_btn.initButton   (&tft, size * 4 + 20, size - 20, size, size, MAGENTA, BLACK, WHITE, "B", 1);
  bot4_btn.drawButton(false);

  bot5_btn.initButton   (&tft, 20, size - 20, size, size, MAGENTA, WHITE, BLACK, "Clear", 1);
  bot5_btn.drawButton(false);
}

void loop(void) {
    bool down = Touch_getXY();

    tft.setCursor(pos_x,pos_y);

    bot_btn.press  (down && bot_btn.contains(width - pixel_x, pixel_y));
    bot2_btn.press (down && bot2_btn.contains(width - pixel_x, pixel_y));
    bot3_btn.press (down && bot3_btn.contains(width - pixel_x, pixel_y));
    bot4_btn.press (down && bot4_btn.contains(width - pixel_x, pixel_y));
    bot5_btn.press (down && bot5_btn.contains(width - pixel_x, pixel_y));

    if (bot_btn.justReleased()) {
      bot_btn.drawButton(true);
      bot2_btn.drawButton(false);
      bot3_btn.drawButton(false);
      bot4_btn.drawButton(false);
    }
        
    if (bot2_btn.justReleased()) {
      bot_btn.drawButton(false);
      bot2_btn.drawButton(true);
      bot3_btn.drawButton(false);
      bot4_btn.drawButton(false);
    }


    if (bot3_btn.justReleased()) {
      bot_btn.drawButton(false);
      bot2_btn.drawButton(false);
      bot3_btn.drawButton(true);
      bot4_btn.drawButton(false);
    }
        
    if (bot4_btn.justReleased()) {
      bot_btn.drawButton(false);
      bot2_btn.drawButton(false);
      bot3_btn.drawButton(false);
      bot4_btn.drawButton(true);
    }

    if (bot_btn.justPressed())
        color = MAGENTA;
    if (bot2_btn.justPressed())
        color = RED;
    if (bot3_btn.justPressed())
        color = GREEN;
    if (bot4_btn.justPressed())
        color = BLUE;
    if (bot5_btn.justPressed()) {
        tft.setCursor(0,0);
        tft.fillRect(0, size + 5, width, height, BLACK);
    }
    tft.drawPixel(width - pixel_x, pixel_y, color);
}
#endif
