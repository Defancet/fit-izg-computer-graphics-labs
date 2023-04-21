/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Soubor obsahuje globalni promenne pro toto cviceni
 *
 * Opravy a modifikace:
 * -
 */

#ifndef Globals_H
#define Globals_H

# if SDL_BYTEORDER == SDL_BIG_ENDIAN
#  define RMASK 0xff000000
#  define GMASK 0x00ff0000
#  define BMASK 0x0000ff00
#  define AMASK 0x000000ff
# else
#  define RMASK 0x000000ff
#  define GMASK 0x0000ff00
#  define BMASK 0x00ff0000
#  define AMASK 0xff000000
# endif

/*****************************************************************************
 * Includes
 */
#include "vector.h"
#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Globalni promenne (definovane v main.c)
 */

/* kreslici buffer knihovny SDL */
extern SDL_Window			* screen;
extern SDL_Renderer         * renderer;
extern SDL_Surface          * screen_surface;

/* kreslici buffer IZG cviceni */
extern S_RGBA				* frame_buffer;

/* pomocna promenna pro ukonceni aplikace */
extern int					width;
extern int					height;

/* Kvalita generovane krivky - pocet segmentu - usecek, ze kterych je vykreslena */
extern int					quality;

extern S_Vector 			* control_points;
extern S_Vector				* curve_points;

#ifdef __cplusplus
}
#endif

#endif /* Globals_H */
/*****************************************************************************/
/*****************************************************************************/
