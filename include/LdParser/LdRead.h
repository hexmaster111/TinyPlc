/*************************
 * LD Reader
 *  Reads LD expressions and converts them to a list of elements to be
 * passed into the LD diagram renderer. dosnt use memory allocations so
 * can be used in embedded systems.
 *
 */

#ifndef LdRead_h
#define LdRead_h
#define MAX_STR_LEN 256
#define MAX_LBL_LEN 32
#define MAX_LD_ELEMS 256
#include <stdio.h>
#include <stdlib.h>

#define TODO(msg)              \
    printf("TODO: %s\n", msg); \
    exit(-1);

void cpystr(char *dest, char *src)
{
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

typedef enum LdElemType
{
    LD_ELEM_DATA,
    LD_ELEM_AND,
    LD_ELEM_OR,
    LD_ELEM_BRANCH_START,
    LD_ELEM_BRANCH_END
} LdElemType;

typedef struct LdDiaElem
{
    char data[MAX_LBL_LEN]; // Data for the element
    int row;
    int col;
    LdElemType type;
} LdDiaElem;

typedef struct Ld
{
    char expr[MAX_STR_LEN];        // Raw LD expression
    char load_str[MAX_STR_LEN];    // Raw Load string
    int curPos;                    // Current position in the expression
    int elemCount;                 // Number of elements in the diagram
    char strBuff[25];              // Buffer for reading strings
    int strBuffPos;                // Current position in the string buffer
    int isParsingData;             // Are we parsing data or a label (0 no, 1 yes)
    LdDiaElem elems[MAX_LD_ELEMS]; // Array of elements parsed from the expression
} Ld;

#define INVALID_EXPR(msg) TODO("Invalid expression: " msg)

void LD_read_load(Ld *ld)
{
    int i = 0;
    while (ld->expr[i] != '=')
    {
        ld->load_str[i] = ld->expr[i];
        i++;
        if (i >= MAX_STR_LEN)
        {
            INVALID_EXPR("No '=' found to end load definition");
        }
    }
    ld->load_str[i] = '\0';
    ld->curPos = i + 1;
    printf("Load: %s\n", ld->load_str);
};

void LD_read_expr(Ld *ld)
{
    int i = ld->curPos;
    while (ld->expr[i] != '\0')
    {
        printf("%c", ld->expr[i]);
        i++;
        if (i >= MAX_STR_LEN)
        {
            INVALID_EXPR("Expr too long");
        }
    }
    printf("\n");
}

#define PARSE_LOG(...) printf(__VA_ARGS__)

void LD_parse_expr(Ld *ld)
{
    // Initialize variables for the parsing loop
    int i = ld->curPos;
    int row = 0;
    int col = 0;
    ld->elemCount = 0;

    while (ld->expr[i] != '\0')
    {
        // Parse the current element

        // Move to the next character
        i++;
    }
}

//"C.O:00=((I:00|O:00)&I:01&(I:02|!I:03))"
// C.O:00     - Coil, data O:00
// = - end of load def
// ( - Branch Start
//    ( - Branch Start
//      I:00     -  row 0, col 0, data I:00
//      |     - OR (row + 1)
//      O:00     -  row 1, col 0, data O:00
//    ) - Branch End
//    & - And (col + 1)
//      I:01     -  row 0, col 1, data I:01
//    & - And (col + 1)
//    ( - Branch Start
//      I:02     -  row 0, col 2, data I:02
//      |    - OR (row + 1)
//      !I:03    -  row 1, col 2 data !I:03
//    ) - Branch End
// ) - Branch End

//"C.O:00=(I:00|I:01)"
// C.O:00     -  data C.O:00
// = - end of load def
// ( - Branch Start
//    I:00     -  row 0, col 0, data I:00
//    |     - OR (row + 1)
//    I:01     -  row 1, col 0, data I:01
// ) - Branch End

//"C.O:00=(I:00|(I:01&I:02))"
// C.O:00     -data C.O:00
// = - end of load def
// ( - Branch Start
//    I:00     - NO Contact row 0, col 0, data I:00
//    |     - OR (row + 1)
//    ( - Branch Start
//      I:01     -  row 1, col 1, data I:01
//      &     - AND (col + 1)
//      I:02     -  row 1, col 2, data I:02
//    ) - Branch End
// ) - Branch End

// O0=(((I0&I1)|(!O0&I1)&!I2)|I3)
// O0    -  data O0
// = - end of load def
// ( - Branch Start
//    ( - Branch Start
//      ( - Branch Start
//        I0     - row 0, col 0, data I0
//        &     - AND (col + 1)
//        I1     -  row 0, col 1, data I1
//      ) - Branch End
//      |     - OR (row + 1, col = 0)
//      ( - Branch Start
//        !O0    -  row 1, col 0, data !O0
//        &     - AND (col + 1)
//        I1     -  row 1, col 1, data I1
//      ) - Branch End
//      &     - AND (col + 1)
//      !I2    - row 0, col 2, data !I2
//    ) - Branch End
//    |     - OR (row + 1)
//    I3     -  row 2, col 0, data I3
// ) - Branch End

// O0=(I0&I1)
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//    I0     -  row 0, col 0, data I0
//    &     - AND (col + 1)
//    I1     -  row 0, col 1, data I1
// ) - Branch End

// C.O:00=((I0|O0)&!I1)
// C.O:00     -  data C.O:00
// = - end of load def
// ( - Branch Start
//    ( - Branch Start
//      I0     -  row 0, col 0, data I0
//      |     - OR (row + 1)
//      O0     -  row 1, col 0, data O0
//    ) - Branch End
//    &     - AND (col + 1)
//    !I1    -  row 0, col 1, data !I1
// ) - Branch End

// O0=((I0&I1&I2)|(I3&I4&I5)|(I6&I7&I8))
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//  ( - Branch Start
//      I0     -  row 0, col 0, data I0
//      &     - AND (col + 1)
//      I1     -  row 0, col 1, data I1
//      &     - AND (col + 1)
//      I2     -  row 0, col 2, data I2
//  ) - Branch End
//  |     - OR (row + 1 col = 0)
//  ( - Branch Start
//      I3     -  row 1, col 0, data I3
//      &     - AND (col + 1)
//      I4     -  row 1, col 1, data I4
//      &     - AND (col + 1)
//      I5     -  row 1, col 2, data I5
//  ) - Branch End
//  |     - OR (row + 1 col = 0)
//  ( - Branch Start
//      I6     -  row 2, col 0, data I6
//      &     - AND (col + 1)
//      I7     -  row 2, col 1, data I7
//      &     - AND (col + 1)
//      I8     -  row 2, col 2, data I8
//  ) - Branch End

// O0=(X0|I0&I1|Y0&I2&I3)
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//    X0     -  row 0, col 0, data X0
//    |     - OR (row + 1 col = 0)
//    I0     -  row 1, col 0, data I0
//    &     - AND (col + 1)
//    I1     -  row 1, col 1, data I1
//    |     - OR (row + 1 col = 0)
//    Y0     -  row 2, col 0, data Y0
//    &     - AND (col + 1)
//    I2     -  row 2, col 1, data I2
//    &     - AND (col + 1)
//    I3     -  row 2, col 2, data I3
// ) - Branch End

void LD_ParseExpr(Ld *ld, char *expr)
{

    cpystr(ld->expr, expr);

    // Split the load and expression
    LD_read_load(ld);
    LD_read_expr(ld);

    // start the parsing
    LD_parse_expr(ld);
}
#endif