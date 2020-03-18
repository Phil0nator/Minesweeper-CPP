//#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include <string>
#include <Math.h>
#include <time.h>
#include "resource1.h"

using namespace sf;
using namespace std;

#define MYWINDOW_HEIGHT 500
#define MYWINDOW_WIDTH 500


class Grid {
public:
	//indexes in 2d array
	int xindex;
	int yindex;
	//0 = normal , 1 = bomb
	int state;
	//the visual
	Sprite vis;
	//for collision with mouse
	IntRect rect;
	//number of surrounding bombs
	int surrounding;
	//has it been clicked or flagged
	bool revealed = false;
	//flagged specifically
	bool flagged = false;

	void draw(RenderWindow* win) {
		vis.setPosition(xindex * 16, yindex * 16);

		win->draw(vis);
	}

	void init(Grid grid[22][22]) {
		int i = xindex;
		int j = yindex;

		//if its not a bomb, mark it based on how many bombs are nearby
		if (state != 1) {
			surrounding = 0;
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					if (grid[i - 1 + x][j - 1 + y].state == 1) {
						surrounding++;
					}
				}
			}
			cout << "Block at " << xindex << ":" << yindex << "with   : " << surrounding << endl;

		}
		else {
			surrounding = 9;
		}
	}


	int reveal(Sprite numbers[], Sprite safe, Sprite bomb, Grid grid[22][22]) {
		revealed = true;
		int bean = surrounding;
		cout << "reveal one" << "    \     " << state << "at     : " << xindex << ":" << yindex << "         " << bean << endl;
		if((xindex > 20 || surrounding > 8 || yindex > 20)&&state != 1) {
			
			return 3;
		}


		if (surrounding == 0 && state == 0) {
			vis = safe;
			//floodfill
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {

					if (grid[(xindex - 1) + i][(yindex - 1) + j].state == 0 && yindex > 0 && xindex > 0 && xindex < 20 && yindex < 20 && grid[(xindex - 1) + i][(yindex - 1) + j].revealed == false) {
						
						if ((xindex - 1) + i < 0 || (yindex - 1) + j < 0 || (xindex - 1) + i > 20 || (yindex - 1) + j > 20) {
							
							return 3;
						}
						
						
						grid[(xindex - 1) + i][(yindex - 1) + j].reveal(numbers, safe, bomb, grid);
					}
				}
			}
		}
		else if (surrounding > 0 && state != 1) {
			vis = numbers[surrounding - 1];
		}
		else if (state == 1) {
			vis = bomb;
		}

	}


};


int randint(int min, int max) {
	if (min > 0) {
		return rand() % min + max;
	}
	else {
		return rand() % max;
	}

}


void drawWin(RenderWindow* win) {

	Texture winScreen;
	winScreen.loadFromFile("winScreen.jpg");
	Sprite screen(winScreen, IntRect(0, 0, winScreen.getSize().x, winScreen.getSize().y));
	screen.setScale(1.5, 1);
	win->draw(screen);



}
void lose(RenderWindow* win) {

	Texture loseScreen;
	loseScreen.loadFromFile("gameover.jpg");
	Sprite screen(loseScreen, IntRect(0, 0, loseScreen.getSize().x, loseScreen.getSize().y));
	screen.setScale(.6, 1);
	screen.setPosition(-20, 0);
	win->draw(screen);

}


void handleTimer(RenderWindow* win, Clock *clock, Sprite numbers[]) {
	Time passed = clock->getElapsedTime();
	Sprite digit1;
	Sprite digit2;
	Sprite digit3;
	
	int seconds = passed.asSeconds();
	if (seconds < 10) {
		digit1 = numbers[0];
		digit2 = numbers[0];
		digit3 = numbers[seconds];
	}
	else if (seconds > 10 && seconds < 100) {
		digit1 = numbers[0];
		int tens = (int)floor(seconds / 10);
		digit2 = numbers[tens];
		digit3 = numbers[seconds - (tens * 10)];
	}
	else if (seconds > 100) {
		int hundreds = (int)floor(seconds / 100);
		digit1 = numbers[hundreds];
		int tens = (int)floor((seconds - hundreds)/ 10);
		digit2 = numbers[tens];
		digit3 = numbers[seconds - (tens * 10)];
	}
	
	digit1.setPosition(415, 10);
	digit2.setPosition(430, 10);
	digit3.setPosition(445, 10);
	win->draw(digit1);
	win->draw(digit2);
	win->draw(digit3);

	

}

void drawNumber(RenderWindow* win, int number, Sprite tnums[], int x, int y) {


	int diglist[3];
	for (int i = 0; i < 3; i++) {
		diglist[i] = number%10;
		number = number / 10;
	}
	Sprite digit1 = tnums[diglist[2]];
	Sprite digit2 = tnums[diglist[1]];
	Sprite digit3 = tnums[diglist[0]];
	digit1.setPosition(x, y);
	digit2.setPosition(x + 16, y);
	digit3.setPosition(x + 32, y);


	win->draw(digit1);
	win->draw(digit2);
	win->draw(digit3);


}



int main()
{



	int state = 0;
	bool showed = false;
	srand(time(NULL));
	
	//texture loading
	Texture sheet;
	sheet.loadFromFile("sheet.png");
	Sprite basic(sheet, IntRect(2,53,16,16));
	
	
	Sprite _1(sheet, IntRect(2, 70, 16, 16));
	Sprite _2(sheet, IntRect(19, 70, 16, 16));
	Sprite _3(sheet, IntRect(36, 70, 16, 16));
	Sprite _4(sheet, IntRect(53, 70, 16, 16));
	Sprite _5(sheet, IntRect(70, 70, 16, 16));
	Sprite _6(sheet, IntRect(87, 70, 16, 16));
	Sprite _7(sheet, IntRect(104, 70, 16, 16));
	Sprite _8(sheet, IntRect(121, 70, 16, 16));

	Sprite bomb(sheet, IntRect(87, 53, 16, 16));

	Sprite safe(sheet, IntRect(19, 53, 16, 16));

	Sprite Flag(sheet, IntRect(36, 53, 16, 16));

	Sprite mouseOver(sheet, IntRect(149,53,16,16));

	Sprite numbers[8];
	numbers[0] = _1;
	numbers[1] = _2;
	numbers[2] = _3;
	numbers[3] = _4;
	numbers[4] = _5;
	numbers[5] = _6;
	numbers[6] = _7;
	numbers[7] = _8;


	//timer numbers;
	Sprite t1(sheet, IntRect(2, 2, 13, 23));
	Sprite t2(sheet, IntRect(16, 2, 13, 23));
	Sprite t3(sheet, IntRect(30, 2, 13, 23));
	Sprite t4(sheet, IntRect(44, 2, 13, 23));
	Sprite t5(sheet, IntRect(58, 2, 13, 23));
	Sprite t6(sheet, IntRect(72, 2, 13, 23));
	Sprite t7(sheet, IntRect(86, 2, 13, 23));
	Sprite t8(sheet, IntRect(100, 2, 13, 23));
	Sprite t9(sheet, IntRect(114, 2, 13, 23));
	Sprite t0(sheet, IntRect(128, 2, 13, 23));
	Sprite tnums[11];
	tnums[1] = t1;
	tnums[2] = t2;
	tnums[3] = t3;
	tnums[4] = t4;
	tnums[5] = t5;
	tnums[6] = t6;
	tnums[7] = t7;
	tnums[8] = t8;
	tnums[9] = t9;
	tnums[0] = t0;



	//2d array setup
	Grid grid[22][22];
	int bombcount = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			Grid block;
			block.xindex = i;
			block.yindex = j;
			block.state = randint(0,10);
			
			if (block.state == 1) {
				bombcount++;
			}
			else {
				block.state = 0;
			}
			
			block.vis = basic;
			block.rect = IntRect(i * 16, j * 16, 16, 16);
			grid[i][j] = block;


		}
	}
	//initiate the surround values for the whole grid.
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			grid[i][j].init(grid);
		}
	}



	cout << "BOMBCOUNT = " << bombcount << endl;
	cout << "NOT BOMB COUNT = " << 400 - bombcount << endl;
	int ogbomb = bombcount;
	RenderWindow window(VideoMode(MYWINDOW_WIDTH, MYWINDOW_HEIGHT), "Mine Sweeper -- 1.0");
	Image icon;
	icon.loadFromFile("icon.jpg");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


	printf("START\n");

	
	
	Clock clock;
	int mousex;
	int mousey;
	bool clicked = false;
	int surrounding = 0;
	bool flag = false;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				mousex = event.mouseButton.x;
				mousey = event.mouseButton.y;

				if (Mouse::isButtonPressed(Mouse::Left)) {
					clicked = true;
				}
				else {
					flag = true;
				}
			
			
			}
			else {
				mousex = event.mouseMove.x;
				mousey = event.mouseMove.y;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}


		


		if (flag == true) {
			flag = false;
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {

					if (grid[i][j].rect.intersects(IntRect(mousex, mousey, 2, 2))) {
						grid[i][j].vis = Flag;
						grid[i][j].revealed = true;
						grid[i][j].flagged = true;
						//update bombs left display
						if (grid[i][j].state == 1) {
							bombcount--;
						}

					}

				}

			}
		}





		//handle clicks
		if (clicked == true) {
			clicked = false;
			//go through all blocks
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					//if its clicked
					if (grid[i][j].rect.intersects(IntRect(mousex, mousey, 2, 2))){
						
						//new way
						grid[i][j].reveal(numbers, safe, bomb, grid);

						if (grid[i][j].state == 1) {
							state = 2;
						}


						break;
					}
					
					


					
				}
			}

			//test win;
			int notRevealed = 0;
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					//if its not been revealed yet, or there are wrong flags
					if (grid[i][j].revealed == false || (grid[i][j].flagged == true && grid[i][j].state != 1)) {
						notRevealed++;
						break;
					}
				}
			}
			if (notRevealed == 0) {
				//win
				for (int b = 0; b < 5; b++) {
					cout << endl;
				}

				cout << "YOU WON!" << endl;
				state = 1;


			}

			//prevent corruption
			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					if (grid[i][j].surrounding == 3 && grid[i][j].revealed == true) {
						grid[i][j].init(grid);
						grid[i][j].reveal(numbers, safe, bomb, grid);
					}
				}
			}




		}
		//handle various displays
		if (state == 0) {
			window.clear();

			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					grid[i][j].draw(&window);
				}
			}
			Time elapsed = clock.getElapsedTime();
			
			
			drawNumber(&window, elapsed.asSeconds(), tnums, 415, 10);

			drawNumber(&window, bombcount, tnums, 415, 40);

			window.display();
		}
		else if (state == 1) {
			window.clear();


			drawWin(&window);


			window.display();
		}
		else if (state == 2) {
			window.clear();
			
			//<show>
			bool stop = false;
			
			int found = 0;
			int count = 0;
			Time wait;
			wait = seconds(.5);

			//graphically show all the locations of the bombs before showing the lose screen
			if (showed == false) {
				showed = true;
				for (int a = 0; a < bombcount; a++) {


					//prevent not responding
					Event event;
					while (window.pollEvent(event))
					{
						if (event.type == Event::Closed)
							window.close();
					}



					sleep(wait);
					window.clear();
					stop = false;
					count = 0;
					for (int i = 0; i < 20; i++) {
						for (int j = 0; j < 20; j++) {
							if (grid[i][j].state == 1) {
								count++;
								if (count > found) {
									found++;
									grid[i][j].reveal(numbers, safe, bomb, grid);
									stop = true;
									break;
								}

							}


						}
						if (stop == true) {
							stop = false;
							break;
						}
					}

					for (int i = 0; i < 20; i++) {
						for (int j = 0; j < 20; j++) {
							grid[i][j].draw(&window);
						}
					}
					window.display();

				}


				


			}
			
			//</show>
			lose(&window);

			window.display();
			}
			
	
		}
	system("pause");
	return 0;
}


