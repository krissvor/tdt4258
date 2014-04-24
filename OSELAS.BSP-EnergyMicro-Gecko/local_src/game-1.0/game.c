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

#include "game.h"

/*
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
*/
static void gameLoop(int signum);

int play = 1;
int totalFrames = 0;


int main(int argc, char *argv[])
{
	printf("Hello World, I'm SUPER-game 1.0!\n");
	
	setupFB();
	printf("FB mapped to: %p\n", fbMap);
	setupDriver();
	blankScreen();
	srand(time(0));
	
	spaceship.x = SCREENW/2-spaceship.w/2;
	spaceship.y = SCREENH/2-spaceship.h/2;
	paintSprite(&spaceship);
	
	
	//union sigval sval;
	//sval.sival_int = 1;
	
	struct sigevent sigev;
	sigev.sigev_notify = SIGEV_SIGNAL;
	//sigev.sigev_value = sval;
	sigev.sigev_signo = SIGUSR1;
	//sigev.sigev_notify_function = gameLoop;
	
	struct sigaction saction = {
		.sa_handler = gameLoop,
		.sa_flags = 0,
		.sa_mask = 0,
	};
	sigemptyset(&saction.sa_mask);
	sigaction(SIGUSR1, &saction, NULL);
	
	timer_t timerid;
	timer_create(CLOCK_MONOTONIC, &sigev, &timerid);
	struct timespec it_interval;
	it_interval.tv_sec = 0;
	it_interval.tv_nsec = FRAME_TIME_NANOS;
	const struct itimerspec new_value = {
		.it_interval = it_interval,
		.it_value = it_interval,
	};
	printf("Frame time: %i\n", new_value.it_interval.tv_nsec);
	timer_settime(timerid, 0, &new_value, NULL);
	
	
	struct timespec systemTime;
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	printf("System time: %d sec %d nanos\n", systemTime.tv_sec, systemTime.tv_nsec);
	time_t timeSeconds = time(0);
	/*
	struct timespec sleepTime;
	sleepTime.tv_sec = 0;
	long timeNanos;
	long elapsedTime;
	
	int count = 0;
	int totalFrames = 0;
	int flag = 0;
	int shotFired = 0;
	
	int moreThan = 0;
	int lessThan = 0;
	
	timeNanos = systemTime.tv_nsec;
	while (play) {
		ssize_t err = read(drfd, buttons, 8);
		if (err) printf("Failed to read button status\n");
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
	
	while (play) pause();
	
	blankScreen();
	printf("Average framerate: %i\n", totalFrames / (time(0) - timeSeconds));
	munmap(fbMap, 320*240*2);
	close(fbfd);
	close(drfd);
	exit(EXIT_SUCCESS);
}


static void gameLoop(int signum) {
	static long timeNanosOld;
	static long sleepTime;
	static struct timespec systemTime;
	static int count = 0;
	static int flag = 0;
	static int shotFired = 0;
	//printf("loop de loop\n");
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	sleepTime = systemTime.tv_nsec - timeNanosOld;
	
	
	ssize_t err = read(drfd, buttons, 8);
	if (err < 0) printf("Failed to read button status\n");
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
			//printf("FPS: %i    Last sleeptime: %i\nLess than: %i    More than: %i\n",
			//	count, sleepTime, lessThan, moreThan);
			printf("FPS: %i    Last sleeptime: %i\n",
				count, sleepTime);
			totalFrames += count;
			count = 0;
			flag = 1;
		}
	} else if (flag) {
		flag = 0;
	}
	clock_gettime(CLOCK_MONOTONIC, &systemTime);
	timeNanosOld = systemTime.tv_nsec;
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







