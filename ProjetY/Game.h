#pragma once

#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include "Room.h"

#define PI 3.14159265359

class Button;

class Game{
public :
	Game();
	~Game();

	void launch();//main method

	void simulate();//moves all objects
	void resolveCollisions();
	void draw();

	static const double getTimeStep(){return timeStep;}
	void endOfFrame(){timeStep=clock.getElapsedTime().asSeconds();clock.restart();}

	const static int invulnerability_frames;//standard number of invulnerability frames after a hit
	static const double friction;//friction coefficient
	const static double reject;//standard repelling coefficient
	const static double maxAxisSpeed;//maximum speed along an axis (x or y)
	const static Point2d g;//gravity
private :
	void LoadRoom();
	void LoadTestRoom();

	std::vector<Button*> buttons;

	Room room;

	static double timeStep;
	sf::Clock clock;
	bool gamePaused;//is the game paused ?

	int fps;//intended number of frames per second
	sf::View view;
	int windowWidth, windowHeight;
	int viewWidth, viewHeight;
	sf::RenderWindow mainWindow;
	int currentGameState;

	sf::Font titleFont;
};

enum DmgType{
	RAW,
	FIRE,
	WATER,
	EARTH,
	WIND
};

enum GameState{
	TITLE_SCREEN,
	MENU,
	GAME_RUNNING
};