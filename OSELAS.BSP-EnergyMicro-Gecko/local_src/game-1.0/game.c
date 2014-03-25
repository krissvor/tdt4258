#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char *argv[])
{
	printf("Hello World, I'm SUPER-game 1.0!\n");


	int fb = open("/dev/fb0", O_WRONLY);
	if (fb<0) printf("error opening fb"); 
	int i;
	int antall = 0;
	for (i = 0; i < 320*240; i++) {
		lseek(fb, i*2, SEEK_SET);
		uint16_t pixel = i;
		if (write(fb, &pixel, 2)<2) {
			printf("ikke nok bytes");
		}
	}


	exit(EXIT_SUCCESS);
}
