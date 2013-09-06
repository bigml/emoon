#include "mheads.h"

void msys_makesure_coredump()
{
    struct rlimit rl;

    rl.rlim_cur = 500*1024*1024;
    rl.rlim_max = 500*1024*1024;
    setrlimit(RLIMIT_CORE, &rl);
}

int msys_systemf(char *fmt, ...)
{
    char key[LEN_HASH_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return system(key);
}

int msys_execvf(char *argv[], char *fmt, ...)
{
    char key[LEN_HASH_KEY];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(key, sizeof(key), fmt, ap);
    va_end(ap);

    return execv(key, argv);
}

bool msys_getdatetime(char *res, int len, const char *fmt, time_t second)
{
    memset(res, 0x0, len);
    /*
     * TODO memory leak?
     * used static memory, no memory leak
     * localtime_r() will
     */
    struct tm *stm = localtime(&second);
    strftime(res, len, fmt, stm);
    res[len-1] = '\0';
    return true;
}

bool msys_getdatetime_gmt(char *res, int len, const char *fmt, time_t second)
{
    memset(res, 0x0, len);
    struct tm *stm = gmtime(&second);
    strftime(res, len, fmt, stm);
    res[len-1] = '\0';
    return true;
}

time_t msys_get_abssec(char *fmt, char *time)
{
    struct tm stm;
    
    if (!fmt || !time) return 0;

    memset(&stm, 0x0, sizeof(struct tm));
    strptime(time, fmt, &stm);
    //strptime(datetime, "%Y-%m-%d %H:%M:%S", &stm);

    return mktime(&stm);
}

void msys_utc_time(struct timespec *ts)
{
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif    
}

