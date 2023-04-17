#include "include.h"

typedef Adafruit_SSD1306 Disp;
typedef Disp *DispPtr;

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void loop(){};

#define DISP_FLAG_UP_TO_NEXT_LEFT 1 // Extend the WIRE_UP_FROM_LEFT asset up to the connecting line
#define DISP_FLAG_UP_TO_NEXT_RIGHT 2 // Extend the WIRE_UP_FROM_RIGHT asset up to the connecting line

void disp_draw_asset_on_grid(DispPtr disp, AssetPtr asset, int x, int y, u_int8_t disp_flags = 0)
{
  x *= LD_ASSET_WIDTH;
  y *= LD_ASSET_HEIGHT + 1;
  y += 2; // Offset

  disp->drawBitmap(
      x,
      y,
      asset,
      LD_ASSET_WIDTH,
      LD_ASSET_HEIGHT,
      1);

  if (disp_flags & DISP_FLAG_UP_TO_NEXT_LEFT)
  {
    const int line_x_offset = 8;
    const int line_y_offset = 2;

    disp->drawLine(
        x + line_x_offset,
        y + line_y_offset,
        x + line_x_offset,
        y - (LD_ASSET_HEIGHT + (.5 * LD_ASSET_HEIGHT) + 1),
        1);
  }

  if (disp_flags & DISP_FLAG_UP_TO_NEXT_RIGHT)
  {
    //TODO: Verify this works
  }
}

void disp_draw_text_on_grid(DispPtr disp, String text, int x, int y)
{
  x *= LD_ASSET_WIDTH;
  y += 1; // Next Line
  y *= LD_ASSET_HEIGHT + 1;

  disp->setCursor(
      x,
      y);

  disp->print(text);
}

void setup()
{
  DEBUG.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS);
  display.clearDisplay();
  display.display();
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setFont(&FONT);

  disp_draw_text_on_grid(&display, "I:00", 0, 0);
  disp_draw_asset_on_grid(&display, CONTACT_NO, 0, 1);

  disp_draw_text_on_grid(&display, "O:00", 0, 2);
  disp_draw_asset_on_grid(&display, CONTACT_NO, 0, 3);

  disp_draw_asset_on_grid(&display, WIRE_UP_FROM_LEFT, 1, 3, DISP_FLAG_UP_TO_NEXT_LEFT);

  disp_draw_asset_on_grid(&display, WIRE, 1, 1);

  disp_draw_text_on_grid(&display, "I:01", 2, 0);
  disp_draw_asset_on_grid(&display, CONTACT_NC, 2, 1);

  disp_draw_asset_on_grid(&display, WIRE, 3, 1);
  disp_draw_asset_on_grid(&display, WIRE, 4, 1);
  disp_draw_asset_on_grid(&display, WIRE, 5, 1);

  disp_draw_text_on_grid(&display, "O:00", 6, 0);
  disp_draw_asset_on_grid(&display, LOAD_COIL, 6, 1);

  disp_draw_asset_on_grid(&display, WIRE, 7, 1);

  disp_draw_asset_on_grid(&display, WIRE, 0, 4);
  disp_draw_asset_on_grid(&display, WIRE, 1, 4);
  disp_draw_asset_on_grid(&display, WIRE, 2, 4);
  disp_draw_asset_on_grid(&display, WIRE, 3, 4);
  disp_draw_asset_on_grid(&display, WIRE, 4, 4);
  disp_draw_asset_on_grid(&display, WIRE, 5, 4);
  disp_draw_asset_on_grid(&display, WIRE, 6, 4);
  disp_draw_asset_on_grid(&display, WIRE, 7, 4);

  disp_draw_text_on_grid(&display, "((I:00 || O:00 ) && I:02) = O:00", 0, 5);

  display.display();
}
