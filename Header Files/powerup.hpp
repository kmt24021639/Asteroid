#pragma once
#include <raylib.h>

class PowerUp{
    public:
        PowerUp(int type, Vector2 position);
        void Update(int direction);
        void Draw();
        int GetType();
        static void UnloadImages();
        Rectangle getRect();
        static Texture2D powerupImages[5];
        int type;
        Vector2 position;
    private:
};

