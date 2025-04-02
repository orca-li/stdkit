#ifndef FILEUUID86e342fe6b2544abb72c53a3140e18b0
#define FILEUUID86e342fe6b2544abb72c53a3140e18b0

#include "kithdrs/kitdef.h"
#include <stdarg.h>

typedef uint64_t itrflags_t;
typedef intmax_t itrstatus_t;
typedef int8_t itrdata_t;
typedef itrdata_t* itrptr_t;
typedef void* (*itrcallback_t)(void* const itr, ...);

#define ITR_F_NONE KIT_TO(itrflags_t, 0ul)
#define ITR_F_AUTORESET KIT_TO(itrflags_t, 1 << 0ul)
#define ITR_F_LIMITATION KIT_TO(itrflags_t, 1 << 1ul)

#endif /*  FILEUUID86e342fe6b2544abb72c53a3140e18b0 */