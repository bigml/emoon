#include "mheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

#define WIN_WIDTH 640
#define WIN_HEIGHT 420

#define POINT_SPACE 10
#define NUM_EARTH_POINT (90 / POINT_SPACE) * (360 / POINT_SPACE) * 4

SDL_Renderer *m_render = NULL;
vec m_evert[NUM_EARTH_POINT][5];

GLuint LoadTextureRAW(const char *filename)
{
    GLuint rtex;
    int width, height;
    unsigned char *data;
    size_t len;
    FILE *file;

    file = fopen(filename, "rb");
    if (!file) return 0;
    
    width = 1024, height = 512; len = width * height *3;
    data = calloc(1, len);
    fread(data, len, 1, file);
    fclose(file);

    glGenTextures(1, &rtex);
    glBindTexture(GL_TEXTURE_2D, rtex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);
    return rtex;
}

static void set_glversion()
{
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
}

static void gl_info( void )
{
    const char* renderer     = (const char*) glGetString(GL_RENDERER);
    const char* version      = (const char*) glGetString(GL_VERSION);
    const char* glsl_version = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    fprintf( stdout, "[GL] Renderer: %s\n", renderer ? renderer : "unknown" );
    fprintf( stdout, "[GL] Version: %s\n", version ? version : "unknown" );
    fprintf( stdout, "[GL] Shading Language: %s\n",
             glsl_version ? glsl_version : "unknown" );
}

void rend_sun()
{
    glClearDepth(1);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void create_earth(double R, double H, double K, double Z)
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

void rend_earth(GLuint etex)
{
    int i = 0;
    static double angle = 0.0;

    glRotatef(1.0, 0.0, 1.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, etex);

    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < NUM_EARTH_POINT; i++) {
        glTexCoord2f(m_evert[i][4], m_evert[i][3]);
        glVertex3f(m_evert[i][0], m_evert[i][1], -m_evert[i][2]);
    }

    for (i = 0; i < NUM_EARTH_POINT; i++) {
        glTexCoord2f(m_evert[i][4], -m_evert[i][3]);
        glVertex3f(m_evert[i][0], m_evert[i][1], m_evert[i][2]);
    }

    glEnd();
}

int main(int argc, char **argv)
{
    mtc_init("planet", 7);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        mtc_err("sdl init error: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        mtc_err("ttf init error: %s", TTF_GetError());
        return 1;
    }

	SDL_Window *win = SDL_CreateWindow("Hello OpenGL!",
                                       100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!win) {
        mtc_err("SDL_CreateWindow Error: %s", SDL_GetError());
		return 1;
	}

	m_render = SDL_CreateRenderer(win, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
	if (!m_render) {
        mtc_err("SDL_CreateRenderer Error: %s", SDL_GetError());
		return 1;
	}

    set_glversion();
    gl_info();

    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_TEXTURE_2D );
    glDepthFunc(GL_LEQUAL);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);

    GLuint etex = LoadTextureRAW("earth.raw");
    if (etex == 0) {
        mtc_err("Load texture failure");
        return 1;
    }

    create_earth(5, 0, 0, 0);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

        rend_sun();
        rend_earth(etex);

        SDL_RenderPresent(m_render);
    }
    
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(win);
	SDL_Quit();

    return 0;
}
