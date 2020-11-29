#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
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
	
	string filePrev = "";
	string fileNext = "";
	fin >> filePrev;
	fin >> fileNext;
		
	if(map[player.get_y()][player.get_x()] == DOOR_NEXT)
    {
		
		cout << fileNext << endl; //debugging level name output
		
		
    	levelLoad(map, fileNext);
    
    	player.set_location(10,10); //set player position for new level
		monster.set_location(60,15); //set goose position for new level
	
		return fileNext;	//returns the current updated file name to update main loop
	}
	
	else if(map[player.get_y()][player.get_x()] == DOOR_PREV)
    {
		
		cout << filePrev << endl; //debugging level name output
		
		
    	levelLoad(map, filePrev);
    
    	player.set_location(10,10); //set player position for new level
		monster.set_location(60,15); //set goose position for new level
	
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
    
    
	if(!monster.is_frozen())
	{
		monster.update_location(xMove, yMove);  
	}   
	else
	{
		monster.unfreeze_cycle();	
	} 
    
}

void levelLoad(int map[NUM_BOARD_Y][NUM_BOARD_X], string file)
{
	
	string filePrev = "";
	string fileNext = "";
	
	ifstream fin(file.c_str());
	
	fin >> filePrev;
	fin >> fileNext;
	
	
	terminal_clear_area(MIN_CONSOLE_X, MIN_CONSOLE_Y, NUM_CONSOLE_X, NUM_CONSOLE_Y);
	
	for(int row = 0; row < NUM_BOARD_Y; row++)
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



