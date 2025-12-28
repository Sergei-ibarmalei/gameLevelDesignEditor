#pragma once

#include "consts.h"
#include "logs.h"
#include "position.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>

struct Transform
{
    SDL_FRect rect{0.0f, 0.0f, 0.0f, 0.0f};
    float origin{0.0f};

    Transform() {SetOrigin();}
    Transform(float x, float y, float w, float h) : rect{x, y, w, h} { SetOrigin(); }

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
    SDL_Rect sourcerect{0, 0, 0, 0}; // source rect спрайта в атласе
    Sprite() {}
    Sprite(const SDL_Rect& rectFromAtlas, float x, float y);
    ~Sprite() = default;

    void SetTransform(float x, float y);

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
    bool MoveProcess{false};
    bool MovesWhite{true};
    bool MovesSprites{false};
#ifdef POS_HORIZONTAL
    bool WhiteAtRightEnd{false};
    bool WhiteAtLeftEnd{false};
#else
    bool WhiteAtTopEnd{false};
    bool WhiteAtBottomEnd{false};
#endif
};

struct Mechanic
{
    Logics logic;
    EDirection dir = EDirection::RIGHT;
    int textureID{0};
    int index{0};
    int sign{0};
    float fullPath{0.0f};
    ChosenRect chRect;
};

class Atlas
{
  private:
    bool init{true};
    SDL_Texture* atlasTexture{nullptr};
    [[nodiscard]] bool MakeAtlas(SDL_Renderer* r,
                                 std::vector<Sprite>& vectorSprite,
                                 size_t size,
                                 const char** filePath);

  public:
    Atlas(SDL_Renderer* r, std::vector<Sprite>& vectorSprite, size_t s, const char** filePath);
    ~Atlas();
    Atlas(const Atlas&) = delete;
    Atlas(Atlas&&) = delete;
    Atlas& operator=(Atlas&&) = delete;
    Atlas& operator=(const Atlas&) = delete;

    SDL_Texture* GetAtlasTexture() const { return atlasTexture; }
    bool Status() const { return init; }
};

class SpriteTable
{
  private:
    bool init{true};
    bool thisSpriteTableIsActive{false};
    size_t spriteTableCountTotal{0};
    Mechanic mechanic;
    std::unique_ptr<Atlas> atlas;
    std::vector<Sprite> vectorSprite;

    bool initSpriteTable(SDL_Renderer* r, SpriteTableBorderType& spriteBorder);
    void firstInit(SpriteTableBorderType& spriteBorder,
                   const size_t spriteTableCountTotal);

    bool chosenRectIsNotAtEnds() const //-
    {
#ifdef POS_HORIZONTAL
        return !mechanic.logic.WhiteAtLeftEnd && !mechanic.logic.WhiteAtRightEnd;
#else
        return !mechanic.logic.WhiteAtTopEnd && !mechanic.logic.WhiteAtBottomEnd;
#endif
    }
    bool chosenRectCanMove() const //-
    {
        return mechanic.logic.MoveProcess && mechanic.logic.MovesWhite;
    }
    void moveChosenRect(float delta); //-
    void moveSprites(float delta);    //-
    bool spritesCanMove() const       //-
    {
        return mechanic.logic.MoveProcess && mechanic.logic.MovesSprites;
    }

  public:
    SpriteTable(SDL_Renderer* r, SpriteTableBorderType& spriteTable, bool active = false);
    ~SpriteTable() = default;
    SpriteTable(const SpriteTable&) = delete;
    SpriteTable(SpriteTable&&) = delete;
    SpriteTable& operator=(const SpriteTable&) = delete;
    SpriteTable& operator=(SpriteTable&&) = delete;

    const bool IsActive() const { return thisSpriteTableIsActive; }
    bool Status() const { return init; }
    void SetActive(bool a) { thisSpriteTableIsActive = a; }
    const std::vector<Sprite>& VectorSprite() const { return vectorSprite; }
    SDL_Texture* AtlasTexture() const { return atlas->GetAtlasTexture(); }

    const SDL_FRect& GetChosenRect() const { return mechanic.chRect.transform.GetRect(); }
    int GetSpriteTableTextureID() const { return mechanic.textureID; }
    void MovingInSpriteTable(float deltaTime);
    void CheckMoveLogic(SpriteTableBorderType& spriteTableBorder); //-

    bool IsMoveProcess() const //-
    {
        return mechanic.logic.MoveProcess;
    }

    void MoveProcessStart() // -
    {
        mechanic.logic.MoveProcess = true;
    }

    void SetDirectrion(EDirection dir) //-
    {
        mechanic.sign = static_cast<int>(dir);
        mechanic.index += mechanic.sign;
    }
#ifdef POS_HORIZONTAL
    bool Cant_move_right(SpriteTableBorderType& spriteTableBorder); //-
    bool Cant_move_left(SpriteTableBorderType& spriteTableBorder);  //-

    void ChosenRectIsNotAtLeftEnd() { mechanic.logic.WhiteAtLeftEnd = false; }
    void ChosenRectIsNotAtRightEnd() //-
    {
        mechanic.logic.WhiteAtRightEnd = false;
    }
    void ChosenRectIsAtRightEnd() //-
    {
        mechanic.logic.WhiteAtRightEnd = true;
        mechanic.logic.MoveProcess = false;
    }
    void ChosenRectIsAtLeftEnd() //-
    {
        mechanic.logic.WhiteAtLeftEnd = true;
        mechanic.logic.MoveProcess = false;
    }

#else
    bool Cant_move_bottom(SpriteTableBorderType& spriteTableBorder);
    bool Cant_move_top(SpriteTableBorderType& spriteTableBorder);
    void ChosenRectIsNotAtTopEnd() { mechanic.logic.WhiteAtTopEnd = false; }
    void ChosenRectIsNotAtBottomEnd() { mechanic.logic.WhiteAtBottomEnd = false; }
    void ChosenRectIsAtBottomEnd()
    {
        mechanic.logic.WhiteAtBottomEnd = true;
        mechanic.logic.MoveProcess = false;
    }
    void ChosenRectIsAtTopEnd()
    {
        mechanic.logic.WhiteAtTopEnd = true;
        mechanic.logic.MoveProcess = false;
    }
#endif
    void IncTextureID();
    void DecTextureID();
};
