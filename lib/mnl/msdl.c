#include "mheads.h"

void msdl_trace_info()
{
    mtc_dbg("OpenGL Info");
    mtc_dbg("Vendor: %s", glGetString(GL_VENDOR));
    mtc_dbg("Renderer: %s", glGetString(GL_RENDERER));
    mtc_dbg("Version: %s", glGetString(GL_VERSION));
    mtc_dbg("Shader Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    //mtc_dbg("OpenGL Extensions: %s", glGetString(GL_EXTENSIONS));
}
