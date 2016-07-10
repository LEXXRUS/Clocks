#define _CRT_SECURE_NO_WARNINGS 

#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp> 
#include <ctime> 
#include <cmath> 
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace sf;
using namespace std;

// Define some variables and constants; 
const int screenWidth = 800;
const int screenHeight = 600;
const float PI = 3.1415927f;
const float clockCircleSize = 250;
const float clockCircleThickness = 10;

struct Screen
{
	ContextSettings settings;
	RenderWindow *window;
	Vector2f *windowCenter;
};

struct Arrow
{
	RectangleShape *hourHand;
	RectangleShape *minuteHand;
	RectangleShape *secondsHand;
};

struct Clocks
{
	Texture *clockImage;
	CircleShape *dot[60];
	CircleShape *clockCircle;
	CircleShape *centerCircle;
	Text *num[12];
};

void createArrows(RectangleShape *arrow, Vector2f windowCenter, Color color);
void createCircle(CircleShape *circle, int PointCount, int outTick, sf::Color color, int isFilled, float Width, float Height);
void createCenterCircle(CircleShape *circle, int PointCount, int outTick, Color color, int isFilled, float Width, float Height);
void createDots(Clocks *myClocks);
void createNumbers(Screen *clocksScreen, Clocks *myClocks);
void drawNumbers(Screen *clocksScreen, Clocks *myClocks);
void createBackground(Clocks *myClocks);

void initArrow(Arrow *clocksArrows, Screen *clocksScreen)
{
	clocksArrows->hourHand = new RectangleShape(Vector2f(5, 180));
	clocksArrows->minuteHand = new RectangleShape(Vector2f(3, 240));
	clocksArrows->secondsHand = new RectangleShape(Vector2f(2, 250));
	createArrows(clocksArrows->hourHand, *clocksScreen->windowCenter, Color::Black);
	createArrows(clocksArrows->minuteHand, *clocksScreen->windowCenter, Color::Black);
	createArrows(clocksArrows->secondsHand, *clocksScreen->windowCenter, Color::Red);
}

void initScreen(Screen *clocksScreen)
{
	clocksScreen->settings.antialiasingLevel = 8;
	clocksScreen->window = new RenderWindow (VideoMode(screenWidth, screenHeight), "SFML Analog Clock", Style::Close, clocksScreen->settings);
	clocksScreen->windowCenter = new Vector2f(screenWidth / 2.0f, screenHeight / 2.0f);
}

void initClocks(Clocks *myClocks, Screen *clocksScreen, Texture *clockImage)
{
	createDots(myClocks);
	drawNumbers(clocksScreen, myClocks);
	myClocks->clockCircle = new CircleShape(clockCircleSize);
	myClocks->clockImage = clockImage;
	myClocks->clockCircle->setTexture(myClocks->clockImage);
	myClocks->clockCircle->setTextureRect(IntRect(40, 0, 500, 500));
	createCircle(myClocks->clockCircle, 100, clockCircleThickness, Color::Black, 0, screenWidth / 2 + clockCircleThickness, screenHeight / 2 + clockCircleThickness);
	myClocks->centerCircle = new CircleShape(10);
	createCenterCircle(myClocks->centerCircle, 100, 1, Color::Red, 1, screenWidth/2, screenHeight/2);
	createBackground(myClocks);

}

// Create dots and place them to very right positions 
void createDots(Clocks *myClocks)
{
	float angle = 0.0;
	float x;
	float y;
	for (int i = 0; i<60; i++)
	{
		x = (clockCircleSize - 10) * cos(angle);
		y = (clockCircleSize - 10) * sin(angle);
		myClocks->dot[i] = (i % 5 == 0) ? new CircleShape(3) : new CircleShape(1);
		myClocks->dot[i]->setFillColor(Color::Black);
		myClocks->dot[i]->setOrigin(myClocks->dot[i]->getGlobalBounds().width / 2, myClocks->dot[i]->getGlobalBounds().height / 2);
		myClocks->dot[i]->setPosition(x + screenWidth / 2, y + screenHeight / 2);
		angle = angle + ((2 * PI) / 60);
	}
}

// Create outline of the clock 
void createCircle(CircleShape *circle, int PointCount, int outTick, Color color, int isFilled, float Width, float Height)
{
	circle->setPointCount(PointCount);
	circle->setOutlineThickness(clockCircleThickness);
	circle->setOutlineColor(color);
	circle->setOrigin(circle->getGlobalBounds().width / 2, circle->getGlobalBounds().height / 2);
	circle->setPosition(Width, Height);
}

//Make numbers 
void createNumbers(Screen *clocksScreen, Clocks *myClocks)
{
	float x;
	float y;
	float angl = (((-4)*PI) / 12);
	Font font;
	if (!font.loadFromFile("Files/Ubuntu-B.ttf"))
	{
		EXIT_FAILURE;
	}
	for (int i = 0; i < 12; i++)
	{
		x = int((clockCircleSize - 35) * cos(angl));
		y = int((clockCircleSize - 35) * sin(angl));
		sf::Text text;
		myClocks->num[i] = new Text;
		myClocks->num[i]->setFont(font);
		std::string variable = std::to_string(i + 1);
		myClocks->num[i]->setString(variable);
		myClocks->num[i]->setCharacterSize(24);
		myClocks->num[i]->setColor(Color::Black);
		myClocks->num[i]->setOrigin(myClocks->num[i]->getGlobalBounds().width / 2, myClocks->num[i]->getGlobalBounds().height / 1.1);
		myClocks->num[i]->setPosition(x + float((*clocksScreen).window->getSize().x) / 2, y + float((*clocksScreen).window->getSize().y) / 2);
		clocksScreen->window->draw(*myClocks->num[i]);
		angl = angl + ((2 * PI) / 12);
	}
}

// Crate another circle for center 
void createCenterCircle(CircleShape *circle, int PointCount, int outTick, Color color, int isFilled, float Width, float Height)
{
	circle->setPointCount(100);
	circle->setFillColor(color);
	circle->setOrigin(circle->getGlobalBounds().width / 2, circle->getGlobalBounds().height / 2);
	circle->setPosition(Width, Height);
}

// Draw Numbers 
void drawNumbers(Screen *clocksScreen, Clocks *myClocks)
{
	createNumbers(clocksScreen, myClocks);
	clocksScreen->window->display();
}

// Make strelki(h,m,s) 
void createArrows(RectangleShape *arrow, Vector2f windowCenter, Color color)
{
	arrow->setOrigin(arrow->getGlobalBounds().width / 2, arrow->getGlobalBounds().height - 25);
	arrow->setPosition(windowCenter);
	arrow->setFillColor(color);
}

// Create clock background 
void createBackground(Clocks *myClocks)
{
	if (!myClocks->clockImage->loadFromFile("Files/clock-image.jpg"))
	{
		EXIT_FAILURE;
	}
}

// Get system time 
void getSystemTime(Arrow* clocksArrows)
{
	time_t currentTime = time(NULL);

	struct tm * ptm = localtime(&currentTime);

	clocksArrows->hourHand->setRotation(ptm->tm_hour * 30 + (ptm->tm_min / 2));
	clocksArrows->minuteHand->setRotation(ptm->tm_min * 6 + (ptm->tm_sec / 12));
	clocksArrows->secondsHand->setRotation(ptm->tm_sec * 6);
}

// Clear the window and Draw all parts of clock 
void drawAllPartsOfClock(Arrow *clocksArrows, Screen *clocksScreen, Clocks *myClocks)
{
	clocksScreen->window->clear(Color::White);
	clocksScreen->window->draw(*myClocks->clockCircle);

	for (int i = 0; i<60; i++)
	{
		clocksScreen->window->draw(*myClocks->dot[i]);
	}
	clocksScreen->window->draw(*clocksArrows->hourHand);
	clocksScreen->window->draw(*clocksArrows->minuteHand);
	clocksScreen->window->draw(*clocksArrows->secondsHand);
	clocksScreen->window->draw(*myClocks->centerCircle);
	drawNumbers(clocksScreen, myClocks);
	// Display things on screen 
	clocksScreen->window->display();
}

void createTurnoverHours(Screen *clocksScreen, Arrow *clocksArrows, Clocks *myClocks, Event *event)
{
	while (clocksScreen->window->isOpen())
	{
		while (clocksScreen->window->pollEvent(*event))
		{
			// Window closed: exit 
			if ((*event).type == Event::Closed)
				clocksScreen->window->close();
		}
		getSystemTime(clocksArrows);
		drawAllPartsOfClock(clocksArrows, clocksScreen, myClocks);
	}
}

int main()
{
	Arrow clocksArrows;
	Screen clocksScreen;
	Clocks myClocks;
	
	Texture clockImage;

	initScreen(&clocksScreen);
	initArrow(&clocksArrows, &clocksScreen);
	initClocks(&myClocks, &clocksScreen, &clockImage);
	
	Event event;
	createTurnoverHours(&clocksScreen, &clocksArrows, &myClocks, &event);

	return EXIT_SUCCESS;
}