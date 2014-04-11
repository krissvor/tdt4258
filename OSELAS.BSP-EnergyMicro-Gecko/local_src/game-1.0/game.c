#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "game.h"

/*
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
*/


int main(int argc, char *argv[])
{
	printf("Hello World, I'm SUPER-game 1.0!\n");
	
	setupFB();
	printf("FB mapped to: %p\n", fbMap);
	setupDriver();
	blankScreen();
	srand(time(NULL));
	
	spaceship.x = SCREENW/2-spaceship.w/2;
	spaceship.y = SCREENH/2-spaceship.h/2;
	paintSprite(&spaceship);
	
	struct timespec systemTime;
	struct timespec sleepTime;
	sleepTime.tv_sec = 0;
	long timeNanos;
	long elapsedTime;
	time_t timeSeconds = time(NULL);
	int play = 1;
	int count = 0;
	int totalFrames = 0;
	int flag = 0;
	int shotFired = 0;
	
	int moreThan = 0;
	int lessThan = 0;
	
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	timeNanos = systemTime.tv_nsec;
	printf("System time: %d sec %d nanos\n", systemTime.tv_sec, timeNanos);
	while (play) {
		ssize_t err = read(drfd, buttons, 8);
		//printf("read() returned: %i\n", err);
		blankSprite(&spaceship);
		if (buttons[0] && spaceship.x > spaceship.pad) spaceship.x -= spaceship.speed;
		if (buttons[1] && spaceship.y > spaceship.pad) spaceship.y -= spaceship.speed;
		if (buttons[2] && spaceship.x < SCREENW-spaceship.w-spaceship.pad) spaceship.x += spaceship.speed;
		if (buttons[3] && spaceship.y < SCREENH-1-spaceship.h-spaceship.pad) spaceship.y += spaceship.speed;
		if (buttons[6]) play = 0;
		paintSprite(&spaceship);
		
		moveEnemy(&enemyBorg);
		
		if (checkCollision(&spaceship, &enemyBorg)) play = 0;
		
		if (buttons[7] && ! shotFired) {
			shotFired = 1;
			shot.x = spaceship.x + spaceship.w/2 - shot.w/2;
			shot.y = spaceship.y;
		}
		
		if (shotFired) {
			moveShot(&shot, &shotFired);
			if (checkCollision(&shot, &enemyBorg)) play = 0;
		}
		
		//totalFrames++;
		count++;
		if (systemTime.tv_nsec > 900000000) {
			if (!flag) {
				printf("FPS: %i    Last sleeptime: %i\nLess than: %i    More than: %i\n",
					count, sleepTime.tv_nsec, lessThan, moreThan);
				totalFrames += count;
				count = 0;
				flag = 1;
			}
		} else if (flag) {
			flag = 0;
		}
		
		timeNanos = systemTime.tv_nsec;
		clock_gettime(CLOCK_MONOTONIC, &systemTime);
		
		if (systemTime.tv_nsec > timeNanos) {
			elapsedTime = systemTime.tv_nsec - timeNanos;
			moreThan++;
		} else {
			elapsedTime = 1000000000 - timeNanos + systemTime.tv_nsec;
			lessThan++;
		}
		
		/*
		elapsedTime = systemTime.tv_nsec - timeNanos;
		if (elapsedTime < 0) elapsedTime += 1000000000;
		*/
		sleepTime.tv_nsec = FRAME_TIME_NANOS - elapsedTime;
		int ret = nanosleep(&sleepTime, NULL);
		clock_gettime(CLOCK_MONOTONIC, &systemTime);
		if (ret != 0) printf("Sleep was interrupted: %i\n", ret);
	}
	
	
	printf("Average framerate: %i\n", totalFrames / (time(NULL) - timeSeconds));
	blankScreen();
	munmap(fbMap, 320*240*2);
	close(fbfd);
	close(drfd);
	exit(EXIT_SUCCESS);
}


void moveEnemy(struct sprite *s) {
	blankSprite(s);
	
	s->dir += rand() % 3 - 1;
	
	switch(s->dir) {
		case 8: s->dir = 0;
		case 0:
			if (s->x < SCREENW - s->w - s->pad) s->x += s->speed;
			break;
		case 1:
			if (s->x < SCREENW - s->w - s->pad) s->x += s->speed;
			if (s->y < SCREENH - 1 - s->h - s->pad) s->y += s->speed;
			break;
		case 2:
			if (s->y < SCREENH - 1 - s->h - s->pad) s->y += s->speed;
			break;
		case 3:
			if (s->x > s->pad) s->x -= s->speed;
			if (s->y < SCREENH - 1 - s->h - s->pad) s->y += s->speed;
			break;
		case 4:
			if (s->x > s->pad) s->x -= s->speed;
			break;
		case 5:
			if (s->x > s->pad) s->x -= s->speed;
			if (s->y > s->pad) s->y -= s->speed;
			break;
		case 6:
			if (s->y > s->pad) s->y -= s->speed;
			break;
		case -1: s->dir = 7;
		case 7:
			if (s->x < SCREENW - s->w - s->pad) s->x += s->speed;
			if (s->y > s->pad) s->y -= s->speed;
			break;
	}
	paintSprite(s);
}


void setupDriver() {
	drfd = open("/dev/GPIO_buttons", O_RDONLY);
	printf("Driver fd: %i\n", drfd);
}


void moveShot(struct sprite *s, int *fired) {
	blankSprite(s);
	
	if (s->y > s->pad) {
		s->y -= s->speed;
		paintSprite(s);
	} else {
		*fired = 0;
		paintRect(s->x, s->y, s->w, s->h+1);
	}
}


int checkCollision(struct sprite *a, struct sprite *b) {
	if (a->x + a->w >= b->x &&
		a->x <= b->x + b->w &&
		a->y + a->h >= b->y &&
		a->y <= b->y + b->h) return 1;
	
	return 0;
}







