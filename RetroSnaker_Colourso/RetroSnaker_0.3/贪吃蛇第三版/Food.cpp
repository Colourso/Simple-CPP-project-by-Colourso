#include "Food.h"
#include <ctime>
#include <stdlib.h>
#include <graphics.h>
#include <algorithm>

Food::Food()
{
	//初始化食物的数据
	this->m_state = true;
	this->m_pos = Point(310, 230);
}

bool Food::getState()
{
	return this->m_state;
}

void Food::setState(bool state)
{
	this->m_state = state;
}

Point Food::getPos()
{
	return this->m_pos;
}

void Food::Generate(Snake * snake)
{
	//产生食物要求获取蛇的身体节点来检查是否生成的食物出现在了蛇的身上
	int x = 0;
	int y = 0;
	bool isOk = false;

	while (true)
	{
		//使用随机函数产生食物
		srand(time(0));
		x = (rand() % 24) * 20 + 10;
		y = (rand() % 24) * 20 + 10;

		//检查是否在蛇的身上
		isOk = 1;
		
		std::list<Point> pos = snake->GetSnakeAllNode();
		std::list<Point>::iterator it = find(pos.begin(),pos.end(),Point(x,y));
		//find()需要用到Point的重载 == 操作符


		if (it == pos.end())//不在
		{
			this->m_pos = Point(x,y);//修改坐标
			this->m_state = true;
			return;
		}
	}
}

void Food::DrawFood()
{
	//红色，全填充，无边框的圆
	setfillcolor(RED);
	setfillstyle(BS_SOLID);
	solidcircle(this->m_pos.x, this->m_pos.y, FOOD_RADIU);
}
