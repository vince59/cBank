#include <stdio.h>
#include <locale.h>
#include <stddef.h>

int main() {
    setlocale(LC_ALL, ""); // Set the locale to support UTF-8

    for (int i = 0; i <= 255; i++) {
        printf("ASCII value: %d, Character: %lc\n", i, (wchar_t)i);
    }
    return 0;
}
