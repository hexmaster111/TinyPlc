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
    char data[MAX_LBL_LEN];
    int row;
    int col;
    LdElemType type;
} LdDiaElem;

typedef struct Ld
{
    char expr[MAX_STR_LEN];        // Raw LD expression
    char load_str[MAX_STR_LEN];    // Raw Load string
    int curPos;                    // Current position in the expression
    int curRow;                    // Current row in the diagram
    int curCol;                    // Current column in the diagram
    int elemCount;                 // Number of elements in the diagram
    char strBuff[MAX_STR_LEN];     // Buffer for reading strings
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
#define CURRENT_ELEM ld->elems[ld->elemCount]

// fills out the ld->elems array
void LD_parse_expr(Ld *ld)
{
    // While not end of string

    PARSE_LOG("Parsing Expression: %s\n", ld->expr);

    // O:00=((I:00|O:00)&I:01&(I:02|!I:03))"
    // ( - Branch Start
    //    ( - Branch Start
    //      I:00     - row 0, col 0, data I:00
    //      |     - OR (row + 1)
    //      O:00     - row 1, col 0, data O:00
    //    ) - Branch End
    //    & - And (col + 1)
    //      I:01     - row 0, col 1, data I:01
    //    & - And (col + 1)
    //    ( - Branch Start
    //      I:02     - row 0, col 2, data I:02
    //      |    - OR (row + 1)
    //      !I:03    - row 1, col 2 data !I:03
    //    ) - Branch End
    // ) - Branch End

    while (ld->expr[ld->curPos] != '\0')
    {
        char c = ld->expr[ld->curPos];
        PARSE_LOG("Parsing Char: %c\n", c);

        // if we are parcing a string of data
        if (ld->isParsingData)
        {
            // And we hit the end of the data
            if (c == '|' || c == '&' || c == '(' || c == ')')
            {
                ld->strBuff[ld->strBuffPos] = '\0';
                cpystr(CURRENT_ELEM.data, ld->strBuff);

                CURRENT_ELEM.type = LD_ELEM_DATA;
                CURRENT_ELEM.row = ld->curRow;
                CURRENT_ELEM.col = ld->curCol;
                PARSE_LOG("END OF DATA: %s, %d, %d (%d)\n", CURRENT_ELEM.data, CURRENT_ELEM.row, CURRENT_ELEM.col, ld->elemCount);
                ld->elemCount++;
                ld->strBuffPos = 0;
                ld->isParsingData = 0;
                continue;
            }

            if (c == '\0')
            {
                INVALID_EXPR("Unexpected end of expression");
            }

            // Add the char to the string buffer
            CURRENT_ELEM.data[ld->strBuffPos] = c;
            ld->strBuffPos++;
            ld->curPos++;
            continue;
        }

        switch (c)
        {
        case ' ':
            // Ignore spaces
            break;
        case '(':
            // Branch Start
            ld->elems[ld->elemCount].type = LD_ELEM_BRANCH_START;
            ld->elems[ld->elemCount].row = ld->curRow;
            ld->elems[ld->elemCount].col = ld->curCol;
            ld->elemCount++;
            ld->curCol++;
            PARSE_LOG("Branch Start: %d, %d (%d)\n", ld->curRow, ld->curCol, ld->elemCount);
            break;
        case ')':
            // Branch End
            ld->elems[ld->elemCount].type = LD_ELEM_BRANCH_END;
            ld->elems[ld->elemCount].row = ld->curRow;
            ld->elems[ld->elemCount].col = ld->curCol;
            ld->elemCount++;
            ld->curCol++;
            PARSE_LOG("Branch End: %d, %d (%d)\n", ld->curRow, ld->curCol, ld->elemCount);
            break;
        case '&':
            // And
            ld->elems[ld->elemCount].type = LD_ELEM_AND;
            ld->elems[ld->elemCount].row = ld->curRow;
            ld->elems[ld->elemCount].col = ld->curCol;
            ld->elemCount++;
            ld->curCol++;
            PARSE_LOG("And: %d, %d (%d)\n", ld->curRow, ld->curCol, ld->elemCount);
            break;
        case '|':
            // Or
            ld->elems[ld->elemCount].type = LD_ELEM_OR;
            ld->elems[ld->elemCount].row = ld->curRow;
            ld->elems[ld->elemCount].col = ld->curCol;
            ld->elemCount++;
            ld->curRow++;
            PARSE_LOG("Or: %d, %d (%d)\n", ld->curRow, ld->curCol, ld->elemCount);
            break;
        case '!':
            // Not
            TODO("Not");
            break;

        default:
            // Anything else is data that will be parsed later in the process
            ld->elems[ld->elemCount].type = LD_ELEM_DATA;
            ld->elems[ld->elemCount].row = ld->curRow;
            ld->elems[ld->elemCount].col = ld->curCol;
            ld->elems[ld->elemCount].data[ld->strBuffPos] = c;
            ld->strBuffPos++;
            ld->isParsingData = 1;
            PARSE_LOG("FOUND START OF DATA\n");
        }

        // Advance to the next character
        ld->curPos++;
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
// C.O:00     - Coil, data O:00
// = - end of load def
// ( - Branch Start
//    I:00     - NO Contact row 0, col 0, data I:00
//    |     - OR (row + 1)
//    I:01     - NO Contact row 1, col 0, data I:01
// ) - Branch End

//"C.O:00=(I:00|(I:01&I:02))"
// C.O:00     - Coil, data O:00
// = - end of load def
// ( - Branch Start
//    I:00     - NO Contact row 0, col 0, data I:00
//    |     - OR (row + 1)
//    ( - Branch Start
//      I:01     - NO Contact row 1, col 1, data I:01
//      &     - AND (col + 1)
//      I:02     - NO Contact row 1, col 2, data I:02
//    ) - Branch End
// ) - Branch End

// O0=(((I0&I1)|(!O0&I1)&!I2)|I3)
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//    ( - Branch Start
//      ( - Branch Start
//        I0     - NO Contact row 0, col 0, data I0
//        &     - AND (col + 1)
//        I1     - NO Contact row 0, col 1, data I1
//      ) - Branch End
//      |     - OR (row + 1, col = 0)
//      ( - Branch Start
//        !O0    - NC Contact row 1, col 0, data O0
//        &     - AND (col + 1)
//        I1     - NO Contact row 1, col 1, data I1
//      ) - Branch End
//      &     - AND (col + 1)
//      !I2    - NC Contact row 0, col 2, data I2
//    ) - Branch End
//    |     - OR (row + 1)
//    I3     - NO Contact row 2, col 0, data I3
// ) - Branch End

// O0=(I0&I1)
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//    I0     - NO Contact row 0, col 0, data I0
//    &     - AND (col + 1)
//    I1     - NO Contact row 0, col 1, data I1
// ) - Branch End

// C.O:00=((I0|O0)&!I1)
// C.O:00     - Coil, data O:00
// = - end of load def
// ( - Branch Start
//    ( - Branch Start
//      I0     - NO Contact row 0, col 0, data I0
//      |     - OR (row + 1)
//      O0     - NO Contact row 1, col 0, data O0
//    ) - Branch End
//    &     - AND (col + 1)
//    !I1    - NC Contact row 0, col 1, data I1
// ) - Branch End

// O0=((I0&I1&I2)|(I3&I4&I5)|(I6&I7&I8))
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//  ( - Branch Start
//      I0     - NO Contact row 0, col 0, data I0
//      &     - AND (col + 1)
//      I1     - NO Contact row 0, col 1, data I1
//      &     - AND (col + 1)
//      I2     - NO Contact row 0, col 2, data I2
//  ) - Branch End
//  |     - OR (row + 1 col = 0)
//  ( - Branch Start
//      I3     - NO Contact row 1, col 0, data I3
//      &     - AND (col + 1)
//      I4     - NO Contact row 1, col 1, data I4
//      &     - AND (col + 1)
//      I5     - NO Contact row 1, col 2, data I5
//  ) - Branch End
//  |     - OR (row + 1 col = 0)
//  ( - Branch Start
//      I6     - NO Contact row 2, col 0, data I6
//      &     - AND (col + 1)
//      I7     - NO Contact row 2, col 1, data I7
//      &     - AND (col + 1)
//      I8     - NO Contact row 2, col 2, data I8
//  ) - Branch End

// O0=(X0|I0&I1|Y0&I2&I3)
// O0    - No Type, data O0
// = - end of load def
// ( - Branch Start
//    X0     - NO Contact row 0, col 0, data X0
//    |     - OR (row + 1 col = 0)
//    I0     - NO Contact row 1, col 0, data I0
//    &     - AND (col + 1)
//    I1     - NO Contact row 1, col 1, data I1
//    |     - OR (row + 1 col = 0)
//    Y0     - NO Contact row 2, col 0, data Y0
//    &     - AND (col + 1)
//    I2     - NO Contact row 2, col 1, data I2
//    &     - AND (col + 1)
//    I3     - NO Contact row 2, col 2, data I3
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