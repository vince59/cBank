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

void dsp_cat2(void);

int main()
{
    load_db();
    init_ascii();
    for (;;)
    {
        init_dsp_main();
        int ch = wait_until("123q");
        switch (ch)
        {
            case '3' :
                dsp_cat();
                break;
            case '2' :
                dsp_acc();
                break;
            case '1' :
                dsp_stat();
                break;
        }
        if (ch == 'q')
            break;
    }
    close_db();
    close_ascii();
    return 0;
}