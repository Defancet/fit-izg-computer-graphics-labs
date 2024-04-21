/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"

/**
 * Converts image into grayscale.
 * Use empiric equation presented in lecture.
 */
void ImageTransform::grayscale()
{
    // TODO: write code here
    for (uint32_t y = 0; y < cfg->h; y++) {
        for (uint32_t x = 0; x < cfg->w; x++) {
            RGB color = getPixel(x, y);
            uint8_t gray = 0.299 * color.r + 0.587 * color.g + 0.0114 * color.b;
            setPixel(x, y, RGB(gray));
        }
    }
}

/**
 * Evaluate threshold value for further usage in thresholding,
 * has to be called after grayscale().
 *
 * Note: evaluation method will DIFFER for each group!
 *
 * @return threshold
 */
uint8_t ImageTransform::evaluateThreshold()
{
    uint8_t th = 128;

    // TODO: write code here or in evaluateThreshold(x, y)

    IZG_INFO("Evaluated threshold: " + std::to_string(th));
    return th;
}

/**
 * Evaluate threshold value for further usage in thresholding,
 * has to be called after grayscale().
 *
 * Note: evaluation method will DIFFER for each group!
 *
 * @param x Input pixel coordinate
 * @param y Input pixel coordinate
 * @return  threshold
 */
uint8_t ImageTransform::evaluateThreshold(int32_t x, int32_t y)
{
    (void)x;
    (void)y;

    uint8_t th = 128;

    // TODO: write code here or in evaluateThreshold()
    if (y > 0) {
        uint32_t sum = 0;
        for (uint32_t i = 0; i < cfg->w; i++) {
            sum += getPixelGrayScale(i, y - 1);
        }
        th = sum / cfg->w;
    }

    return th;
    
}

void ImageTransform::threshold()
{
    // TODO: write code here
    for (unsigned int y = 0; y < cfg->h; y++) {
        for (unsigned int x = 0; x < cfg->w; x++) {
            uint8_t threshold = evaluateThreshold(x, y);
            uint8_t pixel = getPixelGrayScale(x, y);
            if(pixel >= threshold) {
                setPixel(x, y, COLOR_WHITE);
            } else {
                setPixel(x, y, COLOR_BLACK);
            }
        }
    }
}

void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::orderedDithering()
{
    static uint8_t matrix[m_side][m_side] =
            {
                    { 0,   254, 51,  255 },
                    { 68,  136, 187, 119 },
                    { 34,  238, 17,  221 },
                    { 170, 102, 153, 85  }
            };

    // TODO: write code here
    grayscale();

    for (uint32_t y = 0; y < cfg->h; y++) {
        for (uint32_t x = 0; x < cfg->w; x++) {
            RGB color = getPixel(x, y);

            uint8_t th = matrix[x % m_side][y % m_side];

            if (color.r > th) {
                setPixel(x, y, COLOR_WHITE);
            } else {
                setPixel(x, y, COLOR_BLACK);
            }
        }
    }
}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{
    (void)x;
    (void)y;
    (void)err;

    // TODO: write code here
    RGB value = getPixel(x, y);
    float error = std::round(err + value.r);
    uint8_t z = std::max(0.0f, std::min(255.0f, error));
    setPixel(x, y, RGB(z));
}

void ImageTransform::errorDistribution()
{
    // TODO: write code here
    grayscale();
    uint8_t th = 127;

    for (uint32_t x = 0; x < cfg->w; x++) {
        for (uint32_t y = 0; y < cfg->h; y++) {

            RGB value = getPixel(x, y);

            if (value.r > th) {
                float error = value.r - COLOR_WHITE.r;
                if (x + 1 < cfg->w)
                    updatePixelWithError(x + 1, y, error * 3.0 / 8.0);
                if (y + 1 < cfg->h)
                    updatePixelWithError(x, y + 1, error * 3.0 / 8.0);
                if (x + 1 < cfg->w && y + 1 < cfg->h)
                    updatePixelWithError(x + 1, y + 1, error * 2.0 / 8.0);

                setPixel(x, y, COLOR_WHITE);
            } else {
                float error = value.r;
                if (x + 1 < cfg->w)
                    updatePixelWithError(x + 1, y, error * 3.0 / 8.0);
                if (y + 1 < cfg->h)
                    updatePixelWithError(x, y + 1, error * 3.0 / 8.0);
                if (x + 1 < cfg->w && y + 1 < cfg->h)
                    updatePixelWithError(x + 1, y + 1, error * 2.0 / 8.0);

                setPixel(x, y, COLOR_BLACK);
            }
        }
    }
}

