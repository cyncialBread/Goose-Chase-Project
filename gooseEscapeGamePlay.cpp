#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

extern Console out;
/*
With graphics, screens are given an x,y coordinate system with the origin
in the upper left corner.  So it means the coordinate axes are:
---------------->   x direction
|
|
|
|
|
V

y direction 
*/
	

// print the game board function

void printBoard(int map[NUM_BOARD_Y][NUM_BOARD_X])
{
	for(int row = 0; row < NUM_BOARD_Y; row++)
	{
		for(int col = 0; col < NUM_BOARD_X; col++)
		{
			//outputting different tiles
			char output = ' ';
			
			if(map[row][col] == EMPTY)
			{
				output = EMPTY_CHAR;
			}
			else if(map[row][col] == SHALL_NOT_PASS)
			{
				output = WALL_CHAR;
			}
			else if(map[row][col] == WINNER)
			{
				output = WIN_CHAR;
			}
			else if(map[row][col] == DOOR_PREV)
			{
				output = DOOR_PREV_CHAR;	
			}
			else if(map[row][col] == DOOR_NEXT)
			{
				output = DOOR_NEXT_CHAR;	
			}
			else if(map[row][col] == POWER_UP)
			{
				output = POWER_UP_CHAR;
			}
			
			terminal_put(col,row,output);
		}	
	}
}

bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}

void movePlayer(int key, Actor & player, int map[NUM_BOARD_Y][NUM_BOARD_X])
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
        
    if (player.can_move(xMove, yMove) //wall and boundry collision detection
      && map[player.get_y()+yMove][player.get_x()+xMove] != SHALL_NOT_PASS)
        player.update_location(xMove, yMove);
}

string doorDetection (Actor & monster, Actor & player, int map[NUM_BOARD_Y][NUM_BOARD_X], string file)
{
	ifstream fin(file.c_str());
	
	string filePrev = "";	//reading file names, needed to read data below them in file
	string fileNext = "";
	fin >> filePrev;
	fin >> fileNext;
	
	int doorX = 0;
	int doorY = 0;
					
	if(map[player.get_y()][player.get_x()] == DOOR_NEXT)
    {
		
    	levelLoad(map, fileNext);
    	
    	player.set_location(MIN_BOARD_X+2, 10); //set player position for new level
		monster.set_location((MIN_BOARD_X+1) + rand() % (MAX_BOARD_X-1),(MIN_BOARD_Y+1) + rand() % (MAX_BOARD_Y-1));
	
		return fileNext;	//returns the current updated file name to update main loop
	}
	
	else if(map[player.get_y()][player.get_x()] == DOOR_PREV)
    {
		
    	levelLoad(map, filePrev);
    	
    	
    	player.set_location(MAX_BOARD_X-2,10); //set player position for new level
		monster.set_location((MIN_BOARD_X+1) + rand() % (MAX_BOARD_X-1),(MIN_BOARD_Y+1) + rand() % (MAX_BOARD_Y-1));
	
		return filePrev;	//returns the current updated file name to update main loop
	}
}

bool win(Actor const & player, int map[NUM_BOARD_Y][NUM_BOARD_X])
{
    int safetyzone_x = 0, safetyzone_y = 0;

    for(int row = 0; row < NUM_BOARD_Y; row++)
    {
        for(int col = 0; col < NUM_BOARD_X; col++)
        {
            if (map[row][col] == WINNER)
            {
                safetyzone_x = col;
                safetyzone_y = row;
            } 
        }
    }
    return (player.get_x() == safetyzone_x
         && player.get_y() == safetyzone_y);
}

void moveMonster(Actor & monster, Actor & player, int map[NUM_BOARD_Y][NUM_BOARD_X])
{
	int yMove = 0, xMove = 0;
	
    if (player.get_x() > monster.get_x())
        xMove = 1;
    else if (player.get_x() < monster.get_x())
        xMove = -1;
    else if (player.get_y() > monster.get_y())
        yMove = 1;
    else if (player.get_y() < monster.get_y())
        yMove = -1;
        
    if(!monster.is_frozen())	//freezing the goose if powerup is active
	{
		monster.update_location(xMove, yMove);  
	}   
	else
	{
		monster.unfreeze_cycle();	
	} 
}

//loads next room when door collision occurs
void levelLoad(int map[NUM_BOARD_Y][NUM_BOARD_X], string file)
{
	//temporary file read in variables (necessary to read tile data into array)
	string filePrev = "";
	string fileNext = "";
	
	ifstream fin(file.c_str());
	
	fin >> filePrev;
	fin >> fileNext;
		
	
	for(int row = 0; row < NUM_BOARD_Y; row++)	//load into map array
	{
		for(int col = 0; col < NUM_BOARD_X; col++)
		{
			int tile = 0;
			fin >> tile;
			
			map[row][col] = tile;		
		}
	}
	
	fin.close();
}

void generateLevels(int maxRooms)
{
	srand(time(NULL));	//seed for random intiializtion
	
	int winRoom = 0;	//room and location for final win space %
	
	winRoom = rand() % (maxRooms);
	int winRoomX = (MIN_BOARD_X+1) + rand() % (MAX_BOARD_X-1);
	int winRoomY= (MIN_BOARD_Y+1) + rand() % (MAX_BOARD_Y-1);

	for(int index = 0; index < maxRooms; index++)
	{
		stringstream ss;
		ss << index;
		string fileName = "level" + ss.str() +".txt";	//iterative file name string (i.e "leve0.txt")
		
		ofstream fout(fileName.c_str());	//initializing file
		
		fout << "level" << index-1 << ".txt" << endl << "level" << index+1 << ".txt" << endl;	//outputting to txt file prev and next file names
		
		int doorPrevY = (MIN_BOARD_Y+1) + rand() % (MAX_BOARD_Y-1);	//door locations
		int doorNextY = (MIN_BOARD_Y+1) + rand() % (MAX_BOARD_Y-1);
		
		int tempLevel [NUM_BOARD_Y][NUM_BOARD_X] = {0};	//temporary level array - needed for adding random walls
		
		
		for(int row = 0; row < NUM_BOARD_Y; row++)
		{
			int wallPosX = (MIN_BOARD_X+1) + rand() % (MAX_BOARD_X) - (MIN_BOARD_X +1);	//wall object x-position
			
			for(int col = 0; col < NUM_BOARD_X; col++)
			{	
				if(row == MIN_BOARD_Y || row == MAX_BOARD_Y || col == MIN_BOARD_X || col == MAX_BOARD_X) //spawning border walls
				{
					tempLevel[row][col] = 1;
				}
				else if (index == winRoom && row == winRoomY && col == winRoomX) //spawning win space
				{
					tempLevel[row][col] = 2;
				}
				else if (rand() % (SEED_PROBABILITY) == 5) //spawning random powerups (seeds)
				{
					tempLevel[row][col] = 5;
				}
				else if(col == wallPosX) //spawning random walls
				{
					
					
					
					int wallType =  rand() % (NUM_WALL_TYPES); //4 different wall t
					int size = WALL_SCALE;
					
					if(wallType == 0)	//cross (+)
					{
						for(int index = -size; index<=size; index++)
						{
							tempLevel[row][col+index] = 1;
						}
						
						for(int index = -size; index<=size; index++)
						{
							tempLevel[row+index][col] = 1;
						}
					}
					else if(wallType == 1)	//horizontal line (-)
					{
						for(int index = -size; index<=size; index++)
						{
							tempLevel[row][col+index] = 1;
						}
					}
					else if(wallType == 2)	//2x2 square
					{
						tempLevel[row][col] = 1;
						tempLevel[row][col-1] = 1;
						tempLevel[row-1][col] = 1;
						tempLevel[row-1][col-1] = 1;
					}
					else if (wallType == 3)	//vertical line	(|)
					{
						for(int index = -size; index<=size; index++)
						{
							tempLevel[row+index][col] = 1;
						}
					}
				}
				
				//doors are seperate to ensure they are spawned properly
				if(col==MIN_BOARD_X+1 && row == doorPrevY && index != 0) //door to previous room
				{
					tempLevel[row][col] = 4;
				}
				else if(col==MAX_BOARD_X-1 && row == doorNextY && index != (maxRooms-1)) //door to next room
				{
					tempLevel[row][col] = 3;
				}
			}
		}
	
		for(int row = 0; row < NUM_BOARD_Y; row++)	//file writing
		{
		
			for(int col = 0; col < NUM_BOARD_X; col++)
			{
				fout << tempLevel[row][col] << ' ';
			}
			fout << endl;
		}
		
		fout.close();
	}	
}
