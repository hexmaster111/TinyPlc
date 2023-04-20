#ifndef DISP_H
#define DISP_H
#include "include.h"
typedef Adafruit_SSD1306 Disp;
typedef Disp *DispPtr;
#define DF_WIRE_UP_TO_NEXT 1 // Extend the WIRE_UP_FROM_LEFT asset up to the connecting line

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

    if (disp_flags & DF_WIRE_UP_TO_NEXT)
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

#define disp_draw_labled_asset_on_grid(disp, label, asset, x, y) \
    disp_draw_asset_on_grid(disp, asset, x, y + 1);              \
    disp_draw_text_on_grid(disp, label, x, y);

void disp_test_drawing(DispPtr display)
{
    display->clearDisplay();
    disp_draw_labled_asset_on_grid(display, "I:00", CONTACT_NO, 0, 0);
    disp_draw_labled_asset_on_grid(display, "O:00", CONTACT_NO, 0, 2);
    disp_draw_asset_on_grid(display, WIRE_UP_FROM_LEFT, 1, 3, DF_WIRE_UP_TO_NEXT);
    disp_draw_asset_on_grid(display, WIRE, 1, 1);
    disp_draw_labled_asset_on_grid(display, "I:01", CONTACT_NC, 2, 0);
    disp_draw_asset_on_grid(display, WIRE, 3, 1);
    disp_draw_labled_asset_on_grid(display, "I:02", CONTACT_NO, 4, 0);
    disp_draw_asset_on_grid(display, WIRE_UP_FROM_RIGHT, 3, 3, DF_WIRE_UP_TO_NEXT);
    disp_draw_labled_asset_on_grid(display, "I:03", CONTACT_NC, 4, 2);
    disp_draw_asset_on_grid(display, WIRE_UP_FROM_LEFT, 5, 3, DF_WIRE_UP_TO_NEXT);
    disp_draw_asset_on_grid(display, WIRE, 5, 1);
    disp_draw_labled_asset_on_grid(display, "O:00", LOAD_COIL, 6, 0);
    disp_draw_asset_on_grid(display, WIRE, 7, 1);

    disp_draw_asset_on_grid(display, WIRE, 0, 4);
    disp_draw_asset_on_grid(display, WIRE, 1, 4);
    disp_draw_asset_on_grid(display, WIRE, 2, 4);
    disp_draw_asset_on_grid(display, WIRE, 3, 4);
    disp_draw_asset_on_grid(display, WIRE, 4, 4);
    disp_draw_asset_on_grid(display, WIRE, 5, 4);
    disp_draw_asset_on_grid(display, WIRE, 6, 4);
    disp_draw_asset_on_grid(display, WIRE, 7, 4);
    disp_draw_text_on_grid(display, "O:00=((I:00|O:00)&I:01&(I:02|!I:03))", 0, 5);
    display->display();
}

/// @brief  Attempts to draw a logic diagram expression on the display
/// @param disp Display to draw on
/// @param exp  Expression to draw
/// @param err  Error message if expression is invalid
void dsp_draw_ld_exp(DispPtr disp, String exp, String *err = NULL)
{

    // Example expression: O:00=((I:00|O:00)&I:01&(I:02|!I:03))
}
#endif