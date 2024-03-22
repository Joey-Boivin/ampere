#include "amp_backend.h"
#include <stdio.h>

static struct amp_backend* backend;

int
main(void)
{
        backend = amp_backend_connect();
        amp_backend_start(backend); /* blocking */
}
