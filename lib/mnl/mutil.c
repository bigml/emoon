#include "mheads.h"

int mutil_compare_int(const void *a, const void *b)
{
    int *i = (int*)a;
    int *j = (int*)b;
    return *i-*j;
}

int mutil_compare_inta(const void *a, const void *b)
{
    int *i = (int*)a;
    char *j = (char*)b;
    
    return *i - atoi(j);
}

void mutil_rand_numbers_byarray(int in[], int inlen, int out[], int outlen)
{
    int pos;

    if (outlen > inlen) return;
    
    for (int i = 0; i < outlen; i++) {
        pos = neo_rand(inlen-i-1);
        out[i] = in[pos];
        
        in[pos] = in[inlen-i-1];
    }
}

void mutil_rand_numbers(int max, int out[], int outlen)
{
    if (outlen > max) return;

    int in[max];

    for (int i = 0; i < max; i++) in[i] = i+1;

    return mutil_rand_numbers_byarray(in, max, out, outlen);
}

int mutil_satoi(const char *nptr)
{
    if (!nptr) return 0;

    return atoi(nptr);
}

bool mutil_getdatetime(char *res, int len, const char *fmt, time_t second)
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

bool mutil_getdatetime_gmt(char *res, int len, const char *fmt, time_t second)
{
    memset(res, 0x0, len);
    struct tm *stm = gmtime(&second);
    strftime(res, len, fmt, stm);
    res[len-1] = '\0';
    return true;
}

time_t mutil_get_abssec(char *fmt, char *time)
{
    struct tm stm;
    
    if (!fmt || !time) return 0;

    memset(&stm, 0x0, sizeof(struct tm));
    strptime(time, fmt, &stm);
    //strptime(datetime, "%Y-%m-%d %H:%M:%S", &stm);

    return mktime(&stm);
}

void mutil_utc_time(struct timespec *ts)
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

void mutil_dump_matrix(char *prefix)
{
    GLdouble projMatrix[16];
    GLdouble modelMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    
    printf("\n\n\n %s projection matrix: \n", prefix);
    for (int i = 0; i < 16; i++) {
        printf(" %f", projMatrix[i]);
    }
    
    printf("\n %s model matrix \n", prefix);
    for (int i = 0; i < 16; i++) {
        printf(" %f", modelMatrix[i]);
    }
}


#define WIN_WIDTH 640
#define WIN_HEIGHT 420
#define POINT_SPACE 10
#define NUM_EARTH_POINT (90 / POINT_SPACE) * (360 / POINT_SPACE) * 4
float m_evert[NUM_EARTH_POINT][5];
void mutil_create_earth(double R, double H, double K, double Z)
{
    int n;
    double a;
    double b;
    
    n = 0;
    for (b = 0; b <= 90 - POINT_SPACE; b += POINT_SPACE) {
        for (a = 0; a <= 360 - POINT_SPACE; a += POINT_SPACE) {
            m_evert[n][0] = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
            m_evert[n][1] = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
            m_evert[n][2] = R * cos((b) / 180 * PI) - Z;
            m_evert[n][3] = (2 * b) / 360;
            m_evert[n][4] = (a) / 360;
            n++;
            
            m_evert[n][0] = R * sin((a) / 180 * PI) * sin((b + POINT_SPACE) / 180 * PI) - H;
            m_evert[n][1] = R * cos((a) / 180 * PI) * sin((b + POINT_SPACE) / 180 * PI) + K;
            m_evert[n][2] = R * cos((b + POINT_SPACE) / 180 * PI) - Z;
            m_evert[n][3] = (2 * (b + POINT_SPACE)) / 360;
            m_evert[n][4] = (a) / 360;
            n++;
            
            m_evert[n][0] = R * sin((a + POINT_SPACE) / 180 * PI) * sin((b) / 180 * PI) - H;
            m_evert[n][1] = R * cos((a + POINT_SPACE) / 180 * PI) * sin((b) / 180 * PI) + K;
            m_evert[n][2] = R * cos((b) / 180 * PI) - Z;
            m_evert[n][3] = (2 * b) / 360;
            m_evert[n][4] = (a + POINT_SPACE) / 360;
            n++;
            
            m_evert[n][0] = R * sin((a + POINT_SPACE) / 180 * PI) * sin((b + POINT_SPACE) / 180 * PI) - H;
            m_evert[n][1] = R * cos((a + POINT_SPACE) / 180 * PI) * sin((b + POINT_SPACE) / 180 * PI) + K;
            m_evert[n][2] = R * cos((b + POINT_SPACE) / 180 * PI) - Z;
            m_evert[n][3] = (2 * (b + POINT_SPACE)) / 360;
            m_evert[n][4] = (a + POINT_SPACE) / 360;
            n++;
        }
    }
}

void mutil_rend_earth(GLuint texid)
{
    glRotatef(1.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texid);

    glBegin(GL_TRIANGLE_STRIP);

    for (int i = 0; i < NUM_EARTH_POINT; i++) {
        glTexCoord2f(m_evert[i][4], m_evert[i][3]);
        glVertex3f(m_evert[i][0], m_evert[i][1], -m_evert[i][2]);
    }

    for (int i = 0; i < NUM_EARTH_POINT; i++) {
        glTexCoord2f(m_evert[i][4], -m_evert[i][3]);
        glVertex3f(m_evert[i][0], m_evert[i][1], m_evert[i][2]);
    }
    
    glEnd();
}
