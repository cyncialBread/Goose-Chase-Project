#include <iostream>
#include <cmath>
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
	
/*
    Print the game world
    
    The functions should draw characters to present features of the game
    board, e.g. win location, obstacles, power ups
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
			
			terminal_put(col,row,output);
		}
	}
}

/*
    Do something when the goose captures the player
    
    If you want to attack or something else, this is the function you 
    need to change.  For example, maybe the two touch each other and
    then fight.  You could add a health to the Actor class that is
    updated.  Run, use weapons, it's up to you!
*/

bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}

/*
    Move the player to a new location based on the user input.  You may want
    to modify this if there are extra controls you want to add.
    
    All key presses start with "TK_" then the character.  So "TK_A" is the a
    key being pressed.
    
    A look-up table might be useful.
    You could decide to learn about switch statements and use them here.
*/

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
        
    if (player.can_move(xMove, yMove) 
      && map[player.get_y()+yMove][player.get_x()+xMove] != SHALL_NOT_PASS)
        player.update_location(xMove, yMove);
}

/*
    What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/

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
        
    monster.update_location(xMove, yMove);  
}
