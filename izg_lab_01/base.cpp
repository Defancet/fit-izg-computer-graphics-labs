/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"

bool ImageTransform::loadImage(std::string const& file)
{
    SDL_Surface* image = SDL_LoadBMP(file.c_str());
    if (!image)
        return false;

    // reallocate frame buffer to handle arbitrary image size
    delete [] frame_buffer;
    delete [] read_only_frame_buffer;

    cfg->w = image->w; cfg->h = image->h;
    frame_buffer = new RGB[image->w * image->h];
    if (!frame_buffer)
        return false;

    read_only_frame_buffer = new RGB[image->w * image->h];
    if (!read_only_frame_buffer)
        return false;

    for (int y = 0; y < image->h; y++)
    {
        for (int x = 0; x < image->w; x++)
        {
            uint8_t* p = (uint8_t*)image->pixels + y * image->pitch + x * image->format->BytesPerPixel;
            frame_buffer[y * cfg->w + x] = RGB{p[2], p[1], p[0]};
            auto g = static_cast<uint8_t>(std::round(0.299f * p[2] + 0.587f * p[1] + 0.114f * p[0]));
            read_only_frame_buffer[y * cfg->w + x] = RGB{g};
        }
    }

    return true;
}

bool ImageTransform::saveImage(std::string const& file)
{
    SDL_Surface* image = SDL_CreateRGBSurface(0, cfg->w, cfg->h, 32,
            0xFF000000, 0xFF0000, 0xFF00, 0);

    // copy framebuffer into surface
    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);
            *((uint32_t*)image->pixels + y * cfg->w + x) = SDL_MapRGB(image->format, p.r, p.g, p.b);
        }
    }

    return !SDL_SaveBMP(image, file.c_str()); 
}

