#include "sdlUtil.h";
#include "Config.h";
#include "Block.h";
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

class Tetris {

private:
	int quit, frames, rc;
	double worldTime, fpsTimer, fps, distance, etiSpeed;
	
	SDL_Event event;
	SDL_Surface *screen, *charset, *eti;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	char text[128];

	int kolory[9];

	
	double standardSpeed = 1;
	double blockShifter = 1;

	int t1, t2;
	double tickTime = 0;
	double delta;


	Block* blockGrid[20][10];


	void updateTime() {

		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;
		worldTime += delta;
		tickTime += delta * blockShifter * standardSpeed;
		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};
	};

	void cleenUpEnv() {
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();

		delete this;
	};
	void prepareEnv();
	void loop();
	void drawGui();
	void updateEvents();
	void drawBlocks();
	void drawBlock(int kolor, int x, int y);
	void drawBlock(int kolor, int x, int y, int blockShift);
	void moveBlocks();
	void generateBlock();
	void endGame(bool win);

public:

	Tetris() {
		prepareEnv();
		loop();
		cleenUpEnv();
	};

};