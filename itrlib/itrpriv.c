#include "itrprot.h"

#include <stdlib.h>
#include <string.h>

#define public
#define private

typedef struct ITERATOR_CLASS {
public
    intmax_t index;
    uintmax_t limite;
    itrcallback_t step;
    itrcallback_t reset;
    itrcallback_t finish;

private
    itrflags_t flags;
    uintmax_t stepsz;
    itrptr_t locate;
    itrptr_t base;
} itrclass_t;
typedef itrclass_t* itrobj_t;

itrobj_t itrnew(const itrflags_t f)
{
    itrobj_t this = (itrobj_t)malloc(sizeof(itrclass_t));

    this->flags = f;
    this->step = KIT_NULL;
    this->reset = KIT_NULL;
    this->finish = KIT_NULL;

    return this;
}

void itrfree(itrobj_t *this)
{
    KIT_FREE(*this);
}

void itrinit(const itrobj_t this, const uintmax_t stepsz, int8_t* const locate, ...)
{
    this->stepsz = stepsz;
    this->base = locate;
    this->locate = locate;
    this->index = 0;
}

void itrpp(const itrobj_t this)
{
    if (this->step)
        this->step(this);

    this->locate += this->stepsz;
    this->index++;
}

void itradd(const itrobj_t this, intmax_t add)
{
    this->locate += add;
    this->index += add;
}

void itridx(const itrobj_t this, intmax_t index)
{
    if (this->index < index)
        this->locate += (this->stepsz * (index - this->index));
    else
        this->locate -= (this->stepsz * (this->index - index));
    this->index = index;
}

void itrrst(const itrobj_t this)
{
    if (this->reset)
        this->reset(this);

    this->locate = this->base;
    this->index = 0;
}

void itrcpy(itrobj_t* dest, const itrobj_t src)
{
    if (*dest == KIT_NULL)
        *dest = itrnew(ITR_F_NONE);

    memcpy(*dest, src, sizeof(itrclass_t));
}

void itrrun(const itrobj_t this, uintmax_t count)
{
    while(count--)
        itrpp(this);

    if (this->finish)
        this->finish(this);
}

void itrwrite(const itrobj_t this, void* data, uintmax_t datasz)
{
    memcpy(this->locate, data, datasz);
}

itrptr_t itrread(const itrobj_t this)
{
    return this->locate;
}

intmax_t itrcmp(const itrobj_t this, void* data, uintmax_t datasz)
{
    return memcmp(this->locate, data, datasz);
}

itrptr_t itrhook(const itrobj_t this, intmax_t index)
{
    intmax_t current_index = this->index;
    itrptr_t locate_index = NULL;
    itridx(this, index);
    locate_index = itrread(this);
    itridx(this, current_index);
    return locate_index;
}