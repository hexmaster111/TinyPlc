#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Picopixel.h>
// #include <LdParser/LdRead.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SSD1306_ADDRESS 0x3C

#define FONT Picopixel
#define FONT_HEIGHT 6
#define FONT_WIDTH 4

#define DEBUG Serial
#define LOG(...) DEBUG.printf(__VA_ARGS__)

#define LD_ASSET_WIDTH 17
#define LD_ASSET_HEIGHT 5
#define ASSET(x) const uint8_t PROGMEM x[]
typedef const uint8_t PROGMEM *AssetPtr;

#define TEST_PATTERN epd_bitmap_LD_00

#define CONTACT_NC epd_bitmap_LD_01
#define CONTACT_NO epd_bitmap_LD_02

#define LOAD_COIL epd_bitmap_LD_03
#define LOAD_LATCH epd_bitmap_LD_04
#define LOAD_RESET epd_bitmap_LD_05
#define LOAD_ADD epd_bitmap_LD_06
#define LOAD_SUB epd_bitmap_LD_07
#define LOAD_MUT epd_bitmap_LD_08
#define LOAD_DIV epd_bitmap_LD_09
#define LOAD_MOVE epd_bitmap_LD_10

#define UI_BACKARROW epd_bitmap_LD_11
#define UI_FORWARDARROW epd_bitmap_LD_12

#define WIRE_UP_FROM_LEFT epd_bitmap_LD_13
#define WIRE_UP_FROM_RIGHT epd_bitmap_LD_14
#define WIRE epd_bitmap_LD_15

typedef enum
{
  AT_ContactNc = 0,
  AT_ContactNo,
  AT_LoadCoil,
  AT_LoadLatch,
  AT_LoadReset,

  AT_LoadAdd = 100,
  AT_LoadSub,
  AT_LoadMut,
  AT_LoadDiv,

  AT_LoadMove = 200,

  AT_WireUpFromLeft = 300,
  AT_WireUpFromRight,
  AT_Wire,
} AssetType;

// Asset images converted with https://javl.github.io/image2cpp/
// 'LD_01', 17x5px
const unsigned char epd_bitmap_LD_01[] PROGMEM = {
    0x02, 0x20, 0x00, 0x02, 0x60, 0x00, 0xfe, 0xbf, 0x80, 0x03, 0x20, 0x00, 0x02, 0x20, 0x00};
// 'LD_02', 17x5px
const unsigned char epd_bitmap_LD_02[] PROGMEM = {
    0x02, 0x20, 0x00, 0x02, 0x20, 0x00, 0xfe, 0x3f, 0x80, 0x02, 0x20, 0x00, 0x02, 0x20, 0x00};
// 'LD_03', 17x5px
const unsigned char epd_bitmap_LD_03[] PROGMEM = {
    0x04, 0x10, 0x00, 0x08, 0x08, 0x00, 0xf8, 0x0f, 0x80, 0x08, 0x08, 0x00, 0x04, 0x10, 0x00};
// 'LD_04', 17x5px
const unsigned char epd_bitmap_LD_04[] PROGMEM = {
    0x05, 0x10, 0x00, 0x09, 0x08, 0x00, 0xf9, 0x0f, 0x80, 0x09, 0x08, 0x00, 0x05, 0xd0, 0x00};
// 'LD_05', 17x5px
const unsigned char epd_bitmap_LD_05[] PROGMEM = {
    0x05, 0xd0, 0x00, 0x09, 0x48, 0x00, 0xf9, 0xcf, 0x80, 0x09, 0x88, 0x00, 0x05, 0x50, 0x00};
// 'LD_06', 17x5px
const unsigned char epd_bitmap_LD_06[] PROGMEM = {
    0x19, 0x98, 0x00, 0x25, 0x54, 0x00, 0xbd, 0x54, 0x80, 0x25, 0x54, 0x00, 0x25, 0x98, 0x00};
// 'LD_07', 17x5px
const unsigned char epd_bitmap_LD_07[] PROGMEM = {
    0x1a, 0x58, 0x00, 0x22, 0x54, 0x00, 0x9a, 0x58, 0x80, 0x0a, 0x54, 0x00, 0x31, 0x98, 0x00};
// 'LD_08', 17x5px
const unsigned char epd_bitmap_LD_08[] PROGMEM = {
    0x22, 0x97, 0x00, 0x36, 0x92, 0x00, 0xaa, 0x92, 0x80, 0x22, 0x92, 0x00, 0x22, 0xf2, 0x00};
// 'LD_09', 17x5px
const unsigned char epd_bitmap_LD_09[] PROGMEM = {
    0x33, 0xa8, 0x00, 0x29, 0x28, 0x00, 0xa9, 0x28, 0x80, 0x29, 0x28, 0x00, 0x33, 0x90, 0x00};
// 'LD_10', 17x5px
const unsigned char epd_bitmap_LD_10[] PROGMEM = {
    0x22, 0xea, 0x00, 0x36, 0xaa, 0x00, 0xaa, 0xaa, 0x80, 0x22, 0xaa, 0x00, 0x22, 0xe4, 0x00};
// 'LD_11', 17x5px
const unsigned char epd_bitmap_LD_11[] PROGMEM = {
    0x10, 0x00, 0x00, 0x30, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x30, 0x00, 0x00, 0x10, 0x00, 0x00};
// 'LD_12', 17x5px
const unsigned char epd_bitmap_LD_12[] PROGMEM = {
    0x00, 0x04, 0x00, 0x00, 0x06, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x06, 0x00, 0x00, 0x04, 0x00};
// 'LD_13', 17x5px
const unsigned char epd_bitmap_LD_13[] PROGMEM = {
    0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'LD_14', 17x5px
const unsigned char epd_bitmap_LD_14[] PROGMEM = {
    0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 'LD_15', 17x5px
const unsigned char epd_bitmap_LD_15[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define DIAGRAM_MAX_ELEMENTS 20
#define ASSET_MAX_NAME_LENGTH 5
// #define ASSET_MAX_DATA_LENGTH 10

typedef Adafruit_SSD1306 Disp;
typedef Disp *DispPtr;

/// @brief A single item on the diagram
typedef struct
{
  // ptr to image data
  AssetPtr asset;

  // type of asset
  AssetType type;

  // name of the asset
  char name[ASSET_MAX_NAME_LENGTH];

  // TODO: data for the asset, like the value of a coil, or the name of a variable, etc

  uint8_t row; // Row, or x
  uint8_t col; // Column, or y
} GfxAsset;

/// @brief A single diagram, there may be multiple diagrams in a single document
typedef struct
{
  GfxAsset elements[DIAGRAM_MAX_ELEMENTS];
  uint8_t elementCount;
} GfxDiagram;
typedef GfxDiagram *GfxDiagramPtr;

//---------------------------
typedef enum
{
  EAT_BitOpperators, // NO, NC, COIL, LATCH, RESET
  EAT_Arithmatic,    // ADD, SUB, MUT, DIV
                     // Perhaps a category for the "special" keys like backspace, and the save menu or something

  EAT_Wires, // WIRE_UP_FROM_LEFT, WIRE_UP_FROM_RIGHT, WIRE

} Editor_ActiveToolbar;

// NOTE! The order of these items must match the order of the items in the UI Toolbar
typedef enum
{
  ECT_ContactNo,
  ECT_ContactNc,
  ECT_LoadCoil,
  ECT_LoadLatch,
  ECT_LoadReset,
  ECT_LoadAdd,
  ECT_LoadSub,
  ECT_LoadMut,
  ECT_LoadDiv,
  ECT_LoadMove,
  ECT_WireUpFromLeft,
  ECT_Wire,
  ECT_WireUpFromRight,
  ETC_CURRENTLY_SELECTED_TOOLBAR_ITEM_COUNT,
} Editor_CurrentlySelectedToolbarItem;

Editor_ActiveToolbar GetToolbarFromItem(Editor_CurrentlySelectedToolbarItem item)
{
  switch (item)
  {
  case ECT_ContactNo:
  case ECT_ContactNc:
  case ECT_LoadCoil:
  case ECT_LoadLatch:
  case ECT_LoadReset:
    return EAT_BitOpperators;
  case ECT_LoadAdd:
  case ECT_LoadSub:
  case ECT_LoadMut:
  case ECT_LoadDiv:
  case ECT_LoadMove:
    return EAT_Arithmatic;
  case ECT_WireUpFromLeft:
  case ECT_WireUpFromRight:
  case ECT_Wire:
    return EAT_Wires;
  default:
    LOG("GetToolbarFromItem: Unknown item: %i\n", item);
    return EAT_BitOpperators;
  }
}

/// @brief Editor state
typedef struct
{
  // what item withn the toolbar is selected
  Editor_CurrentlySelectedToolbarItem selectedToolbarItem;
} Editor;

typedef Editor *EditorPtr;

#define DF_NONE /*      */ 0b0000000
#define DF_WIRE_UP_TO_NEXT 0b0000001 // Extend the WIRE_UP_FROM_LEFT asset up to the connecting line
#define DF_HIGHLIGHT /* */ 0b0000010 // Highlight the asset

void disp_draw_asset_on_grid(DispPtr disp, AssetPtr asset, int x, int y, u_int8_t disp_flags = DF_NONE)
{
  x *= LD_ASSET_WIDTH;
  x += 2; // Offset

  y *= LD_ASSET_HEIGHT + 1;
  y += 2; // Offset

  // Color changes if the asset is highlighted
  int color = 1;

  if (disp_flags & DF_HIGHLIGHT)
  {
    disp->fillRect(x, y,
                   LD_ASSET_WIDTH,
                   LD_ASSET_HEIGHT,
                   1);
    color = 0;
  }

  disp->drawBitmap(x, y,
                   asset,
                   LD_ASSET_WIDTH,
                   LD_ASSET_HEIGHT,
                   color);

  if (disp_flags & DF_WIRE_UP_TO_NEXT)
  {
    const int line_x_offset = 8;
    const int line_y_offset = 2;

    disp->drawLine(x + line_x_offset,
                   y + line_y_offset,
                   x + line_x_offset,
                   y - (LD_ASSET_HEIGHT + (.5 * LD_ASSET_HEIGHT) + 1),
                   color);
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

#define disp_draw_named_menu_asset_on_grid(disp, label, asset, x, y) \
  disp_draw_asset_on_grid(disp, asset, x, y);                        \
  disp_draw_text_on_grid(disp, label, x, y + 1);

#define disp_draw_labled_asset_on_grid(disp, label, asset, x, y) \
  disp_draw_asset_on_grid(disp, asset, x, y + 1);                \
  disp_draw_text_on_grid(disp, label, x, y);

void disp_highlight_asset_on_grid(DispPtr disp, AssetPtr asset, int x, int y)
{

  // draw a fileld rectangle around the asset
  disp->fillRect(
      x * LD_ASSET_WIDTH,
      y * LD_ASSET_HEIGHT + 2,
      LD_ASSET_WIDTH,
      LD_ASSET_HEIGHT,
      1);

  disp->drawBitmap(
      x * LD_ASSET_WIDTH,
      y * LD_ASSET_HEIGHT + 2,
      asset,
      LD_ASSET_WIDTH,
      LD_ASSET_HEIGHT,
      0);
}

// NOTE: the length is the number of elements - 1 (its zero based)
void disp_draw_diagram(DispPtr disp, GfxDiagramPtr dia, int length, int row)
{
  // LOG("disp_draw_diagram: len: %i\n", length);
  // LOG("_________________NAME, COL, COR ROW\n");
  for (int i = 0; i < length; i++)
  {
    GfxAsset element = dia->elements[i];

    u_int8_t flags = DF_NONE;

    if (element.type == AT_WireUpFromLeft ||
        element.type == AT_WireUpFromRight)
    {
      flags |= DF_WIRE_UP_TO_NEXT;
    }

    int newRow = ((row + element.row) * 2) - 1; // Inital offset, -1 for the menu bar that only takes one row

    // LOG("disp_draw_diagram: %s, %i, %i\n", element.name, element.col, newRow);

    disp_draw_asset_on_grid(disp, element.asset, element.col, newRow + 1, flags);
    disp_draw_text_on_grid(disp, element.name, element.col, newRow);
  }
}

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

//---------------------------

#define DEBUG_WIRE_ASSET(elemNo, __row, __cols, __diagramName) \
  __diagramName.elements[elemNo].asset = WIRE;                 \
  __diagramName.elements[elemNo].type = AT_Wire;               \
  __diagramName.elements[elemNo].row = __row;                  \
  __diagramName.elements[elemNo].col = __cols;                 \
  __diagramName.elements[elemNo].name[0] = '\0';

void editor_poc(EditorPtr editor, DispPtr disp)
{
  GfxDiagram dbg_dgm = {};
  dbg_dgm.elementCount = 10;

  dbg_dgm.elements[0].asset = CONTACT_NO;
  dbg_dgm.elements[0].type = AT_ContactNo;
  dbg_dgm.elements[0].row = 1;
  dbg_dgm.elements[0].col = 0;
  dbg_dgm.elements[0].name[0] = 'I';
  dbg_dgm.elements[0].name[1] = ':';
  dbg_dgm.elements[0].name[2] = '0';
  dbg_dgm.elements[0].name[3] = '0';
  dbg_dgm.elements[0].name[4] = '\0';

  dbg_dgm.elements[1].asset = CONTACT_NC;
  dbg_dgm.elements[1].type = AT_ContactNc;
  dbg_dgm.elements[1].row = 2;
  dbg_dgm.elements[1].col = 0;
  dbg_dgm.elements[1].name[0] = 'I';
  dbg_dgm.elements[1].name[1] = ':';
  dbg_dgm.elements[1].name[2] = '0';
  dbg_dgm.elements[1].name[3] = '1';
  dbg_dgm.elements[1].name[4] = '\0';

  dbg_dgm.elements[2].asset = CONTACT_NC;
  dbg_dgm.elements[2].type = AT_ContactNc;
  dbg_dgm.elements[2].row = 3;
  dbg_dgm.elements[2].col = 0;
  dbg_dgm.elements[2].name[0] = 'I';
  dbg_dgm.elements[2].name[1] = ':';
  dbg_dgm.elements[2].name[2] = '0';
  dbg_dgm.elements[2].name[3] = '2';
  dbg_dgm.elements[2].name[4] = '\0';

  dbg_dgm.elements[2].asset = WIRE;
  dbg_dgm.elements[2].type = AT_Wire;
  dbg_dgm.elements[2].row = 1;
  dbg_dgm.elements[2].col = 1;
  dbg_dgm.elements[2].name[0] = '\0';

  dbg_dgm.elements[3].asset = WIRE_UP_FROM_LEFT;
  dbg_dgm.elements[3].type = AT_WireUpFromLeft;
  dbg_dgm.elements[3].row = 2;
  dbg_dgm.elements[3].col = 1;
  dbg_dgm.elements[3].name[0] = '\0';

  dbg_dgm.elements[4].asset = CONTACT_NC;
  dbg_dgm.elements[4].type = AT_ContactNc;
  dbg_dgm.elements[4].row = 1;
  dbg_dgm.elements[4].col = 2;
  dbg_dgm.elements[4].name[0] = 'I';
  dbg_dgm.elements[4].name[1] = ':';
  dbg_dgm.elements[4].name[2] = '0';
  dbg_dgm.elements[4].name[3] = '3';
  dbg_dgm.elements[4].name[4] = '\0';

  DEBUG_WIRE_ASSET(5, 1, 3, dbg_dgm);
  DEBUG_WIRE_ASSET(6, 1, 4, dbg_dgm);
  DEBUG_WIRE_ASSET(7, 1, 5, dbg_dgm);

  dbg_dgm.elements[8].asset = LOAD_COIL;
  dbg_dgm.elements[8].type = AT_LoadCoil;
  dbg_dgm.elements[8].row = 1;
  dbg_dgm.elements[8].col = 6;
  dbg_dgm.elements[8].name[0] = 'O';
  dbg_dgm.elements[8].name[1] = ':';
  dbg_dgm.elements[8].name[2] = '0';
  dbg_dgm.elements[8].name[3] = '0';
  dbg_dgm.elements[8].name[4] = '\0';

  disp_draw_diagram(disp, &dbg_dgm, dbg_dgm.elementCount - 1, 0);
}
// editor->selectedToolbarItem == ECT_Back ? DF_HIGHLIGHT : DF_NONE
#define TOOLBAR_HIGHLIGHT(__ASSET, __ECT, __ITEM_CTR) \
  disp_draw_asset_on_grid(disp, __ASSET, __ITEM_CTR++, 0, editor->selectedToolbarItem == __ECT ? DF_HIGHLIGHT : DF_NONE);

void editor_draw_toolbar(EditorPtr editor, DispPtr disp)
{
  // ui nav controls
  int item = 1;

  auto toolbar = GetToolbarFromItem(editor->selectedToolbarItem);

  switch (toolbar)
  {
  case EAT_BitOpperators:
    TOOLBAR_HIGHLIGHT(CONTACT_NO, ECT_ContactNo, item)
    TOOLBAR_HIGHLIGHT(CONTACT_NC, ECT_ContactNc, item)
    TOOLBAR_HIGHLIGHT(LOAD_COIL, ECT_LoadCoil, item)
    TOOLBAR_HIGHLIGHT(LOAD_LATCH, ECT_LoadLatch, item)
    TOOLBAR_HIGHLIGHT(LOAD_RESET, ECT_LoadReset, item)
    break;
  case EAT_Arithmatic:
    TOOLBAR_HIGHLIGHT(LOAD_ADD, ECT_LoadAdd, item)
    TOOLBAR_HIGHLIGHT(LOAD_SUB, ECT_LoadSub, item)
    TOOLBAR_HIGHLIGHT(LOAD_MUT, ECT_LoadMut, item)
    TOOLBAR_HIGHLIGHT(LOAD_DIV, ECT_LoadDiv, item)
    TOOLBAR_HIGHLIGHT(LOAD_MOVE, ECT_LoadMove, item)
    break;
  case EAT_Wires:
    TOOLBAR_HIGHLIGHT(WIRE_UP_FROM_LEFT, ECT_WireUpFromLeft, item)
    TOOLBAR_HIGHLIGHT(WIRE, ECT_Wire, item)
    TOOLBAR_HIGHLIGHT(WIRE_UP_FROM_RIGHT, ECT_WireUpFromRight, item)
  default:
    LOG("editor_draw_toolbar: Unknown toolbar type: %i\n", toolbar);
    break;
  }
}

void do_poc_loop_no_return(EditorPtr editor, DispPtr disp)
{
  while (true)
  {

    disp->clearDisplay();
    disp->setCursor(0, 0);
    editor_draw_toolbar(editor, disp);
    editor_poc(editor, disp);
    disp->display();
    delay(1000);

    editor->selectedToolbarItem = (Editor_CurrentlySelectedToolbarItem)((editor->selectedToolbarItem + 1) % ETC_CURRENTLY_SELECTED_TOOLBAR_ITEM_COUNT);
  }
}

void editor_ui(EditorPtr editor, DispPtr disp)
{
  do_poc_loop_no_return(editor, disp);
}

Disp display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Editor editor = {};

void loop(){};
void setup()
{
  DEBUG.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS);
  display.display();
  delay(500);
  display.clearDisplay();

  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setFont(&FONT);
  // disp_test_drawing(&display);

  // TODO: Rendering a line into a display
  // display.clearDisplay();
  // dsp_draw_ld_exp(&display, "O:00=((I:00|O:00)&I:01&(I:02|!I:03))");
  editor_ui(&editor, &display);
  display.display();
  LOG("Setup complete");
}
