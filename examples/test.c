#include "../perf_count.h"

#include <stdio.h> /* for printf, perror */
#include <inttypes.h> /* for PRIu64 */


void lots_of_work() {
    sleep(1);
}

int main() {
    struct perf_counter cycles;
    if (perf_counter_setup(&cycles, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES) == -1) {
        perror("failed to setup perf counter");
        return -1;
    }


    perf_counter_start(&cycles);
    lots_of_work();
    perf_counter_stop(&cycles);

    int64_t num_cycles = perf_counter_get_count(&cycles);
    printf("lots_of_work() took %" PRIu64 " cycles\n", num_cycles);
}