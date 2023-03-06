/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Soubor obsahuje globalni promenne pro toto cviceni
 *
 * Opravy a modifikace:
 * ipolasek@fit.vutbr.cz
 */

#ifndef Globals_H
#define Globals_H

/*****************************************************************************
 * Includes
 */

#include "SDL.h"

#include "rasterizer.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Globalni promenne (definovane v main.c)
 */

/* Vykreslovac knihovny SDL */
extern SDL_Renderer        * gRenderer;

/* kreslici buffer knihovny SDL */
extern SDL_Texture         * gTexture;

/* kreslici buffer IZG cviceni */
extern S_RGBA              * gFrameBuffer;

/* pomocna promenna pro ukonceni aplikace */
extern int                   gQuit;
extern int                   gWidth;
extern int                   gHeight;

/* Kamera pouzita pro projekce */
extern Camera                gCamera;

#ifdef __cplusplus
}
#endif

#endif /* Globals_H */
/*****************************************************************************/
/*****************************************************************************/