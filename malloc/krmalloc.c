/* K&R malloc */

#include <stddef.h>
#include "krmalloc.h"

#define NALLOC 1024

typedef size_t Align;
typedef union header Header;

union header {
    struct {
        union header *ptr;
        size_t size;
    } s;
    Align x;
};

static Header base;
static Header *freep = NULL;

static Header *morecore(unsigned int nu) {
    char *cp, *sbrk(int);
    Header *up;

    if (nu < NALLOC) {
        nu = NALLOC;
    }
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) - 1) {
        return NULL;
    }
    up = (Header *)cp;
    up->s.size = nu;
    _free((void *)(up + 1));
    return freep;
}

void *malloc(size_t nbytes) {
    Header *p, *prevp;
    Header *morecore(unsigned int);
    unsigned int nunits;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            } else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p+1);
        }
        if (p == freep) {
            if ((p = morecore(nunits)) == NULL) {
                return NULL;
            }
        }
    }

    return NULL;
}

void *calloc(size_t n, size_t size) {
    void bzero(void *, size_t);
    void *ptr = malloc(n * size);

    if (ptr) {
        bzero(ptr, n * size);
    }

    return ptr;
}

void _free(void *ap) {
    Header *bp, *p;

    if (ap == NULL) {
        return;
    }

    bp = (Header *)ap - 1;
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) {
            break;
        }
    }
    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }
    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }
    freep = p;
}

