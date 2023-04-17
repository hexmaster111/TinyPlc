#include "include.h"

typedef Adafruit_SSD1306 Disp;
typedef Disp *DispPtr;

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void loop(){};

void disp_draw_asset_on_grid(DispPtr disp, AssetPtr asset, int x, int y)
{
  x *= LD_ASSET_WIDTH;
  y *= LD_ASSET_HEIGHT + 1;
  y += 2; // Scoot down one pixel to account for the font

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
  y += 1; // Next Line from requested as atafrute draws text from below the line
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

  disp_draw_text_on_grid(&display, "I:01", 0, 2);
  disp_draw_asset_on_grid(&display, CONTACT_NC, 0, 3);

  disp_draw_text_on_grid(&display, "I:02", 0, 4);
  disp_draw_asset_on_grid(&display, CONTACT_NC, 0, 5);

  disp_draw_text_on_grid(&display, "I:03", 0, 6);
  disp_draw_asset_on_grid(&display, CONTACT_NC, 0, 7);

  disp_draw_text_on_grid(&display, "I:04", 0, 8);
  disp_draw_asset_on_grid(&display, CONTACT_NC, 0, 9);

  disp_draw_text_on_grid(&display, "I:05", 0, 10);
  disp_draw_asset_on_grid(&display, CONTACT_NC, 0, 11);

  display.display();
}
