This is a template project for the STM32F4DISCOVERY board using PlatformIO,
with some built-in helper functions to use printf() using SWO and for
measuring cycle counts. The clock is initialized at 24 MHz and the ART
accelerator is disabled to achieve deterministic execution times.

It heavily borrows from https://github.com/maxgerhardt/pio-swo-demo,
including its custom SWO viewer target to monitor output directly from
within Visual Studio Code. One can also use `st-trace` from
https://github.com/stlink-org/stlink instead by running the following
command:

`st-trace -c24`

# Example usage for cycle count measurement

```
#include "utils.h"

int benchmark_f() {
    bm_decls;

    bm_start();
    f();
    bm_end();

    printf("cycles = %lu\n", bm_result());
}
```
