#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include "game.h"

/*
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
*/
static void gameLoop(int signum);

/* Game states:
	0 - Keep playing
	1 - Player lost
	2 - Player victory
	9 - Player quit
*/
int exitState = 0;
int totalFrames = 0; // Total frames displayed


int main(int argc, char *argv[])
{
	printf("Hello World, I'm SUPER-game 1.0!\n");
	
	setupFB();
	printf("FB mapped to: %p\n", fbMap);
	setupDriver();
	blankScreen(); // Penguin go *poof* !
	srand(time(0));
	
	/* Place player sprite in middle of screen */
	spaceship.x = SCREENW/2-spaceship.w/2;
	spaceship.y = SCREENH/2-spaceship.h/2;
	paintSprite(&spaceship);
	
	/* Setup Timer signaling */
	struct sigevent sigev = {
		.sigev_notify = SIGEV_SIGNAL,
		.sigev_signo = SIGUSR1,
	};
	struct sigaction saction = {
		.sa_handler = gameLoop,
		.sa_flags = 0,
		.sa_mask = 0,
	};
	sigemptyset(&saction.sa_mask);
	sigaction(SIGUSR1, &saction, NULL);
	
	timer_t timerid;
	timer_create(CLOCK_MONOTONIC, &sigev, &timerid);
	struct timespec it_interval = {
		.tv_sec = 0,
		.tv_nsec = FRAME_TIME_NANOS,
	};
	
	const struct itimerspec new_value = {
		.it_interval = it_interval,
		.it_value = it_interval,
	};
	printf("Frame time: %i\n", new_value.it_interval.tv_nsec);
	timer_settime(timerid, 0, &new_value, NULL);
	/* End timer setup */
	
	struct timespec systemTime;
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	printf("System time: %d sec %d nanos\n", systemTime.tv_sec, systemTime.tv_nsec);
	time_t timeSeconds = time(0); // Get start time to calculate average framerate
	
	/*
	struct timespec sleepTime;
	sleepTime.tv_sec = 0;
	long timeNanos;
	long elapsedTime;
	
	int count = 0;
	int totalFrames = 0;
	int fpsPrintFlag = 0;
	int shotFired = 0;
	
	int moreThan = 0;
	int lessThan = 0;
	
	timeNanos = systemTime.tv_nsec;
	while (!exitState) {
		ssize_t err = read(drfd, buttons, 8);
		if (err) printf("Failed to read button status\n");
		blankSprite(&spaceship);
		if (buttons[0] && spaceship.x > spaceship.pad) spaceship.x -= spaceship.speed;
		if (buttons[1] && spaceship.y > spaceship.pad) spaceship.y -= spaceship.speed;
		if (buttons[2] && spaceship.x < SCREENW-spaceship.w-spaceship.pad) spaceship.x += spaceship.speed;
		if (buttons[3] && spaceship.y < SCREENH-1-spaceship.h-spaceship.pad) spaceship.y += spaceship.speed;
		if (buttons[6]) exitState = 9;
		paintSprite(&spaceship);
		
		moveEnemy(&enemyBorg);
		
		if (checkCollision(&spaceship, &enemyBorg)) exitState = 1;
		
		if (buttons[7] && ! shotFired) {
			shotFired = 1;
			shot.x = spaceship.x + spaceship.w/2 - shot.w/2;
			shot.y = spaceship.y;
		}
		
		if (shotFired) {
			moveShot(&shot, &shotFired);
			if (checkCollision(&shot, &enemyBorg)) exitState = 2;
		}
		
		//totalFrames++;
		count++;
		if (systemTime.tv_nsec > 900000000) {
			if (!fpsPrintFlag) {
				printf("FPS: %i    Last sleeptime: %i\nLess than: %i    More than: %i\n",
					count, sleepTime.tv_nsec, lessThan, moreThan);
				totalFrames += count;
				count = 0;
				fpsPrintFlag = 1;
			}
		} else if (fpsPrintFlag) {
			fpsPrintFlag = 0;
		}
		
		
		clock_gettime(CLOCK_MONOTONIC, &systemTime);
		
		if (systemTime.tv_nsec > timeNanos) {
			elapsedTime = systemTime.tv_nsec - timeNanos;
			moreThan++;
		} else {
			elapsedTime = 1000000000 - timeNanos + systemTime.tv_nsec;
			lessThan++;
		}
		
		sleepTime.tv_nsec = FRAME_TIME_NANOS - elapsedTime;
		int ret = nanosleep(&sleepTime, NULL);
		clock_gettime(CLOCK_MONOTONIC, &systemTime);
		timeNanos = systemTime.tv_nsec;
		if (ret != 0) printf("Sleep was interrupted: %i\n", ret);
	}
	*/
	
	sigset_t sigmask;
	sigemptyset(&sigmask);
	while (!exitState) sigsuspend(&sigmask);
	it_interval.tv_nsec = 0; // Disarm timer
	timer_settime(timerid, 0, &new_value, NULL);
	//while (!exitState) pause(); // Pause after each timer signal
	
	printf("Average framerate: %i\n", totalFrames / (time(0) - timeSeconds));
	blankScreen();
	switch (exitState) {
		case 1: printf("\nYou loose! Game over!\n"); break;
		case 2: printf("\nYou Win! Game over!\n"); break;
		case 9: printf("\nYou quit! Game over!\n"); break;
	}
	
	munmap(fbMap, 320*240*2);
	//munmap(buttons, 8);
	close(fbfd);
	close(drfd);
	exit(EXIT_SUCCESS);
}


/* The main game loop that catches the timer signal */
static void gameLoop(int signum) {
	static long timeNanosOld;
	static long sleepTime;
	static struct timespec systemTime;
	static int count = 0; // Frame counter
	static int fpsPrintFlag = 0; // 
	static int shotFired = 0;
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	sleepTime = systemTime.tv_nsec - timeNanosOld;
	
	
	ssize_t err = read(drfd, buttons, 8); // Poll driver for button status
	if (err < 0) printf("Failed to read button status\n");
	blankSprite(&spaceship);
	/* Move player based on buttons 1-4 pushed */
	if (buttons[0] && spaceship.x > spaceship.pad) spaceship.x -= spaceship.speed;
	if (buttons[1] && spaceship.y > spaceship.pad) spaceship.y -= spaceship.speed;
	if (buttons[2] && spaceship.x < SCREENW-spaceship.w-spaceship.pad) spaceship.x += spaceship.speed;
	if (buttons[3] && spaceship.y < SCREENH-1-spaceship.h-spaceship.pad) spaceship.y += spaceship.speed;
	if (buttons[6]) exitState = 9; // Push button 7 to quit game
	paintSprite(&spaceship);
	
	moveEnemy(&enemyBorg);
	
	if (checkCollision(&spaceship, &enemyBorg)) exitState = 1;
	
	if (buttons[7] && ! shotFired) { // Button 8 fires shot
		shotFired = 1;
		shot.x = spaceship.x + spaceship.w/2 - shot.w/2;
		shot.y = spaceship.y;
	}
	
	if (shotFired) {
		moveShot(&shot, &shotFired);
		if (checkCollision(&shot, &enemyBorg)) exitState = 2;
	}
	
	//totalFrames++;
	count++;
	if (systemTime.tv_nsec > 900000000) {
		if (!fpsPrintFlag) {
			//printf("FPS: %i    Last sleeptime: %i\nLess than: %i    More than: %i\n",
			//	count, sleepTime, lessThan, moreThan);
			printf("FPS: %i    Last sleeptime: %i\n",
				count, sleepTime);
			totalFrames += count;
			count = 0;
			fpsPrintFlag = 1;
		}
	} else if (fpsPrintFlag) {
		fpsPrintFlag = 0;
	}
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	timeNanosOld = systemTime.tv_nsec;
}

/* Move sprite */
void moveEnemy(struct sprite *s) {
	blankSprite(s);
	
	s->dir += rand() % 3 - 1; // Change direction randomly
	
	switch(s->dir) { // Move sprite in the given direction
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

/* Initialize gamepad driver */
void setupDriver() {
	drfd = open("/dev/GPIO_buttons", O_RDONLY);
	printf("Driver fd: %i\n", drfd);
	//buttons = (char*)mmap(NULL, 8, PROT_READ, MAP_SHARED, drfd, 0);
	//if (buttons == (char*)-1) printf("Failed mapping buttons: %i\n", errno);
}

/* Move laser upwards */
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

/* Check for collision between sprites */
int checkCollision(struct sprite *a, struct sprite *b) {
	if (a->x + a->w >= b->x &&
		a->x <= b->x + b->w &&
		a->y + a->h >= b->y &&
		a->y <= b->y + b->h) return 1;
	
	return 0;
}







