/*
 * gba.c
 *
 * Contains all necessary GBA functions for this program to work.
 */
#include "gba.h"

/*
 * Video buffers.
 * videoReadBuffer is the buffer currently being shown.
 * videoWriteBuffer is the hidden buffer that can be drawn to.
 */
u16* videoReadBuffer = (u16*) 0x6000000;
u16* videoWriteBuffer = (u16*) 0x600A000;

/*
 * Sets a pixel at a specified (x, y) coordinate to a specific
 * color in the palette, specified by its index in the palette.
 */
void setPixel4(u8 x, u8 y, u8 index)
{
	if(x > SCREEN_MAX_X || y > SCREEN_MAX_Y)	//Bounds checking
		return;

	u16 dst = OFFSET2D_SCREEN(x, y) / 2;
	videoWriteBuffer[dst] &= (x & 1) ? 0x00FF : 0xFF00;
	videoWriteBuffer[dst] |= (x & 1) ? (index << 8) : index;
}

/*
 * Gets the color at a specified (x, y) coordinate from
 * videoWriteBuffer in the form of an index in the current palette.
 */
u8 getPixel4(u8 x, u8 y)
{
	if(x > SCREEN_MAX_X || y > SCREEN_MAX_Y)
		return 0;

	u16 dst = OFFSET2D_SCREEN(x, y) / 2;
	return (x & 1) ? ((videoWriteBuffer[dst] & 0xFF00) >> 8) : (videoWriteBuffer[dst] & 0x00FF);
}

/*
 * Draws a rectangle with a specified width and height at the
 * specified (x, y) coordinate to the screen, using a color
 * specified by an index in the current palette.
 */
void drawRect4(u8 x, u8 y, u8 w, u8 h, u8 index)
{
	if(x > SCREEN_MAX_X || y > SCREEN_MAX_Y)
		return;

	if(x + w > SCREEN_MAX_X)
		w = SCREEN_MAX_X - x;

	if(y + h > SCREEN_MAX_Y)
		h = SCREEN_MAX_Y - y;
	
	vu16 clrEff = (index << 8) | index;

	for(int i = 0; i < h; i++)
	{
		DMA[3].src = &clrEff;
		DMA[3].dst = &videoWriteBuffer[OFFSET2D_SCREEN(x, y) / 2];
		DMA[3].cnt = (w / 2) | DMA_DST_INC | DMA_SRC_FIXED | DMA_ON;
	}
}

/*
 * Draws a rectangle with a width and height of 240 and 160,
 * respectively, at (0, 0), with a specified color in the form
 * of its index in the palette.
 * This is (theoretically) more useful because no iteration is
 * involved, meaning this is a constant-time procedure.
 */
void drawFullscreenRect4(u8 index)
{
	vu16 clrEff = (index << 8) | index;
	DMA[3].src = &clrEff;
    DMA[3].dst = videoWriteBuffer;
    DMA[3].cnt = BUFFER_LEN4 | DMA_DST_INC | DMA_SRC_FIXED | DMA_ON;
}

/*
 * Draws an image with a specified width and height at the
 * specified (x, y) coordinate to the screen.
 */
void drawImage4(u8 x, u8 y, u8 w, u8 h, const u16* g)
{
	if(x > SCREEN_MAX_X || y > SCREEN_MAX_Y)
		return;

	if(x + w > SCREEN_MAX_X)
		w = SCREEN_MAX_X - x;

	if(y + h > SCREEN_MAX_Y)
		h = SCREEN_MAX_Y - y;

	for(int i = 0; i < h; i++)
	{
		DMA[3].src = (g + (w * (i / 2)));
    	DMA[3].dst = &videoWriteBuffer[OFFSET2D_SCREEN(x, y) / 2];
    	DMA[3].cnt = (w / 2) | DMA_ON;
	}
}

/*
 * Draws an image with a width and height of 240 and 160,
 * respectively, at (0, 0).
 */
void drawFullscreenImage4(const u16* g)
{
	DMA[3].src = g;
    DMA[3].dst = videoWriteBuffer;
    DMA[3].cnt = BUFFER_LEN4 | DMA_ON;
}

/*
 * Sets the palette to a new palette with a specified
 * number of colors.
 */
void setPalette(const u16* p, u16 size)
{
	DMA[3].src = p;
    DMA[3].dst = PALETTE;
    DMA[3].cnt = size | DMA_ON;
}

/*
 * Wait for vertical blanking to finish.
 */
void waitForVblank()
{
	while(SCANLINECTR > 160);
	while(SCANLINECTR < 160);
}

/*
 * Flips the page.
 * videoReadBuffer is set to point to the address that
 * videoWriteBuffer was pointing to, and vice-versa.
 */
void flipPage()
{
	if(REG_DISPCNT & BUFFER1FLAG)
	{
		videoReadBuffer = BUFFER1;
		videoWriteBuffer = BUFFER0;
	}
	else
	{
		videoReadBuffer = BUFFER0;
		videoWriteBuffer = BUFFER1;
	}

	REG_DISPCNT ^= BUFFER1FLAG;
}

/*
 * Copies the contents of videoReadBuffer to videoWriteBuffer.
 */
void copyVRtoVW()
{
	DMA[3].src = videoReadBuffer;
	DMA[3].dst = videoWriteBuffer;
	DMA[3].cnt = BUFFER_LEN4 | DMA_ON;
}
