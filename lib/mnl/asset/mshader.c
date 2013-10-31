#include "mheads.h"

static ShaderAsset* mshader_node_new(GLenum type)
{
    ShaderAsset *sha = calloc(1, sizeof(ShaderAsset));

    sha->shader = glCreateShader(type);

    return sha;
}

GLuint mast_shader_prog_new()
{
    return glCreateProgram();
}

NEOERR* mast_shader_prog_attach_shader(GLuint prog, RendAsset *a)
{
    MCS_NOT_NULLA(a);

    ShaderAsset *ast = (ShaderAsset*)a;
    
    if (!glIsProgram(prog)) return nerr_raise(NERR_ASSERT, "not a shader prog");
    if (!glIsShader(ast->shader)) return nerr_raise(NERR_ASSERT, "not a shader");

    glAttachShader(prog, ast->shader);

    return STATUS_OK;
}

NEOERR* mast_shader_prog_link(GLuint prog)
{
    if (!glIsProgram(prog)) return nerr_raise(NERR_ASSERT, "not a shader prog");

    glLinkProgram(prog);

    GLint linked = false;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (!linked) return nerr_raise(NERR_ASSERT, "error link");

    return STATUS_OK;
}

void mast_shader_prog_free(GLuint prog)
{
    glDeleteProgram(prog);
}

NEOERR* mast_shader_load(char *dir, char *name, GLenum type, RendAsset **a)
{
    char fname[PATH_MAX], *buf;
    NEOERR *err;
    
    MCS_NOT_NULLB(name, a);

    if (dir) snprintf(fname, sizeof(fname), "%s%s", dir, name);
    else strncpy(fname, name, sizeof(fname));

    err = ne_load_file(fname, &buf);
    if (err != STATUS_OK) return nerr_pass(err);

    ShaderAsset *ast = mshader_node_new(type);
    if (!ast) return nerr_raise(NERR_NOMEM, "alloc shader");

    glShaderSource(ast->shader, 1, (const char**)&buf, NULL);
    glCompileShader(ast->shader);

    int compileok = 0;
    glGetShaderiv(ast->shader, GL_COMPILE_STATUS, &compileok);
    if (compileok == GL_FALSE) return nerr_raise(NERR_ASSERT, "compile shader");

    SAFE_FREE(buf);

    *a = (RendAsset*)ast;

    return STATUS_OK;
}

NEOERR* mast_vs_load(char *dir, char *name, RendAsset **a)
{
    return mast_shader_load(dir, name, GL_VERTEX_SHADER, a);
}

NEOERR* mast_fs_load(char *dir, char *name, RendAsset **a)
{
    return mast_shader_load(dir, name, GL_FRAGMENT_SHADER, a);
}

void mast_shader_unload(void *p)
{
    if (!p) return;

    ShaderAsset *a = p;

    glDeleteShader(a->shader);
}
