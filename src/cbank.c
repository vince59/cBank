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
        int ch = wait_until("1234567q");
        switch (ch)
        {
        case '1':
            dsp_stat_month();
            break;
        case '2':
            dsp_stat();
            break;
        case '3':
            dsp_acc();
            break;
        case '4':
            dsp_cat();
            break;
        case '5':
            load_bpn();
            break;
        case '6':
            class_op();
            break;
        case '7':
            disp_op();
            break;
        }
        if (ch == 'q')
            break;
    }
    close_db();
    close_ascii();
    return 0;
}