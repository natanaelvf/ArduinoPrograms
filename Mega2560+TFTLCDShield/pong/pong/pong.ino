#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 6, XM = A2, YP = A1, YM = 7;
const int TS_LEFT = 907, TS_RT = 136, TS_TOP = 942, TS_BOT = 139;

const int bar_width = 20, bar_height = 60;
int bar_pos, bar_vel;
int ai_pos, ai_vel;
int player_score, ai_score;
int bal_x_pos, bal_y_pos, bal_x_vel, bal_y_vel;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

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

#define BLACK   0x0000
#define GREEN   0x07E0
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);            //PORTRAIT
    tft.fillScreen(BLACK);
    bar_vel = 5;
    ai_vel  = 5;
    bar_pos = tft.height()/2;
    ai_pos  = tft.height()/2;
}

void loop(void) {
     TSPoint p = ts.getPoint();

     tft.setCursor(0,0);
     
     if (pixel_y > tft.height()/2 && bar_vel < 0 && bar_pos < tft.height()) {
        bar_vel *= -1;
     }
     
     if (pixel_y < tft.height()/2 && bar_vel > 0 && bar_pos > 0) {
        bar_vel *= -1;
     }

     bar_pos += bar_vel;
     
     tft.fillRect(tft.width()/2, tft.height()/2, bar_width, bar_height, WHITE);
     // tft.fillScreen(BLACK);
}
#endif
