#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <math.h>
#include <time.h>
#include <timer.h>


#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>

    int gettimeofday(struct timeval* p, void*)
    {
        union
        {
            long long ns100; /* time since 1 Jan 1601 in 100ns units */
            FILETIME ft;
        } _now;

        GetSystemTimeAsFileTime(&(_now.ft));
        p->tv_usec = (long)((_now.ns100 / 10LL) % 1000000LL);
        /* time since 1 Jan 1970 */
        p->tv_sec = (long)((_now.ns100 - 116444736000000000LL) / 10000000LL);
        return 0;
    }

#else
#include <sys/time.h>
#endif


    double getcurrenttime()
    {
        struct timeval t;
        gettimeofday(&t, NULL);

        return t.tv_sec + t.tv_usec*1.0e-6;
    }


    double tick(timer* tm)
    {
        tm->start_time   = 0;
        tm->stop_time    = 0;
        tm->elapsed_time = 0;

        return tm->start_time = getcurrenttime();
    }

    double tock(timer* tm)
    {
        tm->stop_time = getcurrenttime();
        return tm->elapsed_time = tm->stop_time - tm->start_time;
    }

    void PrintTimer(timer* tm)
    {
        printf("Elapsed Time: %g \n", tm->elapsed_time);
    }


#ifdef __cplusplus
}
#endif


