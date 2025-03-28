#include "itrprot.h"

#include <stdlib.h>

#define public
#define private

typedef struct ITERATOR_CLASS {
public
    intmax_t index;
    uintmax_t limite;
    itrcallback_t step;

private
    itrflags_t flags;
    uintmax_t stepsz;
    int8_t* locate;
    int8_t* base;
} itrclass_t;
typedef itrclass_t* itrobj_t;

itrobj_t itrnew(const itrflags_t f)
{
    itrobj_t itr = (itrobj_t)malloc(sizeof(itrclass_t));

    itr->flags = f;
    itr->step = NULL;

    return itr;
}

void itrfree(itrobj_t itr)
{
    KIT_FREE(itr);
}

void itrinit(const itrobj_t itr, const uintmax_t stepsz, int8_t* const locate, ...)
{
    itr->stepsz = stepsz;
    itr->base = locate;
    itr->locate = locate;
    itr->index = 0;
}

itrstatus_t itrpp(const itrobj_t itr)
{
    if (itr->step)
        itr->step(itr);

    itr->locate += itr->stepsz;

    return 0;
}

void itrrun(const itrobj_t itr, uintmax_t count)
{
    while(count--)
        itrpp(itr);
}

int8_t* itrread(const itrobj_t itr)
{
    return itr->locate;
}