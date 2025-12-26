#include "Table/app.h"
#include <iostream>

static void showSpriteTableBorder(SDL_Renderer* renderer,
                                  SpriteTableBorderType& spriteTableBorder,
                                  const SDL_Color c);
static void showSimpleSpriteVector(SDL_Renderer* renderer,
                                   SDL_Texture* texture,
                                   const std::vector<Sprite>& vectorSprite);
static void showChosenRect(SDL_Renderer* renderer, const SDL_FRect& r);
// static void showEditorTableBorder(SDL_Renderer* renderer, const SDL_FRect& r);
static void showEditorTableBorder(SDL_Renderer* renderer, const SDL_Rect& r, const SDL_Color c);
static void showLightBox(SDL_Renderer* renderer, MouseActionType& ma);
//static void ShowHelperDots(SDL_Renderer* renderer, SDL_Texture* t, const std::vector<SDL_Rect>& s);
static void ShowHelperDots(SDL_Renderer* renderer, const std::vector<SDL_Point>& dots);


static MouseActionType mouseAction;
static Uint32 takeMouseAction;
static SDL_Rect editorTableBorder;
static HandleMouseAction handeMouseAction;

bool App::initSdl(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window_ = SDL_CreateWindow(
        "Table", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if (!window_)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    renderer_ =
        SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Проверка работы Vsync
    if (SDL_RenderSetVSync(renderer_, 1) != 0)
    {
        std::cout << "RenderSetVSync failed " << SDL_GetError() << '\n';
    }

    int vsync = 0;

    if (SDL_RenderSetVSync(renderer_, 1) != 0)
    {
        SDL_Log("SDL_RenderSetVSync failed: %s", SDL_GetError());
    }

    // Включаем альфа-смешивание для рендерера
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

    running_ = true;
    return true;
}

bool App::initEditorTableAndSpriteTable()
{
#ifdef POS_VERTICAL
    spriteBorderOrientation = ESpriteBorderOrientation::VERTICAL;
#else
    spriteBorderOrientation = ESpriteBorderOrientation::HORIZONTAL;
#endif
    defineSpriteBorderSizes(spriteBorderOrientation, spriteTableBorder);
    // Editor table изначально активен
    editorTable =
        std::make_unique<EditorTable>(spriteBorderOrientation, spriteTableBorder, 20, 29, true);
    if (!editorTable->Status())
    {
#ifdef LOG
        std::cout << "Cannot initiate Editor Table, abort.\n";
#endif
        return false;
    }
    if (spriteTableBorder.spriteBorderRect.w == 0 || spriteTableBorder.spriteBorderRect.h == 0)
    {
#ifdef LOG
        std::cout << "Wrong made sprite border sizes, abort.\n";
#endif
        return false;
    }
    spriteTableBorder.xSpriteMiddle = static_cast<float>(spriteTableBorder.spriteBorderRect.x +
                                                         (2 * SPRITE_SIZE) + (3 * PADDING));
    spriteTableBorder.ySpriteMiddle = static_cast<float>(spriteTableBorder.spriteBorderRect.y +
                                                         (2 * SPRITE_SIZE) + (3 * PADDING));
    spriteTableBorder.xSpriteFirst =
        static_cast<float>(spriteTableBorder.spriteBorderRect.x + PADDING);
    spriteTableBorder.ySpriteFirst =
        static_cast<float>(spriteTableBorder.spriteBorderRect.y + PADDING);

    // Sprite table изначально неактивен
    spriteTable = std::make_unique<SpriteTable>(renderer_, spriteTableBorder, false);
    if (!spriteTable->Status())
        return false;
    return true;
}

void App::run()
{
    if (!initEditorTableAndSpriteTable())
        return;
    editorTableBorder = editorTable->GetIntTableBorder();
    HelperDot helperDot(editorTableBorder, editorTable->GetRealRowsColsEditorTable());
    if (!helperDot.Status())
        return;

    SDL_Event e;

    lastTime = SDL_GetTicks();

    //Uint64 lastCounter = SDL_GetPerformanceCounter();
    //const double freq = (double)SDL_GetPerformanceFrequency();

    while (running_)
    {
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running_ = false;
            else if (e.type == SDL_KEYDOWN && !spriteTable->IsMoveProcess())
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_TAB:
                    {
                        spriteTable->SetActive(!spriteTable->IsActive());
                        editorTable->SetActive(!editorTable->IsActive());
                        break;
                    }
#ifdef POS_HORIZONTAL

                    case SDLK_RIGHT:
                    {
                        if (!spriteTable->IsActive())
                            break;
                        spriteTable->MoveProcessStart();

                        spriteTable->ChosenRectIsNotAtLeftEnd();
                        if (spriteTable->Cant_move_right(spriteTableBorder))
                        {
                            spriteTable->ChosenRectIsAtRightEnd();
                            break;
                        }
                        spriteTable->SetDirectrion(EDirection::RIGHT);
                        spriteTable->CheckMoveLogic(spriteTableBorder);
                        break;
                    }
                    case SDLK_LEFT:
                    {
                        if (!spriteTable->IsActive())
                            break;
                        spriteTable->MoveProcessStart();
                        spriteTable->ChosenRectIsNotAtRightEnd();
                        if (spriteTable->Cant_move_left(spriteTableBorder))
                        {
                            spriteTable->ChosenRectIsAtLeftEnd();
                            break;
                        }
                        spriteTable->SetDirectrion(EDirection::LEFT);
                        spriteTable->CheckMoveLogic(spriteTableBorder);
                        break;
                    }
                    default:
                    {
                    }
#else
                    case SDLK_DOWN:
                    {
                        if (!spriteTable->IsActive())
                            break;
                        spriteTable->MoveProcessStart();
                        spriteTable->ChosenRectIsNotAtTopEnd();
                        if (spriteTable->Cant_move_bottom(spriteTableBorder))
                        {
                            spriteTable->ChosenRectIsAtBottomEnd();
                            break;
                        }
                        spriteTable->SetDirectrion(EDirection::DOWN);
                        spriteTable->CheckMoveLogic(spriteTableBorder);
                        break;
                    }
                    case SDLK_UP:
                    {
                        if (!spriteTable->IsActive())
                            break;
                        spriteTable->MoveProcessStart();
                        spriteTable->ChosenRectIsNotAtBottomEnd();
                        if (spriteTable->Cant_move_top(spriteTableBorder))
                        {
                            spriteTable->ChosenRectIsAtTopEnd();
                            break;
                        }
                        spriteTable->SetDirectrion(EDirection::UP);
                        spriteTable->CheckMoveLogic(spriteTableBorder);
                        break;
                    }
                    default:
                    {
                    }
#endif
                }
            }
        }

        //Uint64 now = SDL_GetPerformanceCounter();
        //double deltatime = (double)(now - lastCounter) / freq;
        //lastCounter = now;

        SDL_SetRenderDrawColor(renderer_, 30, 30, 36, 255);
        SDL_RenderClear(renderer_);

        takeMouseAction = SDL_GetMouseState(&mouseAction.mouseX, &mouseAction.mouseY);
        mouseAction.IsOnEditorTable =
            (mouseAction.mouseX >= editorTableBorder.x &&
             mouseAction.mouseX < (editorTableBorder.x + editorTableBorder.w) &&
             mouseAction.mouseY >= editorTableBorder.y &&
             mouseAction.mouseY < (editorTableBorder.y + editorTableBorder.h));
        if (mouseAction.IsOnEditorTable)
        {
            handeMouseAction.CalculateLightBox(mouseAction, editorTableBorder);
            showLightBox(renderer_, mouseAction);
        }

        showEditorTableBorder(
            renderer_, editorTableBorder, editorTable->IsActive() ? ACTIVE_COLOR : INACTIVE_COLOR);
        spriteTable->MovingInSpriteTable(deltaTime);

        SDL_RenderSetClipRect(renderer_, &spriteTableBorder.spriteBorderRect);
        showSimpleSpriteVector(
            renderer_, spriteTable->AtlasTexture(), spriteTable->MechanicVectorSprite());
        SDL_RenderSetClipRect(renderer_, nullptr);
        showChosenRect(renderer_, spriteTable->GetChosenRect());

        showSpriteTableBorder(
            renderer_, spriteTableBorder, spriteTable->IsActive() ? ACTIVE_COLOR : INACTIVE_COLOR);

        // Показываем белые точки
        //ShowHelperDots(
          // renderer_, helperDot.GetHelperDotTexture(), helperDot.GetHelperSourceRects());
        ShowHelperDots(renderer_, helperDot.GetHelperDots());

        SDL_RenderPresent(renderer_);

        // FPS logger (раз в секунду)
        //static double acc = 0.0;
        //static int frames = 0;
        //static double maxDt = 0.0;

        //acc += deltaTime;
        //frames++;
        //if (deltaTime > maxDt)
        //    maxDt = deltaTime;

        //if (acc >= 1.0)
        //{
        //    double avgMs = (acc * 1000.0) / frames;
        //    std::printf("FPS: %d | avg: %.3f ms | max: %.3f ms\n", frames, avgMs, maxDt * 1000.0);
        //    std::fflush(stdout);

        //    acc = 0.0;
        //    frames = 0;
        //    maxDt = 0.0;
        //}
    }
}

void App::shutdown()
{
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}

void App::defineSpriteBorderSizes(ESpriteBorderOrientation orientation, SpriteTableBorderType& stb)
{
    switch (orientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {
            stb.orientation = ESpriteBorderOrientation::HORIZONTAL;
            stb.spriteBorderSizes.horizontal.w =
                static_cast<int>(SPRITE_TABLE_COUNT_VISIBLES * SPRITE_SIZE +
                                 (SPRITE_TABLE_COUNT_VISIBLES + 1) * PADDING);
            stb.spriteBorderSizes.horizontal.h = static_cast<int>(PADDING * 2 + SPRITE_SIZE);
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            stb.orientation = ESpriteBorderOrientation::VERTICAL;
            stb.spriteBorderSizes.vertical.w = static_cast<int>(PADDING * 2 + SPRITE_SIZE);
            stb.spriteBorderSizes.vertical.h =
                static_cast<int>(SPRITE_TABLE_COUNT_VISIBLES * SPRITE_SIZE +
                                 (SPRITE_TABLE_COUNT_VISIBLES + 1) * PADDING);
            break;
        }
        default:
        {
        }
    }
}

static void ShowHelperDots(SDL_Renderer* renderer, const std::vector<SDL_Point>& dots)
{
    //SDL_Rect source{0, 0, HELPDOT_SPRITESIZE, HELPDOT_SPRITESIZE};

    //for (const auto& r : s)
    //{
    //    SDL_RenderCopy(renderer, t, &source, &r);
    //}
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoints(renderer, dots.data(), (int)dots.size());
}

void HandleMouseAction::CalculateLightBox(MouseActionType& ma, const SDL_Rect& editorTableBorder)
{

    ma.col = static_cast<int>((ma.mouseX - editorTableBorder.x) / SPRITE_SIZE);
    ma.row = static_cast<int>((ma.mouseY - editorTableBorder.y) / SPRITE_SIZE);
    ma.Box.x = editorTableBorder.x + ma.col * static_cast<int>(SPRITE_SIZE);
    ma.Box.y = editorTableBorder.y + ma.row * static_cast<int>(SPRITE_SIZE);
    ma.Box.w = static_cast<int>(SPRITE_SIZE);
    ma.Box.h = static_cast<int>(SPRITE_SIZE);
}

static void showSpriteTableBorder(SDL_Renderer* renderer,
                                  SpriteTableBorderType& spriteTableBorder,
                                  const SDL_Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(renderer, &spriteTableBorder.spriteBorderRect);
}

static void showSimpleSpriteVector(SDL_Renderer* renderer,
                                   SDL_Texture* texture,
                                   const std::vector<Sprite>& vectorSprite)
{
    for (auto& sprite : vectorSprite)
    {
        SDL_RenderCopyF(renderer, texture, &sprite.SourceRect(), &sprite.transform.GetRect());
    }
}

static void showChosenRect(SDL_Renderer* renderer, const SDL_FRect& r)
{
    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
    SDL_RenderDrawRectF(renderer, &r);
}

static void showEditorTableBorder(SDL_Renderer* renderer, const SDL_Rect& r, const SDL_Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(renderer, &r);
}

static void showLightBox(SDL_Renderer* renderer, MouseActionType& ma)
{
    SDL_SetRenderDrawColor(renderer, ma.boxColor.r, ma.boxColor.g, ma.boxColor.b, ma.boxColor.a);
    SDL_RenderFillRect(renderer, &ma.Box);
}
