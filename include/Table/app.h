#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "SpriteTable/spritetable.h"
#include "EditorTable/editortable.h"
#include "Helper/helperdot.h"
#include "position.h"

struct MouseActionType
{
    int row{0};
    int col{0};
    SDL_Rect Box{};
    SDL_Color boxColor{200, 200, 210, 80};
    bool IsOnEditorTable{false};
};

class App
{
  private:
    ESpriteBorderOrientation spriteBorderOrientation;
    SpriteTableBorderType spriteTableBorder;
    std::unique_ptr<EditorTable> editorTable;
    std::unique_ptr<SpriteTable> spriteTable;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    MouseActionType mouseAction;
    bool running_ = false;
    bool doShowCursor{false};
    SDL_Rect editorTableBorder {0, 0, 0, 0};
    Uint32 lastTime{0};
    Uint32 currentTime{0};
    float deltaTime{0.0f};
    void IsMouseOnEditorTable(SDL_MouseMotionEvent& e);
    void HandleMouseMotion(SDL_MouseMotionEvent& e);
    void HandleButton(SDL_MouseButtonEvent& e);
    void CalculateLightBox(SDL_MouseMotionEvent& e);
    void defineSpriteBorderSizes(ESpriteBorderOrientation orientation, SpriteTableBorderType& stb);

  public:
    bool initSdl(int w, int h);
    bool initEditorTableAndSpriteTable();
    void run();
    void shutdown();
};
