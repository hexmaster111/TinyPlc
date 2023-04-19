#include "LdRead.h"

int main(int argc, char *argv[])
{
    Ld ld = {0};
    LD_ParseExpr(&ld, "O:00=((I:00|O:00)&I:01&(I:02|I:03))");

    return 0;
}