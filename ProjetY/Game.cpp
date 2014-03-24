#include "Game.h"
#include "Character.h"
#include "Still.h"
#include "YoungLady.h"
#include "Spearman.h"

const int Game::invulnerability_frames=5;
const double Game::friction = 2.0;
const double Game::reject = 100;
const double Game::maxAxisSpeed = 10;
const Point2d Game::g (0,9.8);

double Game::timeStep = 0.0;

Game::Game() :
	fps(60), windowWidth(800), windowHeight(800), viewWidth(10), viewHeight(10), mainWindow(sf::VideoMode(800, 800), "ProjetY"), view (sf::FloatRect(0,0,10,10)), gamePaused(false)
{
	mainWindow.setFramerateLimit(fps);
	mainWindow.setView(view);
}

Game::~Game()
{

}

void Game::simulate(){
	for(unsigned int i=0;i<room.fiends.size();i++){
		room.fiends[i]->simulate();
	}
	for(unsigned int i=0;i<room.projectiles.size();i++){
		room.projectiles[i]->simulate();
	}
	for(unsigned int i=0;i<room.scenery.size();i++){
		room.scenery[i]->simulate();
	}
	room.player->simulate();
}

void Game::resolveCollisions(){
	//test all possible collisions
	for(unsigned int i=room.fiends.size();i>0;i--){
		if(!room.boundingBox.isInside(room.fiends[i-1]->getGravCenter())||room.fiends[i-1]->isDead()){
			delete room.fiends[i-1];
			room.fiends.erase(room.fiends.begin()+i-1);
		}
	}
	for(unsigned int i=room.projectiles.size();i>0;i--){
		if(!room.boundingBox.isInside(room.projectiles[i-1]->getGravCenter())||room.projectiles[i-1]->isDead()){
			delete room.projectiles[i-1];
			room.projectiles.erase(room.projectiles.begin()+i-1);
		}
	}
	for(unsigned int i=0;i<room.scenery.size();i++){
		for(unsigned int j=0;j<room.fiends.size();j++){
			room.fiends[j]->resolveCollision(room.scenery[i]);	
		}
		for(unsigned int j=0;j<room.projectiles.size();j++){
			room.projectiles[j]->resolveCollision(room.scenery[i]);	
		}
		room.player->resolveCollision(room.scenery[i]);
	}
	for(unsigned int i=0;i<room.fiends.size();i++){
		for(unsigned int j=0;j<room.projectiles.size();j++){
			room.fiends[i]->resolveCollision(room.projectiles[j]);
			room.projectiles[j]->resolveCollision(room.fiends[i]);
		}
		room.player->resolveCollision(room.fiends[i]);
		room.fiends[i]->resolveCollision(room.player);
	}
	for(unsigned int i=0;i<room.projectiles.size();i++){
		room.projectiles[i]->resolveCollision(room.player);
		room.player->resolveCollision(room.projectiles[i]);
	}

}

void Game::draw(){
	mainWindow.clear(sf::Color(112,112,255,555));
	view.setCenter(room.player->getGravCenter().x,room.player->getGravCenter().y);
	mainWindow.setView(view);
	for(unsigned int i=0;i<room.fiends.size();i++){
		room.fiends[i]->draw(&mainWindow);
	}
	for(unsigned int i=0;i<room.projectiles.size();i++){
		room.projectiles[i]->draw(&mainWindow);
	}
	for(unsigned int i=0;i<room.scenery.size();i++){
		room.scenery[i]->draw(&mainWindow);
	}
	room.player->draw(&mainWindow);
	room.player->drawStats(&mainWindow);
	mainWindow.display();
}

void Game::LoadTestRoom(){
	Player* player = new YoungLady (Point2d(0.0,3.0));
	room.player=player;
	Character* fiend1 = new Spearman (Point2d(-2.0,3.0),true,&room);
	room.fiends.push_back(fiend1);
	std::vector<int> ceilingTileIndices (55,1), ceilingTileRotations (55,0);
	Still* ceiling = new Still (Point2d(-5.0,-1.0), Point2d(50.0,0.0),"TileSet.png",100,100,ceilingTileIndices,ceilingTileRotations,55,1);
	room.scenery.push_back(ceiling);
	std::vector<int> ground1TileIndices (30,0), ground1TileRotations (30,0);
	Still* ground1 = new Still (Point2d(-5.0,5.0), Point2d(25.0,6.0),"TileSet.png",100,100,ground1TileIndices,ground1TileRotations,30,1);
	room.scenery.push_back(ground1);
	std::vector<int> ground2TileIndices (23,0), ground2TileRotations (23,0);
	Still* ground2 = new Still (Point2d(27.0,5.0), Point2d(50.0,6.0),"TileSet.png",100,100,ground2TileIndices,ground2TileRotations,23,1);
	room.scenery.push_back(ground2);
	std::vector<int> leftTileIndices (5,1), leftTileRotations (5,0);
	Still* leftWall = new Still (Point2d(-6.0,0.0), Point2d(-5.0,5.0),"TileSet.png",100,100,leftTileIndices,leftTileRotations,1,5);
	room.scenery.push_back(leftWall);
	std::vector<int> rightTileIndices (5,1), rightTileRotations (5,0);
	Still* rightWall = new Still (Point2d(50.0,0.0), Point2d(51.0,5.0),"TileSet.png",100,100,rightTileIndices,rightTileRotations,1,5);
	room.scenery.push_back(rightWall);
	room.boundingBox.include(Point2d(-10.0,-10.0));
	room.boundingBox.include(Point2d(100.0,100.0));
}

void Game::launch(){
	currentGameState=TITLE_SCREEN;
	endOfFrame();
	while(mainWindow.isOpen()){
		sf::Event event;
        while(mainWindow.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				mainWindow.close();
			}else if (event.type == sf::Event::Resized){
				gamePaused=true;
				endOfFrame();
			}else if (event.type == sf::Event::LostFocus){
				gamePaused=true;
				endOfFrame();
			}else{
				gamePaused=false;
			}
		}
		endOfFrame();
		if(currentGameState==TITLE_SCREEN){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				SquareHitbox newGameButton (Point2d(3.0,8.0),Point2d(7.0,9.0));
				SquareHitbox loadButton (Point2d(3.0,6.0),Point2d(7.0,7.0));
				SquareHitbox optionsButton (Point2d(3.0,4.0),Point2d(7.0,5.0));
				sf::Vector2f mousePosition = mainWindow.mapPixelToCoords(sf::Mouse::getPosition(mainWindow));
				Point2d cursor (mousePosition.x,mousePosition.y);
				if(newGameButton.isInside(cursor)){
					LoadTestRoom();
				}else if(loadButton.isInside(cursor)){
					//TODO
				}else if(optionsButton.isInside(cursor)){
					//TODO
				}
			}
		}else if(currentGameState==GAME_RUNNING){
			if(!gamePaused){
				room.player->keyboard(&room);
				room.player->mouse(&room,&mainWindow);
				simulate();
				resolveCollisions();
			}
		}
        draw();
    }
}