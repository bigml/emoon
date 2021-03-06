#include "mheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

#define WIN_WIDTH 640
#define WIN_HEIGHT 420

SDL_Renderer *m_render = NULL;
SDL_Renderer *m_render2 = NULL;

SDL_Texture* create_texture(char *file)
{
    char fname[1024];
    snprintf(fname, sizeof(fname), "studyres/%s", file);
    
    SDL_Texture *tex = IMG_LoadTexture(m_render, fname);
	if (!tex) {
        mtc_err("SDL_LoadBMP Error: %s", SDL_GetError());
		return NULL;
	}

    return tex;
}

SDL_Texture* create_texture_str(char *msg, SDL_Color color, int fontsize)
{
    TTF_Font *font = TTF_OpenFont("studyres/a.ttf", fontsize);
    if (!font) {
        mtc_err("create font error %s", TTF_GetError());
        return NULL;
    }

    SDL_Surface *sur = TTF_RenderText_Blended(font, msg, color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(m_render2, sur);

    SDL_FreeSurface(sur);
    TTF_CloseFont(font);

    return tex;
}

void rend_texture(int x, int y, SDL_Texture *tex)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
    SDL_RenderCopy(m_render, tex, NULL, &pos);
}

void rend2_texture(int x, int y, SDL_Texture *tex)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
    SDL_RenderCopy(m_render2, tex, NULL, &pos);
}

void rend_texture_clip(int x, int y, SDL_Texture *tex, SDL_Rect *clip)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);

    SDL_RenderCopy(m_render, tex, clip, &pos);
}


int main(int argc, char **argv)
{
    mtc_init("study", 7);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        mtc_err("sdl init error: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        mtc_err("ttf init error: %s", TTF_GetError());
        return 1;
    }
    
	SDL_Window *win = SDL_CreateWindow("Hello World!",
                                       100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!win) {
        mtc_err("SDL_CreateWindow Error: %s", SDL_GetError());
		return 1;
	}
    
	m_render = SDL_CreateRenderer(win, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
	m_render2 = SDL_CreateRenderer(win, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
	if (!m_render || !m_render2) {
        mtc_err("SDL_CreateRenderer Error: %s", SDL_GetError());
		return 1;
	}

    SDL_Texture *texbg = create_texture("bg.png");
    SDL_Texture *texsp = create_texture("sprite.png");

    int iw = 100, ih = 100;
    SDL_Rect clips[4];
    for (int i = 0; i < 4; i++) {
        clips[i].x = i / 2 * iw;
        clips[i].y = i % 2 * ih;
        clips[i].w = iw;
        clips[i].h = ih;
    }

    SDL_Event e;
    bool quit = false;
    int curclip = 0, count = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    curclip = 0;
                    break;
                case SDLK_DOWN:
                    curclip = 1;
                    break;
                case SDLK_LEFT:
                    curclip = 2;
                    break;
                case SDLK_RIGHT:
                    curclip = 3;
                    break;
                }
            }
        }

        SDL_Color color = {200, 200, 200};
        char msg[1024];
        snprintf(msg, sizeof(msg), "hello, bigml, ESC to quit %d", count++);
        SDL_Texture *texfont = create_texture_str(msg, color, 48);

        SDL_RenderClear(m_render);
        SDL_RenderClear(m_render2);
        rend_texture(0, 0, texbg);
        rend2_texture(20, 20, texfont);
        rend_texture_clip(WIN_WIDTH/3, WIN_HEIGHT/3, texsp, &clips[curclip]);
        SDL_RenderPresent(m_render);
        SDL_RenderPresent(m_render2);

        SDL_DestroyTexture(texfont);
    }

	//Clean up our objects and quit
	SDL_DestroyTexture(texbg);
    SDL_DestroyTexture(texsp);
	SDL_DestroyRenderer(m_render);
	SDL_DestroyRenderer(m_render2);
	SDL_DestroyWindow(win);
	SDL_Quit();

    return 0;
}
