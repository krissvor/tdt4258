#ifndef _GAME_H
#define _GAME_H

#include <linux/fb.h>

#define SCREENW 320
#define SCREENH 240

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

