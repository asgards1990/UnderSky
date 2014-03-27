#include "Game.h"
#include "Character.h"
#include "Still.h"
#include "YoungLady.h"
#include "Spearman.h"
#include "Peon.h"

const int Game::invulnerability_frames=5;
const double Game::friction = 2.0;
const double Game::reject = 100;
const double Game::maxAxisSpeed = 10;
const Point2d Game::g (0,9.8);

double Game::timeStep = 0.0;

Game::Game() :
	fps(60), windowWidth(800), windowHeight(800), viewWidth(10), viewHeight(10), mainWindow(sf::VideoMode(800, 800), "ProjetY"), view (sf::FloatRect(0,0,10,10)), gamePaused(false)
{
	titleFont.loadFromFile("resources/fonts/arial.ttf");
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
	if(currentGameState==GAME_RUNNING){
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
	}else if(currentGameState==TITLE_SCREEN){
		sf::Text titleText;
		titleText.setString("Undersky");
		titleText.setPosition(300.0f,100.0f);
		titleText.setFont(titleFont);
		titleText.setColor(sf::Color::White);
		titleText.setCharacterSize(100);
		sf::Text newGameText;
		newGameText.setString("New Game");
		newGameText.setPosition(400.0f,400.0f);
		newGameText.setFont(titleFont);
		newGameText.setColor(sf::Color::White);
		newGameText.setCharacterSize(40);
		sf::Text loadGameText;
		loadGameText.setString("Load Game");
		loadGameText.setPosition(395.0f,500.0f);
		loadGameText.setFont(titleFont);
		loadGameText.setColor(sf::Color::White);
		loadGameText.setCharacterSize(40);
		sf::Text optionText;
		optionText.setString("Options");
		optionText.setPosition(430.0f,600.0f);
		optionText.setFont(titleFont);
		optionText.setColor(sf::Color::White);
		optionText.setCharacterSize(40);
		sf::View titleView (sf::FloatRect(0.0f,0.0f,1000.0f,1000.0f));
		mainWindow.setView(titleView);
		mainWindow.draw(titleText);
		mainWindow.draw(newGameText);
		mainWindow.draw(loadGameText);
		mainWindow.draw(optionText);
	}
	mainWindow.display();
}

void Game::LoadTestRoom(){
	Player* player = new YoungLady (Point2d(0.0,3.0));
	room.player=player;
	Character* fiend1 = new Peon (Point2d(-2.0,3.0),true,&room);
	//Character* fiend1 = new Spearman (Point2d(-2.0,3.0),true,&room);
	room.fiends.push_back(fiend1);
	std::vector<int> ceilingTileIndices (55,1), ceilingTileRotations (55,0);
	Still* ceiling = new Still (Point2d(-5.0,-1.0), Point2d(50.0,0.0),"resources/images/TileSet.png",100,100,ceilingTileIndices,ceilingTileRotations,55,1);
	room.scenery.push_back(ceiling);
	std::vector<int> ground1TileIndices (30,0), ground1TileRotations (30,0);
	Still* ground1 = new Still (Point2d(-5.0,5.0), Point2d(25.0,6.0),"resources/images/TileSet.png",100,100,ground1TileIndices,ground1TileRotations,30,1);
	room.scenery.push_back(ground1);
	std::vector<int> ground2TileIndices (23,0), ground2TileRotations (23,0);
	Still* ground2 = new Still (Point2d(27.0,5.0), Point2d(50.0,6.0),"resources/images/TileSet.png",100,100,ground2TileIndices,ground2TileRotations,23,1);
	room.scenery.push_back(ground2);
	std::vector<int> leftTileIndices (5,1), leftTileRotations (5,0);
	Still* leftWall = new Still (Point2d(-6.0,0.0), Point2d(-5.0,5.0),"resources/images/TileSet.png",100,100,leftTileIndices,leftTileRotations,1,5);
	room.scenery.push_back(leftWall);
	std::vector<int> rightTileIndices (5,1), rightTileRotations (5,0);
	Still* rightWall = new Still (Point2d(50.0,0.0), Point2d(51.0,5.0),"resources/images/TileSet.png",100,100,rightTileIndices,rightTileRotations,1,5);
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
			SquareHitbox newGameButton (Point2d(400.0,400.0),Point2d(600.0,450.0));
			SquareHitbox loadButton (Point2d(400.0,500.0),Point2d(600.0,550.0));
			SquareHitbox optionsButton (Point2d(400.0,600.0),Point2d(600.0,650.0));
			sf::Vector2f mousePosition = mainWindow.mapPixelToCoords(sf::Mouse::getPosition(mainWindow));
			Point2d cursor (mousePosition.x,mousePosition.y);
			if(newGameButton.isInside(cursor)){
				newGameButton.draw(&mainWindow,sf::Color(0,0,255,255));
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					LoadTestRoom();
					currentGameState=GAME_RUNNING;
				}
			}else if(loadButton.isInside(cursor)){
				loadButton.draw(&mainWindow,sf::Color(0,0,255,100));
				//TODO
			}else if(optionsButton.isInside(cursor)){
				optionsButton.draw(&mainWindow,sf::Color(0,0,255,100));
				//TODO
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