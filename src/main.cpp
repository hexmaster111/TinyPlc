#include "include.h"

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void loop(){};

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
  display.clearDisplay();
  dsp_draw_ld_exp(&display, "O:00=((I:00|O:00)&I:01&(I:02|!I:03))");
  display.display();
}
