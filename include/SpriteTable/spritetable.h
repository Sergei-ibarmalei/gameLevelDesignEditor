#pragma once


#include <SDL.h>
#include <SDL_image.h>
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

class Atlas
{
private:
    bool init{ true };
    std::vector<SDL_Rect> sourceRects;
    SDL_Texture* atlasTexture{ nullptr };
    [[nodiscard]] bool MakeAtlas(SDL_Renderer* r, size_t size,
        const char** filePath);
public:
    Atlas(SDL_Renderer* r, size_t s, const char** filePath);
    ~Atlas();
    Atlas(const Atlas&) = delete;
    Atlas(Atlas&&) = delete;
    Atlas& operator=(Atlas&&) = delete;
    Atlas& operator=(const Atlas&) = delete;

    SDL_Texture* GetAtlasTexture() const { return atlasTexture; }
    const std::vector<SDL_Rect>& GetSourceRects() const { return sourceRects; }
    bool Status() const { return init; }

};


class SpriteTable
{
private:
    bool init {true};
    bool initSpriteTable(SDL_Renderer* r);
    void firstInit(const std::vector<SDL_Rect>& atlasRects);
    Mechanic mechanic;
    Atlas* atlas {nullptr};
public:
    SpriteTable(SDL_Renderer* r);
    ~SpriteTable();
    SpriteTable(const SpriteTable&) = delete;
    SpriteTable(SpriteTable&&) = delete;
    SpriteTable& operator=(const SpriteTable&) = delete;
    SpriteTable& operator=(SpriteTable&&) = delete;
    bool Status() const {return init;}


};

