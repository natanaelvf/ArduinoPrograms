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

int snake_size = 3;

const int size = 50;

// draw snake; grid is 20 x 20
const int sqr = 20;

// absolute screen_width and screen_height
const int s_w = 240, s_h = 320;
const int width = 240 - size, height = 320 - size;
const int grid_x_size = 11, grid_y_size = 12;

const int first_x_pos = (snake_size + 1) * sqr;
const int first_y_pos = (snake_size + 1) * sqr;

bool snake_corpse[grid_x_size][grid_y_size];

int head_x = 7, head_y = 3;

int tail_x = 4, tail_y = 3;

int v_x = 1, v_y = 0;

void setup(void) {
  ////////////////////////////////////////////////////////////
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);
  tft.setRotation(2);            //PORTRAIT
  tft.fillScreen(BLACK);
  ////////////////////////////////////////////////////////////

  draw_buttons();

  draw_grid();
}

void loop(void) {
  draw_buttons();

  choose_velocity();

  snake_logic(snake_corpse);

  print_corpse(snake_corpse);

}

void choose_velocity() {
  bool down = Touch_getXY();

  top_btn.press  (down && top_btn.contains(pixel_x, pixel_y));
  bot_btn.press  (down && bot_btn.contains(pixel_x, pixel_y));
  right_btn.press(down && right_btn.contains(pixel_x, pixel_y));
  left_btn.press (down && left_btn.contains(pixel_x, pixel_y));

  if (top_btn.justReleased()) {
    v_y = 1;
    v_x = 0;
  }

  if (bot_btn.justReleased()) {
    v_y = -1;
    v_x = 0;
  }

  if (right_btn.justReleased()) {
    v_x = 1;
    v_y = 0;
  }

  if (left_btn.justReleased()) {
    v_x = -1;
    v_y = 0;
  }

  if (top_btn.justPressed()) {
    top_btn.drawButton(true);
    v_y = 1;
    v_x = 0;
  }

  if (bot_btn.justPressed()) {
    bot_btn.drawButton(true);
    v_y = -1;
    v_x = 0;
  }

  if (right_btn.justPressed()) {
    right_btn.drawButton(true);
    v_x = 1;
    v_y = 0;
  }

  if (left_btn.justPressed()) {
    left_btn.drawButton(true);
    v_x = -1;
    v_y = 0;
  }
}

void snake_logic(bool snake_corpse[][12]) {
  if (v_x != 0) {
    head_x = (head_x + v_x) % 11;
    snake_corpse[head_y][head_x] = true;
    snake_corpse[tail_y][tail_x] = false;
    tail_x = (tail_x + v_x) % 11;
    draw_snake();
  }
  if (v_y != 0) {
    head_y = (head_y + v_y) % 12;
    snake_corpse[head_y][head_x] = true;
    snake_corpse[tail_y][tail_x] = false;
    tail_y = (tail_y + v_y) % 12;
    draw_snake();
  }
}

void print_corpse(bool snake_corpse[][12]) {
  for (int i = 0; i < grid_x_size; i++) {
    for (int j = 0; j < grid_y_size; j++) {
      Serial.print(snake_corpse[i][j]);
    }
    Serial.println();
  }
  Serial.println();
  delay(1000);
}

void draw_grid() {
  //vertical lines
  for (int i = 0; i <= 9; i++)
    tft.drawRect(0, i * s_w / 12, s_w, s_h / 8, WHITE);
  // horizontal lines
  for (int i = 0; i <= 10; i++)
    tft.drawRect(i * s_w / 12, 0, s_h / 8, 11 * s_w / 12, WHITE);
}

void draw_snake() {
  for (int i = 0; i < grid_x_size; i++) {
    for (int j = 0; j < grid_y_size; j++) {
      if (!snake_corpse[i][j]) {
        if (head_x == i && head_y == j) {
          tft.fillRect(sqr * i, sqr * j, sqr, sqr, RED);
          tft.drawRect(sqr * i, sqr * j, sqr, sqr, WHITE);
          break;
        }
        tft.fillRect(sqr * i, sqr * j, sqr, sqr, GREEN);
        tft.drawRect(sqr * i, sqr * j, sqr, sqr, WHITE);
      }
      if (tail_x == i && tail_y == j) {
        tft.fillRect(sqr * i, sqr * j, sqr, sqr, BLACK);
        tft.drawRect(sqr * i, sqr * j, sqr, sqr, WHITE);
      }
    }
  }
}

void draw_buttons() {
  left_btn.initButton  (&tft, 3 * size / 2 - 10, height + size / 2, size, size, MAGENTA, WHITE, BLACK, "<", 1);
  bot_btn.initButton   (&tft, width - size / 2 - size + 10, height + size / 2, size, size, MAGENTA, WHITE, BLACK, "V", 1);
  top_btn.initButton   (&tft, width - size / 2 - size + 10, height - size / 2, size, size, MAGENTA, WHITE, BLACK, "^", 1);
  right_btn.initButton (&tft, width - size / 2  + 20, height + size / 2, size, size, MAGENTA, WHITE, BLACK, ">", 1);

  left_btn.drawButton(false);
  bot_btn.drawButton(false);
  top_btn.drawButton(false);
  right_btn.drawButton(false);
}

#endif
