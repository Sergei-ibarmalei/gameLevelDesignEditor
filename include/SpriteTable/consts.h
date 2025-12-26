#pragma once

#include "../position.h"
#include "../video.h"
#include <SDL.h>

enum class EDirection
{
    RIGHT = 1,
    LEFT = -1,
    DOWN = 1,
    UP = -1,
};

enum class ESpriteBorderOrientation
{
    HORIZONTAL,
    VERTICAL,
};

struct SpriteTableBorderType
{
    union USpriteBorderSizes
    {
        SDL_Rect horizontal;
        SDL_Rect vertical;
    } spriteBorderSizes;

    float xSpriteMiddle{0.0f};
    float ySpriteMiddle{0.0f};
    float xSpriteFirst{0.0f};
    float ySpriteFirst{0.0f};

    //SDL_Color activeBorderColor{255, 0, 0, 255};
    //SDL_Color inactiveBorderColor{100, 100, 100, 255};
    SDL_Rect spriteBorderRect;
    ESpriteBorderOrientation orientation;
    bool isActive{false};
};

constexpr SDL_Color ACTIVE_COLOR{255, 0, 0, 255};
constexpr SDL_Color INACTIVE_COLOR{100, 100, 100, 255};

// SPRITE_TABLE_COUNT_VISIBLES - количество видимых спрайтов в спрайтбордере
constexpr size_t SPRITE_TABLE_COUNT_VISIBLES{5};

constexpr float SPRITE_SIZE{64.0f};

// SPRITE_PADDING - отступ внутри спрайта для отрисовки выбранного прямоугольника
constexpr int SPRITE_PADDING{2};

// PADDING - расстояние между спрайтами и от спрайтов до границы спрайтбордера
constexpr float PADDING{32.0};

// CHOSENRECT_PADDING - на столько больше по каждой стороне прямоугольник "выбора"
constexpr float CHOSENRECT_PADDING{2.0f};

constexpr int SPRITESIZE_WITH_PADDING{static_cast<int>(SPRITE_SIZE + PADDING)};



constexpr float SPEED{800.0f};

// Индекс спрайта в середине спрайтбордера, после которого прямоугольник "выбора" не может двигаться
constexpr int MIDDLE_INDEX{2};


