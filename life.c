/*
 * life.c
 *
 * Contains functions for simulating Conway's Game of Life, as well as
 * other variables and several palettes.
 */
#include <stdlib.h>
#include "life.h"

bool editMode = TRUE, paused = FALSE;
const u16 lifePalette[PALETTE_SIZE] = { COLOR_BLACK, COLOR_WHITE, COLOR_BLUE, COLOR_CYAN, COLOR_RED, COLOR_MAGENTA };	//Default palette
const u16 lifePalette2[PALETTE_SIZE] = { COLOR_WHITE, COLOR_BLACK, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA, COLOR_RED };
const u16 lifePalette3[PALETTE_SIZE] = { COLOR_YELLOW, COLOR_BLACK, COLOR_GREEN, COLOR_RED, COLOR_WHITE, COLOR_MAGENTA };

/*
 * Takes in a specified (x, y) coordinate and analyzes the immediate eight
 * neighbors surrounding it.
 */
void pixelLogic(u8 x, u8 y)
{
	vu8 neighbors = 0;
	vu8 numNeighbors = 0;

	if(x > SCREEN_MIN_X + 1 && y > SCREEN_MIN_Y + 1 && getPixel4(x - 1, y - 1) == PX_ALIVE)
	{
		neighbors |= NBR_TOPLEFT;
		numNeighbors++;
	}

	if(y > SCREEN_MIN_Y + 1 && getPixel4(x, y - 1) == PX_ALIVE)
	{
		neighbors |= NBR_TOPCENTER;
		numNeighbors++;
	}

	if(x < SCREEN_MAX_X - 1 && y > SCREEN_MIN_Y + 1 && getPixel4(x + 1, y - 1) == PX_ALIVE)
	{
		neighbors |= NBR_TOPRIGHT;
		numNeighbors++;
	}

	if(x > SCREEN_MIN_X + 1 && getPixel4(x - 1, y) == PX_ALIVE)
	{
		neighbors |= NBR_CENTERLEFT;
		numNeighbors++;
	}

	if(x < SCREEN_MAX_X - 1 && getPixel4(x + 1, y) == PX_ALIVE)
	{
		neighbors |= NBR_CENTERRIGHT;
		numNeighbors++;
	}

	if(x > SCREEN_MIN_X + 1 && y < SCREEN_MAX_Y - 1 && getPixel4(x - 1, y + 1) == PX_ALIVE)
	{
		neighbors |= NBR_BOTTOMLEFT;
		numNeighbors++;
	}

	if(y < SCREEN_MAX_Y - 1 && getPixel4(x, y + 1) == PX_ALIVE)
	{
		neighbors |= NBR_BOTTOMCENTER;
		numNeighbors++;
	}

	if(x < SCREEN_MAX_X - 1 && y < SCREEN_MAX_Y - 1 && getPixel4(x + 1, y + 1) == PX_ALIVE)
	{
		neighbors |= NBR_BOTTOMRIGHT;
		numNeighbors++;
	}

	if(getPixel4(x, y) == PX_DEAD && numNeighbors == 3)
		setPixel4(x, y, PX_ALIVE);
	else
	{
		if(numNeighbors < 2 || numNeighbors > 3)
			setPixel4(x, y, PX_DEAD);
		else
		{
			//Playground for testing more experimental pixel logic
			#ifdef EXPERIMENTAL
				if(neighbors & NBR_TOPLEFT && neighbors & NBR_BOTTOMRIGHT)
				{
					if(getPixel4(x - 1, y + 1) == BG)
						setPixel4(x - 1, y + 1, PX_ALIVE);
					else if(getPixel4(x + 1, y - 1))
						setPixel4(x + 1, y - 1, PX_ALIVE);
				}
			#endif
		}
	}
}

/*
 * Randomizes the screen.
 */
void randomize()
{
	for(int x = 0; x <= SCREEN_MAX_X; x++)
		for(int y = 0; y <= SCREEN_MAX_Y; y++)
			if(rand() & 1)
				setPixel4(x, y, PX_ALIVE);
}
