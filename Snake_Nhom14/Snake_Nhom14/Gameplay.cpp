#include "Gameplay.h"
#include "Graphic.h"
#include "GUI.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>

using namespace std;

bool IsValid(int x, int y)
{
	for (int i = 0; i < snakeSize; i++)
	{
		if (snake_pos[i].x == x && snake_pos[i].y == y)
		{
			return false;
		}
	}
	return true;
}

bool IsWallValid(int x, int y)
{
	for (int i = 0; i < wall_size; i++)
	{
		if (wall_pos[i].x == x && wall_pos[i].y == y)
		{
			return false;
		}
	}
	return true;
}

void GenerateFood()
{
	int x, y;
	do
	{
		x = rand() % (game_field_width - 2) + game_field_pos.x + 1;
		y = rand() % (game_field_height - 2) + game_field_pos.y + 1;
	} while (!IsValid(x, y) || !IsWallValid(x, y));
	food_pos.x = x;
	food_pos.y = y;
	if (current_last_text >= textSize - 1)
	{
		food_text = snake_default_text[0];
	}
	else
	{
		food_text = snake_default_text[current_last_text + 1];
	}
}

void TestFoodSpawn()
{
	while (true)
	{
		if (_kbhit())
		{
			int temp = _getch();
			if (temp)
				GenerateFood();
		}
		DrawPixel(food_pos, 10, 7, 'X');
		Sleep(1000 / 60);
		DrawPixel(food_pos, DEFAULT_BACKGROUND_COLOR);
	}
}

void ResetData()
{
	snakeSize = 2;
	snake_pos[0] = {3, 2};
	snake_pos[1] = {2, 2};
	snake_text[0] = snake_default_text[0];
	snake_text[1] = snake_default_text[1];
	current_last_text = 1;
	snake_dir = Direction::STOP;
	snake_state = State::ALIVE;
	snake_speed = 1;
	score = 0;
	game_time = 0;
	wall_size = 0;
	gate_state = 0;
	level = 1;
}

void MoveRight()
{
	for (int i = snakeSize - 1; i > 0; i--)
	{
		snake_pos[i] = snake_pos[i - 1];
	}
	snake_pos[0].x++;
}

void MoveLeft()
{
	for (int i = snakeSize - 1; i > 0; i--)
	{
		snake_pos[i] = snake_pos[i - 1];
	}
	snake_pos[0].x--;
}

void MoveUp()
{
	for (int i = snakeSize - 1; i > 0; i--)
	{
		snake_pos[i] = snake_pos[i - 1];
	}
	snake_pos[0].y--;
}
void MoveDown()
{
	for (int i = snakeSize - 1; i > 0; i--)
	{
		snake_pos[i] = snake_pos[i - 1];
	}
	snake_pos[0].y++;
}

void TestSnakeMove()
{
	float timer = 1;
	int h = 0, min = 0, sec = 0;
	ResetData();
	GameplayUI();
	GenerateFood();
	DrawPixel(food_pos, food_color, 15, food_text);
	while (1)
	{

		DrawPixels(snake_pos, snakeSize, snake_color, 15, snake_text);
		DrawPixels(wall_pos, wall_size, wall_color);
		if (gate_state == 1)
		{
			DrawGate(gate_pos, gate_color, gate_dir);
		}
		GoToXYPixel(46, 2);
		SetColor(0, 15);
		cout << "SCORE: " << score;
		GoToXYPixel(46, 4);
		SetColor(0, 15);
		cout << "TIME: " << game_time << " => " << h << ":" << min << ":" << sec << "  ";
		if (snake_state != State::DEAD)
		{

			GameInput();
			if (snake_dir != Direction::STOP)
			{
				t2 = time(0);
				game_time += t2 - t1;
				h = game_time / 3600;
				min = game_time / 60 - 60 * h;
				sec = game_time - h * 3600 - min * 60;
				t1 = time(0);
				timer += 2 / fps;
				if (timer >= 1 / snake_speed)
				{
					timer = 0;
					if (snake_pos[0].x == gate_pos.x && snake_pos[0].y == gate_pos.y && gate_state == 1)
					{
						ToTheNextLevel();
					}
					ProcessDead();
					Move();
					Eat();
				}
			}
			else
			{
				timer = 1;
				t1 = time(0);
			}
		}
		else
		{
			GoToXYPixel(20, 20);
			cout << "DEAD";
			GoToXYPixel(20, 21);
			cout << "Press any key to restart";
			if (_kbhit())
			{
				int temp = _getch();
				if (temp == 27)
				{
					break;
				}
				else
				{
					ResetData();
					h = 0;
					min = 0;
					sec = 0;
					GameplayUI();
					GenerateFood();
					DrawPixel(food_pos, food_color);
				}
			}
		}
		Sleep(1000 / fps);
	}
}

void GameInput()
{
	if (_kbhit())
	{
		int temp = _getch();
		if (temp == 27)
		{
			snake_dir = Direction::STOP;
			return;
		}
		switch (toupper(temp))
		{
		case 'A':
			if (snake_dir != Direction::RIGHT)
				snake_dir = Direction::LEFT;
			break;

		case 'D':
			if (snake_dir != Direction::LEFT)
				snake_dir = Direction::RIGHT;
			break;

		case 'W':
			if (snake_dir != Direction::DOWN)
				snake_dir = Direction::UP;
			break;

		case 'S':
			if (snake_dir != Direction::UP)
				snake_dir = Direction::DOWN;
			break;
		}
	}
}

void Move()
{
	last_pos = snake_pos[snakeSize - 1];
	DrawPixel(last_pos, game_field_color);
	switch (snake_dir)
	{
	case Direction::LEFT:
		MoveLeft();
		break;

	case Direction::RIGHT:
		MoveRight();
		break;

	case Direction::UP:
		MoveUp();
		break;
	case Direction::DOWN:
		MoveDown();
		break;
	}
}

bool IsHitTheBorder()
{
	switch (snake_dir)
	{
	case Direction::UP:
		if (snake_pos[0].y == 2)
		{
			return true;
		}
		break;

	case Direction::DOWN:
		if (snake_pos[0].y == 42)
		{
			return true;
		}
		break;
	case Direction::LEFT:
		if (snake_pos[0].x == 2)
		{
			return true;
		}
		break;
	case Direction::RIGHT:
		if (snake_pos[0].x == 42)
		{
			return true;
		}
		break;
	}
	return false;
}

bool IsHitYourself()
{
	for (int i = 1; i < snakeSize; i++)
	{
		if (snake_pos[0].x == snake_pos[i].x && snake_pos[0].y == snake_pos[i].y)
		{
			return true;
		}
	}
	return false;
}

bool IsHitTheWall()
{
	for (int i = 0; i < wall_size; i++)
	{
		switch (snake_dir)
		{
		case Direction::UP:
			if (snake_pos[0].y == wall_pos[i].y + 1 && snake_pos[0].x == wall_pos[i].x)
			{
				return true;
			}
			break;
		case Direction::DOWN:
			if (snake_pos[0].y == wall_pos[i].y - 1 && snake_pos[0].x == wall_pos[i].x)
			{
				return true;
			}
			break;
		case Direction::LEFT:
			if (snake_pos[0].x == wall_pos[i].x + 1 && snake_pos[0].y == wall_pos[i].y)
			{
				return true;
			}
			break;
		case Direction::RIGHT:
			if (snake_pos[0].x == wall_pos[i].x - 1 && snake_pos[0].y == wall_pos[i].y)
			{
				return true;
			}
			break;
		}
	}
	return false;
}

void ProcessDead()
{
	if (IsHitTheBorder() || IsHitYourself() || IsHitTheWall() || IsHitTheGate())
	{
		snake_state = State::DEAD;
		snake_dir = Direction::STOP;
	}
}

void Eat()
{
	if (snake_pos[0].x == food_pos.x && snake_pos[0].y == food_pos.y)
	{
		score += level * 10;
		snakeSize++;
		if (snakeSize % 4 == 0)
		{
			snake_speed += 1;
		}
		snake_pos[snakeSize - 1] = last_pos;
		current_last_text++;
		if (current_last_text >= textSize)
		{
			current_last_text = 0;
		}
		snake_text[snakeSize - 1] = snake_default_text[current_last_text];
		if (snakeSize == 35)
		{
			gate_state = 1;
			GenerateGate();
		}
		SpawnFood();
	}
}

void SpawnFood()
{
	DrawPixel(food_pos, game_field_color);
	GenerateFood();
	DrawPixel(food_pos, food_color, 15, food_text);
}

bool IsAlreadyHad(int x, int y)
{
	for (int i = 0; i < wall_size; i++)
	{
		if (wall_pos[i].x == x && wall_pos[i].y == y)
		{
			return true;
		}
	}
	return false;
}

void GenerateWall()
{
	int x, y;
	do
	{
		x = rand() % (game_field_width - 2) + game_field_pos.x + 1;
		y = rand() % (game_field_height - 2) + game_field_pos.y + 1;
	} while (!IsValid(x, y) || IsAlreadyHad(x, y));
	wall_size++;
	wall_pos[wall_size - 1] = {x, y};
}

void SetGateCollider()
{
	switch (gate_dir)
	{
	case 0:
		gate_colliders[0] = {gate_pos.x - 1, gate_pos.y};
		gate_colliders[1] = {gate_pos.x + 1, gate_pos.y};
		gate_colliders[2] = {gate_pos.x - 1, gate_pos.y + 1};
		gate_colliders[3] = {gate_pos.x, gate_pos.y + 1};
		gate_colliders[4] = {gate_pos.x + 1, gate_pos.y + 1};
		break;

	case 1:
		gate_colliders[0] = {gate_pos.x - 1, gate_pos.y - 1};
		gate_colliders[1] = {gate_pos.x, gate_pos.y - 1};
		gate_colliders[2] = {gate_pos.x + 1, gate_pos.y - 1};
		gate_colliders[3] = {gate_pos.x - 1, gate_pos.y};
		gate_colliders[4] = {gate_pos.x + 1, gate_pos.y};
		break;

	case 2:
		gate_colliders[0] = {gate_pos.x, gate_pos.y - 1};
		gate_colliders[1] = {gate_pos.x + 1, gate_pos.y - 1};
		gate_colliders[2] = {gate_pos.x + 1, gate_pos.y};
		gate_colliders[3] = {gate_pos.x, gate_pos.y + 1};
		gate_colliders[4] = {gate_pos.x + 1, gate_pos.y + 1};
		break;

	case 3:
		gate_colliders[0] = {gate_pos.x - 1, gate_pos.y - 1};
		gate_colliders[1] = {gate_pos.x, gate_pos.y - 1};
		gate_colliders[2] = {gate_pos.x - 1, gate_pos.y};
		gate_colliders[3] = {gate_pos.x - 1, gate_pos.y + 1};
		gate_colliders[4] = {gate_pos.x, gate_pos.y + 1};
		break;
	}
}

bool IsHitTheGate()
{
	if (gate_state == 0)
	{
		return false;
	}
	for (int i = 0; i < 5; i++)
	{
		switch (snake_dir)
		{
		case Direction::UP:
			if (snake_pos[0].y == gate_colliders[i].y + 1 && snake_pos[0].x == gate_colliders[i].x)
			{
				return true;
			}
			break;
		case Direction::DOWN:
			if (snake_pos[0].y == gate_colliders[i].y - 1 && snake_pos[0].x == gate_colliders[i].x)
			{
				return true;
			}
			break;
		case Direction::LEFT:
			if (snake_pos[0].x == gate_colliders[i].x + 1 && snake_pos[0].y == gate_colliders[i].y)
			{
				return true;
			}
			break;
		case Direction::RIGHT:
			if (snake_pos[0].x == gate_colliders[i].x - 1 && snake_pos[0].y == gate_colliders[i].y)
			{
				return true;
			}
			break;
		}
	}
	return false;
}

void GenerateGate()
{
	int x, y;
	do
	{
		x = rand() % (game_field_width - 8) + game_field_pos.x + 1;
		y = rand() % (game_field_height - 8) + game_field_pos.y + 1;
	} while (!IsValid(x, y) || !IsWallValid);
	gate_pos = {x, y};
	gate_dir = rand() % 4;
	SetGateCollider();
}

void ToTheNextLevel()
{
	GameplayUI();
	gate_state = 0;
	level += 1;
	snakeSize = 2;
	current_last_text = 1;
	snake_dir = Direction::STOP;
	snake_pos[1] = gate_pos;
	switch (gate_dir)
	{
	case 0:
		snake_pos[0].x = gate_pos.x;
		snake_pos[0].y = gate_pos.y - 1;
		break;

	case 1:
		snake_pos[0].x = gate_pos.x;
		snake_pos[0].y = gate_pos.y + 1;
		break;

	case 2:
		snake_pos[0].x = gate_pos.x - 1;
		snake_pos[0].y = gate_pos.y;
		break;

	case 3:
		snake_pos[0].x = gate_pos.x + 1;
		snake_pos[0].y = gate_pos.y;
		break;
	}
	for (int i = 0; i < level; i++)
	{
		GenerateWall();
	}
	SpawnFood();
}