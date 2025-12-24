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

constexpr size_t SPRITE_TABLE_COUNT_VISIBLES{5};
constexpr size_t SPRITE_TABLE_COUNT_TOTAL{6};
constexpr float SPRITE_SIZE{64.0f};
constexpr int SPRITE_PADDING{2};
constexpr float PADDING{32.0};
constexpr float CHOSENRECT_PADDING{2.0f};
constexpr int SPRITESIZE_WITH_PADDING{static_cast<int>(SPRITE_SIZE + PADDING)};

#ifdef POS_HORIZONTAL

constexpr float BORDER_W{SPRITE_TABLE_COUNT_VISIBLES * SPRITE_SIZE +
                         (SPRITE_TABLE_COUNT_VISIBLES + 1) * PADDING};

constexpr float BORDER_H{PADDING * 2 + SPRITE_SIZE};
#else
constexpr float BORDER_W{PADDING * 2 + SPRITE_SIZE};
constexpr float BORDER_H{SPRITE_TABLE_COUNT_VISIBLES * SPRITE_SIZE +
                         (SPRITE_TABLE_COUNT_VISIBLES + 1) * PADDING};
#endif

constexpr float BORDER_LEFTX{(WINDOW_W / 2) - (BORDER_W / 2)};
constexpr float BORDER_LEFTY{(WINDOW_H / 2) - (BORDER_H / 2)};

constexpr float SPEED{800.0f};

constexpr int MIDDLE_INDEX{2};

constexpr SDL_FRect BORDER{BORDER_LEFTX, BORDER_LEFTY, BORDER_W, BORDER_H};

constexpr float XSPRITE_FIRST{BORDER.x + PADDING};
constexpr float XSPRITE_MIDDLE{BORDER.x + (2 * SPRITE_SIZE) + (3 * PADDING)};
constexpr float YSPRITE_FIRST{BORDER.y + PADDING};
constexpr float YSPRITE_MIDDLE{BORDER.y + (2 * SPRITE_SIZE) + (3 * PADDING)};

constexpr SDL_Rect BORDER_INT{static_cast<int>(BORDER_LEFTX),
                              static_cast<int>(BORDER_LEFTY),
                              static_cast<int>(BORDER_W),
                              static_cast<int>(BORDER_H)};
