#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>

#include "game.h"




void setupFB() {
	fbfd = open("/dev/fb0", O_WRONLY);
	if (fbfd < 0) printf("Error opening FB");
	fbMap = mmap(NULL, 320*240*2, PROT_WRITE, MAP_SHARED, fbfd, 0);
	printf("FB mapped to: " + fbMap);
	
}


void paintRect(int dx, int dy, int width, int height) {
 	rect.dx = dx;
	rect.dy = dy;
	rect.width = width;
	rect.height = height;
	
	ioctl(fbfd, 0x4680, &rect);
	
}


