#ifndef GRAPHICSRENDERER_H
#define GRAPHICSRENDERER_H


void motionCallback(int x, int y);

void keyboardCallback(unsigned char key, int x, int y);

void mouseCallback(int button, int state, int x, int y);

void idleCallback();

void renderCallback();

void exitCallback(void);

void renderLoop();

#endif
