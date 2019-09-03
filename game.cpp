#include "game.h"


//Constructors and destructors
game::game() {
	this->initVariables();
	this->initWindow();
	this->initObjects();
	this->initScore();
	
}

game::~game() {
	delete this->window;
}


//Initialize the window
void game::initWindow() {
	
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(100);
}


//Initialize our variables
void game::initVariables() {
	this->window = nullptr;
	
	if (!this->scoreFont.loadFromFile("fonts/sansation.ttf"))
	{
		std::cout << "Error loading font\n";
	}	
	
	this->ballDirection = STOP;
	this->scoreLeft = 0;
	this->scoreRight = 0;
	this->ballSpeed = 5.f; //this MUST be less than the paddle width -> otherwise ball can travel through the paddle to score
}

void game::initScore()
{
	this->scoreText.setFont(this->scoreFont);
	this->scoreText.setCharacterSize(20);
	this->scoreText.setStyle(sf::Text::Bold);
	this->scoreText.setPosition(0, 0);
	this->scoreText.setString("0");
}

//Initialize our paddles and ball
void game::initObjects() {
	
	sf::Vector2f paddleSize(10.f, 150.f);
	
	this->ball.setSize(sf::Vector2f(10.f, 10.f));
	this->ball.setPosition(
		0.5 * this->window->getSize().x - 0.5*ball.getSize().x,
		0.5 * this->window->getSize().y - 0.5*ball.getSize().y
	);
	this->ball.setFillColor(sf::Color::Cyan);

	this->paddleLeft.setSize(paddleSize);
	this->paddleLeft.setPosition(
		0.f,
		0.5*this->window->getSize().y - 0.5*this->paddleLeft.getSize().y
	);
	this->paddleLeft.setFillColor(sf::Color::Blue);

	this->paddleRight.setSize(paddleSize);
	this->paddleRight.setPosition(
		this->window->getSize().x - this->paddleRight.getSize().x,
		0.5 * this->window->getSize().y - 0.5 * this->paddleRight.getSize().y
	);
	this->paddleRight.setFillColor(sf::Color::Red);



}


//Game functions
const bool game::isRunning() const {
	return(this->window->isOpen() );
}

void game::pollEvents() {

	while (this->window->pollEvent(this->event)) {
		switch (this->event.type) {
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape) {
				this->window->close();
				break;
			}
		}
	}

}


void game::resetObjects() {
	
	//Move ball to middle of screen and stop it moving
	this->ballDirection = STOP;
	this->ball.setPosition(
		0.5 * this->window->getSize().x - 0.5 * ball.getSize().x,
		0.5 * this->window->getSize().y - 0.5 * ball.getSize().y
	);
	

	//Move paddles back to center position
	this->paddleLeft.setPosition(
		0.f,
		0.5 * this->window->getSize().y - 0.5 * this->paddleLeft.getSize().y
	);

	this->paddleRight.setPosition(
		this->window->getSize().x - this->paddleRight.getSize().x,
		0.5 * this->window->getSize().y - 0.5 * this->paddleRight.getSize().y
	);
}

void game::updateBall() {
	
	//after a reset the ball is stopped -> we kick it off in a random direction
	if (this->ballDirection == STOP) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		ballDirection = (ballDir)((rand() % 4) + 1);
	}

	//Move the ball logic
	switch (this->ballDirection) {
	case(DOWNLEFT):
		this->ball.move(-1.f * ballSpeed, 1.f * ballSpeed);
		break;
	case(UPLEFT):
		this->ball.move(-1.f * ballSpeed, -1.f * ballSpeed);
		break;
	case(UPRIGHT):
		this->ball.move(1.f * ballSpeed, -1.f * ballSpeed);
		break;
	case(DOWNRIGHT):
		this->ball.move(1.f * ballSpeed, 1.f * ballSpeed);
		break;
	case(STOP):
		break;
		
	}
	
	
	//Hitting paddle logic -> we base all hitting logic off the center of the ball for simplicity
	float ballCenterX = this->ball.getPosition().x + 0.5 * this->ball.getSize().x;
	float ballCenterY = this->ball.getPosition().y + 0.5 * this->ball.getSize().y;
	
	if (ballCenterX >= this->paddleRight.getPosition().x                                    //Center of ball <= LHS of R paddle
		&& ballCenterY <= this->paddleRight.getPosition().y + this->paddleRight.getSize().y //Center of ball is above bottom of R paddle
		&& ballCenterY >= this->paddleRight.getPosition().y) {                              //Center of ball is below top of R paddle
		
		if (this->ballDirection == UPRIGHT)   this->ballDirection = UPLEFT;
		if (this->ballDirection == DOWNRIGHT) this->ballDirection = DOWNLEFT;

	}

	if (ballCenterX <= this->paddleLeft.getPosition().x + this->paddleLeft.getSize().x       //Center of ball <= RHS of L paddle
		&& ballCenterY <= this->paddleLeft.getPosition().y + this->paddleLeft.getSize().y    //Center of ball is above bottom of L paddle
		&& ballCenterY >= this->paddleLeft.getPosition().y  ) {                              //Center of ball is below top of L paddle

		if (this->ballDirection == UPLEFT)   this->ballDirection = UPRIGHT;
		if (this->ballDirection == DOWNLEFT) this->ballDirection = DOWNRIGHT;

	}

	//Hitting wall logic -> top and bottom walls
	if (ballCenterY >= this->window->getSize().y) { 
		if (this->ballDirection == DOWNRIGHT) this->ballDirection = UPRIGHT;
		if (this->ballDirection == DOWNLEFT) this->ballDirection  = UPLEFT;
	}
	if (ballCenterY <= 0.f) {
		if (this->ballDirection == UPRIGHT) this->ballDirection = DOWNRIGHT;
		if (this->ballDirection == UPLEFT)  this->ballDirection = DOWNLEFT;
	}

	//Scoring logic -> left and right walls
	if (ballCenterX >= this->window->getSize().x) {
		scoreLeft += 1;
		this->resetObjects();
	}
	if (ballCenterX <= 0.f) {
		scoreRight += 1;
		this->resetObjects();
	}





}

void game::updatePaddles() {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		
		if (this->paddleLeft.getPosition().y >= 0.f) {
			this->paddleLeft.move(0.f, -1.f * ballSpeed);
		}

	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		
		if (this->paddleLeft.getPosition().y + paddleLeft.getSize().y <= this->window->getSize().y) {
			this->paddleLeft.move(0.f, 1.f * ballSpeed);
		}
			
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
		
		if (this->paddleRight.getPosition().y >= 0.f) {
			this->paddleRight.move(0.f, -1.f * ballSpeed);
		}

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		
		if (this->paddleRight.getPosition().y + paddleRight.getSize().y <= this->window->getSize().y) {
			this->paddleRight.move(0.f, 1.f * ballSpeed);
		}

	}
}

void game::updateScore() {

	std::ostringstream ss;
	ss << "Player LEFT: " << this->scoreLeft << "    //    Player RIGHT:" << this->scoreRight;

	this->scoreText.setString(ss.str());
}


void game::update() {
	this->pollEvents();
	this->updateBall();
	this->updatePaddles();
	this->updateScore();
}


void game::renderObjects() {

	this->window->draw(paddleLeft);
	this->window->draw(paddleRight);
	this->window->draw(ball);

}

void game::renderScore() {
	this->window->draw(this->scoreText);
}

void game::render() {
	
	//clear the screen
	this->window->clear();

	//draw the paddles and ball
	this->renderObjects();
	this->renderScore();

	//display what has been drawn
	this->window->display();
}


