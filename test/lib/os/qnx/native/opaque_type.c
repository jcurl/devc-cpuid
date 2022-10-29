#include <stdlib.h>

#include "os/qnx/native/opaque_type.h"

typedef struct opaque_type
{
    int value;
} opaque_type_t;

opaque_type_t* opaque_create()
{
    opaque_type_t* value = (opaque_type_t *)malloc(sizeof(opaque_type_t));
    return value;
}

void opaque_destroy(opaque_type_t *value)
{
    if (value) free(value);
}
