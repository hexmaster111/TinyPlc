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

// Asset images converted with https://javl.github.io/image2cpp/
const unsigned char epd_bitmap_LD_01[] PROGMEM = {0x02, 0x20, 0x00, 0x02, 0x60, 0x00, 0xfe, 0xbf, 0x80, 0x03, 0x20, 0x00, 0x02, 0x20, 0x00};
const unsigned char epd_bitmap_LD_02[] PROGMEM = {0x02, 0x20, 0x00, 0x02, 0x20, 0x00, 0xfe, 0x3f, 0x80, 0x02, 0x20, 0x00, 0x02, 0x20, 0x00};
const unsigned char epd_bitmap_LD_03[] PROGMEM = {0x04, 0x10, 0x00, 0x08, 0x08, 0x00, 0xf8, 0x0f, 0x80, 0x08, 0x08, 0x00, 0x04, 0x10, 0x00};
const unsigned char epd_bitmap_LD_04[] PROGMEM = {0x05, 0x10, 0x00, 0x09, 0x08, 0x00, 0xf9, 0x0f, 0x80, 0x09, 0x08, 0x00, 0x05, 0xd0, 0x00};
const unsigned char epd_bitmap_LD_05[] PROGMEM = {0x05, 0xd0, 0x00, 0x09, 0x48, 0x00, 0xf9, 0xcf, 0x80, 0x09, 0x88, 0x00, 0x05, 0x50, 0x00};
const unsigned char epd_bitmap_LD_06[] PROGMEM = {0x19, 0x98, 0x00, 0x25, 0x54, 0x00, 0xbd, 0x54, 0x80, 0x25, 0x54, 0x00, 0x25, 0x98, 0x00};
const unsigned char epd_bitmap_LD_07[] PROGMEM = {0x1a, 0x58, 0x00, 0x22, 0x54, 0x00, 0x9a, 0x58, 0x80, 0x0a, 0x54, 0x00, 0x31, 0x98, 0x00};
const unsigned char epd_bitmap_LD_08[] PROGMEM = {0x22, 0x97, 0x00, 0x36, 0x92, 0x00, 0xaa, 0x92, 0x80, 0x22, 0x92, 0x00, 0x22, 0xf2, 0x00};
const unsigned char epd_bitmap_LD_09[] PROGMEM = {0x33, 0xa8, 0x00, 0x29, 0x28, 0x00, 0xa9, 0x28, 0x80, 0x29, 0x28, 0x00, 0x33, 0x90, 0x00};
const unsigned char epd_bitmap_LD_10[] PROGMEM = {0x22, 0xea, 0x00, 0x36, 0xaa, 0x00, 0xaa, 0xaa, 0x80, 0x22, 0xaa, 0x00, 0x22, 0xe4, 0x00};
const unsigned char epd_bitmap_LD_11[] PROGMEM = {0x10, 0x00, 0x00, 0x30, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x30, 0x00, 0x00, 0x10, 0x00, 0x00};
const unsigned char epd_bitmap_LD_12[] PROGMEM = {0x00, 0x04, 0x00, 0x00, 0x06, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x06, 0x00, 0x00, 0x04, 0x00};
const unsigned char epd_bitmap_LD_13[] PROGMEM = {0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char epd_bitmap_LD_14[] PROGMEM = {0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char epd_bitmap_LD_15[] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define DIAGRAM_MAX_ELEMENTS 20
#define ASSET_MAX_DATA_LENGTH 14

typedef Adafruit_SSD1306 Disp;
typedef Disp *DispPtr;

typedef enum
{
  AT_ContactNc = 0, // data[0..3] = "I:00" - Bit IO Name
  AT_ContactNo,     // data[0..3] = "I:00" - Bit IO Name
  AT_LoadCoil,      // data[0..3] = "O:00" - Bit IO Name
  AT_LoadLatch,     // data[0..3] = "O:00" - Bit IO Name
  AT_LoadReset,     // data[0..3] = "O:00" - Bit IO Name

  AT_LoadAdd = 100, // data[0..3] = Dest Name    data[4..8] = source A name    data[9..13] = source B name DEST = A + B
  AT_LoadSub,       // data[0..3] = Dest Name    data[4..8] = source A name    data[9..13] = source B name DEST = A - B
  AT_LoadMut,       // data[0..3] = Dest Name    data[4..8] = source A name    data[9..13] = source B name DEST = A * B
  AT_LoadDiv,       // data[0..3] = Dest Name    data[4..8] = source A name    data[9..13] = source B name DEST = A / B

  AT_LoadMove = 200, // data[0..3] = Dest Name    data[4..8] = source A name

  AT_ContactCompare = 300, // Data[0..3] = source A name    data[4..8] = source B name  data[10] Opperator

  AT_WireUpFromLeft = 900,
  AT_WireUpFromRight,
  AT_Wire,
} AssetType;

/// @brief A single item on the diagram
typedef struct
{
  // ptr to image data
  AssetPtr asset;

  // type of asset
  AssetType type;

  // Data for this asset
  char data[ASSET_MAX_DATA_LENGTH];

  // TODO: data for the asset, like the value of a coil, or the name of a variable, etc

  uint8_t row; // Row, or x
  uint8_t col; // Column, or y
} GfxAsset;
typedef GfxAsset *GfxAssetPtr;

/// @brief A single diagram, there may be multiple diagrams in a single document
typedef struct
{
  GfxAsset elements[DIAGRAM_MAX_ELEMENTS];
  uint8_t elementCount;
} GfxDiagram;
typedef GfxDiagram *GfxDiagramPtr;

typedef struct
{
  GfxDiagram diagrams[DIAGRAM_MAX_ELEMENTS];
  uint8_t diagramCount;
} GfxDocument;

typedef GfxDocument *GfxDocumentPtr;

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
  GfxDocument document;
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

void disp_draw_text_on_grid(DispPtr disp, String text, int x, int y, int x_offset = 0)
{
  x *= LD_ASSET_WIDTH;
  y += 1; // Next Line
  y *= LD_ASSET_HEIGHT + 1;
  disp->setCursor(x + x_offset, y);
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

// y offset + (Element number * height of element) + 1PX offset + (Height of the font + Height of the already printed title)
// disp->setCursor(x, y + (FONT_HEIGHT * i) + 1 + (FONT_HEIGHT * 2));
// disp->print(" ");
// disp->print("SRC");
// disp->print(": ");
// disp->print(lables[0]);
// disp->print(lables[1]);
// disp->print(lables[2]);
// disp->println(lables[3]);
// i++;
#define PRINT_LABEL_ELEMENT(__LABEL, __STARTINGINDEX)                \
  disp->setCursor(x, y + (FONT_HEIGHT * i) + 1 + (FONT_HEIGHT * 2)); \
  disp->print(" ");                                                  \
  disp->print(__LABEL);                                              \
  disp->print(": ");                                                 \
  disp->print(lables[__STARTINGINDEX]);                              \
  disp->print(lables[__STARTINGINDEX + 1]);                          \
  disp->print(lables[__STARTINGINDEX + 2]);                          \
  disp->println(lables[__STARTINGINDEX + 3]);                        \
  i++;

#define DFB_NONE 0b00000000
#define DFB__SRC 0b00000001 // lables[0..3]
#define DFB__DST 0b00000010 // labels[4..7]
#define DFB__OPP 0b00000100 // labels[0..3]
#define DFB__SRA 0b00001000 // labels[4..7]
#define DFB__SRB 0b00010000 // labels[7..11]

/// @brief Draw a function block
/// @param disp  Display to draw on
/// @param x  X position of the block
/// @param y  Y position of the block
/// @param lables Array of strings to print See DFB__* for the index of each string
/// @param title Title of the block
/// @param dfb_flags Flags to control what is printed
void disp_draw_function_block(DispPtr disp, int x, int y, char *lables, const char *title, u_int8_t dfb_flags = DFB_NONE)
{
  disp->setCursor(x, y + FONT_HEIGHT);
  disp->print("     ");
  disp->println(title);
  int i = 0;

  if (dfb_flags & DFB__SRC)
  {
    PRINT_LABEL_ELEMENT("SRC", 0);
  }
  if (dfb_flags & DFB__DST)
  {
    PRINT_LABEL_ELEMENT("DST", 4);
  }

  if (dfb_flags & DFB__OPP)
  {
    PRINT_LABEL_ELEMENT("OPP", 0);
  }

  if (dfb_flags & DFB__SRA)
  {
    PRINT_LABEL_ELEMENT("SRA", 4);
  }

  if (dfb_flags & DFB__SRB)
  {
    PRINT_LABEL_ELEMENT("SRB", 7);
  }

  disp->drawRect(x, y,
                 LD_ASSET_WIDTH * 2,
                 LD_ASSET_HEIGHT * (i + 3) - 1,
                 1);
}

void __disp_draw_simple_asset(DispPtr disp, GfxAssetPtr asset, AssetPtr sprite, int newRow)
{
  disp_draw_text_on_grid(disp, asset->data, asset->col, newRow, 5);

  disp_draw_asset_on_grid(disp, sprite, asset->col, newRow + 1, DF_NONE);
}
void __disp_draw_wire(DispPtr disp, GfxAssetPtr asset, int newRow, u_int8_t flags)
{
  disp_draw_asset_on_grid(disp, asset->asset, asset->col, newRow + 1, flags);
}

void __disp_draw_fb(DispPtr disp, GfxAssetPtr asset, int newRow, const char *title)
{
  int x = (asset->col * LD_ASSET_WIDTH) + 2;
  int y = (newRow + 1) * LD_ASSET_HEIGHT;
  disp_draw_function_block(disp, x, y, asset->data, (char *)title, DFB__SRC | DFB__DST);
}

#define CASE_DO_BREAK(__CASE, __FUNC) \
  case __CASE:                        \
    __FUNC;                           \
    break;

// NOTE: the length is the number of elements - 1 (its zero based)
void disp_draw_diagram(DispPtr disp, GfxDiagramPtr dia, int length, int row_to_draw_on)
{
  // LOG("_________________NAME, COL, COR ROW\n");
  // LOG("disp_draw_diagram: len: %i\n", length);
  for (int i = 0; i < length; i++)
  {
    GfxAsset element = dia->elements[i];
    int newRow = ((row_to_draw_on + (element.row + 1)) * 2) - 1; // Inital offset, -1 for the menu bar that only takes one row

    u_int8_t flags = DF_NONE;

    switch (element.type)
    {

      CASE_DO_BREAK(AT_ContactNc, __disp_draw_simple_asset(disp, &element, CONTACT_NC, newRow))
      CASE_DO_BREAK(AT_ContactNo, __disp_draw_simple_asset(disp, &element, CONTACT_NO, newRow))
      CASE_DO_BREAK(AT_LoadCoil, __disp_draw_simple_asset(disp, &element, LOAD_COIL, newRow))
      CASE_DO_BREAK(AT_LoadLatch, __disp_draw_simple_asset(disp, &element, LOAD_LATCH, newRow))
      CASE_DO_BREAK(AT_LoadReset, __disp_draw_simple_asset(disp, &element, LOAD_RESET, newRow))

      CASE_DO_BREAK(AT_LoadAdd, __disp_draw_fb(disp, &element, newRow, "ADD"))
      CASE_DO_BREAK(AT_LoadSub, __disp_draw_fb(disp, &element, newRow, "SUB"))
      CASE_DO_BREAK(AT_LoadMut, __disp_draw_fb(disp, &element, newRow, "MUT"))
      CASE_DO_BREAK(AT_LoadDiv, __disp_draw_fb(disp, &element, newRow, "DIV"))
      CASE_DO_BREAK(AT_LoadMove, __disp_draw_fb(disp, &element, newRow, "MOV"))

    case AT_WireUpFromLeft:
    case AT_WireUpFromRight:
      flags |= DF_WIRE_UP_TO_NEXT;
    case AT_Wire:
      __disp_draw_wire(disp, &element, newRow, flags);
      continue;
    }
  }
}

//---------------------------

#define DEBUG_WIRE_ASSET(elemNo, __row, __cols, __diagramName) \
  __diagramName.elements[elemNo].asset = WIRE;                 \
  __diagramName.elements[elemNo].type = AT_Wire;               \
  __diagramName.elements[elemNo].row = __row;                  \
  __diagramName.elements[elemNo].col = __cols;

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

  GfxDiagram dbg_dgm = {};
  dbg_dgm.elementCount = 3;

  dbg_dgm.elements[0].type = AT_ContactNo;
  dbg_dgm.elements[0].row = 0;
  dbg_dgm.elements[0].col = 0;
  dbg_dgm.elements[0].data[0] = 'I';
  dbg_dgm.elements[0].data[1] = ':';
  dbg_dgm.elements[0].data[2] = '0';
  dbg_dgm.elements[0].data[3] = '0';

  dbg_dgm.elements[1].type = AT_LoadMove;
  dbg_dgm.elements[1].row = 0;
  dbg_dgm.elements[1].col = 1;
  dbg_dgm.elements[1].data[0] = 'N';
  dbg_dgm.elements[1].data[1] = ':';
  dbg_dgm.elements[1].data[2] = '0';
  dbg_dgm.elements[1].data[3] = '0';
  dbg_dgm.elements[1].data[4] = 'C';
  dbg_dgm.elements[1].data[5] = ':';
  dbg_dgm.elements[1].data[6] = '0';
  dbg_dgm.elements[1].data[7] = '0';

  dbg_dgm.elements[2].type = AT_ContactNo;
  dbg_dgm.elements[2].row = 0;
  dbg_dgm.elements[2].col = 3;
  dbg_dgm.elements[2].data[0] = 'I';
  dbg_dgm.elements[2].data[1] = ':';
  dbg_dgm.elements[2].data[2] = '0';
  dbg_dgm.elements[2].data[3] = '2';

  while (true)
  {

    disp->clearDisplay();
    disp->setCursor(0, 0);
    // GOOD!

    editor_draw_toolbar(editor, disp);
    disp_draw_diagram(disp, &dbg_dgm, dbg_dgm.elementCount, 0);

    // Test drawing a function block item
    // TEST FB
    // disp_draw_function_block(disp, 50, 10, testCmp, DFB__OPP | DFB__SRA | DFB__SRB);
    // disp_draw_function_block(disp, 10, 25, testMov, DFB__SRC | DFB__DST);
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
