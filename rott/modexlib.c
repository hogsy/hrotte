/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <stdlib.h>
#include <sys/stat.h>
#include "modexlib.h"
#include "rt_util.h"
#include "rt_net.h" // for GamePaused
#include "myprint.h"

static void StretchMemPicture();
// GLOBAL VARIABLES

boolean StretchScreen = 0;//bn�++
extern boolean iG_aimCross;
extern boolean sdl_fullscreen;
extern int iG_X_center;
extern int iG_Y_center;
char * iG_buf_center;

int linewidth;
//int    ylookup[MAXSCREENHEIGHT];
int ylookup[600];//just set to max res
byte * page1start;
byte * page2start;
byte * page3start;
int screensize;
byte * bufferofs;
byte * displayofs;
boolean graphicsmode = false;
char * bufofsTopLimit;
char * bufofsBottomLimit;

void DrawCenterAim();

#include <SDL2/SDL.h>

/*
====================
=
= GraphicsMode
=
====================
*/

static SDL_Window * sdlWindow = NULL;
static SDL_Renderer * sdlRenderer = NULL;
static SDL_Texture * sdlTexture = NULL;
static SDL_Surface * sdlPalSurface = NULL;
static SDL_Surface * sdlSurface = NULL;

SDL_Surface * GetSdlSurface( void ) {
	return sdlPalSurface;
}

static boolean filter = false;
void VL_SetFilterMode( boolean linear ) {
	filter = linear;

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, linear ? "linear" : "nearest" );

	// Window probably hasn't been created yet
	if ( sdlRenderer == NULL) {
		return;
	}

	if ( sdlTexture != NULL) {
		SDL_DestroyTexture( sdlTexture );
		sdlTexture = NULL;
	}

	sdlTexture = SDL_CreateTextureFromSurface( sdlRenderer, sdlSurface );
	if ( sdlTexture == NULL) {
		Error( "Failed to create SDL texture!\nSDL: %s\n", SDL_GetError());
	}
}

boolean VL_GetFilterMode( void ) {
	return filter;
}

void VL_SetFullscreenMode( boolean fullscreen ) {
	sdl_fullscreen = fullscreen;

	// Window probably hasn't been created yet
	if ( sdlWindow == NULL) {
		return;
	}

	SDL_SetWindowFullscreen( sdlWindow, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 );

	// Always reset to the middle of the screen, otherwise on platforms such as Ubuntu
	// the window will get stuck under the side-menu :(
	SDL_SetWindowPosition( sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void GraphicsMode( void ) {
	if ( SDL_InitSubSystem( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
		Error( "Could not initialize SDL!\nSDL: %s\n", SDL_GetError());
	}

	char windowTitle[32];
	snprintf( windowTitle, sizeof( windowTitle ), "hrotte %s (%s-%s)", GIT_COMMIT_COUNT, GIT_BRANCH, GIT_COMMIT_HASH );

	sdlWindow = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT,
		sdl_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
	);
	if ( sdlWindow == NULL) {
		Error( "Failed to create SDL window!\nSDL: %s\n", SDL_GetError());
	}

	// Setup the renderer

	sdlRenderer = SDL_CreateRenderer( sdlWindow, -1, 0 );
	if ( sdlRenderer == NULL) {
		Error( "Failed to create SDL renderer!\nSDL: %s\n", SDL_GetError());
	}

	SDL_RenderSetLogicalSize( sdlRenderer, iGLOBAL_SCREENWIDTH, iGLOBAL_SCREENHEIGHT );

	// Setup the texture and surface

	sdlPalSurface = SDL_CreateRGBSurfaceWithFormat(
		0,
		iGLOBAL_SCREENWIDTH,
		iGLOBAL_SCREENHEIGHT,
		8,
		SDL_PIXELFORMAT_INDEX8
	);
	if ( sdlPalSurface == NULL) {
		Error( "Failed to create SDL surface!\nSDL: %s\n", SDL_GetError());
	}

	sdlSurface = SDL_CreateRGBSurfaceWithFormat(
		0,
		iGLOBAL_SCREENWIDTH,
		iGLOBAL_SCREENHEIGHT,
		32,
		SDL_PIXELFORMAT_BGRA32
	);
	if ( sdlSurface == NULL) {
		Error( "Failed to create SDL surface!\nSDL: %s\n", SDL_GetError());
	}

	sdlTexture = SDL_CreateTextureFromSurface( sdlRenderer, sdlSurface );
	if ( sdlTexture == NULL) {
		Error( "Failed to create SDL texture!\nSDL: %s\n", SDL_GetError());
	}

	SDL_SetRelativeMouseMode( true );
}

/*
====================
=
= SetTextMode
=
====================
*/
void SetTextMode( void ) {
	if ( SDL_WasInit( SDL_INIT_VIDEO ) == SDL_INIT_VIDEO ) {
		if ( sdlWindow != NULL) {
			SDL_DestroyWindow( sdlWindow );
			sdlWindow = NULL;
		}

		SDL_QuitSubSystem( SDL_INIT_VIDEO );
	}
}

/*
====================
=
= TurnOffTextCursor
=
====================
*/
void TurnOffTextCursor( void ) {
}

/*
====================
=
= WaitVBL
=
====================
*/
void WaitVBL( void ) {
	SDL_Delay( 16667 / 1000 );
}

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode( void ) {
	int i, offset;

	GraphicsMode();

//
// set up lookup tables
//
//bna--   linewidth = 320;
	linewidth = iGLOBAL_SCREENWIDTH;

	offset = 0;

	for ( i = 0; i < iGLOBAL_SCREENHEIGHT; i++ ) {
		ylookup[i] = offset;
		offset += linewidth;
	}

//    screensize=MAXSCREENHEIGHT*MAXSCREENWIDTH;
	screensize = iGLOBAL_SCREENHEIGHT * iGLOBAL_SCREENWIDTH;

	page1start = sdlPalSurface->pixels;
	page2start = sdlPalSurface->pixels;
	page3start = sdlPalSurface->pixels;
	displayofs = page1start;
	bufferofs = page2start;

	iG_X_center = iGLOBAL_SCREENWIDTH / 2;
	iG_Y_center = ( iGLOBAL_SCREENHEIGHT / 2 ) + 10;//+10 = move aim down a bit

	iG_buf_center = bufferofs + ( screensize / 2 );//(iG_Y_center*iGLOBAL_SCREENWIDTH);//+iG_X_center;

	bufofsTopLimit = bufferofs + screensize - iGLOBAL_SCREENWIDTH;
	bufofsBottomLimit = bufferofs + iGLOBAL_SCREENWIDTH;

	// start stretched
	EnableScreenStretch();
	XFlipPage();
}

/*
=======================
=
= VL_CopyPlanarPage
=
=======================
*/
void VL_CopyPlanarPage( byte * src, byte * dest ) {
	memcpy( dest, src, screensize );
}

/*
=======================
=
= VL_CopyPlanarPageToMemory
=
=======================
*/
void VL_CopyPlanarPageToMemory( byte * src, byte * dest ) {
	memcpy( dest, src, screensize );
}

/*
=======================
=
= VL_CopyBufferToAll
=
=======================
*/
void VL_CopyBufferToAll( byte * buffer ) {}

/*
=======================
=
= VL_CopyDisplayToHidden
=
=======================
*/
void VL_CopyDisplayToHidden( void ) {
	VL_CopyBufferToAll( displayofs );
}

/*
=================
=
= VL_ClearBuffer
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearBuffer( byte * buf, byte color ) {
	memset(( byte * ) buf, color, screensize );
}

/*
=================
=
= VL_ClearVideo
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearVideo( byte color ) {
	SDL_SetRenderDrawColor( sdlRenderer, color, color, color, 255 );
	SDL_RenderClear( sdlRenderer );
}

/*
=================
=
= VL_DePlaneVGA
=
=================
*/

void VL_DePlaneVGA( void ) {}

/* C version of rt_vh_a.asm */

void VH_UpdateScreen( void ) {
	DrawCenterAim();

	// Convert palette surface
	typedef struct { unsigned char b, g, r, a; } rgb_t;
	rgb_t * rgb = sdlSurface->pixels;
	for ( unsigned int i = 0; i < sdlPalSurface->pitch * iGLOBAL_SCREENHEIGHT; ++i ) {
		int index = (( uint8_t * ) ( sdlPalSurface->pixels ))[i];
		rgb->r = sdlPalSurface->format->palette->colors[index].r;
		rgb->g = sdlPalSurface->format->palette->colors[index].g;
		rgb->b = sdlPalSurface->format->palette->colors[index].b;
		rgb->a = 255;
		rgb++;
	}

	SDL_UpdateTexture( sdlTexture, NULL, sdlSurface->pixels, sdlSurface->pitch );

	SDL_RenderClear( sdlRenderer );
	SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);
	SDL_RenderPresent( sdlRenderer );
}

/*
=================
=
= XFlipPage
=
=================
*/

void XFlipPage( void ) {
	VH_UpdateScreen();
}

// todo: remove both of these...
void EnableScreenStretch( void ) {}
void DisableScreenStretch( void ) {}

// bna section -------------------------------------------

// bna function added start
extern boolean ingame;
int iG_playerTilt;

void DrawCenterAim() {
	int x;

	int percenthealth = ( locplayerstate->health * 10 ) / MaxHitpointsForCharacter( locplayerstate );
	int color = percenthealth < 3 ? egacolor[RED] : percenthealth < 4 ? egacolor[YELLOW] : egacolor[GREEN];

	if ( iG_aimCross && !GamePaused ) {
		if (( ingame == true ) && ( iGLOBAL_SCREENWIDTH > 320 )) {
			if (( iG_playerTilt < 0 ) || ( iG_playerTilt > iGLOBAL_SCREENHEIGHT / 2 )) {
				iG_playerTilt = -( 2048 - iG_playerTilt );
			}
			if ( iGLOBAL_SCREENWIDTH == 640 ) {
				x = iG_playerTilt;
				iG_playerTilt = x / 2;
			}
			iG_buf_center = bufferofs + (( iG_Y_center - iG_playerTilt ) * iGLOBAL_SCREENWIDTH );//+iG_X_center;

			for ( x = iG_X_center - 10; x <= iG_X_center - 4; x++ ) {
				if (( iG_buf_center + x < bufofsTopLimit ) && ( iG_buf_center + x > bufofsBottomLimit )) {
					*( iG_buf_center + x ) = color;
				}
			}
			for ( x = iG_X_center + 4; x <= iG_X_center + 10; x++ ) {
				if (( iG_buf_center + x < bufofsTopLimit ) && ( iG_buf_center + x > bufofsBottomLimit )) {
					*( iG_buf_center + x ) = color;
				}
			}
			for ( x = 10; x >= 4; x-- ) {
				if ((( iG_buf_center - ( x * iGLOBAL_SCREENWIDTH ) + iG_X_center ) < bufofsTopLimit )
					&& (( iG_buf_center - ( x * iGLOBAL_SCREENWIDTH ) + iG_X_center ) > bufofsBottomLimit )) {
					*( iG_buf_center - ( x * iGLOBAL_SCREENWIDTH ) + iG_X_center ) = color;
				}
			}
			for ( x = 4; x <= 10; x++ ) {
				if ((( iG_buf_center + ( x * iGLOBAL_SCREENWIDTH ) + iG_X_center ) < bufofsTopLimit )
					&& (( iG_buf_center + ( x * iGLOBAL_SCREENWIDTH ) + iG_X_center ) > bufofsBottomLimit )) {
					*( iG_buf_center + ( x * iGLOBAL_SCREENWIDTH ) + iG_X_center ) = color;
				}
			}
		}
	}
}
// bna function added end

// bna section -------------------------------------------
