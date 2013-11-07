#include "mheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

#define WIN_WIDTH 640
#define WIN_HEIGHT 420

SDL_Renderer *m_render = NULL;

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

static void Render()
{
    static float color[8][3] = {
        {1.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 0.0, 1.0},
        {0.0, 0.0, 1.0}
    };
    static float cube[8][3] = {
        {0.5, 0.5, -0.5},
        {0.5, -0.5, -0.5},
        {-0.5, -0.5, -0.5},
        {-0.5, 0.5, -0.5},
        {-0.5, 0.5, 0.5},
        {0.5, 0.5, 0.5},
        {0.5, -0.5, 0.5},
        {-0.5, -0.5, 0.5}
    };

    /* Do our drawing, too. */
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);

#ifdef SHADED_CUBE
    glColor3fv(color[0]);
    glVertex3fv(cube[0]);
    glColor3fv(color[1]);
    glVertex3fv(cube[1]);
    glColor3fv(color[2]);
    glVertex3fv(cube[2]);
    glColor3fv(color[3]);
    glVertex3fv(cube[3]);

    glColor3fv(color[3]);
    glVertex3fv(cube[3]);
    glColor3fv(color[4]);
    glVertex3fv(cube[4]);
    glColor3fv(color[7]);
    glVertex3fv(cube[7]);
    glColor3fv(color[2]);
    glVertex3fv(cube[2]);

    glColor3fv(color[0]);
    glVertex3fv(cube[0]);
    glColor3fv(color[5]);
    glVertex3fv(cube[5]);
    glColor3fv(color[6]);
    glVertex3fv(cube[6]);
    glColor3fv(color[1]);
    glVertex3fv(cube[1]);

    glColor3fv(color[5]);
    glVertex3fv(cube[5]);
    glColor3fv(color[4]);
    glVertex3fv(cube[4]);
    glColor3fv(color[7]);
    glVertex3fv(cube[7]);
    glColor3fv(color[6]);
    glVertex3fv(cube[6]);

    glColor3fv(color[5]);
    glVertex3fv(cube[5]);
    glColor3fv(color[0]);
    glVertex3fv(cube[0]);
    glColor3fv(color[3]);
    glVertex3fv(cube[3]);
    glColor3fv(color[4]);
    glVertex3fv(cube[4]);

    glColor3fv(color[6]);
    glVertex3fv(cube[6]);
    glColor3fv(color[1]);
    glVertex3fv(cube[1]);
    glColor3fv(color[2]);
    glVertex3fv(cube[2]);
    glColor3fv(color[7]);
    glVertex3fv(cube[7]);
#else /* flat cube */
    glColor3f(1.0, 0.0, 0.0);
    glVertex3fv(cube[0]);
    glVertex3fv(cube[1]);
    glVertex3fv(cube[2]);
    glVertex3fv(cube[3]);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(cube[3]);
    glVertex3fv(cube[4]);
    glVertex3fv(cube[7]);
    glVertex3fv(cube[2]);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(cube[0]);
    glVertex3fv(cube[5]);
    glVertex3fv(cube[6]);
    glVertex3fv(cube[1]);

    glColor3f(0.0, 1.0, 1.0);
    glVertex3fv(cube[5]);
    glVertex3fv(cube[4]);
    glVertex3fv(cube[7]);
    glVertex3fv(cube[6]);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3fv(cube[5]);
    glVertex3fv(cube[0]);
    glVertex3fv(cube[3]);
    glVertex3fv(cube[4]);

    glColor3f(1.0, 0.0, 1.0);
    glVertex3fv(cube[6]);
    glVertex3fv(cube[1]);
    glVertex3fv(cube[2]);
    glVertex3fv(cube[7]);
#endif /* SHADED_CUBE */

    glEnd();

    glMatrixMode(GL_MODELVIEW);
    glRotatef(5.0, 1.0, 1.0, 1.0);
}

static void render_point()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(20.0);
    
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    //glRotatef(5.0, 1.0, 1.0, 1.0);
}

static void render_line()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(20.0);
    glEnable(GL_LINE_STIPPLE);
    //glLineStipple(1, 0x3f07);
    
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 1.0, 0.5);
    glEnd();

    //glRotatef(5.0, 1.0, 1.0, 1.0);
}

static void render_polygon()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
        
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(2.0, 5.0, -3.0);
    glVertex3f(0.0, 5.0, -3.0);
    glVertex3f(0.0, 0.0, -3.0);
    glVertex3f(2.0, 0.0, -3.0);
    glEnd();

    //glMatrixMode(GL_MODELVIEW);
    //glTranslatef(0.1, 0.0, 0.0);
    //glRotatef(1.0, 0.0, 0.0, 1.0);
    static int i = 0;

    i++;

    if (i % 2 == 0) {
        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    } else {
        glViewport(0, 0, WIN_WIDTH/2, WIN_HEIGHT);
    }
}

static GLuint VertexVBOID = 0;
static GLuint IndexVBOID = 0;
typedef struct
{
    float x, y, z;        //Vertex
    float nx, ny, nz;     //Normal
    float s0, t0;         //Texcoord0
} MyVertex;
static void vbo_init()
{
 
  MyVertex pvertex[3];
  //VERTEX 0
  pvertex[0].x = 0.0;
  pvertex[0].y = 0.0;
  pvertex[0].z = 0.0;
  pvertex[0].nx = 0.0;
  pvertex[0].ny = 0.0;
  pvertex[0].nz = 1.0;
  pvertex[0].s0 = 0.0;
  pvertex[0].t0 = 0.0;
  //VERTEX 1
  pvertex[1].x = 1.0;
  pvertex[1].y = 0.0;
  pvertex[1].z = 0.0;
  pvertex[1].nx = 0.0;
  pvertex[1].ny = 0.0;
  pvertex[1].nz = 1.0;
  pvertex[1].s0 = 1.0;
  pvertex[1].t0 = 0.0;
  //VERTEX 2
  pvertex[2].x = 0.0;
  pvertex[2].y = 1.0;
  pvertex[2].z = 0.0;
  pvertex[2].nx = 0.0;
  pvertex[2].ny = 0.0;
  pvertex[2].nz = 1.0;
  pvertex[2].s0 = 0.0;
  pvertex[2].t0 = 1.0;
 
  glGenBuffers(1, &VertexVBOID);
  glGenBuffers(1, &IndexVBOID);

  glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*3, &pvertex[0].x, GL_STATIC_DRAW);
 
  ushort pindices[3];
  pindices[0] = 0;
  pindices[1] = 1;
  pindices[2] = 2;
 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort)*3, pindices, GL_STATIC_DRAW);
    
}

#define BUFFER_OFFSET(i) ((void*)(i))
static void render_vbo()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(0));   //The starting point of the VBO, for the vertices

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(12));   //The starting point of normals, 12 bytes away

  glClientActiveTexture(GL_TEXTURE0);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(MyVertex), BUFFER_OFFSET(24));   //The starting point of texcoords, 24 bytes away
 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
  //To render, we can either use glDrawElements or glDrawRangeElements
  //The is the number of indices. 3 indices needed to make a single triangle
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
  //0 and 3 are the first and last vertices
  //glDrawRangeElements(GL_TRIANGLES, 0, 3, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));   //The starting point of the IBO
  //glDrawRangeElements may or may not give a performance advantage over glDrawElements
}

int main(int argc, char **argv)
{
    mtc_init("gl", 7);

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
    vbo_init();

#if 1
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
    //glMatrixMode(GL_PROJECTION);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glShadeModel(GL_SMOOTH);
#endif


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

        //Render();
        render_vbo();

        SDL_RenderPresent(m_render);
    }
    
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(win);
	SDL_Quit();

    return 0;
}
