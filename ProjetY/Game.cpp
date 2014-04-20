#include "Game.h"
#include "Character.h"
#include "Still.h"
#include "YoungLady.h"
#include "Spearman.h"
#include "Peon.h"
#include "HeartOfFire.h"
#include "BasicButton.h"

const int Game::invulnerability_frames=5;
const double Game::friction = 2.0;
const double Game::reject = 100;
const double Game::maxAxisSpeed = 10;
const Point2d Game::g (0,9.8);

double Game::timeStep = 0.0;

const Type Game::typeList[] = {
	{"End", END},
	{"Still", STILL},
	{"Room Bounding Box", ROOM_BOUNDING_BOX},
	{"Peon", PEON},
	{"Spearman", SPEARMAN},
	{"Heart Of Fire", HEART_OF_FIRE},
	{"Position", POSITION},
	{"Facing Right", FACING_RIGHT},
	{"Square Hitbox", SQUARE_HITBOX},
	{"Convex Polygon", CONVEX_POLYGON},
	{"Texture Path", TEXTURE_PATH},
	{"Tile Dimension",TILE_DIMENSION},
	{"Tile Indices",TILE_INDICES},
	{"Tile Rotations",TILE_ROTATIONS},
	{"Tileset Dimension",TILESET_DIMENSION},
	{"Tile Origin",TILE_ORIGIN},
	{"Tile Scale",TILE_SCALE},
	{"Active Exit",ACTIVE_EXIT},
	{"Passive Exit",PASSIVE_EXIT},
	{"Room Path",ROOM_PATH},
	{"Horizontal",HORIZONTAL}
};

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

	for(unsigned int i=0;i<room.passiveExits.size();i++){
		if(room.passiveExits[i].door.isInside(room.player->getGravCenter())){
			Point2d d=room.player->getGravCenter()-room.passiveExits[i].door.getCenter();
			if(room.passiveExits[i].horizontal){
				if(d.x>0)
					d.x=-room.passiveExits[i].door.getWidth()*0.51;
				else
					d.x=room.passiveExits[i].door.getWidth()*0.51;
			}else{
				if(d.y>0)
					d.y=-room.passiveExits[i].door.getHeight()*0.51;
				else
					d.y=room.passiveExits[i].door.getHeight()*0.51;
			}
			room.player->displace(room.passiveExits[i].positionInNewRoom-room.player->getGravCenter()+d);
			loadRoom(room.passiveExits[i].nextRoom);
			return ;
		}
	}
}

void Game::draw(){
	mainWindow.clear(sf::Color(112,112,255,555));
	if(currentGameState==GAME_RUNNING){
		Point2d viewCenter;
		if(room.player->getGravCenter().x-0.5*view.getSize().x<room.boundingBox.min.x)
			viewCenter.x=room.boundingBox.min.x+0.5*view.getSize().x;
		else if(room.player->getGravCenter().x+0.5*view.getSize().x>room.boundingBox.max.x)
			viewCenter.x=room.boundingBox.max.x-0.5*view.getSize().x;
		else
			viewCenter.x=room.player->getGravCenter().x;
		if(room.player->getGravCenter().y-0.5*view.getSize().y<room.boundingBox.min.y)
			viewCenter.y=room.boundingBox.min.y+0.5*view.getSize().y;
		else if(room.player->getGravCenter().y+0.5*view.getSize().y>room.boundingBox.max.y)
			viewCenter.y=room.boundingBox.max.y-0.5*view.getSize().y;
		else
			viewCenter.y=room.player->getGravCenter().y;			
		view.setCenter(viewCenter.x,viewCenter.y);
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
	}
	//elements at fixed screen location
	sf::View defaultView (sf::FloatRect(0.0f,0.0f,1000.0f,1000.0f));
	mainWindow.setView(defaultView);
	for(unsigned int i=0;i<buttons.size();i++)
		buttons[i]->draw(&mainWindow);
	if(currentGameState==GAME_RUNNING){
		room.player->drawStats(&mainWindow);
		mainWindow.setView(view);
	}else if(currentGameState==TITLE_SCREEN){
		sf::Text titleText ("Undersky",titleFont,100);
		titleText.setColor(sf::Color::White);
		titleText.setPosition(300.0,100.0);
		mainWindow.draw(titleText);
	}
	mainWindow.display();
}

void Game::loadTestRoom(){
	Player* player = new YoungLady (Point2d(0.0,3.0));
	room.player=player;
	Character* fiend1 = new HeartOfFire (Point2d(-2.0,3.0),true,&room);
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
	buttons.push_back(new BasicButton("New Game",titleFont,sf::Color::White,sf::Color(0,0,255,100),40,Point2d(400.0,400.0)));
	buttons.push_back(new BasicButton("Load Game",titleFont,sf::Color::White,sf::Color(0,0,255,100),40,Point2d(395.0,500.0)));
	buttons.push_back(new BasicButton("Options",titleFont,sf::Color::White,sf::Color(0,0,255,100),40,Point2d(430.0,600.0)));
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
			if(buttons[0]->isLeftClickedOn(&mainWindow)){
				loadRoom("resources/rooms/testDoor1.room");
				currentGameState=GAME_RUNNING;
				for(unsigned int i=0;i<buttons.size();i++)
					delete buttons[i];
				buttons.clear();
			}else if(buttons[1]->isLeftClickedOn(&mainWindow)){
				//TODO
			}else if(buttons[2]->isLeftClickedOn(&mainWindow)){
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

inline bool Game::isWhiteSpace(char ch){
	return (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r' || ch=='\0');
}

inline char* Game::getTypeKeyword(int id){
	int keyWordCount = sizeof(typeList)/sizeof(typeList[0]);
	for (int i=0;i<keyWordCount;i++)
		if (id == typeList[i].ID)
			return (typeList[i].keyword);
	return NULL;
}

inline int Game::getTypeID(char* &name){
	//declare a list of keywords
	int keyWordCount = sizeof(typeList)/sizeof(typeList[0]);
	for (int i=0;i<keyWordCount;i++){ 
		if (strncmp(name,typeList[i].keyword,strlen(typeList[i].keyword)) == 0 && isWhiteSpace(name[strlen(typeList[i].keyword)])){
			name += strlen(typeList[i].keyword);
			return typeList[i].ID;
		}
	}
	return NOT_FOUND;
}

inline char* Game::lTrim(char* buffer){
	while (*buffer==' ' || *buffer=='\t' || *buffer=='\n' || *buffer=='\r')
		buffer++;
	return buffer;
}

inline char* Game::rTrim(char* buffer){
	int index = (int)strlen(buffer) - 1;
	while(index>=0){
		if(buffer[index]==' ' || buffer[index]=='\t' || buffer[index]=='\n' || buffer[index]=='\r'){
			buffer[index] = '\0';
			index--;
		}else{
			break;
		}
	}
	return buffer;
}

inline char* Game::trim(char* buffer){
	return rTrim(lTrim(buffer));
}


inline bool Game::readValidLine(char* line, FILE* fp, int n){
	line[0] = '\0';
	while (!feof(fp)){
		fgets(line, n, fp);
		if ((int)strlen(line) > n-5)
			return false;
		char* tmp = trim(line);
		if (tmp[0]!='#' && tmp[0]!='\0')
			return true;
	}
	return false;
}


bool Game::loadRoom(const char* path){
	FILE* f = fopen(path, "r");
	if (f == NULL)
		return false;

	char buffer[200];//have a temporary buffer used to read the file line by line...

	room.empty();
	if(room.player==NULL)
		room.player = new YoungLady(Point2d(0.0,3.0));

	while (!feof(f)){
		//get a line from the file...
		readValidLine(buffer, f, sizeof(buffer));
		char *line = lTrim(buffer);
		int lineType = getTypeID(line);
		switch (lineType) {
			case ROOM_BOUNDING_BOX:{
					if(sscanf(line, "%lf %lf %lf %lf", &room.boundingBox.min.x,&room.boundingBox.min.y,&room.boundingBox.max.x,&room.boundingBox.max.y) != 4){ 
						fclose(f);
						return false;
					}
				}break;
			case PASSIVE_EXIT:{
					Exit exit;
					if (!room.loadExitFromFile(f,exit)){
						fclose(f);
						return false;
					}
					room.passiveExits.push_back(exit);
				}break;
			case ACTIVE_EXIT:{
					Exit exit;
					if (!room.loadExitFromFile(f,exit)){
						fclose(f);
						return false;
					}
					room.activeExits.push_back(exit);
				}break;
			case STILL:{
					Still* still = new Still();
					if (!still->loadFromFile(f,&room)){
						fclose(f);
						return false;
					}
					room.scenery.push_back(still);
				}break;
			case SPEARMAN:{
					Spearman* spearman = new Spearman();
					if (!spearman->loadFromFile(f,&room)){
						fclose(f);
						return false;
					}
					room.fiends.push_back(spearman);
				}break;
			case PEON:{
					Peon* peon = new Peon();
					if (!peon->loadFromFile(f,&room)){
						fclose(f);
						return false;
					}
					room.fiends.push_back(peon);
				}break;	
			case HEART_OF_FIRE:{
				HeartOfFire* boss = new HeartOfFire();
				if (!boss->loadFromFile(f,&room)){
					fclose(f);
					return false;
				}
				room.fiends.push_back(boss);
				}break;	
			default:
				fclose(f);
				return false;
		}
	}
	fclose(f);
	return true;
}
