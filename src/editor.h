#ifndef EDITOR_H
#define EDITOR_H
#include "include.h"
#define DIAGRAM_MAX_ELEMENTS 20
#define ASSET_MAX_NAME_LENGTH 5
// #define ASSET_MAX_DATA_LENGTH 10

void disp_draw_diagram(DispPtr disp, GfxDiagramPtr diagram);

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

    // position on the diagram
    uint8_t row;
    uint8_t col;
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
} Editor_ActiveToolbar;

/// @brief Editor state
typedef struct
{
    Editor_ActiveToolbar activeToolbar;
} Editor;

typedef Editor *EditorPtr;
//---------------------------
void editor_debug(EditorPtr editor, DispPtr disp)
{
    GfxDiagram dbg_dgm = {};
    dbg_dgm.elementCount = 2;

    dbg_dgm.elements[0].asset = CONTACT_NO;
    dbg_dgm.elements[0].type = AT_ContactNo;
    dbg_dgm.elements[0].row = 0;
    dbg_dgm.elements[0].col = 0;
    dbg_dgm.elements[0].name[0] = 'I';
    dbg_dgm.elements[0].name[1] = ':';
    dbg_dgm.elements[0].name[2] = '0';
    dbg_dgm.elements[0].name[3] = '0';
    dbg_dgm.elements[0].name[4] = '\0';

    dbg_dgm.elements[1].asset = CONTACT_NC;
    dbg_dgm.elements[1].type = AT_ContactNc;
    dbg_dgm.elements[1].row = 1;
    dbg_dgm.elements[1].col = 0;
    dbg_dgm.elements[1].name[0] = 'I';
    dbg_dgm.elements[1].name[1] = ':';
    dbg_dgm.elements[1].name[2] = '0';
    dbg_dgm.elements[1].name[3] = '1';
    dbg_dgm.elements[1].name[4] = '\0';

    disp_draw_diagram(disp, &dbg_dgm, 0, 0);
}

void editor_ui(EditorPtr editor, DispPtr disp)
{
    disp->clearDisplay();
    disp->setCursor(0, 0);
    editor_debug(editor, disp);
    disp->display();
    return;
    disp_draw_asset_on_grid(disp, CONTACT_NO, 0, 0);
    disp_highlight_asset_on_grid(disp, CONTACT_NC, 1, 0);
    disp_draw_asset_on_grid(disp, LOAD_COIL, 2, 0);
    disp_draw_asset_on_grid(disp, LOAD_LATCH, 3, 0);
    disp_draw_asset_on_grid(disp, LOAD_RESET, 4, 0);

    disp_draw_asset_on_grid(disp, LOAD_ADD, 0, 1);
    disp_draw_asset_on_grid(disp, LOAD_SUB, 1, 1);
    disp_draw_asset_on_grid(disp, LOAD_MUT, 2, 1);
    disp_draw_asset_on_grid(disp, LOAD_DIV, 3, 1);
    disp_draw_asset_on_grid(disp, UI_BACKSPACE, 4, 1);

    // disp_draw_labled_asset_on_grid(disp, "  s S b B m  M :00", LOAD_COIL, 3, 0);

    disp->display();
}
#endif