/*
 * life.c
 *
 * Contains main program loop and update functions necessary
 * for this program to work.
 */
#include <stdlib.h>
#include "gba.h"
#include "life.h"
#include "title.h"

#define DELTA_LIM 16	//Max amount of time that can pass before next update

vu8 delta = 0;	//Amount of time that passes; increments by 1 with each iteration
vu32 seed = 0;	//Random seed for randomize()
Cursor cur;

/* FUNCTION PROTOTYPES */
static void init();
static void loopEdit();
static void updateEdit();
static void loopSim();
static void updateSim();

int main(void)
{
	init();

	while(!KEY_DOWN(BUTTON_START))
	while(KEY_DOWN(BUTTON_START));

	drawFullscreenRect4(BG);

	//Palette swapping, if you feel like adding a #define at the top
	#ifdef PALETTE2
		setPalette(lifePalette2, PALETTE_SIZE);
	#else
		#ifdef PALETTE3
			setPalette(lifePalette3, PALETTE_SIZE);
		#else
			setPalette(lifePalette, PALETTE_SIZE);
		#endif
	#endif

	waitForVblank();
	flipPage();
	
	while(TRUE)
	{
		delta++;

		if(editMode)
			loopEdit();
		else
			loopSim();

		waitForVblank();
		flipPage();
		copyVRtoVW();	//Reverse-blit
	}

	return 0;
}

/*
 * Initializes the program to its initial state (title screen).
 */
static void init()
{
	REG_DISPCNT = MODE4 | BG2_ENABLE;

	cur.x = 120;
	cur.y = 80;
	cur.alt = FALSE;

	setPalette(title_palette, TITLE_PALETTE_SIZE);
	drawFullscreenImage4(title);
	waitForVblank();
	flipPage();
}

/*
 * Edit mode loop.
 */
static void loopEdit()
{
	seed++;

	if(delta > DELTA_LIM)
	{
		updateEdit();
		cur.alt = (cur.alt == TRUE) ? FALSE : TRUE;
		delta = 0;
	}
}

/*
 * Simulation mode loop.
 */
static void loopSim()
{
	if(paused)
	{
		if(KEY_DOWN(BUTTON_START))
		{
			while(KEY_DOWN(BUTTON_START));

			paused = FALSE;
		}
		else if(KEY_DOWN(BUTTON_A))
		{
			while(KEY_DOWN(BUTTON_A));

			updateSim();
		}
	}
	else
		updateSim();
}

/*
 * Updates the screen during edit mode.
 */
static void updateEdit()
{
	cur.x1 = cur.x0;
	cur.y1 = cur.y0;

	cur.x0 = cur.x;
	cur.y0 = cur.y;

	if(getPixel4(cur.x0, cur.y0) == CUR_BG || getPixel4(cur.x0, cur.y0) == CUR_BG_ALT)
		setPixel4(cur.x0, cur.y0, BG);
	else if(getPixel4(cur.x0, cur.y0) == CUR_PX_ALIVE || getPixel4(cur.x0, cur.y0) == CUR_PX_ALIVE_ALT)
		setPixel4(cur.x0, cur.y0, PX_ALIVE);

	if(getPixel4(cur.x1, cur.y1) == CUR_BG || getPixel4(cur.x1, cur.y1) == CUR_BG_ALT)
		setPixel4(cur.x1, cur.y1, BG);
	else if(getPixel4(cur.x1, cur.y1) == CUR_PX_ALIVE || getPixel4(cur.x1, cur.y1) == CUR_PX_ALIVE_ALT)
		setPixel4(cur.x1, cur.y1, PX_ALIVE);

	if(KEY_DOWN(BUTTON_SELECT))
	{
		while(KEY_DOWN(BUTTON_SELECT));

		if(getPixel4(cur.x, cur.y) == CUR_BG || getPixel4(cur.x, cur.y) == CUR_BG_ALT)
			setPixel4(cur.x, cur.y, BG);
		else if(getPixel4(cur.x, cur.y) == CUR_PX_ALIVE || getPixel4(cur.x, cur.y) == CUR_PX_ALIVE_ALT)
			setPixel4(cur.x, cur.y, PX_ALIVE);

		waitForVblank();
		flipPage();
		
		if(getPixel4(cur.x1, cur.y1) == CUR_BG || getPixel4(cur.x1, cur.y1) == CUR_BG_ALT)
			setPixel4(cur.x1, cur.y1, BG);
		else if(getPixel4(cur.x1, cur.y1) == CUR_PX_ALIVE || getPixel4(cur.x1, cur.y1) == CUR_PX_ALIVE_ALT)
			setPixel4(cur.x1, cur.y1, PX_ALIVE);

		editMode = FALSE;
		return;
	}

	if(KEY_DOWN(BUTTON_A) && KEY_DOWN(BUTTON_B))	//Try randomizing repeatedly for interesting results
	{
		while(KEY_DOWN(BUTTON_A) || KEY_DOWN(BUTTON_B));

		srand(seed);
		randomize();
	}
	else
	{
		if(KEY_DOWN(BUTTON_A))
			setPixel4(cur.x, cur.y, PX_ALIVE);
		else if(KEY_DOWN(BUTTON_B))
			setPixel4(cur.x, cur.y, BG);
		else if(KEY_DOWN(BUTTON_L) && KEY_DOWN(BUTTON_R))
		{
			while(KEY_DOWN(BUTTON_L) || KEY_DOWN(BUTTON_R));

			drawFullscreenRect4(BG);
			waitForVblank();
			flipPage();
			drawFullscreenRect4(BG);
		}
	}

	if(KEY_DOWN(BUTTON_LEFT) && cur.x > SCREEN_MIN_X)
		cur.x--;
	else if(KEY_DOWN(BUTTON_RIGHT) && cur.x < SCREEN_MAX_X)
		cur.x++;
	else if(KEY_DOWN(BUTTON_UP) && cur.y > SCREEN_MIN_Y)
		cur.y--;
	else if(KEY_DOWN(BUTTON_DOWN) && cur.y < SCREEN_MAX_Y)
		cur.y++;

	if(getPixel4(cur.x, cur.y) == BG)
		setPixel4(cur.x, cur.y, cur.alt? CUR_BG_ALT : CUR_BG);
	else if(getPixel4(cur.x, cur.y) == PX_ALIVE)
		setPixel4(cur.x, cur.y, cur.alt ? CUR_PX_ALIVE_ALT : CUR_PX_ALIVE);
}

/*
 * Updates the screen during simulation mode.
 */
static void updateSim()
{
	if(KEY_DOWN(BUTTON_START))
	{
		while(KEY_DOWN(BUTTON_START));

		paused = TRUE;
		return;
	}

	if(KEY_DOWN(BUTTON_SELECT))
	{
		while(KEY_DOWN(BUTTON_SELECT));

		editMode = TRUE;
		return;
	}

	for(u8 x = 0; x < SCREEN_MAX_X; x++)
		for(u8 y = 0; y < SCREEN_MAX_Y; y++)
			pixelLogic(x, y);
}
