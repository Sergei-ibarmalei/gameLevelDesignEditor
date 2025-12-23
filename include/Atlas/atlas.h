#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "../logs.h"
#include "SpriteTable/consts.h"



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
