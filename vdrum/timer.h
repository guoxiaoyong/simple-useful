#ifndef __TIMER_H__
#define __TIMER_H__


#ifdef __cplusplus
extern "C" {
#endif

    double getcurrenttime();

    typedef struct timer
    {
        double start_time;
        double stop_time;
        double elapsed_time;
    } timer;

    double tick(timer* tm);
    double tock(timer* tm);
    void PrintTimer(timer* tm);

#ifdef __cplusplus
}
#endif
#endif


