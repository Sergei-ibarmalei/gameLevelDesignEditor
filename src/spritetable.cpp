#include "SpriteTable/spritetable.h"

const char* filePath[] = {"assets/one.png",  "assets/two.png",  "assets/three.png",
                          "assets/four.png", "assets/five.png", "assets/six.png"};

Sprite::Sprite(const SDL_Rect& rectFromAtlas, float x, float y)
{
    sourcerect = rectFromAtlas;
    transform.rect = 
    {
        x, y, SPRITE_SIZE, SPRITE_SIZE
    };
}

Atlas::Atlas(SDL_Renderer* renderer, size_t size,
    const char** filePath)
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

bool Atlas::MakeAtlas(SDL_Renderer* renderer, size_t size,
    const char** filePath)
{
    // Вектор для хранения SDL_Surface от каждого изображения из filePath
    std::vector<SDL_Surface*> surfacesFromImages(size, nullptr);
    sourceRects.reserve(size);

    // загружаем изображения, делаем из них surface
    int totalWidth{ 0 }; // полная ширина получаемой поверхности со всеми
    // изображениями
    int maxHeight{ 0 };  // максимальная высота surface, полученного из
    // загружаемых изображений. При размерах одного спрайта
    // 64х64 maxHeight должен быть равен 64
    int destRectX{ 0 };  // координата Х прямоугольника на конечном surface, где
    // будет рисоваться изображение
    int destRectY{ 0 };  // координата по У прямоугольника на конечном surface,
    // где будет рисоваться изображение
    SDL_Rect destRect{ destRectX, destRectY, 0, 0 }; // инициализация
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
            std::cout << "Could not create surface from file " <<
                filePath[i] << ": " << SDL_GetError() << ",abort.\n";
#endif
            if (!surfacesFromImages.empty())
            {
                for (auto& s : surfacesFromImages)
                    if (s) SDL_FreeSurface(s);
            }
            return false;
        }
        totalWidth += tmpSurface->w + SPRITE_PADDING;
        maxHeight = std::max(maxHeight, tmpSurface->h);
        destRect.x = static_cast<int>(i * tmpSurface->w + SPRITE_PADDING);
        destRect.w = tmpSurface->w;
        destRect.h = tmpSurface->h;

        sourceRects.emplace_back(destRect);
        surfacesFromImages[i] = tmpSurface;
        tmpSurface = nullptr;
    }

    totalWidth -= SPRITE_PADDING;

    // Создаем итоговую поверхность
    SDL_Surface* tmpAtlasSurface = SDL_CreateRGBSurface(0,
        totalWidth, maxHeight, 32, 0, 0, 0, 0);
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

    for (size_t i = 0; i < size; ++i)
    {
        SDL_BlitSurface(surfacesFromImages[i], NULL,
            tmpAtlasSurface, &sourceRects[i]);
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
    SDL_FreeSurface(tmpAtlasSurface);
    tmpAtlasSurface = nullptr;
    return true;

}

SpriteTable::SpriteTable(SDL_Renderer* r)
{
    if (!r)
    {
#ifdef LOG
        std::cout << "Cannot initiate Sprite Table, abort.\n";
#endif
        init = false;
        return;
    }
    init = initSpriteTable(r);






}

SpriteTable::~SpriteTable()
{
    delete atlas;
    atlas = nullptr;
}

bool SpriteTable::initSpriteTable(SDL_Renderer* r)
{
    atlas = new Atlas(r, SPRITE_TABLE_COUNT_TOTAL, filePath);
    if (!atlas->Status())
    {
#ifdef LOG
        std::cout << "Cannot initiate atlas, abort.\n";
#endif
        return false;
    }
    firstInit(atlas->GetSourceRects());
    mechanic.chRect.Init(mechanic.vectorSprite[0].transform.GetRect());
    mechanic.index = 0;

    return true;
}

void SpriteTable::firstInit(const std::vector<SDL_Rect>& atlasRects)
{
    auto startX = BORDER.x;
    auto startY = BORDER.y;
    mechanic.vectorSprite.reserve(SPRITE_TABLE_COUNT_TOTAL);
    for (size_t i = 0; i < SPRITE_TABLE_COUNT_TOTAL; ++i)
    {
#ifdef POS_HORIZONTAL
        auto spriteX = startX + i * (SPRITE_SIZE + PADDING);
        Sprite s {atlasRects[i], spriteX, startY};
#else
        auto spriteY = startY + i * (SPRITE_SIZE + PADDING);
        Sprite s {atlasRects[i], startX, spriteY};
#endif
        mechanic.vectorSprite.emplace_back(s);
    }
}
