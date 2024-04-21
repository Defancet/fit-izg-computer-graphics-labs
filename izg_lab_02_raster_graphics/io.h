/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id:$
 *
 * Popis:
 *    soubor obsahuje funkce pro zapis a cteni frame bufferu z/do souboru
 *
 * Opravy a modifikace:
 * ipolasek@fit.vutbr.cz
 */

#ifndef IO_H
#define IO_H

/*****************************************************************************
 * Includes
 */

#include "color.h"
#include "rasterizer.h"

#include <string>

/*****************************************************************************
 * Funkce pro cteni/zapis do/z souboru
 */

/* Nacteni BMP souboru do gFrameBuffer struktury */
int loadMyBitmap(const std::string &filename, S_RGBA** frame_buffer,int *width, int *height);

/* Ulozeni BMP souboru z gFrameBuffer struktury */
int saveMyBitmap(const std::string &filename, S_RGBA** frame_buffer,int width, int height);

/* Vykresleni usecek ze souboru */
bool drawLinesFromFile(const std::string &filename);

/* Vykresleni testovaciho vzoru na pozici [posX, posY] o prumeru diameter */
void drawLineTestPattern(int posX, int posY, float diameter);

/* Vykresleni testovaciho vzoru */
void drawLineTestPatternAlt();

/* Nacte model krychle */
Model loadPolygonalCube();

/* Nacte model cuboctahedronu */
Model loadPolygonalCuboctahedron();

/* Nacte model ve formatu OBJ */
Model loadPolygonalModel(const std::string &filename);

/* Vykresli polygonalni model z pohledu dane kamery */
bool drawPolygonalModel(const Model &model, 
    S_RGBA lineColor, S_RGBA circleColor, 
    const Camera &camera = { });

/* Vycisti kreslici plochu a nastavi ji na konstantni barvu */
void clearRaster(S_RGBA color);

#endif

/*****************************************************************************/
/*****************************************************************************/
