#include <stdio.h>

#include "libcbank.h"

int main()
{
    load_db();
    init_ascii();
    draw_button();
    draw_welcome();
    int ch = wait_until("qc");
    if (ch == 'c')
    {
        
    }
    close_db();
    close_ascii();
    return 0;
}