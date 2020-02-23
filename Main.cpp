#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <time.h>
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include<math.h>
#include<vector>
#include<cstdlib>

using namespace sf;
using namespace std;

const int N = 15;
const float PI = 3.14159265f;

class Bullet
{
public:
	CircleShape shape;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float radius = 3.f)
		: currVelocity(0.f, 0.f), maxSpeed(7.f)
	{
		this->shape.setRadius(radius);
		this->shape.setFillColor(Color::Red);
	}
};

int main()
{
	// creating the route 
	vector<int> xPath;
	vector<int> yPath;
	for (int i = 0; i < 150; i++) xPath.push_back(i);
	for (int i = 0; i < 80; i++) xPath.push_back(150);
	for (int i = 0; i < 110; i++) xPath.push_back(i + 150);
	for (int i = 0; i < 85; i++) xPath.push_back(260);
	for (int i = 0; i < 123; i++) xPath.push_back(i + 260);
	for (int i = 0; i < 190; i++) xPath.push_back(383);
	for (int i = 0; i < 123; i++) xPath.push_back(383 - i);
	for (int i = 0; i < 185; i++) xPath.push_back(260);
	for (int i = 0; i < 125; i++) xPath.push_back(260 + i);
	for (int i = 0; i < 95; i++) xPath.push_back(383);
	for (int i = 0; i < 120; i++) xPath.push_back(383 + i);
	for (int i = 0; i < 93; i++) xPath.push_back(503);
	for (int i = 0; i < 150; i++) xPath.push_back(503 + i);

	for (int i = 0; i < 150; i++) yPath.push_back(30);
	for (int i = 0; i < 80; i++) yPath.push_back(30 + i);
	for (int i = 0; i < 110; i++) yPath.push_back(110);
	for (int i = 0; i < 85; i++) yPath.push_back(110 - i);
	for (int i = 0; i < 123; i++) yPath.push_back(25);
	for (int i = 0; i < 190; i++) yPath.push_back(25 + i);
	for (int i = 0; i < 123; i++) yPath.push_back(215);
	for (int i = 0; i < 185; i++) yPath.push_back(215 + i);
	for (int i = 0; i < 125; i++) yPath.push_back(400);
	for (int i = 0; i < 95; i++) yPath.push_back(400 - i);
	for (int i = 0; i < 120; i++) yPath.push_back(305);
	for (int i = 0; i < 93; i++) yPath.push_back(305 + i);
	for (int i = 0; i < 150; i++) yPath.push_back(397);

	int livesInt = 15;

	// app's window
	srand(time(0));
	RenderWindow app(VideoMode(800,600),"New window");
	app.setFramerateLimit(60);

	// triangle that shoots
	CircleShape circle(10.f);
	circle.setFillColor(Color::Red);
	circle.setPointCount(3);
	circle.setOrigin(10.f, 10.f);
	circle.setPosition(195, 255);

	// vectors
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
           
	// images
	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("images/map.png");
	t2.loadFromFile("images/chars_50.png");
	t4.loadFromFile("images/castle1_90x90.png");
	t5.loadFromFile("images/castle2_150x150.png");
	t6.loadFromFile("images/lives_25x25.png");

	Sprite background(t1);
	Sprite character(t2);
	Sprite defence(t3);
	Sprite castle1(t4);
	Sprite castle2(t5);
	Sprite lives(t6);

	character.setPosition(10, 50);
	character.setTextureRect(IntRect(170, 30, 50, 60));
	castle1.setPosition(150, 250);
	castle2.setPosition(660, 321);
	lives.setPosition(733, 96);

	// font
	Font font;
	font.loadFromFile("Fonts/font.ttf");
	//texts
	Text text;
	text.setFont(font);
	text.setString("DEFENCE THE TOWER!");
	text.setCharacterSize(45);
	text.setFillColor(Color::White);
	text.setPosition(140, 510);

	Text leftClick;
	leftClick.setFont(font);
	leftClick.setString("Left-Click to shoot");
	leftClick.setCharacterSize(12);
	leftClick.setFillColor(Color::White);
	leftClick.setPosition(10, 560);

	Text livesText;
	livesText.setFont(font);
	livesText.setString("Lives     :");
	livesText.setCharacterSize(14);
	livesText.setFillColor(Color::White);
	livesText.setStyle(Text::Bold);
	livesText.setPosition(695, 100);

	Text livesCount;
	livesCount.setFont(font);
	livesCount.setString(to_string(livesInt));
	livesCount.setCharacterSize(14);
	livesCount.setFillColor(Color::White);
	livesCount.setStyle(Text::Bold);
	livesCount.setPosition(770, 100);

	// bullets
	Bullet b1;
	vector<Bullet> bullets;

	// characters
	vector <Sprite> characters;
	for (int i = 0; i < N; i++) 
	{
		characters.push_back(character);
	}
	// each character's index in route 
	vector<int> charsPositionsX;
	vector<int> charsPositionsY;
	for (int j = 0; j < N; j++)
	{
		charsPositionsX.push_back(-1);
	}
	for (int j = 0; j < N; j++) 
	{
		charsPositionsY.push_back(-1);
	}
	
	

	// variables for deploying characters
	int k = 0;
	int charsNumber = 0;

	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				app.close();
		}

		// deploying characters one by one
		if (k % 75 == 0 && charsNumber < charsPositionsX.size())
		{
			charsPositionsX[charsNumber] = 0;
			charsPositionsY[charsNumber] = 0;
			charsNumber++;
		}
		if (charsNumber < charsPositionsX.size()) 
		{
			k++;
		}

		// directing the shooter and bullets
		playerCenter = Vector2f(circle.getPosition());
		mousePosWindow = Vector2f(Mouse::getPosition(app));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
		circle.setRotation(deg + 90);

		// if button pressed creates bullets
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			b1.shape.setPosition(playerCenter);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;
			bullets.push_back(Bullet(b1));
		}

		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].shape.move(bullets[i].currVelocity);
			//Out of bounds
			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > app.getSize().x
				|| bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > app.getSize().y)
			{
				bullets.erase(bullets.begin() + i);
			}
			// Is character hit by bullet
			else 
			{
				for (int j = 0; j < characters.size(); j++)
				{
					if (bullets[i].shape.getGlobalBounds().intersects(characters[j].getGlobalBounds()))
					{
						bullets.erase(bullets.begin() + i);
						characters.erase(characters.begin() + j);
						charsPositionsX.erase(charsPositionsX.begin() + j);
						charsPositionsY.erase(charsPositionsY.begin() + j);
						break;
					}
				}
			}
		}
		
		// moving the characters
		for (int j = 0; j < characters.size(); j++)
		{
			if (charsPositionsX[j] != -1 && charsPositionsX[j] < xPath.size())
			{
				characters[j].setPosition(xPath[charsPositionsX[j]], yPath[charsPositionsY[j]]);
				charsPositionsX[j]++;
				charsPositionsY[j]++;
			}
			if (charsPositionsX[j] == xPath.size()) {
				livesInt--;
				charsPositionsX[j] = xPath.size() + 1;
				livesCount.setString(to_string(livesInt));
			}
		}


		// drawing
		app.clear();
		app.draw(background);
		app.draw(castle1);
		app.draw(castle2);
		app.draw(circle);
		app.draw(lives);
		app.draw(text);
		app.draw(leftClick);
		app.draw(livesText);
		app.draw(livesCount);
		

		for (int i = 0; i < bullets.size(); i++)
		{
			app.draw(bullets[i].shape);
		}
		for (int i = 0; i < characters.size(); i++)
		{
			if(charsPositionsX[i] != -1 && charsPositionsX[i] < xPath.size())
			app.draw(characters[i]);
		}

		app.display();
		
	}


	return 0;
}
