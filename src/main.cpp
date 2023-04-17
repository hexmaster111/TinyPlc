#include "include.h"

typedef Adafruit_SSD1306 Disp;
typedef Disp *DispPtr;

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void loop(){};

void disp_draw_asset_on_grid(DispPtr disp, AssetPtr asset, int x, int y)
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

  disp_draw_asset_on_grid(&display, WIRE, 1, 1);
  disp_draw_asset_on_grid(&display, WIRE, 2, 1);
  disp_draw_asset_on_grid(&display, WIRE, 3, 1);
  disp_draw_asset_on_grid(&display, WIRE, 4, 1);
  disp_draw_asset_on_grid(&display, WIRE, 5, 1);

  disp_draw_text_on_grid(&display, "O:00", 6, 0);
  disp_draw_asset_on_grid(&display, LOAD_COIL, 6, 1);

  disp_draw_asset_on_grid(&display, WIRE, 7, 1);

  display.display();
}
