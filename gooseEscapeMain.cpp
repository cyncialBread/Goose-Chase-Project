#include <BearLibTerminal.h>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"

//set up the console.   Don't modify this line!
Console out;

int main()
{
	//Set up the window.  Don't edit these two lines
    terminal_open();
  	terminal_set(SETUP_MESSAGE);

 	//generate levels random levels - modify NUM_LEVELS to adjust complexity
 	generateLevels(NUM_LEVELS);
 	
    //make the player
	Actor player(PLAYER_CHAR, 5,5,0);  // you probably don't want to start in the same place each time
	
	//make the monster
	Actor monster(MONSTER_CHAR, 70,12,0);
	
    // Declare the array that will hold the game board "map"
  	int map[NUM_BOARD_Y][NUM_BOARD_X] = {0};
  	

	//first level
	string level = "level0.txt";
	
	//load first level
	levelLoad(map, level);

  	
    // Call the function to print the game board
  	printBoard(map);
  	
	// Printing the instructions
    out.writeLine("Escape the Goose! " + monster.get_location_string());
	out.writeLine("Use the arrow keys to move");
	out.writeLine("If the goose catches you, you lose!");
	out.writeLine("Be careful! Sometimes the goose can jump through walls!");


	
    int keyEntered = TK_A;  // can be any valid value that is not ESCAPE or CLOSE
    
    
	//MAIN LOOP
    while(keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE 
        	&& !captured(player,monster) && !win(player,map)) //add has not won yet
	{
	    // get player key press
	    keyEntered = terminal_read();

        if (keyEntered != TK_ESCAPE && keyEntered != TK_CLOSE)
        {
        	
        	if(map[player.get_y()][player.get_x()] == POWER_UP)
            {
            	player.set_powerup();
            	map[player.get_y()][player.get_x()] = EMPTY;
            	
			}
            
			if(keyEntered == TK_SPACE)
			{
				player.usePowerUp(monster);
			}
			
            //player movement
    	    movePlayer(keyEntered, player, map);
    	    
    	    //goose chase movemenet
			moveMonster(monster, player, map);
			
			if(map[player.get_y()][player.get_x()] == DOOR_PREV || map[player.get_y()][player.get_x()] == DOOR_NEXT)
    		{
    			level = doorDetection(monster, player, map, level);
    			printBoard(map);
    			
			}
            
			printBoard(map);
			player.put_actor();
			monster.put_actor();	    
        }
  	}
	
	
    if (keyEntered != TK_CLOSE)
    {
      	//once we're out of the loop, the game is over
        out.writeLine("Game has ended");
    
        // output why: did the goose get us, or did we get to the win location?
		if (win(player, map))
		{
			out.writeLine("You have reached the safety zone!");
		}
		else if (player.get_x() == monster.get_x() && player.get_y() == monster.get_y())
		{
			out.writeLine("Oh no! The Goose has caught you :c");
		}
		
    	// Wait until user closes the window
        while (terminal_read() != TK_CLOSE);
    }

	
	//game is done, close it  
    terminal_close();
}
