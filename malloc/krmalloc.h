void *malloc(size_t);
void *calloc(size_t, size_t);
void _free(void *);

#define free(p)   \
    do {          \
        _free(p); \
        p = NULL; \
    } while (0)

