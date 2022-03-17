// error.c
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Ondřej Zobal, FIT
// Přeloženo: gcc 11.2

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...) {
    va_list args;
    va_start (args, fmt);
    fprintf(stderr, "VAROVANÍ: ");
    vfprintf (stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end (args); 
}

void error_exit(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
