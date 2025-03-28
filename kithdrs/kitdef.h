#ifndef FILEUUIDc764b4deabde4e1da52b3037e36c7d65
#define FILEUUIDc764b4deabde4e1da52b3037e36c7d65

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define KIT_ASSERT(_cond) assert(_cond)

#define KIT_TO(_type, _value) ((_type)(_value))
#define KIT_UNUSED(_v) (void)(_v)

#define KIT_NULL NULL
#define KIT_FREE(_ptr) do { \
    if (_ptr) { \
        free(_ptr); \
        _ptr = KIT_NULL; \
    } \
} while(0)

#define KIT_STRINGIFY(_str) #_str
#define KIT_TO_STRING(_str) KIT_STRINGIFY(_str)

#define KIT_SIZEOF(_v) sizeof(_v)
#define KIT_ARRAY_SIZE(_array) (sizeof(_array) / sizeof(*_array))
#define KIT_FOR_EACH(_array) \
    for (size_t i = 0; i < KIT_ARRAY_SIZE(_array); ++i)
#define KIT_ELEMENT(_array) _array[i]

#endif /*  FILEUUIDc764b4deabde4e1da52b3037e36c7d65 */