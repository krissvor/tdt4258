#ifndef _GAME_H
#define _GAME_H

#include <linux/fb.h>



struct fb_copyarea rect;
int fbfd;
uint32_t *fbMap;

void setupFB();
void paintRect(int dx, int dy, int width, int height);



#endif
