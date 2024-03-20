#include "amp_backend.h"

static struct amp_backend* backend;

int
main()
{
        backend = amp_backend_connect();
        amp_backend_start(backend); /* blocking */
        amp_backend_disconnect(backend);
        return 0;
}
