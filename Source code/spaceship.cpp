#include "spaceship.hpp"
#include "game.hpp"
#include <cmath>

Spaceship::Spaceship()
{
    image = LoadTexture("D:/Project game/Asset/Ship/player.png");
    position.x = (GetScreenWidth() - image.width) / 2;
    position.y = GetScreenHeight() - image.height - 100;
    lastFireTime = 0.0;
    LaserSound = LoadSound("D:/Project game/Asset/Sound/laser.ogg");
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
    UnloadSound(LaserSound);
}

void Spaceship::Draw()
{
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft()
{
    position.x -= 7;
    if(position.x < 25) {
        position.x = 25;
    }
}

void Spaceship::MoveRight()
{
    position.x += 7;
    if(position.x > GetScreenWidth() - image.width - 25) {
        position.x = GetScreenWidth() - image.width - 25;
    }
}

void Spaceship::FireLaser()
{
    if(PUtripleshoot) {
        PUxtreme = false;
        if(GetTime() - lastFireTime >= ShootTime) {
        for (int i = -5; i <= 5; i += 5) {
            lasers.push_back(Laser({position.x + image.width/2 + 2*i, position.y + 5 + abs(i)},-LaserSpeed));
        }
        lastFireTime = GetTime();
        PlaySound(LaserSound);
        }
    }

    if(PUxtreme) {
        PUtripleshoot = false;
        if(GetTime() - lastFireTime >= ShootTime) {
        for (int i = -50; i <= 50; i += 5) {
            lasers.push_back(Laser({position.x + image.width/2 + 2*i, position.y + 5 + abs(i)},-LaserSpeed));
        }
        lastFireTime = GetTime();
        PlaySound(LaserSound);
        }
    }
    
    if(GetTime() - lastFireTime >= ShootTime) {
        lasers.push_back(Laser({position.x + image.width/2, position.y + 5},-LaserSpeed));
        lastFireTime = GetTime();
        PlaySound(LaserSound);
    }
}

Rectangle Spaceship::getRect()
{
    return {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    position.x = (GetScreenWidth() - image.width)/ 2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear();
    
    PUshield = false;
    PUtripleshoot = false;
    PUxtreme = false;
    ShootTime = 0.35;
    LaserSpeed = 7;
}
