#pragma once
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mysteryship.hpp"

class Game {
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int highscore;
        int level = 1;
        Music music;
        Color ThemeColor;
        Color yellow = {243, 216, 63, 255};
        Color green = {72, 251, 0, 255};
        Color red = {255, 63, 63, 255};
        Color blue = {43, 209, 252, 255};
        Color pink = {255, 72, 196, 255};
    private:
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();\
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
        Sound ExplosionSound;
        Sound GameOverSound;
        Sound RestartSound;
        Sound ShipHitSound;
};
