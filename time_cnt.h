#ifndef TIME_CNT_H
#define TIME_CNT_H

/* for program run time counter
 * @author: xiaobai
 * note: the function get_cycle_counter and get_run_counter
 *       include the process interrupt and sleep time;
 *
 *       the function get_clock_counter
 *       includes the all process time index, such as real time, user time, system time.
 *       but the unit tick for result is a little coarse, perhaps it is 10 ms per tick.
 */

/**
 * @breif Set the process run on cpu
 */
int set_cpu(int cpu);
/**
 * @breif Start count the cpu cycle
 */
void start_cycle_counter();

/**
 * @breif Get the cpu cycles from last start_cycle_counter() running
 *
 * @param p If print the result information
 *          0 not print, 1 print
 *
 * @return unit: cycles, the cpu cycles count
 */
double get_cycle_counter(int p);

/**
 * @breif Start count the cpu clock ticks
 * It is include user, system time and his children time
 */
void start_clock_counter();

/**
 * @breif Get the cpu clock ticks from last start_clock_counter() running
 * It is include user, system time and his children time
 * @param real_time Get the real time with unit ticks
 * @param p If print the result information
 *          0 not print, 1 print
 *
 *
 * @return unit: ticks, include user, system and children time
 *         struct tms:
 *                      tms_utime:  the user time with unit ticks
 *                      tms_stime:  the system time with unit ticks
 *                      tms_cutime:  the children user time with unit ticks
 *                      tms_cstime:  the children system time with unit ticks
 */
struct tms get_clock_counter(int* real_time, int p);

/**
 * @breif Start count the time from start to end on computer
 */
void start_run_counter();

/**
 * @breif Get the program time from last start_run_counter() running
 *
 * @param p If print the result information
 *          0 not print, 1 print
 *
 *
 * @return unit: ms
 */
double get_run_counter(int p);

/**
 * @breif Get the real cpu running frequency
 * @param p If print the result information
 *          0 not print, 1 print
 *
 * @return unit: mhz
 */
double mhz(int p);

#endif
