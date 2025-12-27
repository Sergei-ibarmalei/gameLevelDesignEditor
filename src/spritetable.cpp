#include <new>
#include "SpriteTable/spritetable.h"

const char* filePath[] = {"assets/one.png",
                          "assets/two.png",
                          "assets/three.png",
                          "assets/four.png",
                          "assets/five.png",
                          "assets/six.png"};

Sprite::Sprite(const SDL_Rect& rectFromAtlas, float x, float y)
    : transform(x, y, SPRITE_SIZE, SPRITE_SIZE), sourcerect(rectFromAtlas)
{
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
    //if (atlasTexture)
    //{
    //    SDL_DestroyTexture(atlasTexture);
    //    atlasTexture = nullptr;
    //}
    atlasTexture = nullptr;
}

bool Atlas::MakeAtlas(SDL_Renderer* renderer, size_t size, const char** filePath)
{
    // Вектор для хранения SDL_Surface от каждого изображения из filePath
    std::vector<SDL_Surface*> surfacesFromImages(size, nullptr);
    //sourceRects.reserve(size);

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
    if (!atlasTexture)
    {
        SDL_FreeSurface(tmpAtlasSurface);
        tmpAtlasSurface = nullptr;
#ifdef LOG
        std::cerr << "Could not create atlas texture, abort.\n";
#endif
        return false;
    }
#ifdef LOG
    // Проверка blend mode созданной текстуры атласа
    SDL_BlendMode bm;
    SDL_GetTextureBlendMode(atlasTexture, &bm);
    SDL_Log("Texture blend mode = %d",
            (int)bm); // должно быть SDL_BLENDMODE_BLEND (1)
#endif

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

SpriteTable::SpriteTable(SDL_Renderer* r, SpriteTableBorderType& spriteBorder, bool active)
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
    thisSpriteTableIsActive = active;
}

SpriteTable::~SpriteTable()
{
    if (atlasTexture)
    {
        SDL_DestroyTexture(atlasTexture);
        atlasTexture = nullptr;
    }
}



bool SpriteTable::initSpriteTable(SDL_Renderer* r, SpriteTableBorderType& spriteBorder)
{
    size_t spriteTableCountTotal = std::size(filePath);
    if (!spriteTableCountTotal)
    {
#ifdef LOG
        std::cout << "File path size is 0, abort.\n";
#endif
        return false;
    }

    std::unique_ptr<Atlas> atlas = 
        std::make_unique<Atlas> (r, spriteTableCountTotal, filePath);

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
    firstInit(atlas->GetSourceRects(), spriteBorder, spriteTableCountTotal);
    atlasTexture = atlas->GetAtlasTexture();
    mechanic.chRect.Init(mechanic.vectorSprite[0].transform.GetRect());
    mechanic.atlasID = 0;

    return true;
}

void SpriteTable::firstInit(const std::vector<SDL_Rect>& atlasRects,
                            SpriteTableBorderType& spriteBorder,
                            const size_t spriteTableCountTotal)
{
    auto startX = static_cast<float>(spriteBorder.spriteBorderRect.x + PADDING);
    auto startY = static_cast<float>(spriteBorder.spriteBorderRect.y + PADDING);

    mechanic.vectorSprite.reserve(spriteTableCountTotal);
    for (size_t i = 0; i < spriteTableCountTotal; ++i)
    {

        if (spriteBorder.orientation == ESpriteBorderOrientation::HORIZONTAL)
        {
            auto spriteX = startX + i * (SPRITE_SIZE + PADDING);
            Sprite s{atlasRects[i], spriteX, startY};
            mechanic.vectorSprite.emplace_back(s);
        }
        else
        {
            auto spriteY = startY + i * (SPRITE_SIZE + PADDING);
            mechanic.vectorSprite.emplace_back(atlasRects[i], startX, spriteY);
        }
    }
}

void SpriteTable::CheckMoveLogic(SpriteTableBorderType& spriteTable) //-
{
    bool WhiteAtLeft{false};
    bool WhiteAtRight{false};
    bool WhiteAtTop{false};
    bool WhiteAtBottom{false};
    bool LastSpriteAtTheMiddle{false};

    switch (spriteTable.orientation)
    {
        case ESpriteBorderOrientation::HORIZONTAL:
        {
            WhiteAtLeft = mechanic.atlasID < 0;
            WhiteAtRight = mechanic.atlasID > MIDDLE_INDEX;
            break;
        }
        case ESpriteBorderOrientation::VERTICAL:
        {
            WhiteAtTop = mechanic.atlasID < 0;
            WhiteAtBottom = mechanic.atlasID > MIDDLE_INDEX;
            break;
        }
        default:
        {
        }
    }
    bool we_can_move_white{mechanic.logic.MovesWhite};
    bool we_can_move_sprites{mechanic.logic.MovesSprites};

    if (we_can_move_white)
    {
#ifdef POS_HORIZONTAL
        if (WhiteAtLeft)
#else
        if (WhiteAtTop)
#endif
        {
            mechanic.atlasID = 0;
            mechanic.logic.MovesWhite = false;
            mechanic.logic.MovesSprites = true;
            mechanic.dir = EDirection::RIGHT;
        }
#ifdef POS_HORIZONTAL
        if (WhiteAtRight)
#else
        if (WhiteAtBottom)
#endif
        {
            mechanic.atlasID = MIDDLE_INDEX;
            mechanic.logic.MovesWhite = false;
            mechanic.logic.MovesSprites = true;
            mechanic.dir = EDirection::LEFT;
        }
    }
    if (we_can_move_sprites)
    {
        bool white_at_the_middle{mechanic.atlasID == MIDDLE_INDEX};
        switch (spriteTable.orientation)
        {
            [[likely]] case ESpriteBorderOrientation::HORIZONTAL:
            {
                LastSpriteAtTheMiddle = mechanic.vectorSprite.back() == spriteTable.xSpriteMiddle;
                break;
            }
            case ESpriteBorderOrientation::VERTICAL:
            {
                LastSpriteAtTheMiddle = mechanic.vectorSprite.back() == spriteTable.ySpriteMiddle;
                break;
            }
            default:
            {
            }
        }
        if (white_at_the_middle && LastSpriteAtTheMiddle)
        {
            mechanic.logic.MovesSprites = false;
            mechanic.logic.MovesWhite = true;
            mechanic.logic.MoveProcess = true;
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
        mechanic.logic.MoveProcess = false;
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
        mechanic.logic.MoveProcess = false;
        mechanic.fullPath = SPRITESIZE_WITH_PADDING;
        for (auto& sprite : mechanic.vectorSprite)
        {
            sprite.transform.SetOffsetFromOrigin(mechanic.sign * mechanic.fullPath);
        }
        mechanic.fullPath = 0.0f;
        mechanic.logic.MovesSprites = false;
        mechanic.logic.MovesWhite = true;
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
}

#ifdef POS_HORIZONTAL
bool SpriteTable::Cant_move_left(SpriteTableBorderType& spriteTableBorder) //-
{
    bool white_at_the_left{mechanic.atlasID == 0};
    bool sprites_at_the_left{mechanic.vectorSprite.front() == spriteTableBorder.xSpriteFirst};
    return white_at_the_left && sprites_at_the_left;
}

bool SpriteTable::Cant_move_right(SpriteTableBorderType& spriteTableBorder) //-
{
    bool white_at_the_middle{mechanic.atlasID == MIDDLE_INDEX};
    bool srpites_at_right_end{mechanic.vectorSprite.back() == spriteTableBorder.xSpriteMiddle};
    return white_at_the_middle && srpites_at_right_end;
}
#else
bool SpriteTable::Cant_move_top(SpriteTableBorderType& spriteTableBorder)
{
    bool white_at_the_top{mechanic.index == 0};
    bool sprites_at_the_top{mechanic.vectorSprite.front() == spriteTableBorder.ySpriteFirst};
    return white_at_the_top && sprites_at_the_top;
}
bool SpriteTable::Cant_move_bottom(SpriteTableBorderType& spriteTableBorder)
{
    bool white_at_the_middle{mechanic.index == MIDDLE_INDEX};
    bool srpites_at_bottom_end{mechanic.vectorSprite.back() == spriteTableBorder.ySpriteMiddle};
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


