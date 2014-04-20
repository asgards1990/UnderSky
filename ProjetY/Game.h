#pragma once

#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include "Room.h"

#define PI 3.14159265359

class Button;

struct Type{
	char* keyword;
	int ID;
};

class Game{
public :
	Game();
	~Game();

	/*
	main method
	*/
	void launch();

	/*
	moves all objects
	*/
	void simulate();
	void resolveCollisions();
	void draw();

	static const double getTimeStep(){return timeStep;}
	void endOfFrame(){timeStep=clock.getElapsedTime().asSeconds();clock.restart();}

	static char* Game::lTrim(char* buffer);
	static char* Game::rTrim(char* buffer);
	/*
	removes blank spaces at the beginning and at the ed of the buffer
	*/
	static char* Game::trim(char* buffer);
	/*
	This method returns the name associated with the class of ID id
	*/
	static char* getTypeKeyword(int id);
	/*
	This method returns the ID associated with the class of the provided name
	*/
	static int getTypeID(char* &name);
	/*
	This method reads a line from a file and removes blank spaces and such at the beginning and end of the line
	*/
	static bool readValidLine(char* line, FILE* fp, int n);

	static bool isWhiteSpace(char ch);

	const static int invulnerability_frames;//standard number of invulnerability frames after a hit
	static const double friction;//friction coefficient
	const static double reject;//standard repelling coefficient
	const static double maxAxisSpeed;//maximum speed along an axis (x or y)
	const static Point2d g;//gravity
	const static Type typeList[];
private :
	bool loadRoom(const char* path);
	void loadTestRoom();

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

enum KEYWORDS{
	NOT_FOUND,
	END,
	STILL,
	ROOM_BOUNDING_BOX,
	PASSIVE_EXIT,
	ACTIVE_EXIT,
	PEON,
	SPEARMAN,
	HEART_OF_FIRE,
	POSITION,
	FACING_RIGHT,
	SQUARE_HITBOX,
	CONVEX_POLYGON,
	TEXTURE_PATH,
	TILE_DIMENSION,
	TILE_INDICES,
	TILE_ROTATIONS,
	TILESET_DIMENSION,
	TILE_ORIGIN,
	TILE_SCALE,
	ROOM_PATH,
	HORIZONTAL
};

