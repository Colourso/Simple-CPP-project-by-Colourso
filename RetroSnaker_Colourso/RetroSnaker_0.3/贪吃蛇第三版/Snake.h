#pragma once

#include "common.h"
#include <list>

class Snake
{
public:
	const int MinSpeed = 1;			//蛇的最小速度
	const int MaxSpeed = 25;		//蛇的最大速度
	const int OrgSpeed = 15;		//蛇的原始速度

private:
	int m_len;						//蛇的长度
	int m_speed;					//蛇的速度
	Dir m_direction;				//蛇的方向
	std::list<Point> m_snakelist;	//蛇的链表
	Point m_tail;					//蛇移动过后的尾部节点，主要用于吃食物

public:
	Snake();

	int getLen();					//获取长度
	int getSpeed();					//获取速度
	Dir getDirection();				//获取方向
	bool setSpeed(int speed);		//设置速度，设置成功返回true

	void Move();					//移动一节
	void EatFood();					//吃食物
	void ChangeDir(Dir dir);		//改变方向
	void Dead();					//死亡

	bool ColideWall(int left, int top, int right, int bottom);	//碰撞到墙
	bool ColideSnake();										//碰撞到了自身
	bool ColideFood(Point point);							//碰到了食物

	void DrawSnake();				//绘制蛇
	void DrawSnakeHead(Point pos);	//绘制蛇头
	void DrawSnakeNode(Point pos);	//绘制蛇的身体结点

	std::list<Point> GetSnakeAllNode();


};