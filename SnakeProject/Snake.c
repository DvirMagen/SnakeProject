/*
 *Name: Dvir Magen
 *Id: 318971785
 *Snake - At the beginning of the program, a menu appears for the user, after
 *At the beginning of the program, a menu appears for the user, after
 *immediately after that the board will appear with the snake the size of
 *10 vertebrae in the center and the user will have to navigate it to $.
 *The maximum size of the snake is 20 vertebrae. Once the snake reaches 20 vertebrae,
 *the game will end and the user will be presented with a victory message.
 *If during the game the snake reaches a position where there is a wall or eats itself the game will stop and a loss message will be displayed.
 *
 * 
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>


//Constant values

#define ROWS 25
#define COLS 75
#define MIN_SNAKE_SIZE 10
#define MAX_SNAKE_SIZE 20
#define SPACE ' '
#define STAR '*'
#define LADDER '#'
#define FIRST_CHOOSE '1'
#define SEC_CHOOSE '2'
#define EXIT_CHOOSE '0'
#define THIRD_CHOOSE '3'
#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define SNAKE_SQUAD '@'
#define FOOD '$'

typedef struct point
{
	int x;
	int y;

} Point;

void game();
void start_game(int row_size, int col_size, Point** snake, int snake_size);
void printTable(int row_size, int col_size);//This function prints the game board
void printSnake(Point** snake, int snake_size);//This function prints the characters that represent the snake vertebrae and prints them accordingly using the gotoxy_print function
Point snake_Move(Point** snake, int snake_size, Point* newPoint);// This function is responsible for the movement of the snake from cell to cell 
Point** bigger_snake(Point** snake, int* snake_size, Point* newPoint); // This function enlarge snake dynamic array and build a new squad to the snake
Point* createPoint(int x, int y);
Point createFood(int row_size, int col_size, Point** snake, int snake_size);// The function create food ($) and put the food sign in random location
void freeSnake(Point** snake, int snake_size);
void gotoxy(int x, int y);
void gotoxy_print(int x, int y, char ch);//This function tells the cursor to go to a specific location and prints a character at that location
int getKey();



void main()
{
	//Game menu:
	char userChoice;
	game_menu:
		printf("*******************************************************\n");
		printf("* Pick Your Level, to exit press 0 \n");
		printf("* 0 - Exit\n");
		printf("* 1 - Basic Level\n");
		printf("* 2 - Intermidiate Level\n");
		printf("* 3 - Advanced Level\n");
		printf("*******************************************************\n");
		printf("Please enter which level you prefer:\n");
		scanf("%c", &userChoice);
		getchar();
	if ((userChoice != EXIT_CHOOSE) && (userChoice != FIRST_CHOOSE) && (userChoice != SEC_CHOOSE) && (userChoice != THIRD_CHOOSE))
	{
		system("CLS");
		printf("Invalid value\n");
		goto game_menu;
	}
	else if (userChoice == EXIT_CHOOSE)
	{
		exit(1);
	}
	else if (userChoice == FIRST_CHOOSE)
	{
		system("CLS");
		printf("Lets Start!\n");
		game();
		goto game_menu;
		
	}
	else if(userChoice == SEC_CHOOSE)
	{
		system("CLS");
		printf("This level still in process..\n");
		goto game_menu;
	}
	else if (userChoice == THIRD_CHOOSE)
	{
		system("CLS");
		printf("This level still in process..\n");
		goto game_menu;
	}
}

void game()
{
	const int snake_size = MIN_SNAKE_SIZE;
	int i;
	Point** snake = (Point**)malloc(snake_size * sizeof(Point*));
	if(snake == NULL)
	{
		printf("ERROR!!!!!!!\n");
		exit(-1);
	}
	for (i=0; i < snake_size;i++ )
	{
		snake[i] = NULL;
	}
	
	printTable(ROWS, COLS);
	start_game(ROWS, COLS, snake, snake_size);
}

void start_game(const int row_size, const int col_size, Point** snake, int snake_size)
{
	int i;
	char c,c_tmp;
	// the direction is Right untill an arrow hit
	c = RIGHT;
	int x = row_size / 2, y = col_size / 2;
	Point* point = createPoint(x, y);
	snake_Move(snake, snake_size, point);
	Point food_point = createFood(row_size, col_size,snake,snake_size);
	gotoxy_print(food_point.x,food_point.y, FOOD);
	c_tmp = getKey();
	switch (c_tmp)
	{
		case RIGHT:
		case DOWN:
		case LEFT:
		case UP:
			c = c_tmp;
			break;
	}
	
	while (c != ESC)
	{
		// sleeps for half a second before continuing to the next command
		Sleep(500);
		char last_move = c;
		// only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
			c = getKey();  // change direction
		if (c != RIGHT && c != LEFT && c != DOWN && c != UP && c != ESC)
			c = last_move;
		// make a move in current direction if no change
		switch (c)
		{
		case RIGHT:
			y++;
			break;
		case DOWN:
			x++;
			break;
		case LEFT:
			y--;
			break;
		case UP:
			x--;
			break;
		}
		// check if valid point
		if(x == 0 || y == 0 || x == row_size || y == col_size) // snake touch the wall
		{
			goto endgame;
		}
		for(i = 0;i<snake_size-1;i++)
		{
			if(snake[i] != NULL && snake[i]->x == x && snake[i]->y == y)
			{
				goto endgame;
			}
		}
		//after all checking add point to snake
		point = createPoint(x, y);
		if(food_point.x == point->x && food_point.y == point->y)
		{
			snake = bigger_snake(snake, &snake_size, point);
			food_point = createFood(row_size, col_size, snake, snake_size);
			gotoxy_print(food_point.x, food_point.y, FOOD);
		}
		else
		{
			// snake move when snake not eaten
			const Point tail = snake_Move(snake, snake_size, point);
			if (tail.x != -1 && tail.y != -1)
			{
				gotoxy_print(tail.x, tail.y, SPACE);
			}
		}
		gotoxy_print(point->x, point->y, SNAKE_SQUAD);
		gotoxy(26, 81);
		//printSnake(snake, snake_size);
		if(snake_size >=MAX_SNAKE_SIZE)
		{
			goto endgame_win;;
		}
		
	}
endgame:;
	system("CLS");
	printf("############################################\n");
	printf("\t\tGame Over\n");
	printf("############################################\n");
	freeSnake(snake, snake_size);
	return;
endgame_win:;
	system("CLS");
	printf("############################################\n");
	printf("\t\tYou Win!\n");
	printf("############################################\n");
	freeSnake(snake, snake_size);
	return;
}


void printTable(int row_size, int col_size) //This function prints the game board
{
	int i;
	for(i = 0;i<col_size;i++)
	{
		gotoxy_print(0, i, LADDER);
	}
	for (i = 0; i < col_size; i++)
	{
		gotoxy_print(row_size, i, LADDER);
	}
	for (i = 0; i < row_size; i++)
	{
		gotoxy_print(i, 0, LADDER);
	}
	for (i = 0; i <= row_size; i++)
	{
		gotoxy_print(i, col_size, LADDER);
	}
	printf("\n");
}

void printSnake(Point** snake, const int snake_size) //This function prints the characters that represent the snake vertebrae and prints them accordingly using the gotoxy_print function
{
	int i;
	for(i = 0;i<snake_size ;i++)
	{
		Point* p = snake[i];
		if(p != NULL)
		{
			gotoxy_print(p->x, p->y, SNAKE_SQUAD);
		}
	}
}

Point snake_Move(Point** snake, const int snake_size, Point* newPoint) // This function is responsible for the movement of the snake from cell to cell 
{
	int i;
	Point p = {
		.x = -1,
		.y = -1
	};
	if(snake[snake_size-1]!= NULL)
	{
		p.x = snake[snake_size - 1]->x;
		p.y = snake[snake_size - 1]->y;
		free(snake[snake_size - 1]);
		snake[snake_size - 1] = NULL;
	}
	for(i = snake_size - 1; i > 0; i--)
	{
		snake[i] = snake[i - 1];
	}
	snake[0] = newPoint;
	return p;
}

Point** bigger_snake(Point** snake, int* snake_size, Point* newPoint) // This function enlarge snake dynamic array and build a new squad to the snake
{
	if(snake[(*snake_size) -1] != NULL && (*snake_size) < MAX_SNAKE_SIZE)
	{
		const int size = (*snake_size);
		Point** snake_tmp = (Point**)realloc(snake, (size + 1) * sizeof(Point*));
		if (snake_tmp == NULL)
		{
			printf("ERROR!!!!!!!\n");
			exit(-1);
		}
		snake_tmp[size] = NULL;
		snake = snake_tmp;
		(*snake_size)++;
	}
	snake_Move(snake, (*snake_size), newPoint);
	return snake;
}

Point* createPoint(const int x,const int y)
{
	Point* point = (Point*)malloc(sizeof(Point));
	if(point == NULL)
	{
		printf("ERROR!!!!!!!\n");
		exit(-2);
	}
	point->x = x;
	point->y = y;
	return point;
}

Point createFood(const int row_size,const int col_size, Point** snake,const int snake_size) // The function create food ($) and put the food sign in random location
{
	srand((unsigned int)time(NULL));
	int x, y, i;
	bool flag = false;
	do {
		x = 2 + (rand() % (row_size - 2));
		y = 2 + (rand() % (col_size - 2));
		for (i = 0; i < snake_size; i++)
		{
			const Point* p = snake[i];
			if (p != NULL && p->x == x && p->y == y)
			{
				flag = true;
			}
		}
	} while (flag);
	const Point p = {
		.x = x,
		.y = y
	};
	return p;
	
}

void freeSnake(Point** snake, const int snake_size) 
{
	int i;
	if(snake !=NULL)
	{
		for (i = 0; i < snake_size; i++)
		{
			Point* p = snake[i];
			if (p != NULL)
			{
				
				snake[i] = NULL;
				free(p);
			}
		}
		free(snake);
	}
}


// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(const int x, const int y)
{
	printf("\x1b[%d;%df", x, y);
}

void gotoxy_print(const int x, const int y, const char ch)  //This function tells the cursor to go to a specific location and prints a character at that location
{
	gotoxy(x, y);
	printf("%c", ch);
}


int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
