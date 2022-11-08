#include "GLUT.h"
#include "Room.h"
#include <time.h>
#include <vector>
#include <queue>
#include "Cell.h"
#include "CompareCells.h"
#include <iostream>
#include <ctime>
#include "Monster.h"
#include "Pacman.h"

using namespace std;

const int MSZ = 150; // maze size
const int WALL = 0;
const int ROOM = 1;
const int START = 2;
const int TARGET = 3;
const int GRAY = 4;
const int BLACK = 5;
const int PATH = 6;
const int BLACK_TARGET = 7;
const int GRAY_TARGET = 8;
const int COIN = 9;
const int PACMAN = 10;
const int MONSTER = 11;
Monster* monster = new  Monster();
Pacman* pacman = new  Pacman();
vector<int>*monsterCords= new vector<int>();
int maze[MSZ][MSZ] = {0};
int mazeDup[MSZ][MSZ] = { 0 };
int mazeCoins[MSZ][MSZ] = { 0 };
bool find_pass = false;
bool escape = true;
bool loop = true;
int points = 0;
int totalPoints = 0;
int monsterCount = 3;
int changesCounter = 1;
time_t startTime=time(&startTime);
time_t currentTime = time(&currentTime);
int pacmanX, pacmanY;
// defines queue of Cells
priority_queue < Cell*, vector<Cell*>, CompareCells> pq;
queue <Cell*> grays;
vector<Room*>rooms;

void InitMaze();
void game();
void resetGrays();
void SetCoins();
void setMazeDup();
void setMaze();
void BuildPassRooms();
vector<int>* CheckNeighborEscape(int row, int col, Cell* pcurrent, int centerX, int centerY,int target,bool bfs);
void BuildPass(int centerX,int centerY);
bool checkIfMonsterIsClose();
int ManhattanDistance(int x1, int y1, int x2, int y2);
void paintRoom(int startRow,int startCol,int heightSize,int WidthSize);
vector<int> findEnemy(int start, int target, vector<int>*indexes);
vector<int> findRoomIndexes(int height, int width);
void EscapePacman();
void CollectCoins();
vector<int> findClosestEnemy(int start,int target, vector<int>*indexes);
boolean validRoom(int i,int j,int height,int width);
boolean MonsterInRoom(Room* room);


void setMazeDup()
{
	for (size_t i = 0; i < MSZ; i++)
	{
		for (size_t j = 0; j < MSZ; j++)
			mazeDup[i][j] = maze[i][j];

	}
}

void setMaze()
{
	for (size_t i = 0; i < MSZ; i++)
	{
		for (size_t j = 0; j < MSZ; j++)
			maze[i][j] = mazeDup[i][j];

	}
}

void init()
{
	srand(time(0)); // seed random numbers
	glClearColor(0, 0, 0.2, 0); // RGB - sets background color
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // setup coordinate system
	InitMaze();
}

void paintRoom(int startRow, int startCol, int heightSize, int widthSize)
{
	for (int i = startRow; i < startRow+heightSize; i++)
	{
		for (int j = startCol; j < startCol + widthSize; j++)
			maze[i][j] = ROOM;
						
	}
}

void SetCoins()
{
	for (size_t i = 0; i < MSZ; i++)
	{
		for (size_t j = 0; j < MSZ; j++)
		{
			int temp = (rand() % 99)+1;
			if (temp <= 3 && maze[i][j] == ROOM)
			{
				maze[i][j] = COIN;
				mazeDup[i][j] = COIN;
				mazeCoins[i][j] = COIN;
				totalPoints += 1;
			}
		}

	}
}
boolean validRoom(int row , int col,int height ,int width)	//checks if a room with width and height can be placed from (row,col) to (row+height,col+height) 
{
	for (int i = row; i < row+height; i++)
	{
		for (int j = col; j < col+width ; j++)
		{
			if (maze[i][j] == ROOM)	// room already exists at these coordinates,need to find new space for room
				return false;
			 if (i == row && maze[i - 1][j] == ROOM)
				return false;
			 if (j == col && maze[i][j - 1] == ROOM)
				 return false;
			 if (i + 1 == row + height && maze[i + 1][j] == ROOM)
				 return false;
			 if (j + 1 == col + width && maze[i][j+1] == ROOM)
				 return false;
		}
	}

	return true;

}
vector<int>findRoomIndexes(int height,int width)
{	
	int i, j;
	do
	{
		i = (rand() % (MSZ - 2 - height)) + 1;		// note , MSZ-1 is border -> wall , validRoom checks all squares in range , except for borders 
		j = (rand() % (MSZ - 2 - width)) + 1;
	} while (!validRoom(i, j, height, width));		
	vector<int>indexes;
	indexes.push_back(i);
	indexes.push_back(j);
	return indexes;
		
}

boolean MonsterInRoom(Room* room)
{
	for (size_t i = room->getRow(); i < room->getRow()+room->getHeight(); i++)
	{
		for (size_t j = room->getCol(); j < room->getCol() +room->getWidth(); j++)
		{
			if (maze[i][j] == MONSTER)
				return true;
		}
	}
	return false;
}

void InitMaze()
{

	int numOfRooms =  (rand() % 2 )+5;	// we would like to randomize the number of rooms (between 5 - 7)
	// the border is WALL by default 

	for (int i = 0; i < numOfRooms; i++)
	{
		int tempHeight = (rand() % 21) + 24;
		int tempWidth = (rand() % 21) + 24;
		vector<int> indexes = findRoomIndexes(tempHeight,tempWidth);	// indexes is a set of (i,j) that represent starting coordinates of a new room , if -1,-1 then rooms cant be made
		if (indexes.at(0) != -1)
		{
			paintRoom(indexes.at(0), indexes.at(1), tempHeight, tempWidth);	// paint room
			Room* tempRoom=new Room(indexes.at(0), indexes.at(1), tempHeight, tempWidth);	//rooms set their center inside the class
			rooms.push_back(tempRoom);// push room to rooms vector
		}

	}
	BuildPassRooms();	// create pass between all rooms

		//place 3 monsters in empty ROOM space
	for (size_t i = 0; i < 3; i++)
	{
		int x, y;
		do
		{
			x = rand() % MSZ - 1;
			y = rand() % MSZ - 1;
		} while (maze[y][x] != ROOM);
		maze[y][x] = MONSTER;
		monsterCords->push_back(y);
		monsterCords->push_back(x);
	}

	// place pacman on the map
	// find random room for pac man with no monsters inside
	int pacmanStartRoom ,pacmanStartX, pacmanStartY;
	do
	{
	pacmanStartRoom = (rand() % (rooms.size() - 1));
	pacmanStartX = (rand() % rooms.at(pacmanStartRoom)->getWidth() - 1) + rooms.at(pacmanStartRoom)->getCol();
	pacmanStartY = (rand() % rooms.at(pacmanStartRoom)->getHeight() - 1) + rooms.at(pacmanStartRoom)->getRow();
	} while (MonsterInRoom(rooms.at(pacmanStartRoom)));
	
	maze[pacmanStartY][pacmanStartX] = PACMAN;
	
	pacmanX = pacmanStartX;
	pacmanY = pacmanStartY;

	SetCoins();

	setMazeDup();

}

void resetGrays()
{
	for (size_t i = 0; i < MSZ; i++)
	{
		for (size_t j = 0; j < MSZ; j++)
		{
	/*		if (maze[i][j] != ROOM && maze[i][j] != WALL)
				cout << maze[i][j];*/
			if (maze[i][j] != ROOM && maze[i][j]!=WALL)
				maze[i][j] = ROOM;
		}

	}
}
void DrawMaze()
{
	int i, j;

	for(i=0;i<MSZ;i++)
		for (j = 0; j < MSZ; j++)
		{
			// set color
			switch (maze[i][j])
			{
			case START:
				glColor3d(0.3, 0, 0);// dark red
				break;
			case ROOM:
				glColor3d(0, 0, 0); // black
				break;
			case WALL:
				glColor3d(0.5, 0.7, 1);// light blue
				break;
			case TARGET:
				glColor3d(0, 0, 0); // black -> target
				break;
			case GRAY:
				glColor3d(0.7, 0, 0.7); // black ->in queue Room
				break;
			case BLACK:
				glColor3d(0.7, 0, 0.7); // visited black
				break;
			case PATH:
				glColor3d(0.7, 0, 0.7); // magenta
				break;
			case BLACK_TARGET:
				glColor3d(0.2, 0, 0.9); // dark purple
				break;
			case GRAY_TARGET:
				glColor3d(0.7, 0, 0); // dark red
				break;
			case COIN:
				glColor3d(1,1,1); // white
				break;
			case PACMAN:
				glColor3d(1, 0.87, 0); // gold yellow
				break;
			case MONSTER:
				glColor3d(0.7, 0, 0); // dark red
				break;

			}
			glBegin(GL_POLYGON);

			glVertex2d(j, i);
			glVertex2d(j, i+1);
			glVertex2d(j+1, i+1);
			glVertex2d(j+1, i);
			glEnd();
		}
}

void RestorePath(Cell* pc)	// used for building the pass between rooms
{
	while (pc != nullptr)
	{	
		maze[pc->getRow()][pc->getCol()] = ROOM;
		pc = pc->getParent();
	}
}
vector<int>* RestorePath2(Cell* pc)	// used for building the pass between rooms
{
	if (maze[pc->getRow()][pc->getCol()]!=PACMAN && maze[pc->getRow()][pc->getCol()] != MONSTER)
	{
		while (pc->getParent())
		{
			if (pc->getParent()->getParent() == NULL)
				break;
			pc = pc->getParent();
		}
	}
	int r = pc->getRow();
	int c = pc->getCol();
	maze[r][c] = ROOM;
	vector<int>*vect=new vector<int>();
	vect->push_back(r);
	vect->push_back(c);
	return vect;
}

void BuildPassRooms()	// for each room , create pass towards any other room
{
	for (auto room : rooms)
	{
		Cell* pc = new Cell(room->getCenterY(), room->getCenterX());
		
		for (auto room2 : rooms)
		{
			if (room->getCenterX() != room2->getCenterX() || room->getCenterY() != room2->getCenterY())	// check if room != room2
			{	
				find_pass = true;

				//set color
				pc->setColor(maze[room->getCenterY()][room->getCenterX()]);	

				//set g
				if (pc->getColor() == ROOM)
					pc->setG(1);
				else
					pc->setG(1000);

				//set h
				pc->setH(ManhattanDistance(room2->getCenterX(), room2->getCenterY(), pc->getCol(),pc->getRow()));

				pq.push(pc);
				maze[room2->getCenterY()][room2->getCenterX()] = TARGET;
				BuildPass(room2->getCenterX(), room2->getCenterY());

				// reset black , gray colors
				resetGrays();	// reset matrix back to ROOM and WALL only	
				pq = priority_queue<Cell*, vector<Cell*>, CompareCells>();
				grays = queue <Cell*>();	// init queue for BFS escape algorithm
			}
		}
	}
}
void printMatrix()
{
	for (int i = 0; i < MSZ; i++)
	{
		for (int j = 0; j < MSZ; j++)
		{
			if (maze[i][j] == PACMAN)
				printf("pacman:(%d,%d)\n", i, j);
			if (maze[i][j] == MONSTER)
				printf("monster:(%d,%d)\n", i, j);
			if (maze[i][j] == GRAY || maze[i][j] == BLACK)
				printf("wrong: (%d,%d)\n,i,j");
		}

	}
}
vector<int> findClosestEnemy(int start,int target, vector<int>*indexes)	//find closest enemy BFS return its coords
{
	int r, c;
	Cell* pcurrent;

	if (grays.empty())
	{
		printMatrix();
		cout << "There is no monster left in the maze!\n";
		find_pass = false;
		return *indexes;
	}
	else
	{
		pcurrent = grays.front();
		grays.pop();
		// remove pcurrent from grays and paint it black
		// erase first value from pq;
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		if (maze[r][c] != start)	// mark as visited , unless its the square of pacman
			maze[r][c] = BLACK;
		// add non-visited neighbors
		if (find_pass)
		{
			// up
			if (r + 1 < MSZ)
				if (find_pass && maze[r + 1][c] != GRAY && maze[r + 1][c] != BLACK && maze[r + 1][c] != WALL && maze[r + 1][c] != start)
					indexes = CheckNeighborEscape(r + 1, c, pcurrent, -1, -1, target, true);
			// down
			if (r - 1 >= 0)
				if (find_pass && maze[r - 1][c] != GRAY && maze[r - 1][c] != BLACK && maze[r - 1][c] != WALL && maze[r - 1][c] != start)
					indexes = CheckNeighborEscape(r - 1, c, pcurrent, -1, -1, target, true);
			// left
			if (c - 1 >= 0)
				if (find_pass && maze[r][c - 1] != GRAY && maze[r][c - 1] != BLACK && maze[r][c - 1] != WALL && maze[r][c - 1] != start)
					indexes = CheckNeighborEscape(r, c - 1, pcurrent, -1, -1, target, true);
			// right
			if (c + 1 < MSZ)
				if (find_pass && maze[r][c + 1] != GRAY && maze[r][c + 1] != BLACK && maze[r][c + 1] != WALL && maze[r][c + 1] != start)
					indexes = CheckNeighborEscape(r, c + 1, pcurrent, -1, -1, target, true);


		}
	}
	if (find_pass == false) // found target!
		return *indexes;
	return findClosestEnemy(start,target,indexes);
		

} 
void  printMonsters()
{
	for (size_t i = 0; i < 6; i+=2)
	{
		printf("monster%d :(%d,%d)\n", i / 2 + 1, monsterCords->at(i), monsterCords->at(i + 1));
	}
}
void ChaseMonsters()
{
		
	find_pass = true;
	grays = queue <Cell*>();	// init queue for BFS escape algorithm
	Cell* pc = new Cell(pacmanY, pacmanX);
	pc->setColor(GRAY);
	grays.push(pc);
	//printMatrix();
	vector<int>enemyCoords = findClosestEnemy(PACMAN, MONSTER, &vector<int>());
	/*printMonsters();*/
	if (enemyCoords.empty())	// check if game is over due to - no monsters are left in maze!
	{
		loop = false;
		return;
	}
	setMaze();	//reset maze to former state
	//--------------------------------------------------------------------------------------------------- up = find closest monster to pacman, down = chase monster
	find_pass = true;
	pq = priority_queue<Cell*, vector<Cell*>, CompareCells>();	
    pc = new Cell(pacmanY, pacmanX);
	pc->setColor(GRAY);
	pc->setG(1);
	//set h
	pc->setH(ManhattanDistance(pacmanX, pacmanY, enemyCoords.at(1), enemyCoords.at(0)));
	pq.push(pc);
	enemyCoords = findEnemy(PACMAN, MONSTER, &vector<int>());

	// set maze to previous state
	setMaze();

	//check if a monster was captured
	if (maze[enemyCoords.at(0)][enemyCoords.at(1)] == MONSTER)
	{
		monsterCount--;
		for (size_t i = 0; i < 6; i += 2)
		{
			int y = monsterCords->at(i + 0);
			int x = monsterCords->at(i + 1);
			if (x == enemyCoords.at(1))
			{
				monsterCords->at(i+1) = -1;
				monsterCords->at(i) = -1;
			}
		}
	}
	else if (maze[enemyCoords.at(0)][enemyCoords.at(1)] == COIN)
		points += 1;
	//move monster to returned coordinates
	maze[pacmanY][pacmanX] = ROOM;
	mazeDup[pacmanY][pacmanX] = ROOM;
	maze[enemyCoords.at(0)][enemyCoords.at(1)] = PACMAN;
	mazeDup[enemyCoords.at(0)][enemyCoords.at(1)] = PACMAN;
	mazeCoins[enemyCoords.at(0)][enemyCoords.at(1)] = 0;
	pacmanX = enemyCoords.at(1);
	pacmanY = enemyCoords.at(0);

	if (monsterCount == 0)
	{
		loop = false;
		printf("pacman won! game is over , no monsters exist!\n");
		return;
	}
}
void EscapeMonsters()
{
	if (!loop)
		return;
	for (size_t i = 0; i < 6; i += 2)
	{
		if (monsterCords->at(1) != -1) // check if monster still exists , -1 indicates that pacman ate the monster 
		{
			find_pass = true;
			grays = queue <Cell*>();	// init queue for BFS escape algorithm
			Cell* pc = new Cell(monsterCords->at(i), monsterCords->at(i+1));
			int x = monsterCords->at(i + 1);
			int y = monsterCords->at(i);
			pc->setColor(GRAY);
			grays.push(pc);
			vector<int>enemyCoords = findClosestEnemy(MONSTER, PACMAN, &vector<int>());
			if (enemyCoords.empty())	// check if game is over due to - no monsters are left in maze!
			{
				loop = false;
				return;
			}
			setMaze();	//reset maze to former state

			//choose square with highest distance from enemyCoords
			double max = ManhattanDistance(pacmanX, pacmanY, x,y);
			int tempX = x;
			int tempY = y;
			double temp;

			if (x + 1 < MSZ) // 0,+1
			{
				temp = ManhattanDistance(pacmanX, pacmanY,x+1, y);
				if (temp >= max && maze[y][x + 1] != WALL && maze[y][x + 1] != MONSTER)
				{
					max = temp;
					tempX = x + 1;
					tempY = y;
				}
			}
			if (pacmanX - 1 >= 0) // 0,-1
			{
				temp = ManhattanDistance(pacmanX, pacmanY, x-1, y);

				if (temp >= max && maze[y][x - 1] != WALL && maze[y][x - 1] != MONSTER)
				{
					max = temp;
					tempX = x - 1;
					tempY = y;
				}
			}
			if (pacmanY + 1 < MSZ) // +1,0
			{
				temp = ManhattanDistance(pacmanX, pacmanY ,x,y+1);
				if (temp >= max && maze[y+ 1][x] != WALL && maze[y + 1][x] != MONSTER)
				{
					max = temp;
					tempX = x;
					tempY = y + 1;
				}
			}
			if (pacmanY - 1 >= 0) // -1,0
			{
				temp = ManhattanDistance(pacmanX, pacmanY, x, y-1);

				if (temp >= max && maze[y - 1][x] != WALL && maze[y - 1][x] != MONSTER)
				{
					max = temp;
					tempX = x;
					tempY = y - 1;
				}
			}
			// change position of monster
			if (mazeCoins[y][x] == COIN)
			{
				maze[y][x] = COIN;
				mazeDup[y][x] = COIN;
			}
			else
			{ 
			maze[y][x] = ROOM;
			mazeDup[y][x] = ROOM;
			}
			
			//set new coordinates
			maze[tempY][tempX] = MONSTER;
			mazeDup[tempY][tempX] = MONSTER;
			monsterCords->at(i) = tempY;
			monsterCords->at(i + 1) = tempX;
		}
	}
}
void EscapePacman()
{
	if (!loop)
		return;
	find_pass = true;
	grays = queue <Cell*>();	// init queue for BFS escape algorithm
	Cell* pc = new Cell(pacmanY,pacmanX);
	pc->setColor(GRAY);
	grays.push(pc);
	vector<int>enemyCoords = findClosestEnemy(PACMAN,MONSTER, &vector<int>());
	if (enemyCoords.empty())	// check if game is over due to - no monsters are left in maze!
	{
		loop = false;
		return;
	}

	//set maze to previous state
	setMaze();
	//choose square with highest distance from enemyCoords
	double max = ManhattanDistance(pacmanX, pacmanY, enemyCoords.at(1), enemyCoords.at(0)); 
	int tempX = pacmanX;
	int tempY=pacmanY;
	double temp;

	if (pacmanX + 1 < MSZ) // 0,+1
	{
		temp = ManhattanDistance(pacmanX + 1, pacmanY, enemyCoords.at(1), enemyCoords.at(0));
		if (temp >= max && maze[pacmanY][pacmanX + 1]!=WALL && maze[pacmanY][pacmanX + 1] != MONSTER)
		{
			max = temp;
			tempX = pacmanX + 1;
			tempY = pacmanY;
		}
	}
	if (pacmanX - 1 >= 0) // 0,-1
	{
		temp = ManhattanDistance(pacmanX - 1, pacmanY, enemyCoords.at(1), enemyCoords.at(0));
	
		if (temp >= max && maze[pacmanY][pacmanX - 1] != WALL && maze[pacmanY][pacmanX - 1] != MONSTER)
		{
			max = temp;
			tempX = pacmanX - 1;
			tempY = pacmanY;
		}
	}
	if (pacmanY + 1 < MSZ) // +1,0
	{
		temp = ManhattanDistance(pacmanX , pacmanY + 1,  enemyCoords.at(1), enemyCoords.at(0));
		if (temp >= max && maze[pacmanY+1][pacmanX] != WALL && maze[pacmanY + 1][pacmanX] != MONSTER)
		{
			max = temp;
			tempX = pacmanX;
			tempY = pacmanY + 1;
		}
	}
	if (pacmanY - 1 >= 0) // -1,0
	{
		temp = ManhattanDistance(pacmanX, pacmanY - 1, enemyCoords.at(1), enemyCoords.at(0));
	
		if (temp >= max && maze[pacmanY-1][pacmanX] != WALL && maze[pacmanY - 1][pacmanX] != MONSTER)
		{
			max = temp;
			tempX = pacmanX;
			tempY = pacmanY - 1;
		}
	}
	

	maze[pacmanY][pacmanX] = ROOM;
	mazeDup[pacmanY][pacmanX] = ROOM;
	mazeCoins[pacmanY][pacmanX] = 0;	// remove cell from coin maze
	// change pacman position , in maze and in mazeDup
	//set new coordinates
	pacmanX = tempX;
	pacmanY = tempY;
	if (maze[pacmanY][pacmanX] == COIN)
		points += 1;
	maze[pacmanY][pacmanX] = PACMAN;
	mazeDup[pacmanY][pacmanX] = PACMAN;
	if (points == totalPoints)
	{
		printf("pacman won! all coins have been collected!\n");
		loop = false;
	}


}

vector<int> findEnemy(int start, int target, vector<int>*indexes)
{
	int r, c;
	Cell* pcurrent;

	if (pq.empty())
	{
		cout << "enemy could not be found!";
		find_pass = false;
		return *indexes;
	}
	else
	{
		pcurrent = pq.top();
		pq.pop();
		// remove pcurrent from grays and paint it black
		// erase first value from pq;
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		if (maze[r][c] != start)	// mark as visited , unless its the square of pacman
			maze[r][c] = BLACK;
		// add non-visited neighbors
		int temp = (rand() % 2) + 1;
		if (temp == 1)
		{
			if (find_pass)
			{
				// left
				if (c - 1 >= 0)
					if (find_pass && maze[r][c - 1] != GRAY && maze[r][c - 1] != BLACK && maze[r][c - 1] != WALL && maze[r][c - 1] != start)
						indexes = CheckNeighborEscape(r, c - 1, pcurrent, pacmanX, pacmanY, target, false);
				// down
				if (r - 1 >= 0)
					if (find_pass && maze[r - 1][c] != GRAY && maze[r - 1][c] != BLACK && maze[r - 1][c] != WALL && maze[r - 1][c] != start)
						indexes = CheckNeighborEscape(r - 1, c, pcurrent, pacmanX, pacmanY, target, false);

				// right
				if (c + 1 < MSZ)
					if (find_pass && maze[r][c + 1] != GRAY && maze[r][c + 1] != BLACK && maze[r][c + 1] != WALL && maze[r][c + 1] != start)
						indexes = CheckNeighborEscape(r, c + 1, pcurrent, pacmanX, pacmanY, target, false);
				// up
				if (r + 1 < MSZ)
					if (find_pass && maze[r + 1][c] != GRAY && maze[r + 1][c] != BLACK && maze[r + 1][c] != WALL && maze[r + 1][c] != start)
						indexes = CheckNeighborEscape(r + 1, c, pcurrent, pacmanX, pacmanY, target, false);


			}
		}
		else
		{
			if (find_pass)
			{
				// right
				if (c + 1 < MSZ)
					if (find_pass && maze[r][c + 1] != GRAY && maze[r][c + 1] != BLACK && maze[r][c + 1] != WALL && maze[r][c + 1] != start)
						indexes = CheckNeighborEscape(r, c + 1, pcurrent, pacmanX, pacmanY, target, false);
				// up
				if (r + 1 < MSZ)
					if (find_pass && maze[r + 1][c] != GRAY && maze[r + 1][c] != BLACK && maze[r + 1][c] != WALL && maze[r + 1][c] != start)
						indexes = CheckNeighborEscape(r + 1, c, pcurrent, pacmanX, pacmanY, target, false);
				// left
				if (c - 1 >= 0)
					if (find_pass && maze[r][c - 1] != GRAY && maze[r][c - 1] != BLACK && maze[r][c - 1] != WALL && maze[r][c - 1] != start)
						indexes = CheckNeighborEscape(r, c - 1, pcurrent, pacmanX, pacmanY, target, false);
				// down
				if (r - 1 >= 0)
					if (find_pass && maze[r - 1][c] != GRAY && maze[r - 1][c] != BLACK && maze[r - 1][c] != WALL && maze[r - 1][c] != start)
						indexes = CheckNeighborEscape(r - 1, c, pcurrent, pacmanX, pacmanY, target, false);

			}

		}
	
	}
	if (find_pass == false) // found target!
	{
		//cout << "found pacman\n";
		return *indexes;
	}
	return findEnemy(start, target, indexes);


}

void ChasePacman()
{
	for (size_t i = 0; i < 6; i += 2)
	{
		int y = monsterCords->at(i + 0);
		int x = monsterCords->at(i + 1);
		if (x != -1)	// if x=-1 , monster was captured by pacman in the past
		{
			find_pass = true;
			pq =priority_queue<Cell*, vector<Cell*>, CompareCells>();		//reset pq	// init queue for BFS escape algorithm
			Cell* pc = new Cell(y, x);
			pc->setColor(GRAY);
			pc->setG(1);
			//set h
			pc->setH(ManhattanDistance(pacmanX, pacmanY, x, y)); // restorepath2 needs fix!
			pq.push(pc);
			vector<int>enemyCoords = findEnemy(MONSTER, PACMAN, &vector<int>());
			if (enemyCoords.empty())	// check if game is over due to - no pacman!
			{
				cout << "no pacman exists";
				loop = false;
				return;
			}
			// set maze to previous state
			setMaze();
			//move monster to returned coordinates
			maze[enemyCoords.at(0)][enemyCoords.at(1)] = MONSTER;
			mazeDup[enemyCoords.at(0)][enemyCoords.at(1)] = MONSTER;

			// if monster was on a coin , draw that coin again
			if (mazeCoins[monsterCords->at(i + 0)][monsterCords->at(i + 1)] != COIN)
			{
				maze[y][x] = ROOM;
				maze[y][x] = ROOM;
				mazeDup[y][x] = ROOM;
				mazeDup[y][x] = ROOM;
			}
			else
			{
				maze[y][x] = COIN;
				maze[y][x] = COIN;
				mazeDup[y][x] = COIN;
				mazeDup[y][x] = COIN;
			}
			monsterCords->at(i + 0) = enemyCoords.at(0);
			monsterCords->at(i + 1) = enemyCoords.at(1);

			if (enemyCoords.at(0) == pacmanY && enemyCoords.at(1) == pacmanX)
			{
				cout << "pacman was caught!\n";
				printf("points: %d\n", points);
				loop = false;
				return;
			}
		}
	}
}
bool checkIfMonsterIsClose()
{
	if (!loop)
		return false;
	find_pass = true;
	grays = queue <Cell*>();	// init queue for BFS escape algorithm
	Cell* pc = new Cell(pacmanY, pacmanX);
	pc->setColor(GRAY);
	grays.push(pc);
	vector<int>enemyCoords = findClosestEnemy(PACMAN, MONSTER, &vector<int>());
	if (enemyCoords.empty())	// check if game is over due to - no monsters are left in maze!
	{
		loop = false;
		return false;
	}

	//set maze to previous state
	setMaze();
	if (ManhattanDistance(pacmanX, pacmanY, enemyCoords.at(1), enemyCoords.at(0)) >= 25) //if monster is equal or less than 25 steps away , its NOT considered close!
		return false;
	return true;
}
void CollectCoins() //bfs to closest coin
{
	if (!loop)
		return;
	find_pass = true;
	grays = queue <Cell*>();	// init queue for BFS escape algorithm
	Cell* pc = new Cell(pacmanY, pacmanX);
	pc->setColor(GRAY);
	grays.push(pc);
	vector<int>enemyCoords = findClosestEnemy(PACMAN, COIN, &vector<int>());
	if (enemyCoords.empty())	// check if game is over due to - no monsters are left in maze!
	{
		loop = false;
		return;
	}

	//set maze to previous state
	setMaze();

	//choose square with highest distance from enemyCoords
	double min = ManhattanDistance(pacmanX, pacmanY, enemyCoords.at(1), enemyCoords.at(0));
	int tempX = pacmanX;
	int tempY = pacmanY;
	double temp;

	if (pacmanX + 1 < MSZ) // 0,+1
	{
		temp = ManhattanDistance(pacmanX + 1, pacmanY, enemyCoords.at(1), enemyCoords.at(0));
		if (temp < min && maze[pacmanY][pacmanX + 1] != WALL)
		{
			min = temp;
			tempX = pacmanX + 1;
			tempY = pacmanY;
		}
	}
	if (pacmanX - 1 >= 0) // 0,-1
	{
		temp = ManhattanDistance(pacmanX - 1, pacmanY, enemyCoords.at(1), enemyCoords.at(0));

		if (temp < min && maze[pacmanY][pacmanX - 1] != WALL)
		{
			min = temp;
			tempX = pacmanX - 1;
			tempY = pacmanY;
		}
	}
	if (pacmanY + 1 < MSZ) // +1,0
	{
		temp = ManhattanDistance(pacmanX, pacmanY + 1, enemyCoords.at(1), enemyCoords.at(0));
		if (temp < min && maze[pacmanY + 1][pacmanX] != WALL)
		{
			min = temp;
			tempX = pacmanX;
			tempY = pacmanY + 1;
		}
	}
	if (pacmanY - 1 >= 0) // -1,0
	{
		temp = ManhattanDistance(pacmanX, pacmanY - 1, enemyCoords.at(1), enemyCoords.at(0));

		if (temp < min && maze[pacmanY - 1][pacmanX] != WALL)
		{
			min = temp;
			tempX = pacmanX;
			tempY = pacmanY - 1;
		}
	}


	maze[pacmanY][pacmanX] = ROOM;
	mazeDup[pacmanY][pacmanX] = ROOM;
	mazeCoins[pacmanY][pacmanX] = 0;	// remove cell from coin maze
	// change pacman position , in maze and in mazeDup
	//set new coordinates
	pacmanX = tempX;
	pacmanY = tempY;
	if (maze[pacmanY][pacmanX] == COIN)
		points += 1;
	maze[pacmanY][pacmanX] = PACMAN;
	mazeDup[pacmanY][pacmanX] = PACMAN;
	if (points == totalPoints)
	{
		printf("pacman won! all coins have been collected!\n");
		loop = false;
	}


}
void game()	//main game loop
{
	// start iteration only if loop -> only if game is not over (monsters , pacman , and coins exist on board)
	if(loop)
	{ 
		//change roles based on currentTime-startTime
		currentTime = time(&currentTime);	
		int diff = currentTime - startTime;
		if (  diff >= changesCounter*3)
		{
			escape = !escape;
			changesCounter += 1;
			cout << "switch roles\n";
		}
		bool close = true;
		if(pacman->getIsRunning() || pacman->getIsCollectingCoins())
		close = checkIfMonsterIsClose();

		pacman->Execute(escape,close);
		monster->Execute(escape);
		if (pacman->getIsRunning())
			EscapePacman();
		else if (pacman->getIsChasing())
			ChaseMonsters();
		else
			CollectCoins();
		if (monster->getIsChasing())
			ChasePacman();
		else
			EscapeMonsters();
			
	}

}

void BuildPass(int centerX,int centerY)		//A* algorithm
{
	if (find_pass == false)
		return;
	int r, c;
	Cell* pcurrent;

	if (pq.empty())
	{
		cout << "There is no solution\n";
		find_pass = false;
		return;
	}
	else // grays is not empty
	{
		pcurrent = pq.top();
		pq.pop();
	
		// remove pcurrent from grays and paint it black
		// erase first value from pq;
		r = pcurrent->getRow();
		c = pcurrent->getCol();
		maze[r][c] = BLACK;
		// add non-visited neighbors
		// up
		if(r + 1 < MSZ)
		if (maze[r + 1][c] !=GRAY  && maze[r + 1][c] != BLACK)
			CheckNeighborEscape(r + 1, c, pcurrent,centerX,centerY,TARGET,false);
		// down
		if(r - 1 >= 0)
		if (maze[r - 1][c] != GRAY && maze[r - 1][c] != BLACK)
			CheckNeighborEscape(r - 1, c, pcurrent, centerX, centerY,TARGET,false);
		// left
		if(c - 1 >= 0)
		if (maze[r][c - 1] != GRAY && maze[r][c - 1]!=BLACK)
			CheckNeighborEscape(r, c - 1, pcurrent, centerX, centerY,TARGET,false);
		// right
		if(c + 1 < MSZ)
		if (maze[r][c + 1] != GRAY && maze[r][c + 1] != BLACK)
			CheckNeighborEscape(r, c + 1, pcurrent, centerX, centerY,TARGET,false);
	}
	return BuildPass(centerX, centerY);
}

vector<int>* CheckNeighborEscape(int row, int col, Cell* pcurrent,int centerX,int centerY,int target,bool bfs)	//returns target coordinates when a pass is found
{
	vector<int>*coords = new vector<int>();
	if (find_pass == true)
	{ 
		// is maze[row][col] a target
		if (maze[row][col] == target)
		{
			coords->push_back(row);
			coords->push_back(col);
			if(target==TARGET)
			cout << "the solution has been found\n";  // creating room
			//else
			//	cout << "target has been located\n";	// locating target (MONTER/PACMAN)
			find_pass = false;
			if (target == TARGET)	// build pass between rooms
			{
				RestorePath(pcurrent);
				return coords;
			}
			else if (!bfs)	//  chase mode , coordinates of first cell in route to target -> used for A*
			{
				if((maze[pcurrent->getRow()][pcurrent->getCol()]!=MONSTER && escape) || (maze[pcurrent->getRow()][pcurrent->getCol()] != PACMAN && !escape)) // more than one cell away from pacman
					return RestorePath2(pcurrent);
				else // one cell away from pacman , capture it!
				{
					vector<int>*vect =new vector<int>();
					vect->push_back(row);
					vect->push_back(col);
					return vect;
				}
			}

			return coords;	//escape mode , coordinates of target -> used for bfs

		}
		else // this is white neighbor
		{
			Cell* pneighbor;
			pneighbor = new Cell(row, col, pcurrent); // and pcurrent as parent
			maze[row][col] = GRAY;
			pneighbor->setColor(GRAY);

			//set g
			if (pneighbor->getColor() != WALL)
				pneighbor->setG(1);
			else
				pneighbor->setG(1000);

			//set h
			if (centerY!=-1 && centerX!=-1)
			pneighbor->setH(ManhattanDistance(centerX, centerY, col, row));

			/*cout << row;
			cout << ",";
			cout << col;
			cout << "\n";*/
				if(bfs)
				grays.push(pneighbor);
				else
					pq.push(pneighbor);
			
		}
	}
	return coords;
}


int ManhattanDistance(int x1,int y1,int x2,int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}




void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // fills all the frame buffer with background color

	DrawMaze();
	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	if (loop)
		game();
	glutPostRedisplay(); // calls indirectly to display
}

void menu(int choice)
{

}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // double means it uses double buffer
	glutInitWindowSize(1200, 900);
	glutInitWindowPosition(600, 100);
	glutCreateWindow("Pac-Man by Omer&Maor");	

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // runs all the time in the background
// add menu
	glutCreateMenu(menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	init();	//initializing game , setting rooms , creating pass between rooms , placing pacman and 3 monsters on map

	
	glutMainLoop();
}