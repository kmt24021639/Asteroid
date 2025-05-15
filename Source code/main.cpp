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
    
    int windowWidth = 750;
    int windowHeight = 750;
    int offset = 50;
    

    InitWindow(windowWidth + offset, windowHeight + offset * 2, "Asteroid");
    InitAudioDevice();

    SetTargetFPS(60);
    Font font = LoadFontEx("D:/Project game/Asset/Font/pixel.ttf", 64, 0, 0);
    

    Game game;

    while(WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLinesEx({10, 10, 780, 830}, 0.18f, 20, 2, game.ThemeColor);
        DrawLineEx({25, 780}, {775, 780}, 3, game.ThemeColor);
        
        if(game.run) {
            std::string LevelText = FormatLevel(game.level);
            DrawTextEx(font, LevelText.c_str(), {570, 790}, 34, 2, game.ThemeColor);
        } else {
            DrawTextEx(font, "GAME OVER", {570, 790}, 34, 2, game.ThemeColor);
            DrawTextEx(font, "PRESS ENTER TO RESTART", {50, 790}, 34, 2, game.ThemeColor);
        }
        

        DrawTextEx(font, "SCORE", {50, 25}, 34, 2, game.ThemeColor);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 50}, 34, 2, game.ThemeColor);
        
        DrawTextEx(font, "HI-SCORE", {570, 25}, 34, 2, game.ThemeColor);
        std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {600, 50}, 34, 2, game.ThemeColor);

        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    CloseAudioDevice();
}
