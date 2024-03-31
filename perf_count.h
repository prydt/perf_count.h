#ifndef _PERF_COUNT_H_
#define _PERF_COUNT_H_

#define _GNU_SOURCE
#include <assert.h>
#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

struct perf_counter
{
    int fd;
    uint64_t count;
};

/* glibc does not provide a wrapper for `perf_event_open`
   so we need to use the raw syscall. */
static long
perf_event_open(struct perf_event_attr* attr,
                pid_t pid,
                int cpu,
                int group_fd,
                unsigned long flags)
{
    return syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}

int
perf_counter_setup(struct perf_counter* pc, uint32_t type, uint64_t config)
{
    struct perf_event_attr attr;
    memset(&attr, 0, sizeof(struct perf_event_attr));
    attr.type = type;
    attr.config = config;
    attr.size = sizeof(struct perf_event_attr);
    attr.disabled = 1;

    /* TODO can set more perf_event_attr here */

    int fd = perf_event_open(&attr,
                             0 /* calling process/thread */,
                             -1 /* any CPU */,
                             -1 /* leader group_fd (not used) */,
                             0 /* not setting any flags */);

    if (fd == -1) {
        return -1;
    }

    pc->fd = fd;
    pc->count = 0;

    return 0;
}

void
perf_counter_start(struct perf_counter* pc)
{
    if (pc->fd != -1) {
        ioctl(pc->fd, PERF_EVENT_IOC_RESET, 0);
        ioctl(pc->fd, PERF_EVENT_IOC_ENABLE, 0);
    }
}

void
perf_counter_stop(struct perf_counter* pc)
{
    if (pc->fd != -1) {
        ioctl(pc->fd, PERF_EVENT_IOC_DISABLE, 0);
        ssize_t bytes_read = read(pc->fd, &pc->count, sizeof(pc->count));
        assert(bytes_read == sizeof(pc->count));
    }
}

uint64_t
perf_counter_get_count(struct perf_counter* pc)
{
    return pc->count;
}

#endif