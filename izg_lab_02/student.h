/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * isolony@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * ipolasek@fit.vutbr.cz
 */

#ifndef Student_H
#define Student_H

/*****************************************************************************
 * Includes
 */
#include "color.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Ukoly pro cviceni: 
 */

/* Vykresli usecku od [x1, y1] po [x2, y2] vcetne koncovych bodu */
void rasterizeLine(int x1, int y1, int x2, int y2, S_RGBA color = COLOR_GREEN);

/* Vykresli kruznici se stredem v [sx, sy] o polomeru radius */
void rasterizeCircle(int sx, int sy, float radius, S_RGBA color = COLOR_RED);

/*****************************************************************************/


/*****************************************************************************
 * Pomocne funkce: 
 */

/**
 * Provede vykresleni pixelu na dane pozici ve vystupnim rasteru. Pokud je
 * souradnice mimo hranice rasteru, potom tato funkce neprovede zadnou zmenu.
 */
void setPixelColor(int x, int y, S_RGBA color);
/** 
 * Vyčte barvu pixelu na dane pozici ve vystupnim rasteru. Pokud je souradnice
 * mimo hranice rasteru, potom funkce vraci barvu (0u, 0u, 0u, 255u).
 */
S_RGBA getPixelColor(int x, int y);

/*****************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* Student_H */

/*****************************************************************************/
/*****************************************************************************/