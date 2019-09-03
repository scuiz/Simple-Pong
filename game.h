#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>

#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>

class game
{
private:
	
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;

	sf::Font scoreFont;
	sf::Text scoreText;

	sf::RectangleShape paddleLeft, paddleRight;
	sf::RectangleShape ball;

	enum ballDir { STOP = 0, UPLEFT = 1, DOWNLEFT = 2, UPRIGHT = 3, DOWNRIGHT = 4 };
	ballDir ballDirection;
	float ballSpeed;

	unsigned int scoreLeft, scoreRight;


	//Initializers
	void initVariables();
	void initWindow();
	void initScore();
	void initObjects();

public:
	//Constructors and destructors
	game();
	~game();

	//Game functions
	const bool isRunning() const;
	
	void pollEvents();
	
	void resetObjects();

	void updateBall();
	void updatePaddles();
	void updateScore();
	void update();
	
	void renderObjects();
	void renderScore();
	void render();

	


};

#endif
