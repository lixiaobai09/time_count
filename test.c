/* @brief This file is a demo to use the function in time_cnt.h
 *
 * @author: xiaobai
 *
 * Use a vector-vector operation to test.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "time_cnt.h"
#define N (10000000)
#define OP *
#define P 0
#define CLOCKS_PER_SEC_FOR_TIMES sysconf(_SC_CLK_TCK)
int a[N], b[N], c[N];

int main(int argc, char** argv) {
    for (int i = 0; i < N; ++i) {
        a[i] = i + 1;
        b[i] = i + 1;
    }
    set_cpu(1); // Set this program to run second CPU.
    mhz(1);     // Give the second CPU frequency.
    printf("\n");

    memset(c, 0, sizeof(c));
    start_cycle_counter();
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] OP b[i];
    }
    double res = get_cycle_counter(P);
    printf("Run %f cycles.\n", res);
    int len = (N > 10? 10 : N);
    for (int i = 0; i < len; ++i) {
        printf("%d ", c[i]);
    }
    printf("\n\n");

    memset(c, 0, sizeof(c));
    start_clock_counter();
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] OP b[i];
    }
    double real_time = get_clock_counter(P);
    printf("Run %f ms. (real time)\n", real_time);
    for (int i = 0; i < len; ++i) {
        printf("%d ", c[i]);
    }
    printf("\n\n");

    memset(c, 0, sizeof(c));
    start_run_counter();
    for (int i = 0; i < N; ++i) {
        c[i] = a[i] OP b[i];
    }
    res = get_run_counter(P);
    printf("Run %f ms\n", res);
    for (int i = 0; i < len; ++i) {
        printf("%d ", c[i]);
    }
    printf("\n\n");

    return 0;
}
