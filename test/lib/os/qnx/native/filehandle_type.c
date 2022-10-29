#include "os/qnx/native/filehandle_type.h"

#define UNUSED(x) (void)(x)

int file_create(int success)
{
    if (success) return 1;
    return -1;
}

void file_close(int filehandle)
{
    UNUSED(filehandle);
    /* Nothing to do for the simulation. */
}
