#include "powerup.hpp"

Texture2D PowerUp::powerupImages[5] = {};

PowerUp::PowerUp(int type, Vector2 position)
{
    this -> type = type;
    this -> position = position;

    if(powerupImages[type - 1].id == 0) {

        switch (type)
        {
            case 1:
                powerupImages[0] = LoadTexture("D:/Project game/Asset/PowerUp/blaster.png");
                break;
            case 2:
                powerupImages[1] = LoadTexture("D:/Project game/Asset/PowerUp/flasher.png");
                break;
            case 3:
                powerupImages[2] = LoadTexture("D:/Project game/Asset/PowerUp/shield.png");
                break;
            case 4:
                powerupImages[3] = LoadTexture("D:/Project game/Asset/PowerUp/tripleshoot.png");
                break;
            case 5:
                powerupImages[4] = LoadTexture("D:/Project game/Asset/PowerUp/xtreme.png");
                break;
        }
    }
}

void PowerUp::Draw()
{
    DrawTextureV(powerupImages[type - 1], position, WHITE);
}

void PowerUp::Update(int direction)
{
    position.y +=5;
}

int PowerUp::GetType() 
{
    return type;
}

void PowerUp::UnloadImages()
{
    for(int i = 0; i < 5; i++) {
        UnloadTexture(powerupImages[i]);
    }
}

Rectangle PowerUp::getRect()
{
    return {position.x, position.y,
    float(powerupImages[type - 1].width),
    float(powerupImages[type - 1].height)
    };
}