#include "error.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
 
void warning_msg(const char *fmt, ...) {
    va_list args;
    va_start (args, fmt);
    printf("VAROVANI: ");
    vprintf ( fmt, args);
    va_end (args); 
}

void error_exit(const char *fmt, ...) {
    va_list args;
    va_start (args, fmt);
    printf("CHYBA: ");
    vprintf ( fmt, args);
    va_end (args);
    exit(1);
}
