#pragma once

#include "common.h"
#include "Snake.h"

class Food
{
private:
	Point m_pos;
	bool m_state;

public:
	Food();

	bool getState();
	void setState(bool state);
	Point getPos();				//获取食物坐标

	void Generate(Snake *snake);//产生新的食物

	void DrawFood();

};

