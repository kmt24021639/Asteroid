#include "game.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

int cellSize = 55;
Game::Game()
{
    music = LoadMusicStream("D:/Project game/Asset/Sound/music.ogg");
    ExplosionSound = LoadSound("D:/Project game/Asset/Sound/boom.ogg");
    GameOverSound = LoadSound("D:/Project game/Asset/Sound/gameover.ogg");
    RestartSound = LoadSound("D:/Project game/Asset/Sound/restart.ogg");
    ShipHitSound = LoadSound("D:/Project game/Asset/Sound/shiphit.ogg");
    ShieldHitSound = LoadSound("D:/Project game/Asset/Sound/ting.ogg");
    PowerUpSound = LoadSound("D:/Project game/Asset/Sound/powerup.ogg");
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
        
        float x = 50;
        for(int i = 1; i <= playerlives; i++) {
            DrawTextureV(spaceshipImage, {x, 795}, WHITE);
            x+= 70;
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

        for(auto& powerup: powerups) {
            powerup.Update(4);
        }

        CheckForCollisions();
        
        LevelUpdate();
        
        if(playerlives > 0) {
            if(spaceship.PUblaster){
                DrawTextureV(blasterImage, {273 + 51 * 4, 795}, WHITE);
            } 

            if(spaceship.PUFflasher){
                DrawTextureV(flasherImage, {273 + 51, 795}, WHITE);
            } 

            if(spaceship.PUshield){
                DrawTextureV(shieldImage, {273 + 51 * 2, 795}, WHITE);
            }

            if(spaceship.PUtripleshoot){
                DrawTextureV(tripleshootImage, {273 + 51 * 3, 795}, WHITE);
            } 

            if(spaceship.PUxtreme){
                DrawTextureV(xtremeImage, {273, 795}, WHITE);
            }
        }
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

    for(auto& powerup: powerups) {
        powerup.Draw();
    }
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
    alienlives1 = 1 + level / 5;
    alienlives2 = 1 + level / 3;
    alienlives3 = 1 + level / 2;
    if (alienlives1 > 2) alienlives1 = 2;
    if (alienlives2 > 3) alienlives2 = 3;
    if (alienlives3 > 4) alienlives3 = 4;
    for(int row = 0; row < 6; row++) {
        for(int column = 0; column < 11; column++) {
            int alienType;
            int alienLives;
            if(row == 0 || row == 1) {
                alienType = 3;
                alienLives = alienlives3;
            } else if (row == 2 || row == 3) {
                alienType = 2;
                alienLives = alienlives2;
            } else {
                alienType = 1;
                alienLives = alienlives1;
            }

            float x = 75 + column * cellSize;
            float y = 110 + row * cellSize;
            Alien newAlien(alienType, {x, y});
            newAlien.lives = alienLives;
            aliens.push_back(newAlien);
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
                it->lives--;
                laser.active = false;
    
                if(it->lives <= 0) {
                    if(it -> type == 1) {
                        score += 100;
                    } else if (it -> type == 2) {
                        score += 200;
                    } else {
                        score += 300;
                    }
                    it = aliens.erase(it);
                    PlaySound(ExplosionSound);
                } else {
                    it++;
                }
                CheckForHighScore();
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
    }

    //Alien laser
    for(auto& laser: alienLasers) {
        if(CheckCollisionRecs(laser.getRect(), spaceship.getRect())) {
            
            laser.active = false;
            if(!spaceship.PUshield) {
                playerlives--;
                PlaySound(ShipHitSound);
            }
            if(spaceship.PUshield) {
                PlaySound(ShieldHitSound);
                score += 10;
                if(level > currentLevel) {
                    spaceship.PUshield = false;
                }
            }
            if(playerlives == 0) {
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

    //power up
    for(auto it = powerups.begin(); it != powerups.end();) {
        if(CheckCollisionRecs(it->getRect(), spaceship.getRect())) {
            switch (it -> GetType())
            {
            case 1:
                spaceship.PUblaster = true;
                spaceship.ShootTime /= 2;
                PlaySound(PowerUpSound);
                break;
            case 2:
                spaceship.PUFflasher = true;
                spaceship.LaserSpeed *= 2;
                PlaySound(PowerUpSound);
                break;
            case 3:
                spaceship.PUshield = true;
                PlaySound(PowerUpSound);
                currentLevel = level;
                break;
            case 4:
                spaceship.PUtripleshoot = true;
                PlaySound(PowerUpSound);
                break;
            case 5:
                spaceship.PUxtreme = true;
                PlaySound(PowerUpSound);
                break;
            default:
                break;
            }         
            it = powerups.erase(it);
        } else {
            ++it;
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
    playerlives = 3;
    score = 0;
    level = 1;
    highscore = LoadHighScoreFromFile();
    run = true;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    powerUpCount = 0;
    spaceship.ShootTime = 0.35;
    spaceship.LaserSpeed = 7;
    spaceship.PUtripleshoot = false;
    spaceship.PUxtreme = true;
    spaceship.PUshield = false;
    spaceship.PUblaster = false;
    spaceship.PUFflasher = false;
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

void Game::SpawnPowerUp()
{
    if(powerUpCount >= 5) return;

    std::vector<int> availableTypes;
    for(int i = 1; i <= 5; ++i) {
        if(std::find(usedPowerUpTypes.begin(), usedPowerUpTypes.end(), i) == usedPowerUpTypes.end()) {
            availableTypes.push_back(i);
        }
    }

    if(!availableTypes.empty()) {
        int randomIndex = GetRandomValue(0, availableTypes.size() - 1);
        int selectedType = availableTypes[randomIndex];

        usedPowerUpTypes.push_back(selectedType);

        float x = GetRandomValue(50, GetScreenWidth() - 50);
        Vector2 position = {x, 0};
        powerups.push_back(PowerUp(selectedType, position));

        powerUpCount++;

        if(usedPowerUpTypes.size() >= 5) {
            usedPowerUpTypes.clear();
        }
    }
}

void Game::LevelUpdate() {
    if (run) {
        if (level % 3 == 0) {
            obstacles.clear();
            obstacles = CreateObstacles();
        }

        switch (level % 5)
        {
        case 1:
            ThemeColor = yellow;
            break;
        case 2:
            ThemeColor = green;
            break;
        case 3:
            ThemeColor = red;
            break;
        case 4:
            ThemeColor = blue;
            break;
        case 0:
            ThemeColor = pink;
            break;
        default:
            break;
        }

        if(aliens.empty()){
            aliens = CreateAliens();
            alienLaserShootInterval *= 0.95;
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
            if(playerlives < 3) {
                playerlives++;
            }
            level++;
            spaceship.lasers.clear();
            SpawnPowerUp();
            PlaySound(RestartSound);
        }
    }
}

void Game::Reset()
{
    level = 1;
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
    PlaySound(RestartSound);
    powerUpCount = 0;
}
