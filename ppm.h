// ppm.h
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Ondřej Zobal, FIT
// Přeloženo: gcc 11.2

struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[]; // RGB bajty, celkem 3*xsize*ysize
};

struct ppm *ppm_read(const char *filename);
void ppm_free(struct ppm *p);
