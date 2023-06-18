#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "libcbank.h"

extern Node *head_kw;
extern Node *head_op;
extern Node *head_cat;
extern Node *head_acc;

int main()
{
    load_db();
    init_ascii();
    for (;;)
    {
        init_dsp_main();
        int ch = wait_until("csq");
        printf("%c",ch);
        switch (ch)
        {
            case 'c' :
                dsp_cat();
                break;
            case 's' :
                dsp_acc();
                break;
        }
        if (ch == 'q')
            break;
    }
    close_db();
    close_ascii();
    return 0;
}