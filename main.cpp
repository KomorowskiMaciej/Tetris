#include "main.h";


void Tetris::prepareEnv() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
	}
	
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "Tetris Maciej Komorowski");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_ENABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};
	SDL_SetColorKey(charset, true, 0x000000);

	eti = SDL_LoadBMP("./eti.bmp");
	if (eti == NULL) {
		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};

	char text[128];
	kolory[1] = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	kolory[2] = SDL_MapRGB(screen->format, 0x3C, 0x7A, 0x06);
	kolory[3] = SDL_MapRGB(screen->format, 0x7F, 0x00, 0x11);
	kolory[4] = SDL_MapRGB(screen->format, 0x85, 0x51, 0x0E);
	kolory[5] = SDL_MapRGB(screen->format, 0x33, 0x99, 0xFF);
	kolory[6] = SDL_MapRGB(screen->format, 0x85, 0x56, 0x7E);
	kolory[7] = SDL_MapRGB(screen->format, 0xB3, 0x9B, 0x0F);
	kolory[8] = SDL_MapRGB(screen->format, 0x07, 0x7C, 0x85);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
			blockGrid[i][j] = new Block(0);
};

void Tetris::drawBlocks() {
	sdlUtil::DrawRectangle(
		screen,
		0,
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		kolory[8],
		kolory[8]
		);

	int blockShift = BLOCK_SIZE * tickTime;
	for (int y = 0; y < 20; y++)
		for (int x = 0; x < 10; x++)
			if (blockGrid[y][x]->block_active == false)
				switch (blockGrid[y][x]->block_type)
				{
					case 1:
						drawBlock(kolory[1], x, y);
					break;
					case 2:
						drawBlock(kolory[2], x, y);
						break;
					case 3:
						drawBlock(kolory[3], x, y);
						break;
					case 4:
						drawBlock(kolory[4], x, y);
						break;
					case 5:
						drawBlock(kolory[5], x, y);
						break;
					case 6:
						drawBlock(kolory[6], x, y);
						break;
					case 7:
						drawBlock(kolory[7], x, y);
						break;
					case 8:
						drawBlock(kolory[8], x, y);
						break;
					default:
						break;
				}
			else switch (blockGrid[y][x]->block_type)
			{
			case 1:
				drawBlock(kolory[1], x, y, blockShift);
				break;
			case 2:
				drawBlock(kolory[2], x, y, blockShift);
				break;
			case 3:
				drawBlock(kolory[3], x, y, blockShift);
				break;
			case 4:
				drawBlock(kolory[4], x, y, blockShift);
				break;
			case 5:
				drawBlock(kolory[5], x, y, blockShift);
				break;
			case 6:
				drawBlock(kolory[6], x, y, blockShift);
				break;
			case 7:
				drawBlock(kolory[7], x, y, blockShift);
				break;
			case 8:
				drawBlock(kolory[8], x, y, blockShift);
				break;
				default:
					break;
			}
}

void Tetris::drawBlock(int kolor, int x, int y) {
	sdlUtil::DrawRectangle(
		screen,
		SCREEN_WIDTH / 2 - 5 * BLOCK_SIZE + x * BLOCK_SIZE,
		SCREEN_HEIGHT - 20 * BLOCK_SIZE + y * BLOCK_SIZE - 50,
		BLOCK_SIZE,
		BLOCK_SIZE,
		kolory[0],
		kolor
		);
}

void Tetris::drawBlock(int kolor, int x, int y, int blockShift) {
	sdlUtil::DrawRectangle(
		screen,
		SCREEN_WIDTH / 2 - 5 * BLOCK_SIZE + x * BLOCK_SIZE,
		SCREEN_HEIGHT - 20 * BLOCK_SIZE + y * BLOCK_SIZE - 50 + blockShift,
		BLOCK_SIZE,
		BLOCK_SIZE,
		kolory[0],
		kolor
	);
}

void Tetris::loop() {
	while (!quit) {

		updateTime();

		if (tickTime > 1) {
			tickTime = 0;
			moveBlocks();
		}			

		drawBlocks();
		drawGui();

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		updateEvents();
		frames++;
	};
}

void Tetris::endGame(bool win){
	quit = 1;
}

void Tetris::generateBlock(){
	
	if (blockGrid[0][5]->block_type != 0)
		endGame(false);
	
	srand(time(NULL));

	switch (rand()%7)
	{
		case 0:{
			blockGrid[0][5] = new Block(1, true);
			blockGrid[0][3] = new Block(1, true);
			blockGrid[0][4] = new Block(1, true);
			blockGrid[1][4] = new Block(1, true);
			break;
		};
		case 1:{
			blockGrid[0][5] = new Block(2, true);
			blockGrid[0][4] = new Block(2, true);
			blockGrid[1][4] = new Block(2, true);
			blockGrid[1][5] = new Block(2, true);
			break;
		};
		case 2:{
			blockGrid[0][5] = new Block(3, true);
			blockGrid[1][5] = new Block(3, true);
			blockGrid[2][5] = new Block(3, true);
			blockGrid[2][6] = new Block(3, true);
			break;
		};
		case 3:{
			blockGrid[0][5] = new Block(4, true);
			blockGrid[1][5] = new Block(4, true);
			blockGrid[2][5] = new Block(4, true);
			blockGrid[2][4] = new Block(4, true);
			break;
		};
		case 4:{
			blockGrid[0][5] = new Block(5, true);
			blockGrid[1][5] = new Block(5, true);
			blockGrid[2][5] = new Block(5, true);
			blockGrid[3][5] = new Block(5, true);
			break;
		};
		case 5:{
			blockGrid[0][6] = new Block(6, true);
			blockGrid[0][5] = new Block(6, true);
			blockGrid[1][5] = new Block(6, true);
			blockGrid[1][4] = new Block(6, true);
			break;
		};
		case 6:{
			blockGrid[0][4] = new Block(7, true);
			blockGrid[0][5] = new Block(7, true);
			blockGrid[1][5] = new Block(7, true);
			blockGrid[1][6] = new Block(7, true);
			break;
		};
	}



	
}

void Tetris::moveBlocks(){
	
	bool thereIsActiveBlock = false;
	int activeGroup = 0;
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (blockGrid[y][x]->block_active){
				activeGroup = blockGrid[y][x]->block_active;
				thereIsActiveBlock = true;
				break;
			}
		}
		if (thereIsActiveBlock)
			break;
	}

	if (!thereIsActiveBlock){
		generateBlock();
		return;
	}

	//sprawdzenie czy klocki moga jeszcze spadac
	bool greenLight = true;
	for (int j = 0; j < 20; j++) {
		for (int i = 0; i < 10; i++) {
			if (blockGrid[j][i]->group == activeGroup){

				if (j > 5 ) {
					greenLight = false;
					break;
				}
				else if (!(blockGrid[j + 1][i]->block_type == 0 || blockGrid[j + 1][i]->group == activeGroup)) {
					greenLight = false;
					break;
				}
			}
		}
		if (!greenLight)
			break;
	}

		for (int y = 19; y >= 0; y--)
			for (int x = 0; x < 10; x++) {
				if (greenLight) {
				blockGrid[y + 1][x] = new Block(blockGrid[y][x]->block_type, true);
				blockGrid[y][x] = new Block(0);
				}
				else {
					blockGrid[y][x]->block_active = false;
				}

	}
};

void Tetris::drawGui() {
	sdlUtil::DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 20, kolory[1], kolory[1]);
	sprintf(text, "TETRIS", worldTime, fps);
	sdlUtil::DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
};


void Tetris::updateEvents() {
	bool keyPressed = false;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_UP) blockShifter = 10.0;
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					for (int y = 0; y < 20; y++)
						for (int x = 9; x > 0; x--)
						{
							if (blockGrid[y][x]->block_active && blockGrid[y][x + 1]->block_type == 0){
								blockGrid[y][x + 1] = new Block(blockGrid[y][x]->block_type, blockGrid[y][x]->block_active);
								blockGrid[y][x] = new Block(0);
							}
						}
						
				}
				else if (event.key.keysym.sym == SDLK_LEFT) {
					for (int y = 0; y < 20; y++)
						for (int x = 0; x < 9; x++)
						{
							if (blockGrid[y][x]->block_active && blockGrid[y][x - 1]->block_type == 0){
								blockGrid[y][x - 1] = new Block(blockGrid[y][x]->block_type, blockGrid[y][x]->block_active);
								blockGrid[y][x] = new Block(0);
							}
						}
					
				}
			break;
		case SDL_KEYUP:
			blockShifter = 1;
			
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		};
	};
}


int main(int argc, char **argv) {
	new Tetris();
	return 0;
};