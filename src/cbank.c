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
        int ch = wait_until("12345q");
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
        }
        if (ch == 'q')
            break;
    }
    close_db();
    close_ascii();
    return 0;
}