#include "include.h"

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void loop(){};

void editor_ui(DispPtr disp)
{
  disp->clearDisplay();
  disp->setCursor(0, 0);

  disp_draw_asset_on_grid(disp, CONTACT_NO, 0, 0);
  disp_highlight_asset_on_grid(disp, CONTACT_NC, 1, 0);
  disp_draw_asset_on_grid(disp, LOAD_COIL, 2, 0);
  disp_draw_labled_asset_on_grid(disp, "  s S b B m  M :00", LOAD_COIL, 3, 0);

  disp->display();
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
  // disp_test_drawing(&display);

  // TODO: Rendering a line into a display
  // display.clearDisplay();
  // dsp_draw_ld_exp(&display, "O:00=((I:00|O:00)&I:01&(I:02|!I:03))");
  editor_ui(&display);

  display.display();
}
