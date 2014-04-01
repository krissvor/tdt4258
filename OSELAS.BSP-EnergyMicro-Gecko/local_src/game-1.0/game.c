#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#include "game.h"


#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>

struct fb_copyarea rect;
int fbfd;
uint16_t *fbMap;

void setupFB();
void paintRect(int dx, int dy, int width, int height);





int main(int argc, char *argv[])
{
	printf("Hello World, I'm SUPER-game 1.0!\n");
	
	setupFB();
	printf("FB mapped to: %p\n", fbMap);
	printf("FB mapped to: %p\n", &fbMap);
	
	int i;
	printf("Do loop\n");
	for (i = 0; i < 320*240; i++) {
		printf("%i\n", i);
		uint16_t pixel = i;
		printf("Dill\n");
		*fbMap = pixel;
		printf("Dall\n");
	}
	printf("Loop done\n");
	
	paintRect(0, 0, 320, 240);
	
	exit(EXIT_SUCCESS);
}




void setupFB() {
	printf("Initializing FB\n");
	fbfd = open("/dev/fb0", O_RDWR);
	printf("fbfd: %i\n", fbfd);
	if (fbfd < 0) printf("Error opening FB\n");
	fbMap = (uint16_t*)mmap(NULL, 320*240*2, PROT_READ | PROT_WRITE, MAP_PRIVATE, fbfd, 0);
	
}

void paintRect(int dx, int dy, int width, int height) {
 	rect.dx = dx;
	rect.dy = dy;
	rect.width = width;
	rect.height = height;
	
	ioctl(fbfd, 0x4680, &rect);
}




