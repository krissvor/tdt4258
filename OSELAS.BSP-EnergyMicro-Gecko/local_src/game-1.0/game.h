#ifndef _GAME_H
#define _GAME_H

#include <linux/fb.h>

#define SCREENW 320
#define SCREENH 240

enum color {
	BLACK = 0x0,
	WHITE = 0xffff,
	RED = 31 << 11,
	GREEN = 63 << 5,
	BLUE = 31,
	CYAN = 63 << 5 | 31,
	MAGENTA = 31 << 11 | 31,
	YELLOW = 31 << 11 | 63 << 5,
};

struct fb_copyarea rect;
int fbfd;
uint16_t *fbMap;

struct sprite {
	int w;
	int h;
	uint16_t a[];
};

void setupFB();
void paintSprite(struct sprite *s, int posX, int posY);
void paintRect(int dx, int dy, int width, int height);






#endif

