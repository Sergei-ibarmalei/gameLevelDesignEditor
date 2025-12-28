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
    using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
    using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
    WindowPtr window_ {nullptr, SDL_DestroyWindow};
    RendererPtr renderer_ {nullptr, SDL_DestroyRenderer};

    MouseActionType mouseAction;
    bool running_ = false;
    bool doShowCursor{false};
    SDL_Rect editorTableBorder {0, 0, 0, 0};
    Uint32 lastTime{0};
    Uint32 currentTime{0};
    float deltaTime{0.0f};
    void IsMouseOnEditorTable(const SDL_MouseMotionEvent& e);
    void HandleMouseMotion(const SDL_MouseMotionEvent& e);
    void HandleButton(const SDL_MouseButtonEvent& e);
    void CalculateLightBox(const SDL_MouseMotionEvent& e);
    void DefineSpriteBorderSizes(ESpriteBorderOrientation orientation, SpriteTableBorderType& stb);

  public:
      SDL_Renderer* Renderer() const noexcept {return renderer_.get();}
      SDL_Window* Window() const noexcept {return window_.get();}
    bool initSdl(int w, int h);
    bool initEditorTableAndSpriteTable();
    void run();
    void shutdown();
};
