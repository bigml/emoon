/*
 * viki 主脑
 */

#include "mheads.h"
#include "lheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

int main(int argc, char **argv)
{
    NEOERR *err;
    
    msys_makesure_coredump();

    err = mcfg_parse_file(APPCONFIG, &g_cfg);
    OUTPUT_NOK(err);

    err = lerr_init();
    DIE_NOK(err);
    
    mtc_init(hdf_get_value(g_cfg, PRE_CONFIG".trace_file", "viki"),
             hdf_get_int_value(g_cfg, PRE_CONFIG".trace_level", TC_DEFAULT_LEVEL));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        mtc_err("sdl init error: %s", SDL_GetError());
        return 1;
    }
    
	SDL_Window *win = SDL_CreateWindow("Hello World!",
                                       100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (!win) {
        mtc_err("SDL_CreateWindow Error: %s", SDL_GetError());
		return 1;
	}
    
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
        mtc_err("SDL_CreateRenderer Error: %s", SDL_GetError());
		return 1;
	}

	//SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
	//this lets us choose when to upload or remove textures from the GPU
	SDL_Surface *bmp = SDL_LoadBMP(PATH_FLY"hello.bmp");
	if (!bmp) {
        mtc_err("SDL_LoadBMP Error: %s", SDL_GetError());
		return 1;
	}

	//To use a hardware accelerated texture for rendering we can create one from
	//the surface we loaded
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	//We no longer need the surface
	SDL_FreeSurface(bmp);
	if (!tex) {
        mtc_err("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
		return 1;
	}

	//Now lets draw our image
	//First clear the renderer
	SDL_RenderClear(ren);
	//Draw the texture
	SDL_RenderCopy(ren, tex, NULL, NULL);
	//Update the screen
	SDL_RenderPresent(ren);

	//Have the program wait for 2000ms so we get a chance to see the screen
	SDL_Delay(2000);

	//Clean up our objects and quit
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

    mcfg_cleanup(&g_cfg);

    return 0;
}
