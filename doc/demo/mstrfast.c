#include "mheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

int main()
{
    char s[100] = "aaa\nbbb\nccc\nddd\n", *line;

    ULIST *lines;
    mstr_array_fast_split(&lines, s, "\n", 333);
    //string_array_split(&lines, s, "\n", 333);

    MLIST_ITERATE(lines, line) {
        printf("%s\n", line);
    }
    
    return 0;
}
