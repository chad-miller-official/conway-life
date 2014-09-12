/*
 * life.h
 *
 * Contains #defines, typedefs, and function prototypes necessary
 * for Conway's Game of Life to work.
 */
#ifndef LIFE_H_INCLUDED
#define LIFE_H_INCLUDED

#include "gba.h"

/* NEIGHBOR DEFINES */
//These are only really used when EXPERIMENTAL is #defined
#define NBR_TOPLEFT 1
#define NBR_TOPCENTER (1 << 1)
#define NBR_TOPRIGHT (1 << 2)
#define NBR_CENTERLEFT (1 << 3)
#define NBR_CENTERRIGHT (1 << 4)
#define NBR_BOTTOMLEFT (1 << 5)
#define NBR_BOTTOMCENTER (1 << 6)
#define NBR_BOTTOMRIGHT (1 << 7)

/* PALETTE DEFINES */
#define PALETTE_SIZE 6

//Palette color indices
#define BG 0		//Background
#define PX_DEAD 0	//Dead pixel (background and dead pixel are identical)
#define PX_ALIVE 1	//Alive pixel
#define CUR_BG 2	//Cursor on background
#define CUR_PX_DEAD 2	//Cursor on dead pixel
#define CUR_PX_ALIVE 3	//Cursor on alive pixel
#define CUR_BG_ALT 4	//Cursor on background (alternate color)
#define CUR_PX_DEAD_ALT 4	//Cursor on dead pixel (alternate color)
#define CUR_PX_ALIVE_ALT 5	//Cursor on alive pixel (alternate color)

/* TYPEDEFS */
typedef struct Cursor
{
	u8 x, y;
	u8 x0, y0;
	u8 x1, y1;
	bool alt;
} Cursor;

/* VARIABLES */
extern bool editMode, paused;
extern const u16 lifePalette[PALETTE_SIZE];
extern const u16 lifePalette2[PALETTE_SIZE];
extern const u16 lifePalette3[PALETTE_SIZE];

/* FUNCTION PROTOTYPES */
void pixelLogic(u8 x, u8 y);
void randomize();

#endif //LIFE_H_INCLUDED
