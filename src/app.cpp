#include "Table/app.h"
#include <iostream>

static void showSpriteTableBorder(SDL_Renderer* renderer, SpriteTableBorderType& spriteTableBorder);
static void showSimpleSpriteVector(SDL_Renderer* renderer,
                                   SDL_Texture* texture,
                                   const std::vector<Sprite>& vectorSprite);
static void showChosenRect(SDL_Renderer* renderer, const SDL_FRect& r);
static void showEditorTableBorder(SDL_Renderer* renderer, const SDL_FRect& r);

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
    spriteTableBorder.xSpriteMiddle =
        static_cast<float>(spriteTableBorder.spriteBorderRect.x +
                                                         (2 * SPRITE_SIZE) + (3 * PADDING));
    spriteTableBorder.ySpriteMiddle = 
        static_cast<float>(spriteTableBorder.spriteBorderRect.y +
                                                         (2 * SPRITE_SIZE) + (3 * PADDING));
    spriteTableBorder.xSpriteFirst =
        static_cast<float>(spriteTableBorder.spriteBorderRect.x + PADDING);
    spriteTableBorder.ySpriteFirst =
        static_cast<float>(spriteTableBorder.spriteBorderRect.y + PADDING);

    spriteTable = std::make_unique<SpriteTable>(renderer_, spriteTableBorder);
    if (!spriteTable->Status())
        return false;
    return true;
}

void App::run()
{
    if (!initEditorTableAndSpriteTable()) return;

    SDL_Event e;

    lastTime = SDL_GetTicks();

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
#ifdef POS_HORIZONTAL
                    case SDLK_RIGHT:
                    {

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

        SDL_SetRenderDrawColor(renderer_, 30, 30, 36, 255);
        SDL_RenderClear(renderer_);


        showEditorTableBorder(renderer_, editorTable->GetTableBorder());
        spriteTable->MovingInSpriteTable(deltaTime);

        SDL_RenderSetClipRect(renderer_, &spriteTableBorder.spriteBorderRect);
        showSimpleSpriteVector(renderer_,
                                spriteTable->AtlasTexture(), spriteTable->MechanicVectorSprite());
        SDL_RenderSetClipRect(renderer_, nullptr);
        showChosenRect(renderer_, spriteTable->GetChosenRect());


        showSpriteTableBorder(renderer_, spriteTableBorder);

        SDL_RenderPresent(renderer_);
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

static void showSpriteTableBorder(SDL_Renderer* renderer, SpriteTableBorderType& spriteTableBorder)
{
    if (spriteTableBorder.isActive)
        SDL_SetRenderDrawColor(renderer,
                               spriteTableBorder.activeBorderColor.r,
                               spriteTableBorder.activeBorderColor.g,
                               spriteTableBorder.activeBorderColor.b,
                               spriteTableBorder.activeBorderColor.a);
    else
        SDL_SetRenderDrawColor(renderer,
                               spriteTableBorder.inactiveBorderColor.r,
                               spriteTableBorder.inactiveBorderColor.g,
                               spriteTableBorder.inactiveBorderColor.b,
                               spriteTableBorder.inactiveBorderColor.a);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 255);
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

static void showEditorTableBorder(SDL_Renderer* renderer, const SDL_FRect& r)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
    SDL_RenderDrawRectF(renderer, &r);
}
