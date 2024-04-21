/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include <iostream>
#include <iomanip>
#include <SDL.h>
#include "base.h"

//#undef main

const std::string file = "../data/img1.bmp";
const std::string file2 = "../data/img2.bmp";
const std::string outFile = "../data/out.bmp";

void printHelp()
{
    auto printElement = [](std::string const& ch, std::string const& msg, int width) {
        std::cout << std::left << std::setw(width) << std::setfill(' ') << ch << msg << std::endl;
    };

    std::cout << "IZG - Lab 01 - help manual\n\n";
    std::cout << "Keybinds:\n";
    printElement("L", "Loads the first example image", 15);
    printElement("K", "Loads the second example image", 15);
    printElement("S", "Stores the framebuffer into a BMP image", 15);

    printElement("G", "Transform the frame buffer into grayscale", 15);
    printElement("T", "Converts the frame buffer into B/W using thresholding", 15);
    printElement("R", "Converts the frame buffer into B/W using random thresholding", 15);
    printElement("O", "Transform the frame buffer into B/W with ordered dithering", 15);
    printElement("E", "Transform the frame buffer into B/W with error distribution", 15);
    printElement("H", "Prints this message", 15);
}

void onKeyboard(SDL_KeyboardEvent* key, ImageTransform* transform)
{
    switch (key->keysym.sym)
    {
        case SDLK_ESCAPE:
            IZG_INFO("Stop");
            transform->cfg->running = false;
            break;
        case SDLK_l:
            if (!transform->loadImage(file))
                throw std::runtime_error("Failed to load an input image");
            break;
        case SDLK_k:
            if (!transform->loadImage(file2))
                throw std::runtime_error("Failed to load an input image");
            break;
        case SDLK_s:
            if (!transform->saveImage(outFile))
                throw std::runtime_error("Failed to save an output image");
            IZG_INFO("Image saved");
            break;
        case SDLK_g:
            IZG_INFO("Grayscale");
            transform->grayscale();
            break;
        case SDLK_t:
            IZG_INFO("Thresholding");
            transform->threshold();
            break;
        case SDLK_r:
            IZG_INFO("Random dithering");
            transform->randomDithering();
            break;
        case SDLK_o:
            IZG_INFO("Ordered dithering");
            transform->orderedDithering();
            break;
        case SDLK_e:
            IZG_INFO("Error distribution");
            transform->errorDistribution();
            break;
        case SDLK_h:
            printHelp();
            break;
        default:
            break;
    }
}

void onMouseDown(SDL_MouseMotionEvent* mouse, ImageTransform* transform)
{
    if (mouse->state & SDL_BUTTON_LMASK)
    {
        auto pixel = transform->getPixel(mouse->x, mouse->y);
        std::cout << "Info: pixel (" << mouse->x << ", " << mouse->y << "), color: " << pixel << std::endl;
    }
}

int main(int argc, char** argv)
{
    Config cfg =  { 800, 600, true };

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Error: failed to initialize SDL with error: " << SDL_GetError() << std::endl;
        return 1;
    }

    auto window = SDL_CreateWindow("IZG lab 01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cfg.w, cfg.h, 0);
    if (!window)
    {
        std::cerr << "Error: failed to create SDL window with error: " << SDL_GetError() << std::endl;
        return 1;
    }

    auto surface = SDL_GetWindowSurface(window);
    if (!surface)
    {
        std::cerr << "Error: failed to create a surface from window with error: " << SDL_GetError() << std::endl;
        return 1;
    }

    ImageTransform transform(&cfg);

    while (cfg.running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    cfg.running = false;
                    break;
                case SDL_KEYDOWN:
                    onKeyboard(&event.key, &transform);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    onMouseDown(&event.motion, &transform);
                    break;
            }
        }

        SDL_LockSurface(surface);

        // resize window, in case of frame buffer resize
        if (cfg.w != (uint32_t)surface->w || cfg.h != (uint32_t)surface->h)
        {
            SDL_SetWindowSize(window, cfg.w, cfg.h);
            surface = SDL_GetWindowSurface(window);
        }

        auto write = [&](uint32_t x, uint32_t y, RGB const& p) {
            auto pixels = (uint8_t*)surface->pixels;
            auto pixel = pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
            pixel[0] = p.b;
            pixel[1] = p.g;
            pixel[2] = p.r;
        };

        for (uint32_t y = 0; y < cfg.h; y++)
            for (uint32_t x = 0; x < cfg.w; x++)
                write (x, y, transform.getPixel(x, y));

        SDL_UnlockSurface(surface);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    return 0;
} 
