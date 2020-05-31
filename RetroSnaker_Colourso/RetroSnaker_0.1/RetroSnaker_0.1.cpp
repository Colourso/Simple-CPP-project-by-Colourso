#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

//蛇头方向预定义
#define UP 1	//↑
#define RIGHT 2 //→
#define DOWN 3  //↓
#define LEFT 4  //←

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
Snake snake;	 //游戏中的蛇本体
Food food;		 //食物

/*-------------函数声明---------------*/

void init();	 //初始化
void drawgame(); //绘制界面
void gameplay(); //开始游戏
void close();	//释放资源

void drawfood(Food food);		  //绘制食物
void drawsnake(Snake snake);	  //绘制蛇
void drawsnakenode(Node node);	//绘制蛇的结点
void clearsnakenode(Node node);   //清除蛇的结点
void snakebodymove(Snake &snake); //蛇身体结点向前复制
bool collidesnake(Snake snake);   //碰撞自身检测
void drawscore();				  //绘制分数
void clearscore();				  //清除分数

/*-------------程序入口---------------*/

int main()
{
	init();
	drawgame();
	gameplay();
	close();
	return 0;
}

/*-------------函数实现---------------*/

//初始化
void init()
{
	initgraph(640, 480);   //初始化窗口
	//setorigin(0, 480);	 //设置坐标原点
	//setaspectratio(1, -1); //使y轴向上为正

	//蛇与食物初始化
	//食物初始坐标为(310,230)
	food.x = 310;
	food.y = 230;
	food.exist = true;

	//蛇初始长度为3，蛇头坐标为(210,230)

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
}

//绘制界面
void drawgame()
{
	//画宽度为2的棕色实线，分割区域
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 0, 482, 480);

	//绘制食物
	drawfood(food);

	//绘制蛇
	drawsnake(snake);

	//绘制分数
	drawscore();
}

//开始游戏
void gameplay()
{
	Node ends; //末尾结点
	while (1)
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

		//按键
		if (_kbhit())
		{
			char key = _getch();
			switch (key)
			{
			case 72: //↑
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = UP;
				break;
			case 80: //↓
				if (snake.direction != UP && snake.direction != DOWN)
					snake.direction = DOWN;
				break;
			case 75: //←
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = LEFT;
				break;
			case 77: //→
				if (snake.direction != LEFT && snake.direction != RIGHT)
					snake.direction = RIGHT;
				break;
			}
		}
		//前进
		switch (snake.direction)
		{
		case UP:
		{
			//头部y-20,x不变
			snakebodymove(snake);
			snake.head->y -= 20;
			drawsnakenode(*(snake.head));
			break;
		}
		case RIGHT:
		{
			//头部x+20,y不变
			snakebodymove(snake);
			snake.head->x += 20;
			drawsnakenode(*(snake.head));
			break;
		}
		case DOWN:
		{
			//头部y+20,x不变
			snakebodymove(snake);
			snake.head->y += 20;
			drawsnakenode(*(snake.head));
			break;
		}
		case LEFT:
		{
			//头部x-20,y不变
			snakebodymove(snake);
			snake.head->x -= 20;
			drawsnakenode(*(snake.head));
			break;
		}
		}
		//消除走过的结点
		clearsnakenode(ends);

		//蛇吃食物
		if (snake.head->x == food.x && snake.head->y == food.y && food.exist)
		{
			food.exist = false;
			//isClear = false;

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

			clearscore();
			drawscore();
		}

		//碰撞判断
		bool collidewall = snake.head->x < 10 || snake.head->x > 470 || snake.head->y < 10 || snake.head->y > 470;
		if (collidewall || collidesnake(snake))
		{
			cleardevice();
			TCHAR s[] = _T("game over");
			settextstyle(48, 0, _T("黑体"));
			outtextxy(230, 280, s);
			break;
		}
		Sleep(150);
	}
}

//释放资源
void close()
{
	_getch();
	closegraph();
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

//绘制分数
void drawscore()
{
	TCHAR s[] = _T("获得分数：");
	settextstyle(16, 0, _T("黑体"));
	outtextxy(485, 300, s);

	TCHAR t[5];
	_stprintf_s(t, _T("%d"), g_score); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(560, 300, t);
}

//清除分数
void clearscore()
{
	//黑色，全填充，无边框的正方形
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	solidrectangle(485, 480, 640, 300);
}
