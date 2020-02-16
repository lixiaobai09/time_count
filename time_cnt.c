#define _GNU_SOURCE
#include <sys/times.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
//# ifndef CLOCKS_PER_SEC
//#define CLOCKS_PER_SEC sysconf(_SC_CLK_TCK)
//#endif
#define MINUS(res, a, b, attr) res.attr = a.attr - b.attr

static uint64_t cycle_start;
static struct tms clock_start;
static clock_t real_clock_start;
static struct timespec run_start;

#include <sched.h>
/* Set the process run on cpu */
int set_cpu(int cpu) {
    cpu_set_t cpu_mask;
    CPU_ZERO(&cpu_mask);
    CPU_SET(cpu, &cpu_mask);
    return sched_setaffinity(0, sizeof(cpu_set_t), &cpu_mask);
}

/* Get the cpu timestamps count register value */
static uint64_t get_cycle() {
    uint32_t a, b;
    asm (
            "rdtsc; \n\t"
            "movl %%edx, %0; \n\t"
            "movl %%eax, %1; \n\t"
            : "=r"(a), "=r"(b)
            :
            : "%edx", "%eax"
        );
//    printf("0x%08x 0x%08x \n", a, b);
    return ((((uint64_t)a) << 32) | b);
}

 /* Start count the cpu cycle */
void start_cycle_counter() {
    cycle_start = get_cycle();
}

/* Get the cpu cycles from last start_cycle_counter() running */
double get_cycle_counter(int p) {
    uint64_t cycle_end = get_cycle();
    double ret = 0.0;
    if (cycle_end < cycle_start) {
        ret = ((1l << 63) - cycle_end + cycle_start + (1l << 63));
    }
    else {
        ret = (cycle_end - cycle_start);
    }
    if (ret < 0) {
        fputs("count cycles error!", stderr);
        exit(-1);
    }
    if (p) {
        printf("This code segment runs %f cpu cycles.\n", ret);
    }
    return ret;
}

/* Get the tms a minus tms b */
static inline struct tms minus(struct tms a, struct tms b) {
    struct tms ret;
    MINUS(ret, a, b, tms_utime);
    MINUS(ret, a, b, tms_stime);
    MINUS(ret, a, b, tms_cutime);
    MINUS(ret, a, b, tms_cstime);
    return ret;
}

/* Start count the cpu clock ticks */
void start_clock_counter() {
    real_clock_start = times(&clock_start);
}

/* Get the cpu clock ticks from last start_clock_counter() running */
struct tms get_clock_counter(int* real_time, int p) {
    struct tms clock;
    clock_t real_clock_end;
    real_clock_end = times(&clock);
    *real_time = real_clock_end - real_clock_start;
    clock = minus(clock, clock_start);
    if (p) {
        int cps = CLOCKS_PER_SEC;
        printf("This code segment runs %d ticks, %f ms user time.\n",
                clock.tms_utime, clock.tms_utime / cps * 1000.0);
        printf("This code segment runs %d ticks, %f ms system time.\n",
                clock.tms_stime, clock.tms_stime / cps * 1000.0);
        printf("This code segment runs %d ticks, %f ms real time.\n",
                *real_time, (double) *real_time / cps  * 1000.0);
    }
    return clock;
}

/* Start count the time from start to end on computer */
void start_run_counter() {
    clock_gettime(CLOCK_MONOTONIC, &run_start);
}

/* Get the program time from last start_run_counter() running */
double get_run_counter(int p) {
    struct timespec run_end;
    clock_gettime(CLOCK_MONOTONIC, &run_end);
    double ret = (run_end.tv_sec - run_start.tv_sec) * 1000.0 + (run_end.tv_nsec - run_start.tv_nsec) / 1000000.0;
    if (p) {
        printf("This code segment runs %f ms.\n", ret);
    }
    return ret;
}

/* Get the real cpu running frequency */
double mhz(int p) {
    start_cycle_counter();
    sleep(1);
    double cycles = get_cycle_counter(0) / 1000000.0;
    if (p) {
        printf("The frequecy of this CPU is %0.3f mHZ.\n", cycles);
    }
    return cycles;
}
