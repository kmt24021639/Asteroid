#include "game.hpp"
#include <iostream>
#include <fstream>

int cellSize = 55;
Game::Game()
{
    music = LoadMusicStream("D:/Project game/Asset/music.ogg");
    ExplosionSound = LoadSound("D:/Project game/Asset/boom.ogg");
    GameOverSound = LoadSound("D:/Project game/Asset/gameover.ogg");
    RestartSound = LoadSound("D:/Project game/Asset/restart.ogg");
    ShipHitSound = LoadSound("D:/Project game/Asset/shiphit.ogg");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(ExplosionSound);
    UnloadSound(GameOverSound);
    UnloadSound(RestartSound);
    UnloadSound(ShipHitSound);
}

void Game::Update()
{
    if(run) {
        double currentTime = GetTime();
        if(currentTime - timeLastSpawn > mysteryShipSpawnInterval) {
            mysteryship.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }

        for(auto& laser: spaceship.lasers) {
            laser.Update();
        }

        MoveAliens();

        AlienShootLaser();

        for(auto& laser: alienLasers) {
            laser.Update();
        }

        DeleteInactiveLasers();
        mysteryship.Update();

        CheckForCollisions();
    } else {
        if(IsKeyDown(KEY_ENTER)) {
            Reset();
            InitGame();
        }
    }
}

void Game::Draw()
{
    spaceship.Draw();

    for(auto& laser: spaceship.lasers) {
        laser.Draw();
    }

    for(auto& obstacle: obstacles) {
        obstacle.Draw();
    }

    for(auto& alien: aliens) {
        alien.Draw();
    }

    for(auto& laser: alienLasers) {
        laser.Draw();
    }

    mysteryship.Draw();
}

void Game::HandleInput()
{
    if(run) {
        if(IsKeyDown(KEY_LEFT)) {
            spaceship.MoveLeft();
        } else if(IsKeyDown(KEY_RIGHT)) {
            spaceship.MoveRight();
        } else if(IsKeyDown(KEY_SPACE)) {
            spaceship.FireLaser();
        }
    }
}

void Game::DeleteInactiveLasers()
{
    for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();) {
        if(!it -> active) {
            it = spaceship.lasers.erase(it);
        } else {
            ++it;
        }
    }

    for(auto it = alienLasers.begin(); it != alienLasers.end();) {
        if(!it -> active) {
            it = alienLasers.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for(int i = 0; i < 4; i++) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 200)}));
    }
    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    
    for(int row = 0; row < 6; row++) {
        for(int column = 0; column < 11; column++) {
            
            int alienType;
            if(row == 0 || row == 1) {
                alienType = 3;
            } else if (row == 2 || row == 3) {
                alienType = 2;
            } else {
                alienType = 1;
            }

            float x = 75 + column * cellSize;
            float y = 110 + row * cellSize;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for(auto& alien: aliens) {
        if(alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25) {
            alienDirection = -1;
            MoveDownAliens(3);
        }
        if (alien.position.x < 25) {
            alienDirection = 1;
            MoveDownAliens(3);
        }
        alien.Update(alienDirection);
    }

}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien: aliens) {
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()) {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien& alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width/2,
                                    alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = GetTime();                        
    }   
}

void Game::CheckForCollisions()
{
    //Player laser

    for(auto& laser: spaceship.lasers) {
        auto it = aliens.begin();
        while(it != aliens.end()) {
            if(CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                PlaySound(ExplosionSound);
                if(it -> type == 1) {
                    score += 100;
                } else if (it -> type == 2) {
                    score += 200;
                } else {
                    score += 300;
                }
                CheckForHighScore();
                it = aliens.erase(it);
                laser.active = false;
            } else {
                ++it;
            }
        }

        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }

        if(CheckCollisionRecs(mysteryship.getRect(), laser.getRect())) {
            if(mysteryship.alive) {
                PlaySound(ExplosionSound);
                score += 500;
            }
            mysteryship.alive = false;
            laser.active = false;
            CheckForHighScore();
            
        }

        if(aliens.empty()){
            aliens = CreateAliens();
            alienLaserShootInterval *= 0.9;
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
            if(lives < 3) {
                lives++;
            }
            level++;
            PlaySound(RestartSound);
        }
    }

    //Alien laser
    for(auto& laser: alienLasers) {
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())) {
            PlaySound(ShipHitSound);
            laser.active = false;
            lives--;
            if(lives == 0) {
                GameOver();
            }
        }

        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), laser.getRect())) {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                } else {
                    ++it;
                }
            }
        }
    }

    //Alien with block
    for(auto& alien: aliens) {
        for(auto& obstacle: obstacles) {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end()) {
                if(CheckCollisionRecs(it -> getRect(), alien.getRect())) {
                    it = obstacle.blocks.erase(it);
                } else {
                    it++;
                }
            }
        }

        if(CheckCollisionRecs(alien.getRect(), spaceship.getRect())) {
            GameOver();
        }
    }
}

void Game::GameOver()
{
    run = false;
    PlaySound(GameOverSound);
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    alienDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    lives = 3;
    score = 0;
    highscore = LoadHighScoreFromFile();
    run = true;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
}

void Game::CheckForHighScore()
{
    if (score > highscore) {
        highscore = score;
        SaveHighScoreToFile(highscore);
    }
}

void Game::SaveHighScoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

int Game::LoadHighScoreFromFile()
{
    int loadedHighScore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile >> loadedHighScore;
        highscoreFile.close();
    } else {
        std::cerr << "Failed to load highscore from file." << std::endl;
    }
    return loadedHighScore;
}

void Game::Reset()
{
    level = 1;
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
    PlaySound(RestartSound);
}
