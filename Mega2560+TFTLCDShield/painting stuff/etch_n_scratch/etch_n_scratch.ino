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

Adafruit_GFX_Button top_btn, bot_btn, left_btn, right_btn;

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

  //nitButton (Adafruit_GFX *gfx, int x, int y, int w, int h, int outline, int fill, int textcolor, char *label, int textsize)

  top_btn.initButton   (&tft, size * 2 + 20, size * 2, size, size, MAGENTA, WHITE, BLACK, "v", 1);
  top_btn.drawButton(false);

  bot_btn.initButton   (&tft, size * 2 + 20, size - 20, size, size, MAGENTA, WHITE, BLACK, "^", 1);
  bot_btn.drawButton(false);

  right_btn.initButton (&tft, width - size, size - 20, size, size, MAGENTA, WHITE, BLACK, ">", 1);
  right_btn.drawButton(false);

  left_btn.initButton  (&tft, size, size - 20, size, size, MAGENTA, WHITE, BLACK, "<", 1);
  left_btn.drawButton(false);
}

void loop(void) {
    bool down = Touch_getXY();

    top_btn.press  (down && top_btn.contains(pixel_x, pixel_y));
    bot_btn.press  (down && bot_btn.contains(pixel_x, pixel_y));
    right_btn.press(down && right_btn.contains(pixel_x, pixel_y));
    left_btn.press (down && left_btn.contains(pixel_x, pixel_y));

    if (top_btn.justReleased())
        top_btn.drawButton();
        
    if (bot_btn.justReleased())
        bot_btn.drawButton();

    if (right_btn.justReleased())
        left_btn.drawButton();
        
    if (left_btn.justReleased())
        right_btn.drawButton();
    
    if (top_btn.justPressed()) {
        top_btn.drawButton(true);
        if ( pos_y + 10 > height ) {
          pos_y = 170;
          
          tft.setCursor(pos_x, pos_y);
          tft.drawLine(pos_x, pos_y, pos_x, pos_y + 10, CYAN);
        } else {
          int old_y = pos_y;
          pos_y += 10;
          
          tft.drawLine(pos_x, old_y, pos_x, pos_y, CYAN);
        }
    }

    if (bot_btn.justPressed()) {
        bot_btn.drawButton(true);
        if ( pos_y - 10 < 150 ) {
          pos_y = height;
          
          tft.setCursor(pos_x, pos_y);
          tft.drawLine(pos_x, pos_y, pos_x, pos_y - 10, RED);
        } else {
          int old_y = pos_y;
          pos_y -= 10;
          
          tft.drawLine(pos_x, old_y, pos_x, pos_y, RED);
        }
    }

    if (right_btn.justPressed()) {
        left_btn.drawButton(true);
        if ( pos_x - 10 < 0 ) {
          pos_x = width;
          
          tft.setCursor(pos_x, pos_y);
          tft.drawLine(pos_x, pos_y, pos_x - 10, pos_y, GREEN);
        } else {
          int old_x = pos_x;
          pos_x -= 10;
          
          tft.drawLine(old_x, pos_y, pos_x, pos_y, GREEN);
        }
    }

    if (left_btn.justPressed()) {
        right_btn.drawButton(true);
        if ( pos_x + 10 > width ) {
          pos_x = 0;

          tft.setCursor(pos_x, pos_y);
          tft.drawLine(pos_x, pos_y, pos_x + 10, pos_y, BLUE);
        } else {
          int old_x = pos_x;
          pos_x += 10;
          
          tft.drawLine(old_x, pos_y, pos_x, pos_y, BLUE);
        }
    }
}
#endif
