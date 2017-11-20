#ifndef UTIL_H
#define UTIL_H

#include "sys/time.h"

/* Convert timeval to seconds (double). */
static inline double
timeval_s(struct timeval tv)
{

    return (double)tv.tv_usec / 1000000.0 + (double)tv.tv_sec;
}

/* Substract timeval b from timeval a. */
static inline struct timeval
timeval_sub(struct timeval a, struct timeval b)
{
    struct timeval c;

    if (a.tv_usec < b.tv_usec) {
        c.tv_usec = 1000000L + a.tv_usec - b.tv_usec;
        c.tv_sec = a.tv_sec - 1;
    } else {
        c.tv_usec = a.tv_usec - b.tv_usec;
    }

    c.tv_sec = a.tv_sec - b.tv_sec;

    return c;
}

/* Sleep thread for s seconds. */
static inline void
wait(double s)
{
    struct timespec ts;
    double chr = floor(s);
    double man = s - chr;

    ts.tv_sec = (time_t)chr;
    ts.tv_nsec = (long)(man * 1000000000.0);

    nanosleep(&ts, &ts);
}

#endif //UTIL_H
