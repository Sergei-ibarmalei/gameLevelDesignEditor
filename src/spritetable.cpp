#include <new>
#include "SpriteTable/spritetable.h"

const char* filePath[] = {"assets/one.png",
                          "assets/two.png",
                          "assets/three.png",
                          "assets/four.png",
                          "assets/five.png",
                          "assets/six.png"};

Sprite::Sprite(const SDL_Rect& rectFromAtlas, float x, float y)
{
    sourcerect = rectFromAtlas;
    transform.rect = {x, y, SPRITE_SIZE, SPRITE_SIZE};
    transform.SetOrigin();
}

Atlas::Atlas(SDL_Renderer* renderer, size_t size, const char** filePath)
{
    if (!renderer)
    {
#ifdef LOG
        std::cout << "Renderer is absent, cannot initiate Atlas.\n";
#endif
        init = false;
        return;
    }
    if (size <= 0)
    {
#ifdef LOG
        std::cout << "The size of file path is less or equal to 0.\n";
#endif
        init = false;
        return;
    }
    if (!filePath)
    {
#ifdef LOG
        std::cout << "File path is absent, cannot initiate Atlas.\n";
#endif
        init = false;
        return;
    }
    init = MakeAtlas(renderer, size, filePath);
}

Atlas::~Atlas()
{
    if (atlasTexture)
    {
        SDL_DestroyTexture(atlasTexture);
        atlasTexture = nullptr;
    }
}

bool Atlas::MakeAtlas(SDL_Renderer* renderer, size_t size, const char** filePath)
{
    // Вектор для хранения SDL_Surface от каждого изображения из filePath
    std::vector<SDL_Surface*> surfacesFromImages(size, nullptr);
    sourceRects.reserve(size);

    // загружаем изображения, делаем из них surface
    int totalWidth{0}; // полная ширина получаемой поверхности со всеми
    // изображениями
    int maxHeight{0}; // максимальная высота surface, полученного из
    // загружаемых изображений. При размерах одного спрайта
    // 64х64 maxHeight должен быть равен 64
    int destRectX{0}; // координата Х прямоугольника на конечном surface, где
    // будет рисоваться изображение
    int destRectY{0}; // координата по У прямоугольника на конечном surface,
    // где будет рисоваться изображение
    SDL_Rect destRect{destRectX, destRectY, 0, 0}; // инициализация
    // прямоугольника на
    // конечном surface, где
    // будет рисоваться
    // изображение. Ширину и
    // высоту прямоугольника
    // будем высчитывать из
    // рамеров surface каждого
    // спрайта
    for (size_t i = 0; i < size; ++i)
    {
        SDL_Surface* tmpSurface = IMG_Load(filePath[i]);

        if (!tmpSurface)
        {
#ifdef LOG
            std::cout << "Could not create surface from file " << filePath[i] << ": "
                      << SDL_GetError() << ",abort.\n";
#endif
            if (!surfacesFromImages.empty())
            {
                for (auto& s : surfacesFromImages)
                    if (s)
                        SDL_FreeSurface(s);
            }
            return false;
        }
#ifdef LOG
        // Проверка
        SDL_Log("Amask: %08x, BytesPerPixe: %d",
                tmpSurface->format->Amask,
                tmpSurface->format->BytesPerPixel);
#endif

        maxHeight = std::max(maxHeight, tmpSurface->h);
        destRect.x = destRectX;
        destRect.y = 0;
        destRect.w = tmpSurface->w;
        destRect.h = tmpSurface->h;

        sourceRects.emplace_back(destRect);
        surfacesFromImages[i] = tmpSurface;

        destRectX += tmpSurface->w + SPRITE_PADDING;
    }

    totalWidth = destRectX - SPRITE_PADDING;

    // Создаем итоговую поверхность
    SDL_Surface* tmpAtlasSurface =
        SDL_CreateRGBSurfaceWithFormat(0, totalWidth, maxHeight, 32, SDL_PIXELFORMAT_RGBA32);

    if (!tmpAtlasSurface)
    {
        for (auto& s : surfacesFromImages)
        {
            SDL_FreeSurface(s);
        }
#ifdef LOG
        std::cerr << "Could not create RGB surface, abort.\n";
#endif
        return false;
    }

    // Очищаем итоговую поверхность (делаем прозрачной)
    SDL_FillRect(tmpAtlasSurface, NULL, SDL_MapRGBA(tmpAtlasSurface->format, 0, 0, 0, 0));

    for (size_t i = 0; i < size; ++i)
    {
        // Включаем альфу у источников
        SDL_SetSurfaceBlendMode(surfacesFromImages[i], SDL_BLENDMODE_BLEND);

        SDL_BlitSurface(surfacesFromImages[i], NULL, tmpAtlasSurface, &sourceRects[i]);
        SDL_FreeSurface(surfacesFromImages[i]);
        surfacesFromImages[i] = nullptr;
    }

    atlasTexture = SDL_CreateTextureFromSurface(renderer, tmpAtlasSurface);
#ifdef LOG
    // Проверка blend mode созданной текстуры атласа
    SDL_BlendMode bm;
    SDL_GetTextureBlendMode(atlasTexture, &bm);
    SDL_Log("Texture blend mode = %d",
            (int)bm); // должно быть SDL_BLENDMODE_BLEND (1)
#endif
    if (!atlasTexture)
    {
        SDL_FreeSurface(tmpAtlasSurface);
        tmpAtlasSurface = nullptr;
#ifdef LOG
        std::cerr << "Could not create atlas texture, abort.\n";
#endif
        return false;
    }
    SDL_FreeSurface(tmpAtlasSurface);
    tmpAtlasSurface = nullptr;

    // Установка blend mode на созданную текстуру атласа
    SDL_SetTextureBlendMode(atlasTexture, SDL_BLENDMODE_BLEND);
#ifdef LOG
    // Проверка blend mode созданной текстуры атласа
    SDL_GetTextureBlendMode(atlasTexture, &bm);
    SDL_Log("\nTexture blend mode = %d",
            (int)bm); // должно быть SDL_BLENDMODE_BLEND (1)
#endif

    return true;
}

SpriteTable::SpriteTable(SDL_Renderer* r, SpriteBorderType& spriteBorder)
{
    if (!r)
    {
#ifdef LOG
        std::cout << "Cannot initiate Sprite Table, abort.\n";
#endif
        init = false;
        return;
    }
    init = initSpriteTable(r, spriteBorder);
}

SpriteTable::~SpriteTable()
{
    delete atlas;
    atlas = nullptr;
}

bool SpriteTable::initSpriteTable(SDL_Renderer* r, SpriteBorderType& spriteBorder)
{
    atlas = new (std::nothrow) Atlas(r, SPRITE_TABLE_COUNT_TOTAL, filePath);
    if (!atlas)
    {
#ifdef LOG
        std::cout << "Cannot allocate memory for atlas, abort.\n";
#endif
        return false;
    }

    if (!atlas->Status())
    {
#ifdef LOG
        std::cout << "Cannot initiate atlas, abort.\n";
#endif
        return false;
    }
    firstInit(atlas->GetSourceRects(), spriteBorder);
    mechanic.chRect.Init(mechanic.vectorSprite[0].transform.GetRect());
    mechanic.index = 0;

    return true;
}

void SpriteTable::firstInit(const std::vector<SDL_Rect>& atlasRects, SpriteBorderType& spriteBorder)
{
    // auto startX = BORDER.x + PADDING;
    // auto startY = BORDER.y + PADDING;
    auto startX = static_cast<float>(spriteBorder.spriteBorderRect.x + PADDING);
    auto startY = static_cast<float>(spriteBorder.spriteBorderRect.y + PADDING);

    mechanic.vectorSprite.reserve(SPRITE_TABLE_COUNT_TOTAL);
    for (size_t i = 0; i < SPRITE_TABLE_COUNT_TOTAL; ++i)
    {
        // #ifdef POS_HORIZONTAL
        //         auto spriteX = startX + i * (SPRITE_SIZE + PADDING);
        //         Sprite s{atlasRects[i], spriteX, startY};
        // #else
        //         auto spriteY = startY + i * (SPRITE_SIZE + PADDING);
        //         Sprite s{atlasRects[i], startX, spriteY};
        // #endif
        if (spriteBorder.orientation == ESpriteBorderOrientation::HORIZONTAL)
        {
            auto spriteX = startX + i * (SPRITE_SIZE + PADDING);
            Sprite s{atlasRects[i], spriteX, startY};
            mechanic.vectorSprite.emplace_back(s);
        }
        else
        {
            auto spriteY = startY + i * (SPRITE_SIZE + PADDING);
            Sprite s{atlasRects[i], startX, spriteY};
            mechanic.vectorSprite.emplace_back(s);
        }
        // mechanic.vectorSprite.emplace_back(s);
    }
}

static bool white_at_left{false};
static bool white_at_right{false};
static bool white_at_top{false};
static bool white_at_bottom{false};
static bool last_sprite_at_the_middle{false};


void SpriteTable::CheckMoveLogic(SpriteTableBorderType& spriteTable) //-
{
//#ifdef POS_HORIZONTAL
//    bool white_at_left{mechanic.index < 0};
//    bool white_at_right{mechanic.index > MIDDLE_INDEX};
//#else
//    bool white_at_top{mechanic.index < 0};
//    bool white_at_bottom{mechanic.index > MIDDLE_INDEX};
//#endif

    switch (spriteTable.orientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {
            white_at_left = mechanic.index < 0;
            white_at_right = mechanic.index > MIDDLE_INDEX;
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            white_at_top = mechanic.index < 0;
            white_at_bottom = mechanic.index > MIDDLE_INDEX;
            break;
        }
        default:
        {
        }
    }
    bool we_can_move_white{mechanic.logic.moves_white};
    bool we_can_move_sprites{mechanic.logic.moves_sprites};

    if (we_can_move_white)
    {
#ifdef POS_HORIZONTAL
        if (white_at_left)
#else
        if (white_at_top)
#endif
        {
            mechanic.index = 0;
            mechanic.logic.moves_white = false;
            mechanic.logic.moves_sprites = true;
            mechanic.dir = EDirection::RIGHT;
        }
#ifdef POS_HORIZONTAL
        if (white_at_right)
#else
        if (white_at_bottom)
#endif
        {
            mechanic.index = MIDDLE_INDEX;
            mechanic.logic.moves_white = false;
            mechanic.logic.moves_sprites = true;
            mechanic.dir = EDirection::LEFT;
        }
    }
    if (we_can_move_sprites)
    {
        bool white_at_the_middle{mechanic.index == MIDDLE_INDEX};
//#ifdef POS_HORIZONTAL
//        bool last_sprite_at_the_middle{mechanic.vectorSprite.back() == XSPRITE_MIDDLE};
//#else
//        bool last_sprite_at_the_middle{mechanic.vectorSprite.back() == YSPRITE_MIDDLE};
//#endif
        switch (spriteTable.orientation)
        {
            [[likely]] case ESpriteBorderOrientation::HORIZONTAL:
            {
                last_sprite_at_the_middle = mechanic.vectorSprite.back() == spriteTable.xSpriteMiddle;
                break;
            }
            case ESpriteBorderOrientation::VERTICAL:
            {
                last_sprite_at_the_middle = mechanic.vectorSprite.back() == spriteTable.ySpriteMiddle;
                break;
            }
            default: {}
        }
        if (white_at_the_middle && last_sprite_at_the_middle)
        {
            mechanic.logic.moves_sprites = false;
            mechanic.logic.moves_white = true;
            mechanic.logic.move_process = true;
        }
    }
}

void SpriteTable::moveChosenRect(float delta) //-
{
    float currentPace = SPEED * delta;
    mechanic.fullPath += currentPace;
    bool move_complete{mechanic.fullPath >= SPRITESIZE_WITH_PADDING};

    if (move_complete)
    {
        mechanic.fullPath = SPRITESIZE_WITH_PADDING;
        mechanic.chRect.transform.SetOffsetFromOrigin(mechanic.sign * mechanic.fullPath);
        mechanic.fullPath = 0.0f;
        mechanic.chRect.SetOrigin();
        mechanic.logic.move_process = false;
    }
    else
    {
        mechanic.chRect.transform.SetOffsetFromOrigin(mechanic.sign * mechanic.fullPath);
    }
}

void SpriteTable::moveSprites(float delta) //-
{
    float currentPace = SPEED * delta;
    mechanic.fullPath += currentPace;
    mechanic.sign = static_cast<int>(mechanic.dir);
    bool move_complete{mechanic.fullPath >= SPRITESIZE_WITH_PADDING};
    if (move_complete)
    {
        mechanic.logic.move_process = false;
        ;
        mechanic.fullPath = SPRITESIZE_WITH_PADDING;
        for (auto& sprite : mechanic.vectorSprite)
        {
            sprite.transform.SetOffsetFromOrigin(mechanic.sign * mechanic.fullPath);
        }
        mechanic.fullPath = 0.0f;
        mechanic.logic.moves_sprites = false;
        mechanic.logic.moves_white = true;
        for (auto& sprite : mechanic.vectorSprite)
        {
            sprite.transform.SetOrigin();
        }
    }
    else
    {
        for (auto& sprite : mechanic.vectorSprite)
        {
            sprite.transform.SetOffsetFromOrigin(mechanic.sign * mechanic.fullPath);
        }
    }

    //}
}

#ifdef POS_HORIZONTAL
bool SpriteTable::Cant_move_left(SpriteTableBorderType& spriteTableBorder) //-
{
    bool white_at_the_left{mechanic.index == 0};
    bool sprites_at_the_left{mechanic.vectorSprite.front() == spriteTableBorder.xSpriteFirst};//XSPRITE_FIRST};
    return white_at_the_left && sprites_at_the_left;
}

bool SpriteTable::Cant_move_right(SpriteTableBorderType& spriteTableBorder) //-
{
    bool white_at_the_middle{mechanic.index == MIDDLE_INDEX};
    bool srpites_at_right_end{mechanic.vectorSprite.back() == spriteTableBorder.xSpriteMiddle};//XSPRITE_MIDDLE};
    return white_at_the_middle && srpites_at_right_end;
}
#else
bool SpriteTable::Cant_move_top(SpriteTableBorderType& spriteTableBorder)
{
    bool white_at_the_top{mechanic.index == 0};
    bool sprites_at_the_top{mechanic.vectorSprite.front() ==
        spriteTableBorder.ySpriteFirst};// YSPRITE_FIRST};
    return white_at_the_top && sprites_at_the_top;
}
bool SpriteTable::Cant_move_bottom(SpriteTableBorderType& spriteTableBorder)
{
    bool white_at_the_middle{mechanic.index == MIDDLE_INDEX};
    bool srpites_at_bottom_end{mechanic.vectorSprite.back() == 
        spriteTableBorder.ySpriteMiddle};//  YSPRITE_MIDDLE};
    return white_at_the_middle && srpites_at_bottom_end;
}
#endif

void SpriteTable::MovingInSpriteTable(float deltaTime)
{
    if (chosenRectIsNotAtEnds())
    {
        if (chosenRectCanMove())
        {
            moveChosenRect(deltaTime);
        }
        else if (spritesCanMove())
        {
            moveSprites(deltaTime);
        }
    }
}
