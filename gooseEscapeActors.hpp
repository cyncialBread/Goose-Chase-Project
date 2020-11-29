#ifndef GOOSE_ESCAPE_ACTORS
#define GOOSE_ESCAPE_ACTORS
#include <cmath>
#include <BearLibTerminal.h>
#include "gooseEscapeUtil.hpp"

/*
    Modify this class to contain more characteristics of the "actor".  Add
    functions that will be useful for playing the game that are specific to
    the Actor.
    
    Feel free to add additional Classes to your program.
*/

/* 
    Going further:  Learn the other syntax for implementing a class that is
    more appropriate for working with multiple files, and improve the class code.
*/

class Actor
{
  private:
    int actorChar;      
    int location_x, location_y;
    int powerUp;

  public:
    Actor()
    {
        actorChar = int('A');
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        powerUp = 0;
        
        put_actor();
    }

    Actor(char initPlayerChar, int x0, int y0, int powerUp0)
    {
        change_char(initPlayerChar);
        location_x = MIN_SCREEN_X;
        location_y = MIN_SCREEN_Y;
        // havePowerUp(map[location_x][location_y], const POWER_UP)
  
        update_location(x0,y0);
    }
    
    int get_x() const
    {
        return location_x;
    }
    
    int get_y() const
    {
        return location_y;
    }
    
    void set_location(int x0, int y0)
    {
    	location_x = x0;
    	location_y = y0;
	}
	
    string get_location_string() const
    {
        char buffer[80];
        itoa(location_x,buffer,10);
        string formatted_location = "(" + string(buffer) + ",";
        itoa(location_y,buffer,10);
        formatted_location += string(buffer) + ")";
        return formatted_location;
    }
    
    void change_char(char new_actor_char)
    {
        actorChar = min(int('~'),max(int(new_actor_char),int(' ')));
    }

    bool can_move(int delta_x, int delta_y) const
    {
        int new_x = location_x + delta_x;
        int new_y = location_y + delta_y;

        return new_x >= MIN_BOARD_X && new_x <= MAX_BOARD_X
          && new_y >= MIN_BOARD_Y && new_y <= MAX_BOARD_Y;
    }

    void update_location(int delta_x, int delta_y)
    {
        if (can_move(delta_x, delta_y))
        {
            terminal_clear_area(location_x, location_y, 1, 1);
            location_x += delta_x;
            location_y += delta_y;
            put_actor();
        }
    }
    
    void put_actor() const
    {
        terminal_put(location_x, location_y, actorChar);
        terminal_refresh();
    }
    
    //Function to know whether the actor has or not powerUp 
    bool havePowerUp(int map[NUM_BOARD_Y][NUM_BOARD_X], int const POWER_UP)
	{
		if(map[location_x][location_y] == POWER_UP)
		{
			terminal_clear_area(location_x, location_y, 1, 1); // Make * dissappear once it is overlapped
			return 1;
		}
		return 0;
	}
   
	void usePowerUp(Actor const && otherActor, int delta_x, int delta_y) const
	{
		if (havePowerUp())
		{
			while (delta_x + delta_y == 3)
			{
				otherActor.x  
				otherActor.y
			}
		}
	}
    
};
#endif
