#ifndef __MSDL_H__
#define __MSDL_H__

#include "mheads.h"

#define SDL_PROC(ret, func, params) ret (APIENTRY *func) params;
#include "msdl_glfuncs.h"
#undef SDL_PROC

__BEGIN_DECLS

int  msdl_load_extensions();
void msdl_trace_info();

__END_DECLS
#endif    /* __MSDL_H__ */
