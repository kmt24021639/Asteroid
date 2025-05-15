#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>

class Spaceship {
    public:
        Spaceship();
        ~Spaceship();
        void Draw();
        void MoveLeft();
        void MoveRight();
        void FireLaser();
        Rectangle getRect();
        void Reset();
        std::vector<Laser> lasers;
        float ShootTime;
        int LaserSpeed;
        bool PUblaster;
        bool PUFflasher;
        bool PUtripleshoot;
        bool PUxtreme;
        bool PUshield;
    
    private:
        Texture2D image;
        Vector2 position;
        double lastFireTime;
        Sound LaserSound;
};
