#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"
#include "powerup.hpp"

class Game {
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int playerlives;
        int score;
        int highscore;
        int level = 1;
        int alienlives1;
        int alienlives2;
        int alienlives3;
        Music music;
        Color ThemeColor;
        Color yellow = {243, 216, 63, 255};
        Color green = {72, 251, 0, 255};
        Color red = {255, 63, 63, 255};
        Color blue = {43, 209, 252, 255};
        Color pink = {255, 72, 196, 255};
        Texture2D spaceshipImage = LoadTexture("D:/Project game/Asset/Ship/player.png");
        Texture2D blasterImage = LoadTexture("D:/Project game/Asset/PowerUp/blaster.png");
        Texture2D flasherImage = LoadTexture("D:/Project game/Asset/PowerUp/flasher.png");
        Texture2D tripleshootImage = LoadTexture("D:/Project game/Asset/PowerUp/tripleshoot.png");
        Texture2D shieldImage = LoadTexture("D:/Project game/Asset/PowerUp/shield.png");
        Texture2D xtremeImage = LoadTexture("D:/Project game/Asset/PowerUp/xtreme.png");
    private:
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveDownAliens(int distance);
        void AlienShootLaser();
        void CheckForCollisions();
        void GameOver();
        void Reset();
        void InitGame();
        void CheckForHighScore();
        void SaveHighScoreToFile(int highscore);
        int LoadHighScoreFromFile();
        void SpawnPowerUp();
        void LevelUpdate();
        Spaceship spaceship;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        int alienDirection;
        std::vector<Laser> alienLasers;
        float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        MysteryShip mysteryship;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        std::vector<PowerUp> powerups;
        std::vector<int> usedPowerUpTypes;
        int powerUpCount = 0;
        int currentLevel;
        Sound ExplosionSound;
        Sound GameOverSound;
        Sound RestartSound;
        Sound ShipHitSound;
        Sound ShieldHitSound;
        Sound PowerUpSound;
};
