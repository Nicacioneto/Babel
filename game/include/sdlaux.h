#ifndef SDL_AUX_H
#define SDL_AUX_H

#include <SDL/SDL.h>

extern Uint32 getpixel(SDL_Surface *surface, int x, int y);
extern void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif
