#ifndef LdRead_h
#define LdRead_h
#define MAX_STR_LEN 256
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

typedef struct Ld
{
    char expr[MAX_STR_LEN];
    char load_str[MAX_STR_LEN];
    int curPos;
} Ld;


#define INVALID_EXPR(msg) TODO("Invalid expression: " msg)

void LD_ParseLoad(Ld* ld)
{
    int i = 0;
    while (ld->expr[i] != '=')
    {
        ld->load_str[i] = ld->expr[i];
        i++;
        if (i >= MAX_STR_LEN)
        {
            INVALID_EXPR("No '=' found");
        }
    }
    ld->load_str[i] = '\0';
    ld->curPos = i + 1;
    printf("Load: %s\n", ld->load_str);
};

void LD_ParseExprStr(Ld *ld)
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

void LD_ParseExpr(Ld *ld, char *expr)
{
    //"O:00=((I:00|O:00)&I:01&(I:02|!I:03))"

    // ( - Branch Start
    //    ( - Branch Start
    //      I:00     - NO Contact: row 0, col 0, lbl I:00
    //      |     - OR (row + 1) 
    //      O:00     - NO Contact row 1, col 0, lbl O:00
    //    ) - Branch End
    //    & - And (col + 1)
    //      I:01     - NO Contact row 0, col 1, lbl I:01
    //    & - And (col + 1)
    //    ( - Branch Start
    //      I:02     - NO Contact row 0, col 2, lbl I:02
    //      |    - OR (row + 1)
    //      !I:03    - NC Contact row 1, col 2 lbl I:03
    //    ) - Branch End
    // ) - Branch End

    cpystr(ld->expr, expr);
    LD_ParseLoad(ld);
    LD_ParseExprStr(ld);
}
#endif