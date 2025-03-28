#ifndef FILEUUIDd722eeef75d64bf59618f53367e905bf
#define FILEUUIDd722eeef75d64bf59618f53367e905bf

#include "itrprot.h"

typedef struct ITERATOR_CLASS {
    uintmax_t index;
    uintmax_t limite;
    itrcallback_t step;
} itrclass_t;
typedef itrclass_t* itrobj_t;


itrobj_t itrnew(const itrflags_t);
void itrfree(itrobj_t itr);
void itrinit(const itrobj_t itr, const uintmax_t stepsz, int8_t* const locate, ...);

void itrpp(const itrobj_t);

itrstatus_t itrrun(const itrobj_t itr, uintmax_t count);

int8_t* itrread(const itrobj_t);

#endif /*  FILEUUIDd722eeef75d64bf59618f53367e905bf */