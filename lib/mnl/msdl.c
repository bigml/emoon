#include "mheads.h"

#define SDL_PROC(ret, func, params) ret (APIENTRY *func) params = NULL;
#include "msdl_glfuncs.h"
#undef SDL_PROC

int msdl_load_extensions()
{
#define SDL_PROC(ret, func, params)                                     \
    do {                                                                \
        func = SDL_GL_GetProcAddress(#func);                            \
        if (func == NULL) {                                             \
            mtc_dbg("Failed to load '%s', looking for '%s'...", #func, #func"EXT"); \
            func = SDL_GL_GetProcAddress(#func"EXT");                   \
        }                                                               \
        if (func == NULL) {                                             \
            mtc_dbg("Failed to load '%s', looking for '%s'...", #func"EXT", #func"ARB"); \
            func = SDL_GL_GetProcAddress(#func"ARB");                   \
        }                                                               \
        if (func == NULL) {                                             \
            mtc_err("Failed to load OpenGL '%s'. Use it will crash moon", #func); \
            return 1;                                                   \
        }                                                               \
    } while (0);
#include "msdl_glfuncs.h"
#undef SDL_PROC
    return 0;
}

void msdl_trace_info()
{
    mtc_dbg("OpenGL Info");
    mtc_dbg("Vendor: %s", glGetString(GL_VENDOR));
    mtc_dbg("Renderer: %s", glGetString(GL_RENDERER));
    mtc_dbg("Version: %s", glGetString(GL_VERSION));
    mtc_dbg("Shader Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    //mtc_dbg("OpenGL Extensions: %s", glGetString(GL_EXTENSIONS));
}
