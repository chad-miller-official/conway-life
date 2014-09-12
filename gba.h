/*
 * gba.h
 *
 * Contains all #defines, typedefs, and function prototypes necessary
 * for this program to work.
 */
#ifndef GBA_H_INCLUDED
#define GBA_H_INCLUDED

/* DISPLAY DEFINES */
#define REG_DISPCNT *(u16*) 0x4000000

#define BUFFER0 (u16*) 0x6000000
#define BUFFER1 (u16*) 0x600A000
#define BUFFER1FLAG 0x10

#define PALETTE (u16*) 0x5000000

#define BG2_ENABLE (1 << 10)
#define MODE4 4

#define SCREEN_MIN_X 0
#define SCREEN_MAX_X 239
#define SCREEN_MIN_Y 0
#define SCREEN_MAX_Y 159
#define BUFFER_LEN4 19200

#define SCANLINECTR *(vu16*) 0x4000006

/* RGB DEFINES */
#define RGB(r, g, b) ((r) | (g) << 0x5 | (b) << 0xA)
#define COLOR_BLACK RGB(0, 0, 0)
#define COLOR_RED RGB(31, 0, 0)
#define COLOR_GREEN RGB(0, 31, 0)
#define COLOR_BLUE RGB(0, 0, 31)
#define COLOR_YELLOW RGB(31, 31, 0)
#define COLOR_MAGENTA RGB(31, 0, 31)
#define COLOR_CYAN RGB(0, 31, 31)
#define COLOR_WHITE RGB(31, 31, 31)

/* DMA DEFINES */
#define DMA ((volatile DMACNT*) 0x40000B0)

#define DMA_DST_INC (0 << 21)
#define DMA_DST_DEC (1 << 21)
#define DMA_DST_FIXED (2 << 21)
#define DMA_DST_RELOAD (3 << 21)

#define DMA_SRC_INC (0 << 23)
#define DMA_SRC_DEC (1 << 23)
#define DMA_SRC_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

/* BUTTON DEFINES */
#define BUTTONS *(vu16*) 0x4000130

#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN (1 << 7)
#define BUTTON_R (1 << 8)
#define BUTTON_L (1 << 9)

#define KEY_DOWN(k) (~(BUTTONS) & (k))

/* MISC DEFINES */
#define TRUE 1
#define FALSE 0

#define OFFSET2D(x, y, w) ((x) + ((y) * (w)))
#define OFFSET2D_SCREEN(x, y) ((x) + ((y) * 240))

/* TYPEDEFS */
typedef char bool;

typedef char s8;
typedef unsigned char u8;
typedef volatile unsigned char vu8;

typedef short s16;
typedef unsigned short u16;
typedef volatile unsigned short vu16;

typedef int s32;
typedef unsigned int u32;
typedef volatile unsigned int vu32;

typedef struct
{
	const volatile void* src;
	volatile void* dst;
	volatile u32 cnt;
} DMACNT;

/* VARIABLES */
extern u16* videoReadBuffer;
extern u16* videoWriteBuffer;

/* FUNCTION PROTOTYPES */
void setPixel4(u8, u8, u8);
u8 getPixel4(u8, u8);
void drawRect4(u8, u8, u8, u8, u8);
void drawFullscreenRect4(u8);
void drawImage4(u8, u8, u8, u8, const u16*);
void drawFullscreenImage4(const u16*);
void setPalette(const u16*, u16);
void waitForVblank();
void flipPage();
void copyVRtoVW();

#endif //GBA_H_INCLUDED
