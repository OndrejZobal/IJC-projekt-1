struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[]; // RGB bajty, celkem 3*xsize*ysize
};

struct ppm *ppm_read(const char *filename);
