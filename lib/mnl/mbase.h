#ifndef __MBASE_H__
#define __MBASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>            /* strcasecmp() */
#include <stdbool.h>
#include <time.h>
#include <ctype.h>                /* isdigit() */
#include <inttypes.h>
#include <stdint.h>             /* uint32_t... */
#include <sys/types.h>
#include <sys/time.h>           /* gettimeofday() */
#include <dlfcn.h>                /* dlope()... */
#include <fcntl.h>                /* fcntl() */
#include <dirent.h>                /* scandir()... */
#include <iconv.h>
#include <signal.h>

#include <stdarg.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/resource.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#endif  /* __MBASE_H__ */
