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

#if 0
void msdl_load_extensions()
{
    SDL_GL_LoadExtension(GLPROGRAMPARAMETERIFN, glProgramParameteri);
    SDL_GL_LoadExtension(GLGENERATEMIPMAPFN, glGenerateMipmap);
    
    SDL_GL_LoadExtension(GLGENFRAMEBUFFERSFN, glGenFramebuffers);
    SDL_GL_LoadExtension(GLBINDFRAMEBUFFERFN, glBindFramebuffer);
    SDL_GL_LoadExtension(GLGENRENDERBUFFERSFN, glGenRenderbuffers);
    SDL_GL_LoadExtension(GLBINDRENDERBUFFERFN, glBindRenderbuffer);
    SDL_GL_LoadExtension(GLRENDERBUFFERSTORAGEFN, glRenderbufferStorage);
    SDL_GL_LoadExtension(GLFRAMEBUFFERRENDERBUFFERFN, glFramebufferRenderbuffer);
    SDL_GL_LoadExtension(GLFRAMEBUFFERTEXTURE2DFN, glFramebufferTexture2D);
    SDL_GL_LoadExtension(GLCHECKFRAMEBUFFERSTATUSFN, glCheckFramebufferStatus);
    SDL_GL_LoadExtension(GLDELETEFRAMEBUFFERSFN, glDeleteFramebuffers);
    SDL_GL_LoadExtension(GLDELETERENDERBUFFERSFN, glDeleteRenderbuffers);
    
    SDL_GL_LoadExtension(GLRENDERBUFFERSTORAGEMULTISAMPLEFN,
                         glRenderbufferStorageMultisample);
    SDL_GL_LoadExtension(GLBLITFRAMEBUFFERFN, glBlitFramebuffer);
    SDL_GL_LoadExtension(GLPATCHPARAMETERIFN, glPatchParameteri);
    SDL_GL_LoadExtension(GLDRAWELEMENTSINSTANCEDFN, glDrawElementsInstanced);
}
#endif

void msdl_trace_info()
{
    mtc_dbg("OpenGL Info");
    mtc_dbg("Vendor: %s", glGetString(GL_VENDOR));
    mtc_dbg("Renderer: %s", glGetString(GL_RENDERER));
    mtc_dbg("Version: %s", glGetString(GL_VERSION));
    mtc_dbg("Shader Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    //mtc_dbg("OpenGL Extensions: %s", glGetString(GL_EXTENSIONS));
}
