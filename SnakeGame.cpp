//Created by Brandon Chenze //

#include "pch.h"
#include <iostream>
#include<vector>
#include <deque>
#include "SDL.h"
#include "SDL_image.h"
#include <ctime>

using namespace std;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct Candy {
	SDL_Rect rect{ 400, 320, 16, 16 };

	void genPos() {
		rect.x = rand() % (SCREEN_WIDTH-rect.w);
		rect.y = rand() % (SCREEN_HEIGHT-rect.h);
	}
};
//Create a structure for the game as a whole 
struct game {

};

//structure to save position on screen
struct position {
	int x;
	int y;
	int x1;
	int y2;
	int xvelocity;
	int yvelocity;
};

//structure for our main snake
//need to add snakes vector into snake.
struct Snake {
	SDL_Rect myRect{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 32, 32 };

	int xvelocity = 0;
	int yvelocity = 0;
	


	void updatePosition() {
		myRect.x += xvelocity;
		myRect.y += yvelocity;
	}

	bool checkBoundary() {
		if (myRect.x + myRect.w > SCREEN_WIDTH) { 
			xvelocity = 0;
			return true;
		}

		if (myRect.x < 0) { 
			xvelocity = 0;
			return true;
		}
		if ((myRect.y + myRect.h) >= SCREEN_HEIGHT) {
			yvelocity = 0;
			return true;
		}
		if (myRect.y < 0) { yvelocity = 0;
		return true;
		}
		return false;
	
	}
	void orientation() {
		if (yvelocity == 1 || yvelocity == -1) {
			if (myRect.w != 32) {
				myRect.h = myRect.w;
				myRect.w = 32;
			}
			return;
		}
		else if (xvelocity == 1 || xvelocity == -1) {
			if (myRect.h != 32) {
				myRect.w = myRect.h;
				myRect.h = 32;
			}
			return;
		}
	}
};



position getPosition(Snake mySnake) {
	position x;
	x.xvelocity = mySnake.xvelocity;
	x.yvelocity = mySnake.yvelocity;
	x.x = mySnake.myRect.x;
	x.x1 = mySnake.myRect.x + mySnake.myRect.w;
	x.y = mySnake.myRect.y;
	x.y2 = mySnake.myRect.y + mySnake.myRect.h;

	return x;
	
}

Snake addSnake(Snake snake) {
	Snake newSnake;
	newSnake.myRect.x = 
	newSnake.yvelocity = snake.yvelocity;
	if (snake.xvelocity == 1) {
		newSnake.myRect.x = (snake.myRect.x - snake.myRect.w);
		newSnake.myRect.y = snake.myRect.y;
		newSnake.xvelocity = 1;
	}
	//Left
	else if (snake.xvelocity == -1) {
		newSnake.myRect.x = (snake.myRect.x + snake.myRect.w);
		newSnake.myRect.y = snake.myRect.y;
		newSnake.xvelocity = -1;
	}
	//Up
	else if (snake.yvelocity == -1) {
		newSnake.myRect.x = (snake.myRect.x);
		newSnake.myRect.y = snake.myRect.y + snake.myRect.h;
		newSnake.yvelocity = -1;
	}
	//Down
	else if (snake.yvelocity == 1) {
		newSnake.myRect.x = (snake.myRect.x);
		newSnake.myRect.y = snake.myRect.y - snake.myRect.h;
		newSnake.yvelocity = 1;
	}
	return newSnake;
}
void updateAll(vector<Snake> &snakes, Snake snake, deque<position> &coords) {
	if (coords.size() != 0) {
		//x and i dont update with the delete
		//if we delete at 1 and i = 1 we miss 2!!!!
		  for (int i = 0; i < snakes.size(); ++i) {
			for (int x = 0; x < coords.size(); ++x) {
				if (snakes.at(i).myRect.x != coords.at(x).x) continue;
				if ((snakes.at(i).myRect.x + snakes.at(i).myRect.w) != coords.at(x).x1)continue;
				if (snakes.at(i).myRect.y != coords.at(x).y) continue;
				if ((snakes.at(i).myRect.y + snakes.at(i).myRect.h) > coords.at(x).y2)continue;

				
				snakes.at(i).xvelocity = coords.at(x).xvelocity;
				snakes.at(i).yvelocity = coords.at(x).yvelocity;
				if (i == snakes.size() - 1 && x ==0) {
					cout << "Erasing coordinate\n";
					coords.erase(coords.begin() + x);
				}
			}
		  }
	}
}

bool detectCollision(SDL_Rect a, SDL_Rect b) {
	int aLeft = a.x;
	int aRight = a.x + a.w;
	int aTop = a.y;
	int aBot = a.y + a.h;

	int bLeft = b.x;
	int bRight = b.x + b.w;
	int bTop = b.y;
	int bBot = b.y + b.h;

	if (aBot < bTop) {
		return false;
	}
	if (aTop > bBot) {
		return false;
	}
	if (aRight < bLeft) {
		return false;
	}
	if (aLeft > bRight) {
		return false;
	}

	return true;

}
int main(int argc, char *argv[])
{

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to initialize SDL!";
		return 1;
	};

	SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	deque<position> coords;


	Snake snake;
	Candy candy1;
	bool quit = false;

	vector<Snake> snakes;

	SDL_Event e;
	int score = 0;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {

			switch (e.type) {
			case SDL_QUIT:
				quit = true;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_w) {
					if (snake.yvelocity == 1)break;
					if (snake.yvelocity == -1) break;

					if (snakes.size() > 0) {
						snake.yvelocity = -1;
						snake.xvelocity = 0;
						position x = getPosition(snake);
						
						coords.push_back(x);
					}

					snake.yvelocity = -1;
					snake.xvelocity = 0;
					
				}
				if (e.key.keysym.sym == SDLK_s) {
					if (snake.yvelocity == -1) break;
					if (snake.yvelocity == 1) break;

					if (snakes.size() > 0) {
						snake.yvelocity = 1;
						snake.xvelocity = 0;
						position x = getPosition(snake);
						coords.push_back(x);
					}

					snake.yvelocity = 1;
					snake.xvelocity = 0;

				}
				if (e.key.keysym.sym == SDLK_a) {
					if (snake.xvelocity == 1) break;
					if (snake.xvelocity == -1) break;

					if (snakes.size() > 0) {
						snake.xvelocity = -1;
						snake.yvelocity = 0;
						position x = getPosition(snake);
						coords.push_back(x);
					}

					snake.xvelocity = -1;
					snake.yvelocity = 0;
					
				}
				if (e.key.keysym.sym == SDLK_d) {
					if (snake.xvelocity == -1) break;
					if (snake.xvelocity == 1) break;

					if (snakes.size() > 0) {
						snake.xvelocity = 1;
						snake.yvelocity = 0;
						position x = getPosition(snake);
						coords.push_back(x);
					}

					snake.xvelocity = 1;
					snake.yvelocity = 0;
				}
				if (e.key.keysym.sym == SDLK_g) {
					candy1.genPos();
				}
			}

		}


		snake.updatePosition(); 
		if (snake.checkBoundary()) {
			snake.xvelocity = 0;
			snake.yvelocity = 0;
			for (int i = 0; i < snakes.size(); ++i) {
				snakes.at(i).xvelocity = 0;
				snakes.at(i).yvelocity = 0;
			}
		};
		
		if (detectCollision(snake.myRect, candy1.rect)) {
			score += 10;
			cout << score << " Points!\n";
			candy1.genPos();
			Snake *temp = new Snake;
			if (snakes.size() > 0) {
				temp = &addSnake(snakes.at(snakes.size()-1));
			}
			else {
				temp = &addSnake(snake);
			}
			snakes.push_back(*temp);
		}
		for (int i = 0; i < snakes.size(); ++i) {
			snakes.at(i).updatePosition();
		}
		updateAll(snakes, snake, coords);

		
		
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &candy1.rect);

		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
		SDL_RenderFillRect(renderer, &snake.myRect);

		//Draw all snake parts
		for (Snake x : snakes) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
			SDL_RenderFillRect(renderer, &x.myRect);
		}

		
		SDL_RenderPresent(renderer);

		SDL_Delay(10);

	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	for (Snake x : snakes) {
		delete &x;
	}



	return 0;
}

