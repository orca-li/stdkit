#include <stdkit.h>
#include <stdio.h>
#include <string.h>

void *cbstep_putc(void *const itr, ...)
{
    putchar(KIT_TO(char, *itrread(itr)));

    return KIT_NULL;
}

void *cbfinish_endl(void *const itr, ...)
{
    KIT_UNUSED(itr);

    printf("\r\n");
    return KIT_NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <string>", argv[0]);
        return 1;
    }

    itrobj_t itr = itrnew(ITR_F_AUTORST_BASE);
    itrinit(itr, KIT_SIZEOF(**argv), argv[1]);
    itr->step = cbstep_putc;
    itr->finish = cbfinish_endl;
    itrrun(itr, strlen(argv[1]));

    itrfree(&itr);

    return 0;
}