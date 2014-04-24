#ifndef _GAME_H
#define _GAME_H

#include <linux/fb.h>
#include <stdint.h>
#include <signal.h>

#define SCREENW 320
#define SCREENH 240
#define FRAME_LIMITER 30
#define FRAME_TIME_NANOS (1000000000 / FRAME_LIMITER)

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
	GY = 31/2 << 11 | 63/2 << 5 | 31/2	//gray
};

struct fb_copyarea rect;
int fbfd;
uint16_t *fbMap;
int drfd;
char buttons[8];

struct sprite {
	int x;
	int y;
	int dir;
	int speed;
	int pad; // Should be 2 * speed - 1
	int w;
	int h;
	uint16_t *a;
};

struct sprite spaceship;
struct sprite enemyBorg;
struct sprite shot;

void setupFB();
void setupDriver();
void blankScreen();
void paintSprite(struct sprite *s);
void blankSprite(struct sprite *s);
void paintRect(int dx, int dy, int width, int height);
void moveEnemy();
void moveShot(struct sprite *s, int *fired);
int checkCollision(struct sprite *a, struct sprite *b);
//void gameLoop(union sigval sval);






#endif

