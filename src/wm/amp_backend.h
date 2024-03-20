#ifndef AMPERE_AMP_BACKEND_H
#define AMPERE_AMP_BACKEND_H

struct amp_backend;

struct amp_backend* amp_backend_connect(void);

int amp_backend_start(struct amp_backend* backend);

void amp_backend_disconnect(struct amp_backend* backend);

#endif // AMPERE_AMP_BACKEND_H
