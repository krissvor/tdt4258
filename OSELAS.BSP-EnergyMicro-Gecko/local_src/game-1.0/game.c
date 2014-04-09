#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "game.h"


#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>



struct sprite spaceship = {
	.w = 26,
	.h = 21,
	.a = {
		BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,GR,GR,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,GR,GR,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,GR,GR,GR,GR,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,GR,GR,GR,GR,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,YE,BL,BL,BL,BL,BL,GR,GR,GR,GR,GR,GR,BL,BL,BL,BL,BL,YE,BL,BL,BL,BL,
		BL,BL,BL,GR,GR,BL,BL,BL,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,BL,BL,BL,GR,GR,BL,BL,BL,
		BL,BL,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,BL,BL,
		BL,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,BL,
		BL,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,BL,
		BL,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,BL,
		BL,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,GR,BL,
		BL,GR,GR,GR,GR,GR,GR,GR,WH,WH,WH,GR,GR,GR,GR,WH,WH,WH,GR,GR,GR,GR,GR,GR,GR,BL,
		BL,GR,GR,GR,GR,GR,GR,WH,RE,RE,RE,WH,GR,GR,WH,RE,RE,RE,WH,GR,GR,GR,GR,GR,GR,BL,
		BL,GR,GR,GR,GR,GR,WH,RE,RE,RE,RE,RE,GR,GR,RE,RE,RE,RE,RE,WH,GR,GR,GR,GR,GR,BL,
		BL,BL,BL,GR,GR,WH,RE,RE,RE,RE,RE,RE,WH,WH,RE,RE,RE,RE,RE,RE,WH,GR,GR,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,RE,RE,RE,RE,RE,BL,BL,RE,RE,RE,RE,RE,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,RE,RE,RE,BL,BL,BL,BL,RE,RE,RE,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,RE,RE,RE,BL,BL,BL,BL,RE,RE,RE,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,BL,RE,BL,BL,BL,BL,BL,BL,RE,BL,BL,BL,BL,BL,BL,BL,BL,BL,
		BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,BL,
	}
};




int main(int argc, char *argv[])
{
	printf("Hello World, I'm SUPER-game 1.0!\n");
	
	setupFB();
	printf("FB mapped to: %p\n", fbMap);
	setupDriver();
	blankScreen();
	
	int posX = SCREENW/2-spaceship.w/2;
	int posY = SCREENH/2-spaceship.h/2;
	paintSprite(&spaceship, posX, posY);
	
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
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	timeNanos = systemTime.tv_nsec;
	printf("System time: %d sec %d nanos\n", systemTime.tv_sec, timeNanos);
	while (play) {
		ssize_t err = read(drfd, buttons, 8);
		//printf("read() returned: %i\n", err);
		if (buttons[0] && posX > 0) posX -= SHIP_SPEED;
		if (buttons[1] && posY > 0) posY -= SHIP_SPEED;
		if (buttons[2] && posX < SCREENW-spaceship.w) posX += SHIP_SPEED;
		if (buttons[3] && posY < SCREENH-1-spaceship.h) posY += SHIP_SPEED;
		if (buttons[6]) play = 0;
		paintSprite(&spaceship, posX, posY);
		
		count++;
		if (systemTime.tv_nsec > 900000000) {
			if (!flag) {
				printf("FPS: %i\n", count);
				totalFrames += count;
				count = 0;
				flag = 1;
				//timeNanos = systemTime.tv_nsec;
			}
		} else if (flag) {
			flag = 0;
		}
		
		timeNanos = systemTime.tv_nsec;
		clock_gettime(CLOCK_MONOTONIC, &systemTime);
		elapsedTime = systemTime.tv_nsec - timeNanos;
		if (elapsedTime < 0) elapsedTime += 1000000000;
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


void blankScreen() {
	int i;
	uint16_t *tempfbmap = fbMap;
	printf("Blank screen\n");
	for (i = 0; i < SCREENW*SCREENH; i++) { // Blank screen
		*tempfbmap = BL;
		tempfbmap++;
	}
	paintRect(0, 0, SCREENW, SCREENH);
}

void paintSprite(struct sprite *s, int posX, int posY) {
	uint16_t *tempfbmap = fbMap;
	tempfbmap += SCREENW * posY + posX;
	//printf("Draw sprite\n");
	int j;
	int i;
	for (j = 0; j < s->h; j++) {
		for (i = j*s->w; i < j*s->w+s->w; i++) {
			*tempfbmap = s->a[i];
			tempfbmap++;
		}
		tempfbmap += SCREENW - s->w;
	}
	paintRect(posX, posY, s->w, s->h+1);
}

void setupFB() {
	fbfd = open("/dev/fb0", O_RDWR);
	printf("Framebuffer fd: %i\n", fbfd);
	if (fbfd < 0) printf("Error opening FB\n");
	fbMap = (uint16_t*)mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
}

void setupDriver() {
	drfd = open("/dev/GPIO_buttons", O_RDONLY);
	printf("Driver fd: %i\n", drfd);
}

void paintRect(int dx, int dy, int width, int height) {
 	rect.dx = dx;
	rect.dy = dy;
	rect.width = width;
	rect.height = height;
	
	ioctl(fbfd, 0x4680, &rect);
}




