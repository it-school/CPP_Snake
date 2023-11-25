#include <iostream>
#include <conio.h>
#include <windows.h>
#include "color.h"

using namespace std;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

const int WIDTH = 20;
const int HEIGHT = 20;

bool gameOver;
int headX, headY, fruitX, fruitY, score, speed;
int tailX[WIDTH * HEIGHT], tailY[WIDTH * HEIGHT];
int nTail;
eDirection dir;

static void HideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

static void Setup() {
	gameOver = false;
	dir = STOP;
	headX = WIDTH / 2;
	headY = HEIGHT / 2;
	fruitX = rand() % WIDTH;
	fruitY = rand() % HEIGHT;
	score = 0;
	speed = 100;
}

static void Draw() {
	system("cls");
	for (int i = 0; i <= WIDTH + 1; i++)
		cout << hue::red_on_light_red << "+" << hue::reset;
	cout << endl;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j <= WIDTH; j++) {
			if (j == 0 || j == WIDTH) {
				cout << hue::red_on_light_red << "+" << hue::reset;
			}
			if (i == headY && j == headX) {
				cout << hue::bright_white_on_light_green << "8" << hue::reset;
			}
			else if (i == fruitY && j == fruitX) {
				cout << hue::yellow<< '@' << hue::reset;
			}
			else {
				bool printTail = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						cout << hue::light_green_on_green << "o" << hue::reset;
						printTail = true;
					}
				}
				if (!printTail)
					cout << " ";
			}
		}
		cout << endl;
	}

	for (int i = 0; i <= WIDTH + 1; i++)
		cout << hue::red_on_light_red << "+" << hue::reset;
	cout << endl;
	cout << "Speed:" << 101 - speed << endl;
	cout << "Score:" << score << endl;
}

static void Input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'a': case 'A':
			dir = LEFT;
			break;
		case 'd': case 'D':
			dir = RIGHT;
			break;
		case 'w': case 'W':
			dir = UP;
			break;
		case 's': case 'S':
			dir = DOWN;
			break;
		case 27:
			gameOver = true;
			break;
		}
	}
}

static void Logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = headX;
	tailY[0] = headY;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir) {
	case LEFT:
		headX--;
		break;
	case RIGHT:
		headX++;
		break;
	case UP:
		headY--;
		break;
	case DOWN:
		headY++;
		break;
	default:
		break;
	}
	if (headX >= WIDTH) headX = 0; else if (headX < 0) headX = WIDTH - 1;
	if (headY >= HEIGHT) headY = 0; else if (headY < 0) headY = HEIGHT - 1;

	for (int i = 0; i < nTail; i++)
		if (tailX[i] == headX && tailY[i] == headY)
			gameOver = true;

	if (headX == fruitX && headY == fruitY) {
		score += 10;
		fruitX = rand() % WIDTH;
		fruitY = rand() % HEIGHT;
		nTail++;
		speed--;
	}
}

int main() {
	HideCursor();
	Setup();
	while (!gameOver) {
		Draw();
		Input();
		Logic();
		Sleep(speed);
	}
	return 0;
}