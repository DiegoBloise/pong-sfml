#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;
using namespace sf;

Vector2f iaMovement(RectangleShape iaPaddleShape, bool &iaPursue, const float &iaResponseTime, const float &scrWidth, const float &scrHeight, const Vector2f &ballDir, const RectangleShape &ball);
Vector2f normalized(const Vector2f source);
// void debug(void);


int main() {

	// Create Window Setup
	float scrWidth = 1280;
	float scrHeight = 720;

	VideoMode vm(scrWidth, scrHeight);
	RenderWindow window(vm, "Pong", Style::Default);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(false);

	// Capture Mouse
	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);


	///////////////////// Game Logic /////////////////////

	Clock deltaClock;
	float deltaTime;

	float initialBallSpeed;
	float ballSpeed;
	Vector2f ballDir;

	float paddleSpeed;
	Vector2f paddleVelocity;

	Vector2f iaPaddleLeftVelocity;
	Vector2f iaPaddleRightVelocity;

	float iaResponseTime;
	bool iaPursue;

	int playerScore;
	int iaScore;

	FloatRect ballCollisionBox;
	FloatRect paddleLeftCollisionBox;
	FloatRect paddleRightCollisionBox;


	///////////////////// Game Shapes /////////////////////

	// Ball Shape
	RectangleShape ball;
	ball.setFillColor(Color::Green);
	ball.setSize(Vector2f(20.f, 20.f));
	ball.setOrigin(ball.getSize().x / 2.f, ball.getSize().y / 2.f);
	ball.setPosition(scrWidth / 2.f, scrHeight / 2.f);


	// Field Line Shape
	RectangleShape line;
	line.setFillColor(Color::White);
	line.setSize(Vector2f(5.f, scrHeight));
	line.setOrigin(line.getSize().x / 2.f, scrHeight / 2.f);
	line.setPosition(scrWidth / 2.f, scrHeight / 2.f);


	// General Paddle Shape
	RectangleShape paddleShape;
	paddleShape.setFillColor(Color::White);
	paddleShape.setSize(Vector2f(15.f, 95.f));
	paddleShape.setOrigin(paddleShape.getSize().x / 2.f, paddleShape.getSize().y / 2.f);


	// Paddles
	RectangleShape paddleLeftShape;
	RectangleShape paddleRightShape;
	paddleLeftShape = paddleShape;
	paddleRightShape = paddleShape;
	paddleLeftShape.setPosition(42.f, scrHeight / 2.f);
	paddleRightShape.setPosition(scrWidth - 42.f, scrHeight / 2.f);


	// UI
	String text;
	Font font;
	Text scoreUI;
	font.loadFromFile("font/font.ttf");
	scoreUI.setFillColor(Color::White);
	scoreUI.setCharacterSize(42);
	scoreUI.setFont(font);


	// Audio
	Sound sound;
	SoundBuffer goal;
	SoundBuffer padWall;

	goal.loadFromFile("audio/goal.wav");
	padWall.loadFromFile("audio/pad_wall.wav");


	// Debug
	CircleShape pivotPoint;


	/*
	************************************************************************************
	INIT
	************************************************************************************
	*/

	initialBallSpeed = 242.f;
	ballSpeed = initialBallSpeed;
	ballDir = Vector2f(1.f, 1.f);

	paddleSpeed = 2000.f; //35
	paddleVelocity = Vector2f(0, 0);

	iaPaddleLeftVelocity = Vector2f(0, 0);
	iaPaddleRightVelocity = Vector2f(0, 0);

	iaResponseTime = 250;
	iaPursue = true;

	playerScore = 0;
	iaScore = 0;

	ballCollisionBox = ball.getGlobalBounds();
	paddleLeftCollisionBox = paddleLeftShape.getGlobalBounds();
	paddleRightCollisionBox = paddleRightShape.getGlobalBounds();


	// Debug
	pivotPoint.setRadius(4.f);
	pivotPoint.setFillColor(Color::Red);
	pivotPoint.setOrigin(pivotPoint.getLocalBounds().width/2.f, pivotPoint.getLocalBounds().height/2.f);



	/*
	************************************************************************************
	GAME LOOP
	************************************************************************************
	*/
	while (window.isOpen()) {
		deltaTime = deltaClock.restart().asSeconds();

		///////////////////// Close Game /////////////////////
		sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}




		/*
		************************************************************************************
		Handle the players input
		************************************************************************************
		*/

		paddleVelocity.y = 0;

		// Play with keyboard
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
			if (paddleRightShape.getPosition().y > paddleRightShape.getSize().y / 2) {
				paddleVelocity.y = -paddleSpeed;
			}
		} else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
			if (paddleRightShape.getPosition().y < scrHeight - paddleRightShape.getSize().y / 2) {
				paddleVelocity.y = paddleSpeed;
			}
		}




		/*
		************************************************************************************
		Update Game Logic
		************************************************************************************
		*/

		///////////////////// Ball Movement /////////////////////
		// When ball hit the walls
		if (ball.getPosition().y < ball.getSize().y && ballDir.y < 0){
			ballDir.y *= -1.f;
			sound.setBuffer(padWall);
			sound.setPitch(1);
			sound.play();
		} else if (ball.getPosition().y > scrHeight - ball.getSize().y && ballDir.y > 0) {
			ballDir.y *= -1.f;
			sound.setBuffer(padWall);
			sound.setPitch(1);
			sound.play();
		}


		ballCollisionBox = ball.getGlobalBounds();
		paddleLeftCollisionBox = paddleLeftShape.getGlobalBounds();
		paddleRightCollisionBox = paddleRightShape.getGlobalBounds();


		///////////////////// When ball hit the paddles - Collisions /////////////////////
		if (ballCollisionBox.intersects(paddleLeftCollisionBox) && ballDir.x < 0) {
			ballDir.x *= -1.f;
			//srand(time(0)*20);
			//ballSpeed = (rand() % 21) + 21;
			ballSpeed += 50;

			srand(time(0)*20);
			int rndResponseTime = (rand() % 500);
			iaResponseTime = 0;//rndResponseTime;

			// TODO: Melhorar angulo
			double rnd = 1.0 * rand() / (RAND_MAX / 2) - 1;
			ballDir.y = rnd;

			sound.setBuffer(padWall);
			sound.setPitch(1.5f);
			sound.play();
		}
		if (ballCollisionBox.intersects(paddleRightCollisionBox) && ballDir.x < 0) {
			ballDir.x *= -1.f;
			//srand(time(0)*20);
			//ballSpeed = (rand() % 21) + 21;
			ballSpeed += 50;

			srand(time(0)*20);
			int rndResponseTime = (rand() % 500);
			iaResponseTime = 0;//rndResponseTime;

			// TODO: Melhorar angulo
			double rnd = 1.0 * rand() / (RAND_MAX / 2) - 1;
			ballDir.y = rnd;

			sound.setBuffer(padWall);
			sound.setPitch(1.5f);
			sound.play();
		}


		///////////////////// Scores /////////////////////
		// Player score
		if (ball.getPosition().x < 0) {
			playerScore += 1;
			ballDir = Vector2f(1.f, 1.f);

			sound.setBuffer(goal);
			sound.setPitch(1);
			sound.play();

			// Reset game
			ballSpeed = initialBallSpeed;
			ball.setPosition(scrWidth / 2.f, scrHeight / 2.f);
			iaResponseTime = 0;
		}

		// IA score
		else if (ball.getPosition().x > scrWidth) {
			iaScore += 1;
			ballDir = Vector2f(-1.f, 1.f);
			sound.setBuffer(goal);
			sound.setPitch(1);
			sound.play();

			// Reset game
			ballSpeed = initialBallSpeed;
			ball.setPosition(scrWidth / 2.f, scrHeight / 2.f);
			iaResponseTime = 0;
		}


		/*
		************************************************************************************
		Update the scene
		************************************************************************************
		*/
		text = to_string(iaScore) + "  " + to_string(playerScore);

		scoreUI.setString(text);
		scoreUI.setOrigin(scoreUI.getGlobalBounds().width / 2.f, scoreUI.getGlobalBounds().height / 2.f);
		scoreUI.setPosition((scrWidth - 8) / 2.f, scrHeight / 2.f);


		//paddleRightShape.move(paddleVelocity); TODO
		paddleLeftShape.move(iaMovement(paddleLeftShape, iaPursue, iaResponseTime, scrWidth, scrHeight, ballDir, ball) * paddleSpeed * deltaTime);
		paddleRightShape.move(iaMovement(paddleRightShape, iaPursue, iaResponseTime, scrWidth, scrHeight, ballDir, ball) * paddleSpeed * deltaTime);

		ballDir = normalized(ballDir);
		ball.move(Vector2f(ballDir.x * ballSpeed, ballDir.y * ballSpeed) * deltaTime);


		// Debug
		//debug();


		/*
		************************************************************************************
		Draw the scene
		************************************************************************************
		*/
		window.clear();


		window.draw(line);
		window.draw(scoreUI);
		window.draw(ball);
		window.draw(paddleLeftShape);
		window.draw(paddleRightShape);


		window.display();
	}

	return 0;
}


/*
Vector2f normalize(const Vector2f& source) {
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return Vector2f(source.x / length, source.y / length);
	else
		return source;
}*/


Vector2f normalized(const Vector2f source) {
	Vector2f normalized;
	double length;

	length = sqrt((source.x * source.x) + (source.y * source.y));
	normalized.x = source.x / length;
	normalized.y = source.y / length;
	return normalized;
}


Vector2f iaMovement(RectangleShape iaPaddleShape, bool &iaPursue, const float &iaResponseTime, const float &scrWidth, const float &scrHeight, const Vector2f &ballDir, const RectangleShape &ball) {
	Vector2f iaPaddleVelocity = Vector2f(0, 0);

	if (ball.getPosition().x + iaResponseTime < scrWidth / 2.f && ballDir.x < 0) {
		iaPursue = true;
	}
	else {
		iaPursue = false;
	}

	if (iaPursue) {
		if (iaPaddleShape.getPosition().y > ball.getPosition().y) {
			if (iaPaddleShape.getPosition().y > iaPaddleShape.getSize().y / 2) {
				iaPaddleVelocity.y = -1;
			}
		}
		if (iaPaddleShape.getPosition().y < ball.getPosition().y) {
			if (iaPaddleShape.getPosition().y < scrHeight - iaPaddleShape.getSize().y / 2) {
				iaPaddleVelocity.y = 1;
			}
		}
	}
	if (!iaPursue) {
		if (iaPaddleShape.getPosition().y < scrHeight / 2.f) {
			iaPaddleVelocity.y = 1;
		}
		if (iaPaddleShape.getPosition().y > scrHeight / 2.f) {
			iaPaddleVelocity.y = -1;
		}

	}
	return iaPaddleVelocity;
}


/*
void print_vector(float x, float y) {
	cout << "(X: " << to_string(x) << ", Y: " << to_string(y) << ")\n";
}


void debug(void) {
	// print_vector(ball.getPosition().x, ball.getPosition().y);
	// print_vector(paddleRightShape.getOrigin().x, paddleRightShape.getOrigin().y);
	// print_vector(paddleLeftShape.getPosition().x, paddleLeftShape.getPosition().y);
	// print_vector(paddleRightShape.getOrigin().x, paddleRightShape.getOrigin().y);
	// print_vector(paddleLeftShape.getOrigin().x, paddleLeftShape.getOrigin().y);
	// print_vector(ballSpeed.x, ballSpeed.y);
	// print_vector(ballDir.x, ballDir.y);

	// std::cout << "\nball_state: HIT" << std::endl;
	// std::cout << "\nball_state: enabled!" << std::endl;

	// FloatRect textRect = scoreUI.getLocalBounds();
	// string debug_text = "textRect.top: " + to_string(textRect.width) + "\ntextRect.width: " + to_string(textRect.width) + "\n";
	// cout << debug_text;
	// pivotPoint.setPosition(paddleLeftShape.getPosition());

	//cout << "ballSpeed: " << ballSpeed << endl; // Debug
	//cout << "iaResponseTime is now: " << iaResponseTime << endl; // Debug
	//cout << "Ball: " << rnd << endl; // Debug

	//HUEHUAHEUAHEUAH experimentar mudar o deltaTime de "asSeconds()" para "asMilliseconds()" HAUEHAUHEUAHEUAHEUAHUE
}
*/