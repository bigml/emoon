#include <stdio.h>
#include <unistd.h>

SDL_Texture* SDL_CreateTexture() 
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

int main(int argc, char *argv][])
{
        if (err != STATUS_OK) return nerr_pass(err);

    
    return 0;
}
