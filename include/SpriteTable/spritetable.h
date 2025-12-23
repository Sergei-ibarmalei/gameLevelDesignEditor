#pragma once


#include <SDL.h>
#include <vector>
#include "logs.h"
#include "position.h"
#include "consts.h"

struct Transform
{
    SDL_FRect rect;
    float origin{0.0f};

#ifdef POS_HORIZONTAL
    void SetOffsetFromOrigin(float path) {rect.x = origin + path;}
    void SetOrigin() {origin = rect.x;}
#else
    void SetOffsetFromOrigin(float path) {rect.y = origin + path;}
    void SetOrigin() {origin = rect.y;}
#endif
    const SDL_FRect& GetRect() const {return rect;}
};


class Sprite
{
public:
    Transform transform;
    SDL_Rect  sourcerect;
    Sprite(const SDL_Rect& rectFromAtlas, float x, float y);

#ifdef POS_HORIZONTAL
    bool operator==(float other_x)
    {
        return std::abs(transform.rect.x - other_x) < 0.01f;
    }
#else
    bool operator==(float other_y)
    {
        return std::abs(transform.rect.y - other_y) < 0.01f;
    }
#endif

};

class ChosenRect
{
public:
    Transform transform;
    ChosenRect() = default;
    void Init(const SDL_FRect& target)
    {
        transform.rect =
        {
            target.x - CHOSENRECT_PADDING,
            target.y - CHOSENRECT_PADDING,
            target.w + 2 * CHOSENRECT_PADDING,
            target.h + 2 * CHOSENRECT_PADDING
            
        };
        transform.SetOrigin();
    }
    void SetOrigin() {transform.SetOrigin(); }
};

struct Logics
{
    bool move_process{ false };
    bool moves_white{ true };
    bool moves_sprites{ false };
    bool white_at_right_end{ false };
    bool white_at_left_end{ false };
};

struct Mechanic
{
    Logics logic;
    ESpritesDirection dir = ESpritesDirection::RIGHT;
    int index{ 0 };
    int sign{ 0 };
    float fullPath{ 0.0f };
    ChosenRect chRect;
    std::vector<Sprite> vectorSprite;
};

