#include "Game.h"
int window_height = 1200;
int window_width = 1600;
//Private functions
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(window_width, window_height), "Asteroid", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(120);
	this->window->setVerticalSyncEnabled(false);
}

//Con/Des
Game::Game()
{
	this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

//Hàm để chạy game
void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	sf::Event e;
	//Đóng game lại và di chyển title bar
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed) {
			this->window->close();
		}
		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape) {
			this->window->close();
		}
	}
}

void Game::render()
{
	this->window->clear();

	//Ve do hoa cho game

	this->window->display();
}
