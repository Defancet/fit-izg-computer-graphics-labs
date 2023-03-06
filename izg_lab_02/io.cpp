/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * Popis:
 *    soubor obsahuje funkce pro zapis a cteni frame bufferu z/do souboru
 *
 * Opravy a modifikace:
 * ipolasek@fit.vutbr.cz
 * itomesek@fit.vutbr.cz
 */

/******************************************************************************
 ******************************************************************************
 * Includes
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>

/* nase */
#include "io.h"
#include "base.h"
#include "globals.h"
#include "student.h"
#include "rasterizer.h"

/* SDL knihovna */
#include <SDL.h>

/******************************************************************************
 ******************************************************************************
 * Nacteni BMP souboru do gFrameBuffer struktury
 */
int loadMyBitmap(const std::string &filename, S_RGBA** frame_buffer, int *width, int *height)
{
    /* Vytvorime SDL surface a nacteme do neho soubor */
    SDL_Surface *image;
    int x, y, bpp;
    if (!(image = SDL_LoadBMP(filename.c_str())))
    {
        IZG_SDL_ERROR("Input file does not exist");
        return 0;
    }

    /* Kontrola frame_bufferu, nasledne uvolneni a realokace (z duvodu jine velikosti - nezname velikosti, nemuzeme tudiz porovnat)*/
    IZG_ASSERT(*frame_buffer);
    free(*frame_buffer);

    if (!(*frame_buffer = (S_RGBA *)malloc(sizeof(S_RGBA) * image->h * image->w)))
    {
        IZG_ERROR("Cannot allocate the frame buffer");
        return 0;
    }

    /* Ulozime globalni promenne width a height */
    *width = image->w;
    *height = image->h;
    bpp = image->format->BytesPerPixel;

    /* pixel po pixelu ulozime do frame_bufferu (lepsi memcpy, ale pro demonstraci PutPixel) */
    for (y = 0; y < *height; ++y)
    {
        for (x = 0; x < *width; ++x)
        {
            Uint8 *p = (Uint8 *)image->pixels + y * image->pitch + x * bpp;
            setPixelColor(x, y, makeColor(p[2], p[1], p[0]));
        }
    }
    return 1;
}

/******************************************************************************
 ******************************************************************************
 * Ulozeni BMP souboru z gFrameBuffer struktury
 */
int saveMyBitmap(const std::string &filename, S_RGBA** frame_buffer,int width, int height)
{
    int x, y;
    /* Vytvorime RGB SDL Surface pro ukladani */
    SDL_Surface *image;

    IZG_ASSERT(*frame_buffer);
    if (!(image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0xFF000000, 0xFF0000, 0xFF00, 0)))
    {
        IZG_SDL_ERROR("Cannot allocate the saving object");
        return 0;
    }

    /* Projdeme vsechny body framebufferu a ulozime do obrazovych dat (lepsi memcpy, ale pro demonstraci funkcnosti GetPixel) */
    for (y = 0; y < height; ++y)
        for (x = 0; x < width; ++x)
        {
            S_RGBA color = getPixelColor(x, y);
                *((Uint32 *)image->pixels + y * width + x) = SDL_MapRGB(image->format, color.red, color.green, color.blue);
        }

    /* Ulozime BMP */
    return !SDL_SaveBMP(image, filename.c_str());
    //return 1;
}

/* Vykresleni usecek ze souboru */
bool drawLinesFromFile(const std::string &filename)
{
    std::ifstream source(filename);
    if(!source.is_open()) 
    { return false; }
    int draw_offset = 50;
    for(std::string line; std::getline(source, line); ) 
    {
        std::istringstream isstream(line);
        float x1, x2, y1, y2;
        isstream >> x1 >> y1 >> x2 >> y2;
        rasterizeLine((int)x1, (int)y1 + draw_offset, (int)x2, (int)y2 + draw_offset);
    }
    return true;
}

/* Vykresleni testovaciho vzoru na pozici [posX, posY] o prumeru diameter */
void drawLineTestPattern(int posX, int posY, float diameter)
{
    /* Pocet bodu, ktere budeme zobrazovat po obvodu kruznice. */
    static constexpr auto POINT_COUNT{ 32u };

    static const auto LINE_COLOR_BASE{ COLOR_GREEN };
    static const auto LINE_COLOR_AXIS{ COLOR_BLACK };
    static const auto CIRCLE_COLOR{ COLOR_RED };

    /* Testovaci "nulova" usecka */
    rasterizeLine(posX, posY, posX, posY, LINE_COLOR_AXIS);

    for (std::size_t pIdx = 0u; pIdx < POINT_COUNT; ++pIdx)
    {
        /* Casova promenna t, transformace z <0, POINT_COUNT - 1> -> <0.0, POINT_COUNT - 1 / POINT_COUNT> */
        const auto t{ static_cast<float>(pIdx) / POINT_COUNT };

        /* Offset po ose X od stredu kruznice */
        const auto offsetX{ static_cast<int>(
            cos(t * PI_2) * diameter
        ) };
        /* Offset po ose Y od stredu kruznice */
        const auto offsetY{ static_cast<int>(
            sin(t * PI_2) * diameter
        ) };

        /* Vyber specifickou barvu pro osy (AXIS), jinak pouzije normalni (BASE) */
        const auto lineColor{
            (abs(offsetX) < 0.001f || abs(offsetY) < 0.001f) ? 
                LINE_COLOR_AXIS :
                LINE_COLOR_BASE
        };

        /* Vykresleni usecky od stredu k bodu na kruznici */
        rasterizeLine(
            posX, posY,
            posX + offsetX, posY + offsetY,
            lineColor
        );
    }

    /* Vykresleni kruznice okolo */
    rasterizeCircle(posX, posY, diameter, CIRCLE_COLOR);
}

/* Vykresleni testovaciho vzoru */
void drawLineTestPatternAlt()
{
    /* Pocet bodu, ktere budeme zobrazovat po obvodu kruznice. */
    static constexpr auto POINT_COUNT{ 32u };

    clearRaster({ 255u, 255u, 255u, 255u });
    const auto diameter{ std::min(gWidth, gHeight) * 0.25f };
    const auto posX{ gWidth / 2.0f };
    const auto posY{ gHeight / 2.0f };

    for (std::size_t pIdx = 0u; pIdx < POINT_COUNT; ++pIdx)
    {
        /* Casova promenna t, transformace z <0, POINT_COUNT - 1> -> <0.0, POINT_COUNT - 1 / POINT_COUNT> */
        const auto t{ static_cast<float>(pIdx) / POINT_COUNT };

        /* Offset po ose X od stredu kruznice */
        const auto offsetX{ static_cast<int>(
            cos(t * PI_2) * diameter
        ) };
        /* Offset po ose Y od stredu kruznice */
        const auto offsetY{ static_cast<int>(
            sin(t * PI_2) * diameter
        ) };

        S_RGBA lc{ };

        if (std::abs(offsetX) >= std::abs(offsetY))
        { lc = { 0u, 255u, 0u, 255u}; }
        if (std::abs(offsetY) > std::abs(offsetX))
        { lc = { 255u, 0u, 0u, 255u }; }
        if (offsetX < 0.0f)
        { lc = { 60u, 200u, 235u, 255u }; }
        if (std::abs(offsetX) < 0.001f || std::abs(offsetY) < 0.001f)
        { lc = { 0u, 0u, 0u, 255u }; }

        rasterizeLine(
            posX, posY,
            posX + offsetX, posY + offsetY,
            lc
        );
    }

    rasterizeCircle(posX, posY, diameter * 0.1f, { 0u, 0u, 255u, 255u });
    rasterizeCircle(posX, posY, diameter * 0.095f, { 0u, 0u, 255u, 255u });
}

/* Nacte model krychle */
Model loadPolygonalCube()
{
    std::vector<Vector> vertices{ };
    std::vector<Triangle> faces{ };
    if (!loadCube(vertices, faces))
    { return { }; }

    Model m{ };
    m.vertices = std::move(vertices);
    m.faces = std::move(faces);
    m.scale = { 2.0f, 2.0f, 2.0f };
    m.rotate = { };
    m.translate = { 0.0f ,0.0f, 0.0f };

    return m;
}

Model loadPolygonalCuboctahedron()
{
    std::vector<Vector> vertices{ };
    std::vector<Triangle> faces{ };
    if (!loadObjFile("../data/cuboctahedron.obj", vertices, faces))
    { return { }; }

    Model m{ };
    m.vertices = std::move(vertices);
    m.faces = std::move(faces);
    m.scale = { 1.0f, 1.0f, 1.0f };
    m.rotate = { };
    m.translate = { 0.0f ,0.0f, 0.0f };

    return m;
}

/* Nacte model ve formatu OBJ */
Model loadPolygonalModel(const std::string &filename)
{
    std::vector<Vector> vertices{ };
    std::vector<Triangle> faces{ };
    if (!loadObjFile(filename, vertices, faces))
    { return { }; }

    Model m{ };
    m.vertices = std::move(vertices);
    m.faces = std::move(faces);
    m.scale = { 1.0f, 1.0f, 1.0f };
    m.rotate = { };
    m.translate = { 0.0f ,0.0f, 0.0f };

    return m;
}

/* Vykresli polygonalni model z pohledu dane kamery */
bool drawPolygonalModel(const Model &model, 
    S_RGBA lineColor, S_RGBA circleColor, 
    const Camera &camera)
{
    drawPolygons(model.vertices, model.faces, lineColor, circleColor, 
        model.scale, model.rotate, model.translate, camera);
    
    return true;
}

/* Vycisti kreslici plochu a nastavi ji na konstantni barvu */
void clearRaster(S_RGBA color)
{
    for (int y = 0u; y < gHeight; ++y)
    {
        for (int x = 0u; x < gWidth; ++x)
        { gFrameBuffer[y * gWidth + x] = color; }
    }
}

/*****************************************************************************/
/*****************************************************************************/
