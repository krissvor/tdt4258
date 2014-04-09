#ifndef _GAME_H
#define _GAME_H

#include <linux/fb.h>

#define SCREENW 320
#define SCREENH 240
#define FRAME_LIMITER 30
#define FRAME_TIME_NANOS 1000000000/30
#define SHIP_SPEED 5

enum color {
	BL = 0x0,					//black
	WH = 0xffff,				//white
	RE = 31 << 11,				//red
	GR = 63 << 5,				//green
	BU = 31,					//blue
	CY = 63 << 5 | 31,			//cyan
	MA = 31 << 11 | 31,			//magenta
	YE = 31 << 11 | 63 << 5,	//yellow
	DG = 63/2 << 5,				//dark green
};

struct fb_copyarea rect;
int fbfd;
uint16_t *fbMap;
int drfd;
char buttons[8];

struct sprite {
	int w;
	int h;
	uint16_t a[];
};

void setupFB();
void setupDriver();
void blankScreen();
void paintSprite(struct sprite *s, int posX, int posY);
void paintRect(int dx, int dy, int width, int height);






#endif

