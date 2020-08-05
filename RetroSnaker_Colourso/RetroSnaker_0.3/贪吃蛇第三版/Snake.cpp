#include "Snake.h"
#include <stdio.h>
#include <ctime>
#include <graphics.h>

Snake::Snake()
{
	Point pos0(210, 230);
	Point pos1(190, 230);
	Point pos2(170, 230);

	this->m_snakelist.push_back(pos0);
	this->m_snakelist.push_back(pos1);
	this->m_snakelist.push_back(pos2);

	this->m_direction = Dir::DIR_RIGHT;
	this->m_len = 3;
	this->m_speed = this->OrgSpeed;
}

int Snake::getLen()
{
	return this->m_len;
}

int Snake::getSpeed()
{
	return this->m_speed;
}

Dir Snake::getDirection()
{
	return this->m_direction;
}

bool Snake::setSpeed(int speed)
{
	if (speed > this->MaxSpeed)
	{
		if (this->m_speed != this->MaxSpeed)
		{
			this->m_speed = this->MaxSpeed;
			return true;
		}
		return false;
	}
	else if (speed < this->MinSpeed)
	{
		if (this->m_speed != this->MinSpeed)
		{
			this->m_speed = this->MinSpeed;
			return true;
		}
		return false;
	}
	else
	{
		this->m_speed = speed;
		return true;
	}
}

void Snake::Move()
{
	this->m_tail = this->m_snakelist.back();

	//移除最后一个节点，复制第一个节点两份
	this->m_snakelist.pop_back();
	Point headPos = this->m_snakelist.front();
	this->m_snakelist.push_front(headPos);

	switch (this->m_direction)
	{
	case Dir::DIR_UP:
		this->m_snakelist.begin()->y -= SNAKE_WIDTH;
		break;

	case Dir::DIR_RIGHT:
		this->m_snakelist.begin()->x += SNAKE_WIDTH;
		break;

	case Dir::DIR_DOWN:
		this->m_snakelist.begin()->y += SNAKE_WIDTH;
		break;
	case Dir::DIR_LEFT:
		this->m_snakelist.begin()->x -= SNAKE_WIDTH;
		break;
	}
}

void Snake::EatFood()
{
	//吃到食物尾部增长
	this->m_snakelist.push_back(this->m_tail);

	this->m_len += 1;
}

void Snake::ChangeDir(Dir dir)
{
	switch (dir)
	{
	case Dir::DIR_UP:
	case Dir::DIR_DOWN:
		if (m_direction != Dir::DIR_UP && m_direction != Dir::DIR_DOWN)
		{
			m_direction = dir;
		}
		break;
	case Dir::DIR_RIGHT:
	case Dir::DIR_LEFT:
		if (m_direction != Dir::DIR_RIGHT && m_direction != Dir::DIR_LEFT)
		{
			m_direction = dir;
		}
		break;
	}
}

void Snake::Dead()
{
	//TODO
	int x = 0;
	int y = 0;	//x、y表示坐标的该变量
	int z = 0;	//z表示改变方向

	//使用随机函数
	srand(time(0));

	std::list<Point>::iterator it = this->m_snakelist.begin();
	for (; it != this->m_snakelist.end(); ++it)
	{
		x = (rand() % 4) * SNAKE_WIDTH;
		y = (rand() % 4) * SNAKE_WIDTH;
		z = (rand() % 8);

		switch (z)
		{
		case 0:
			//右
			(*it).x += x;
			break;
		case 1:
			//下
			(*it).y += y;
			break;
		case 2:
			//左
			(*it).x -= x;
			break;

		case 3:
			//上
			(*it).y -= y;
			break;
		case 4:
			//右下
			(*it).x += x;
			(*it).y += y;
			break;
		case 5:
			//左下
			(*it).x -= x;
			(*it).y += y;
			break;
		case 6:
			//左上
			(*it).x -= x;
			(*it).y -= y;
			break;

		case 7:
			//右上
			(*it).x += x;
			(*it).y -= y;
			break;
		}
	}
}

bool Snake::ColideWall(int left, int top, int right, int bottom)
{
	int x = this->m_snakelist.front().x;
	int y = this->m_snakelist.front().y;
	return (x < left || x > right || y < top || y > bottom);
}

bool Snake::ColideSnake()
{

	if (m_len <= 3) return false;
	std::list<Point>::iterator it = this->m_snakelist.begin();

	Point pos = *it;
	Point next_pos;
	it++;

	while(it != this->m_snakelist.end())
	{
		next_pos = *it;

		if (pos == next_pos)
		{
			return true;
		}

		it++;
	}
	return false;
}

bool Snake::ColideFood(Point point)
{
	if (this->m_snakelist.front() == point)
	{
		return true;
	}
	return false;
}

void Snake::DrawSnake()
{
	std::list<Point>::iterator it = this->m_snakelist.begin();
	for (; it != this->m_snakelist.end(); ++it)
	{
		DrawSnakeNode(*it);
	}
	DrawSnakeHead(this->m_snakelist.front());
}

void Snake::DrawSnakeHead(Point pos)
{
	//紫色，全填充，无边框的正方形
	setfillcolor(0xAA00AA);
	setfillstyle(BS_SOLID);
	solidrectangle(pos.x - SNAKE_RADIU, pos.y + SNAKE_RADIU, pos.x + SNAKE_RADIU, pos.y - SNAKE_RADIU);
}

void Snake::DrawSnakeNode(Point pos)
{
	//绿色，全填充，无边框的正方形
	setfillcolor(GREEN);
	setfillstyle(BS_SOLID);
	solidrectangle(pos.x - SNAKE_RADIU, pos.y + SNAKE_RADIU, pos.x + SNAKE_RADIU, pos.y - SNAKE_RADIU);
}

std::list<Point> Snake::GetSnakeAllNode()
{
	return this->m_snakelist;
}
