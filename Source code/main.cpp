#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    numberText = std::string(leadingZeros, '0') + numberText;
    return numberText;
}

std::string FormatLevel(int number)
{
    std::string levelText = std::to_string(number);
    return "LEVEL 0" + levelText;
}

int main() 
{
    //Gameloop
    Color grey = {29, 29, 27, 255};
    Color yellow ={243, 216, 63, 255};
    int windowWidth = 750;
    int windowHeight = 750;
    int offset = 50;

    InitWindow(windowWidth + offset, windowHeight + offset * 2, "Asteroid");
    InitAudioDevice();

    SetTargetFPS(60);
    Font font = LoadFontEx("D:/Project game/Asset/pixel.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("D:/Project game/Asset/player.png");

    Game game;

    while(WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLinesEx({10, 10, 780, 830}, 0.18f, 20, 2, yellow);
        DrawLineEx({25, 780}, {775, 780}, 3, yellow);
        
        if(game.run) {
            std::string LevelText = FormatLevel(game.level);
            DrawTextEx(font, LevelText.c_str(), {570, 790}, 34, 2, yellow);
        } else {
            DrawTextEx(font, "GAME OVER", {570, 790}, 34, 2, yellow);
            DrawTextEx(font, "PRESS ENTER TO RESTART", {50, 790}, 34, 2, yellow);
        }

        float x = 50.0;
        for(int i = 0; i < game.lives; i++) {
            DrawTextureV(spaceshipImage, {x, 795}, WHITE);
            x += 70;
        }

        DrawTextEx(font, "SCORE", {50, 25}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 50}, 34, 2, yellow);
        
        DrawTextEx(font, "HI-SCORE", {570, 25}, 34, 2, yellow);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {600, 50}, 34, 2, yellow);

        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    CloseAudioDevice();
}
