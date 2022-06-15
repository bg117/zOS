#include <stdint.h>

typedef int (*entry_fn_t)(void *);

static int entry_local(void *any)
{
    (void)any;
    return 1;
}

entry_fn_t main = &entry_local;
