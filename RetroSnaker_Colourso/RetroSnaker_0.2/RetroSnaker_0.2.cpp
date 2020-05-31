#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

//蛇头方向预定义
#define UP 1	//↑
#define RIGHT 2 //→
#define DOWN 3  //↓
#define LEFT 4  //←

#define MAX_SPEED 5		//最大速度
#define MIN_SPEED 29	//最小速度
#define ORG_SPEED 18	//原始速度

/*-------------结构体定义---------------*/

typedef struct Node //蛇的节点
{
	int x;			   //横左边
	int y;			   //纵坐标
	struct Node *next; //指向下一个节点的指针
	struct Node *pre;  //指向前一个节点的指针
} *LinkNode;

struct Snake //蛇的结构体
{
	LinkNode head; //指向头节点的指针
	LinkNode tail; //指向尾节点的指针
	int direction; //蛇头方向
	int num;	   //节点数目
};

struct Food //食物结构体
{
	int x;		//横坐标
	int y;		//纵坐标
	bool exist; //是否存在，1表示存在
};

/*-------------全局变量---------------*/

int g_score = 0; //获得分数
int g_speed = ORG_SPEED; //控制速度，即延迟等待时间
Snake snake;	 //游戏中的蛇本体
Food food;		 //食物

/*-------------函数声明---------------*/

void init();	 //初始化
void close();	 //释放资源
void play();	 //开始游戏

void initdata(); //游戏数据初始化
void drawgame(); //绘制界面
void gameplay(); //游戏流程控制


		/*---游戏区相关函数---*/

void drawfood(Food food);		  //绘制食物
void drawsnake(Snake snake);	  //绘制蛇
void drawsnakenode(Node node);	  //绘制蛇的结点
void drawsnakehead(Node node);  //画蛇的头部
void clearsnakenode(Node node);   //清除蛇的结点
void snakebodymove(Snake &snake); //蛇身体结点向前复制
bool collidesnake(Snake snake);   //碰撞自身检测

		/*---数据展示区相关函数---*/

void cleardisplay();			  //清除数据展示区内容
void drawscore();				  //绘制分数
void drawnodes();				  //绘制蛇长度
void drawspeed();				  //绘制速度

		/*---程序状态区相关函数---*/

void clearstate();				//清除程序状态区内容
void drawpause();				//输出暂停信息
void drawruntime();				//输出正在运行的信息
void drawover();				//输出游戏结束信息
void drawoverchoose();			//绘制游戏结束后的选项


/*-------------程序入口---------------*/

int main()
{
	init();
	play();
	close();
	return 0;
}

/*-------------函数实现---------------*/

//初始化
void init()
{
	initgraph(640, 480);   //初始化窗口
}

//游戏
void play()
{
	initdata();
	drawgame();
	gameplay();
}

//释放资源
void close()
{
	_getch();
	closegraph();
}

//初始化数据
void initdata()
{
	//蛇与食物初始化
	//食物初始坐标为(310,230)
	food.x = 310;
	food.y = 230;
	food.exist = true;

	//蛇初始长度为3，蛇头坐标为(210,230)

	//如果蛇的头节点和尾节点有信息
	if (snake.head != NULL && snake.tail != NULL)
	{
		LinkNode linknode = snake.head;
		while (linknode != snake.tail)
		{
			linknode = linknode->next;
			free(linknode->pre);
		}
		free(snake.tail);
		snake.head = NULL;
		snake.tail = NULL;
	}


	//在堆上开辟空间，防止此函数的生命周期结束后结点全部被释放掉
	LinkNode nodes2 = (LinkNode)malloc(sizeof(Node));
	nodes2->x = 170;
	nodes2->y = 230;
	nodes2->next = nullptr;
	LinkNode nodes1 = (LinkNode)malloc(sizeof(Node));
	nodes1->x = 190;
	nodes1->y = 230;
	nodes1->next = nodes2;
	LinkNode nodes0 = (LinkNode)malloc(sizeof(Node));
	nodes0->x = 210;
	nodes0->y = 230;
	nodes0->next = nodes1;
	nodes2->pre = nodes1;
	nodes1->pre = nodes0;
	nodes0->pre = nullptr;

	snake.head = nodes0;
	snake.tail = nodes2;
	snake.direction = RIGHT;
	snake.num = 3;

	//初始分数
	g_score = (snake.num - 3) * 10;
	//初始化速度
	g_speed = ORG_SPEED;
}

//绘制界面
void drawgame()
{
	//清空屏幕
	setbkcolor(0);//设置背景色黑色
	cleardevice();

	//画宽度为2的棕色实线，分割游戏区
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 0, 482, 480);

	/*--------------绘制功能介绍区----------*/
	settextstyle(16, 0, L"宋体");
	settextcolor(0XFFFFFF);
	outtextxy(530, 20, L"玩法介绍");
	settextstyle(14, 0, L"宋体");
	outtextxy(490, 50, L"↑ ← ↓ → 控制方向");
	outtextxy(490, 70, L"w a s d 控制方向");
	outtextxy(490, 90, L"速度等级1-25，默认12");
	outtextxy(500, 110, L"c键加速，x键减速");
	outtextxy(500, 130, L"z键恢复原始速度");
	outtextxy(500, 150, L"空格键暂停/继续");
	outtextxy(500, 170, L"Esc键直接关闭程序");

	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 190, 640, 190);

	/*--------------绘制数据展示区----------*/
	settextstyle(16, 0, L"宋体");
	settextcolor(0XFFFFFF);
	outtextxy(530, 200, L"数据展示");

	//绘制分数
	drawscore();

	//绘制蛇身长度
	drawnodes();

	//绘制速度
	drawspeed();

	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 300, 640, 300);

	/*--------------绘制程序状态区----------*/
	settextstyle(16, 0, L"宋体");
	settextcolor(0XFFFFFF);
	outtextxy(530, 310, L"游戏状态");

	//绘制程序正在运行中
	drawruntime();

	settextstyle(14, 0, L"宋体");
	settextcolor(0XFFFFFF);
	outtextxy(530, 450, L"By Colourso");//作者信息
	outtextxy(510, 465, L"www.colourso.top");

	/*--------------游戏区----------*/
	//绘制食物
	drawfood(food);

	//绘制蛇
	drawsnake(snake);
}

//开始游戏
void gameplay()
{
	Node ends; //末尾结点
	bool speedchange = false;
	BeginBatchDraw();
	while (true)
	{
		ends.x = snake.tail->x;
		ends.y = snake.tail->y;

		if (!food.exist)
		{
			int x;
			int y;
			while (1)
			{
				srand(time(0));
				x = (rand() % 24) * 20 + 10;
				y = (rand() % 24) * 20 + 10;

				//食物位置检测算法
				LinkNode linknode = snake.head;
				bool cont = true;
				while (linknode != snake.tail->next)
				{
					if (linknode->x == x && linknode->y == y)
					{
						cont = false;
						break;
					}
					linknode = linknode->next;
				}
				if (cont)
				{
					break;
				}
			}

			food.x = x;
			food.y = y;
			food.exist = true;
			drawfood(food);
		}

		//按键检测
		if (_kbhit())
		{
			char key = _getch();
			switch (key)
			{
			case 72:  //↑
			case 119: //w
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = UP;
				break;
			case 80:  //↓
			case 115: //s
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = DOWN;
				break;
			case 75: //←
			case 97: //a
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = LEFT;
				break;
			case 77:  //→
			case 100: //d
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = RIGHT;
				break;
			case 99: //c 加速
				if (g_speed > MAX_SPEED)
				{
					g_speed -= 1;
					speedchange = true;
				}
				break;
			case 120://x 减速
				if (g_speed < MIN_SPEED)
				{
					g_speed += 1;
					speedchange = true;
				}
				break;
			case 122://z 回归原速
				g_speed = ORG_SPEED;
				speedchange = true;
				break;
			case 32: //空格暂停
				drawpause();	//状态区显示
				while (true)
				{
					if (_kbhit())
					{
						char key = _getch();
						if (key == 32)		//按空格继续
						{
							clearstate();
							drawruntime();	//绘制程序正在运行中
							break;
						}
					}
				}
				break;
			case 27://Esc直接退出
				exit(0);
				break;
			}
		}

		//速度变化的话绘制数据展示区内容
		if (speedchange)
		{
			speedchange = false;
			cleardisplay();
			drawscore();
			drawnodes();
			drawspeed();
		}

		//消除走过的尾端结点
		clearsnakenode(ends);

		//前进，绘制头部走过的节点
		switch (snake.direction)
		{
		case UP:
		{
			//头部y-20,x不变
			snakebodymove(snake);
			snake.head->y -= 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));//将原来的头部变成身体的颜色
			break;
		}
		case RIGHT:
		{
			//头部x+20,y不变
			snakebodymove(snake);
			snake.head->x += 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));
			break;
		}
		case DOWN:
		{
			//头部y+20,x不变
			snakebodymove(snake);
			snake.head->y += 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));
			break;
		}
		case LEFT:
		{
			//头部x-20,y不变
			snakebodymove(snake);
			snake.head->x -= 20;
			drawsnakehead(*(snake.head));
			drawsnakenode(*(snake.head->next));
			break;
		}
		}

		//蛇吃食物
		if (snake.head->x == food.x && snake.head->y == food.y && food.exist)
		{
			food.exist = false;

			//分数变化
			g_score += 10;

			//在末尾增加新的结点，维护双向链表的关系
			LinkNode nodes = (LinkNode)malloc(sizeof(Node));
			nodes->x = ends.x;
			nodes->y = ends.y;
			nodes->next = snake.tail->next;
			nodes->pre = snake.tail;
			snake.tail->next = nodes;
			snake.tail = nodes;
			++snake.num;

			drawsnakenode(ends);

			cleardisplay();
			drawscore();
			drawnodes();
			drawspeed();
		}


		FlushBatchDraw();

		//碰撞判断
		bool collidewall = snake.head->x < 10 || snake.head->x > 470 || snake.head->y < 10 || snake.head->y > 470;
		if (collidewall || collidesnake(snake))
		{
			drawover();
			drawoverchoose();
			while (true)
			{
				if (_kbhit())
				{
					char key = _getch();
					if (key == 114)		//按r重新开始
					{
						Sleep(1000);	//延迟1秒
						play();
					}
					else if (key == 113)//按q退出
					{
						cleardevice();
						TCHAR s[] = _T("Game Over");
						settextstyle(48, 0, _T("黑体"));
						outtextxy(230, 210, s);

						settextstyle(24, 0, _T("宋体"));
						outtextxy(250, 260, L"获得分数：");

						settextcolor(0xFF5555);//亮蓝
						TCHAR t[5];
						_stprintf_s(t, _T("%d"), g_score); // 高版本 VC 推荐使用 _stprintf_s 函数
						outtextxy(380, 260, t);
						settextcolor(0xFFFFFF);//白

						Sleep(1500);
						exit(0);
					}
				}
			}
			break;
		}
		Sleep(g_speed * 10);
		EndBatchDraw();
	}
}

//绘制食物
void drawfood(Food food)
{
	//红色，全填充，无边框的圆
	setfillcolor(RED);
	setfillstyle(BS_SOLID);
	solidcircle(food.x, food.y, 8);
}

//绘制蛇的结点
void drawsnakenode(Node node)
{
	//绿色，全填充，无边框的正方形
	setfillcolor(GREEN);
	setfillstyle(BS_SOLID);
	solidrectangle(node.x - 9, node.y + 9, node.x + 9, node.y - 9);
}

//画蛇的头部
void drawsnakehead(Node node)
{
	//紫色，全填充，无边框的正方形
	setfillcolor(0xAA00AA);
	setfillstyle(BS_SOLID);
	solidrectangle(node.x - 9, node.y + 9, node.x + 9, node.y - 9);
}

//清除蛇的结点
void clearsnakenode(Node node)
{
	//黑色，全填充，无边框的正方形
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(node.x - 9, node.y + 9, node.x + 9, node.y - 9);
}

//绘制蛇
void drawsnake(Snake snake)
{
	LinkNode linknode = snake.head;
	drawsnakehead(*linknode);//画头部
	linknode = linknode->next;
	while (linknode != snake.tail->next)
	{
		drawsnakenode(*linknode);
		linknode = linknode->next;
	}
}

//蛇身体结点向前复制
void snakebodymove(Snake &snake)
{
	LinkNode linknode = snake.tail;
	while (linknode != snake.head)
	{
		linknode->x = linknode->pre->x;
		linknode->y = linknode->pre->y;

		linknode = linknode->pre;
	}
}

//碰撞自身检测
bool collidesnake(Snake snake)
{
	if (snake.num <= 3)
		return false;
	LinkNode node = snake.head->next;
	while (node != snake.tail->next)
	{
		if (node->x == snake.head->x && node->y == snake.head->y)
			return true;
		node = node->next;
	}
	return false;
}

//清除功能展示区
void cleardisplay()
{
	//黑色，全填充，无边框的正方形
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(485, 220, 640, 290);
}

//绘制分数
void drawscore()
{
	TCHAR s[] = _T("获得分数：");
	settextstyle(16, 0, _T("宋体"));
	outtextxy(500, 230, s);

	settextcolor(0xFF5555);//亮蓝
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), g_score); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(580, 230, t);
	settextcolor(0xFFFFFF);//白
}

//绘制蛇长度
void drawnodes()
{
	settextstyle(16, 0, L"宋体");
	outtextxy(500, 250, L"蛇身长度：");

	settextcolor(0xFF55FF);//亮紫
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), snake.num); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(580, 250, t);
	settextcolor(0xFFFFFF);//白
}

//绘制速度
void drawspeed()
{
	TCHAR s[] = _T("当前速度：");
	settextstyle(16, 0, _T("宋体"));
	outtextxy(500, 270, s);

	int speed = 30 - g_speed;//速度等级显示为1 - 25
	if (speed <= 9)
	{
		settextcolor(0x0000AA);//红-
	}
	else if (speed >= 18)
	{
		settextcolor(0x00AA00);//绿
	}
	else
	{
		settextcolor(0x55FFFF);//黄
	}
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), speed); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(580, 270, t);
	settextcolor(0xFFFFFF);//白
}

//清除程序状态区内容
void clearstate()
{
	//黑色，全填充，无边框的正方形
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(485, 330, 640, 450);
}

//绘制程序暂停状态
void drawpause()
{
	settextcolor(0xFF55FF);//亮紫
	settextstyle(16, 0, L"宋体");
	outtextxy(530, 340, L"游戏暂停");
	outtextxy(500, 360, L"按空格键继续");
	settextcolor(0xFFFFFF);//白
}

//绘制程序正在运行中
void drawruntime()
{
	settextcolor(0x55FF55);//亮绿
	settextstyle(16, 0, L"宋体");
	outtextxy(530, 340, L"畅玩ing");
	settextcolor(0xFFFFFF);//白
}

void drawover()
{
	settextcolor(0x5555FF);//亮红
	settextstyle(16, 0, L"宋体");
	outtextxy(530, 340, L"GameOver");
	settextcolor(0xFFFFFF);//白
}

void drawoverchoose()
{
	setlinecolor(0xFFFFFF);
	rectangle(530, 370, 600, 400);
	settextcolor(0x55FF55);//亮绿
	settextstyle(16, 0, L"宋体");
	outtextxy(540, 377, L"重来(r)");
	settextcolor(0xFFFFFF);//白

	setlinecolor(0xFFFFFF);
	rectangle(530, 420, 600, 450);
	settextcolor(0x5555FF);//亮绿
	settextstyle(16, 0, L"宋体");
	outtextxy(540, 427, L"退出(q)");
	settextcolor(0xFFFFFF);//白
}