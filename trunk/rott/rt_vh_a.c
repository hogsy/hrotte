#include "rt_def.h"

#include "rt_vh_a.h"

/* C version of rt_vh_a.asm */

void VH_UpdateScreen (void)
{
	SDL_UpdateRect (SDL_GetVideoSurface (), 0, 0, 0, 0);
}
