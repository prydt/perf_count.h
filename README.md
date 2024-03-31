# 🧮 perf_count.h
Easily setup and read `perf` counters on just a portion of code. 
Only works on Linux (since perf is a Linux feature).
Supported on any version of C, ANSI and onward.

# Usage

Just include `perf_count.h` header file in your C/C++ program.

Basic usage looks like:
```C
#include <stdio.h>
#include "perf_count.h"

int main() {
    struct perf_counter cycles;
    perf_counter_setup(&cycles, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
    perf_counter_start(&cycles);

    // code to profile...

    perf_counter_stop(&cycles);

    int64_t num_cycles = perf_counter_read(&cycles);
    printf("took %zd cycles to run.\n", num_cycles);
}
```

```Bash
gcc your_c_file.c
sudo ./a.out
```
We require sudo to setup the hardware perf counter.

See a full example in the [examples dir](/example/test.c).

# Full API

```C
int perf_counter_setup(struct perf_counter *pc, uint32_t type, uint64_t config)
```
Setup `perf_counter` struct to listen to event specified by `type` and `config`. Returns 0 if successful, returns -1 on error.

See all possible `type`s and `config`s [here (under "config" field)](https://man7.org/linux/man-pages/man2/perf_event_open.2.html).

```C
void perf_counter_start(struct perf_counter *pc)
```
Starts perf counter.

```C
void perf_counter_stop(struct perf_counter *pc)
```
Stops perf counter.

```C
uint64_t perf_counter_get_count(struct perf_counter *pc)
```
Returns value of specified perf counter.

## FAQ

<details>
    <summary>(ISO C99 and later) Why am I getting `warning: implicit declaration of function ‘syscall’ [-Wimplicit-function-declaration]`?</summary>

This might be because you are `#include`-ing a header file before `_GNU_SOURCE` is defined. Either add `#define _GNU_SOURCE` before you include any headers or move `#include "perf_count.h"` to the top of your imports.
</details>
