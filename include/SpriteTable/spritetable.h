#pragma once

#include "consts.h"
#include "logs.h"
#include "position.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

struct Transform
{
    SDL_FRect rect;
    float origin{0.0f};

#ifdef POS_HORIZONTAL
    void SetOffsetFromOrigin(float path) { rect.x = origin + path; }
    void SetOrigin() { origin = rect.x; }
#else
    void SetOffsetFromOrigin(float path) { rect.y = origin + path; }
    void SetOrigin() { origin = rect.y; }
#endif
    const SDL_FRect& GetRect() const { return rect; }
};

class Sprite
{
  public:
    Transform transform;
    SDL_Rect sourcerect;
    Sprite(const SDL_Rect& rectFromAtlas, float x, float y);

#ifdef POS_HORIZONTAL
    bool operator==(const float other_x) const
    {
        return std::abs(transform.rect.x - other_x) < 0.01f;
    }
#else
    bool operator==(const float other_y) const
    {
        return std::abs(transform.rect.y - other_y) < 0.01f;
    }
#endif
    const SDL_Rect& SourceRect() const { return sourcerect; }
};

class ChosenRect
{
  public:
    Transform transform;
    ChosenRect() = default;
    void Init(const SDL_FRect& target)
    {
        transform.rect = {target.x - CHOSENRECT_PADDING,
                          target.y - CHOSENRECT_PADDING,
                          target.w + 2 * CHOSENRECT_PADDING,
                          target.h + 2 * CHOSENRECT_PADDING

        };
        transform.SetOrigin();
    }
    void SetOrigin() { transform.SetOrigin(); }
};

struct Logics
{
    bool move_process{false};
    bool moves_white{true};
    bool moves_sprites{false};
#ifdef POS_HORIZONTAL
    bool white_at_right_end{false};
    bool white_at_left_end{false};
#else
    bool white_at_top_end{false};
    bool white_at_bottom_end{false};
#endif
};

struct Mechanic
{
    Logics logic;
    EDirection dir = EDirection::RIGHT;
    int index{0};
    int sign{0};
    float fullPath{0.0f};
    ChosenRect chRect;
    std::vector<Sprite> vectorSprite;
};

class Atlas
{
  private:
    bool init{true};
    std::vector<SDL_Rect> sourceRects;
    SDL_Texture* atlasTexture{nullptr};
    [[nodiscard]] bool MakeAtlas(SDL_Renderer* r, size_t size, const char** filePath);

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
    bool init{true};
    bool initSpriteTable(SDL_Renderer* r, SpriteTableBorderType& spriteBorder);
    void firstInit(const std::vector<SDL_Rect>& atlasRects, SpriteTableBorderType& spriteBorder);
    Mechanic mechanic;
    Atlas* atlas{nullptr};

    bool chosenRectIsNotAtEnds() const //-
    {
#ifdef POS_HORIZONTAL
        return !mechanic.logic.white_at_left_end && !mechanic.logic.white_at_right_end;
#else
        return !mechanic.logic.white_at_top_end && !mechanic.logic.white_at_bottom_end;
#endif
    }
    bool chosenRectCanMove() const //-
    {
        return IsMoveProcess() && mechanic.logic.moves_white;
    }
    void moveChosenRect(float delta); //-
    void moveSprites(float delta);    //-
    bool spritesCanMove() const       //-
    {
        return IsMoveProcess() && mechanic.logic.moves_sprites;
    }

  public:
    SpriteTable(SDL_Renderer* r, SpriteTableBorderType& spriteTable);
    ~SpriteTable();
    SpriteTable(const SpriteTable&) = delete;
    SpriteTable(SpriteTable&&) = delete;
    SpriteTable& operator=(const SpriteTable&) = delete;
    SpriteTable& operator=(SpriteTable&&) = delete;

    bool Status() const { return init; }
    const std::vector<Sprite>& MechanicVectorSprite() const { return mechanic.vectorSprite; }
    SDL_Texture* AtlasTexture() const { return atlas->GetAtlasTexture(); }

    const SDL_FRect& GetChosenRect() const { return mechanic.chRect.transform.GetRect(); }
    void MovingInSpriteTable(float deltaTime);
    void CheckMoveLogic(SpriteTableBorderType& spriteTableBorder); //-

    bool IsMoveProcess() const //-
    {
        return mechanic.logic.move_process;
    }

    void MoveProcessStart() // -
    {
        mechanic.logic.move_process = true;
    }

    void SetDirectrion(EDirection dir) //-
    {
        mechanic.sign = static_cast<int>(dir);
        mechanic.index += mechanic.sign;
    }
#ifdef POS_HORIZONTAL
    bool Cant_move_right(SpriteTableBorderType& spriteTableBorder); //-
    bool Cant_move_left(SpriteTableBorderType& spriteTableBorder);  //-

    void ChosenRectIsNotAtLeftEnd() { mechanic.logic.white_at_left_end = false; }
    void ChosenRectIsNotAtRightEnd() //-
    {
        mechanic.logic.white_at_right_end = false;
    }
    void ChosenRectIsAtRightEnd() //-
    {
        mechanic.logic.white_at_right_end = true;
        mechanic.logic.move_process = false;
    }
    void ChosenRectIsAtLeftEnd() //-
    {
        mechanic.logic.white_at_left_end = true;
        mechanic.logic.move_process = false;
    }
#else
    bool Cant_move_bottom(SpriteTableBorderType& spriteTableBorder);
    bool Cant_move_top(SpriteTableBorderType& spriteTableBorder);
    void ChosenRectIsNotAtTopEnd() { mechanic.logic.white_at_top_end = false; }
    void ChosenRectIsNotAtBottomEnd() { mechanic.logic.white_at_bottom_end = false; }
    void ChosenRectIsAtBottomEnd()
    {
        mechanic.logic.white_at_bottom_end = true;
        mechanic.logic.move_process = false;
    }
    void ChosenRectIsAtTopEnd()
    {
        mechanic.logic.white_at_top_end = true;
        mechanic.logic.move_process = false;
    }
#endif
};
