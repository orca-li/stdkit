#ifndef FILEUUIDd722eeef75d64bf59618f53367e905bf
#define FILEUUIDd722eeef75d64bf59618f53367e905bf

#include "itrprot.h"

typedef struct ITERATOR_CLASS {
    const intmax_t index;
    uintmax_t limite;
    itrcallback_t step;
    itrcallback_t reset;
    itrcallback_t start;
    itrcallback_t finish;
} itrclass_t;
typedef itrclass_t* itrobj_t;

itrobj_t itrnew(const itrflags_t);
void itrfree(itrobj_t* itr);
void itrinit(const itrobj_t itr, const uintmax_t stepsz, const itrptr_t locate, ...);

void itrpp(const itrobj_t);                         // itr++;
void itradd(const itrobj_t, intmax_t add);          // itr += add;
void itridx(const itrobj_t, intmax_t index);        // itr[i];
void itrrst(const itrobj_t);                        // itr ~= itr;
void itrcpy(itrobj_t* dest, const itrobj_t src);    // memcpy(itr, src, sizeof(itr));

itrstatus_t itrrun(const itrobj_t itr, uintmax_t count);

void itrwrite(const itrobj_t, void* data, uintmax_t datasz);
itrptr_t itrhook(const itrobj_t, intmax_t index);
itrptr_t itrread(const itrobj_t);
intmax_t itrcmp(const itrobj_t, void* data, uintmax_t datasz);

#endif /*  FILEUUIDd722eeef75d64bf59618f53367e905bf */
