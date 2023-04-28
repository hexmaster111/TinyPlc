#include "include.h"

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Editor editor = {};

void loop(){};

void IH_on_key_in(char key)
{
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
  editor_ui(&editor, &display);
  display.display();
}
