#include "SpriteTable/spritetable.h"

Sprite::Sprite(const SDL_Rect& rectFromAtlas, float x, float y)
{
    sourcerect = rectFromAtlas;
    transform.rect = 
    {
        x, y, SPRITE_SIZE, SPRITE_SIZE
    };
}
