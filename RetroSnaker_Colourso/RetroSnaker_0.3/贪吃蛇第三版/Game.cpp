#include "Game.h"
#include <conio.h>
#include <graphics.h>

Game::Game()
{
	this->m_snake = nullptr;
	this->m_food = nullptr;

	this->m_GameState = 0;
	this->m_msg = PlayerMsg();
	this->m_ranklist = new RankList();
}

void Game::Init()
{
	initgraph(640, 480);
}

void Game::Run()
{
	ShowMainUI();

	while (true)
	{
		if (m_GameState == 0 && MouseHit())	//在主界面点击鼠标
		{
			MOUSEMSG mouse = GetMouseMsg();//获取鼠标点击消息
			if (mouse.mkLButton)
			{
				if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 195 && mouse.y <= 235)
				{
					//如果点击到了开始选项
					ChangeChooseUI(240, 195, 400, 235, 1);
					m_GameState = 1;

					FlushMouseMsgBuffer();//清空鼠标消息缓冲区。

					Sleep(500);
					PlayGame();
				}
				else if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 255 && mouse.y <= 295)
				{
					//排行榜选项
					ChangeChooseUI(240, 255, 400, 295, 2);
					m_GameState = 2;

					FlushMouseMsgBuffer();//清空鼠标消息缓冲区。

					Sleep(500);
					ShowRank();
				}
				else if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 315 && mouse.y <= 355)
				{
					//帮助选项
					ChangeChooseUI(240, 315, 400, 355, 3);
					m_GameState = 3;

					FlushMouseMsgBuffer();//清空鼠标消息缓冲区。

					Sleep(500);
					ShowRule();
				}
				else if (mouse.x >= 240 && mouse.x <= 400 && mouse.y >= 375 && mouse.y <= 415)
				{
					//退出选项
					ChangeChooseUI(240, 375, 400, 415, 4);
					Sleep(1000);
					return;
				}
			}
		}

		if ((m_GameState == 2 || m_GameState == 3) && MouseHit()) //在排行榜或者游戏帮助中点击
		{
			MOUSEMSG mouse = GetMouseMsg();//获取鼠标点击消息
			if (mouse.mkLButton)
			{
				if (mouse.x >= 20 && mouse.x <= 63 && mouse.y >= 20 && mouse.y <= 43)
				{
					//点击返回选项
					ChangeChooseUI(20, 20, 63, 43, 5);
					Sleep(500);

					FlushMouseMsgBuffer();//清空鼠标消息缓冲区。

					m_GameState = 0;
					ShowMainUI();
				}
			}
		}
	}
}

void Game::Close()
{
	closegraph();
}

void Game::InitData()
{
	if (this->m_snake != nullptr)
	{
		delete(this->m_snake);
		this->m_snake = nullptr;
	}
	if (this->m_food != nullptr)
	{
		delete(this->m_food);
		this->m_food = nullptr;
	}

	this->m_msg = PlayerMsg();
	this->m_snake = new Snake();
	this->m_food = new Food();
}

void Game::PlayGame()
{
	InitData();
	DrawGamePlay();

	BeginBatchDraw();

	bool backMainUI = false;
	bool rePlayGame = false;
	bool changeShowData = false;

	while (true)
	{
		changeShowData = false;
		//食物
		if (this->m_food->getState() == false)
		{
			m_food->Generate(this->m_snake);
			m_food->DrawFood();
		}

		//按键检测
		if (_kbhit())
		{
			char key = _getch();
			switch (key)
			{
			case 72:  //↑
			case 119: //w
				m_snake->ChangeDir(Dir::DIR_UP);
				break;
			case 80:  //↓
			case 115: //s
				m_snake->ChangeDir(Dir::DIR_DOWN);
				break;
			case 75: //←
			case 97: //a
				m_snake->ChangeDir(Dir::DIR_LEFT);
				break;
			case 77:  //→
			case 100: //d
				m_snake->ChangeDir(Dir::DIR_RIGHT);
				break;
			case 99: //c 加速
				changeShowData = m_snake->setSpeed(m_snake->getSpeed() + 1);
				break;
			case 120://x 减速
				changeShowData = m_snake->setSpeed(m_snake->getSpeed() - 1);
				break;
			case 122://z 回归原速
				changeShowData = m_snake->setSpeed(m_snake->OrgSpeed);
				break;
			case 32: //空格暂停
				DrawPause();	//状态区显示
				FlushBatchDraw();//立即批量绘制
				while (true)
				{
					if (_kbhit())
					{
						char key = _getch();
						if (key == 32)		//按空格继续
						{
							ClearRegion(505, 240, 640, 480);
							DrawRunning();	//绘制程序正在运行中
							FlushBatchDraw();//立即批量绘制
							break;
						}
						else if (key == 27) //esc键 退出
						{
							ChangeChooseUI(510, 347, 588, 373, 6);
							FlushBatchDraw();//立即批量绘制
							Sleep(500);

							backMainUI = true;
							break;
						}
					}

					if (MouseHit())
					{
						MOUSEMSG mouse = GetMouseMsg();//获取鼠标点击消息
						if (mouse.mkLButton)
						{
							if (mouse.x >= 510 && mouse.x <= 588 && mouse.y >= 347 && mouse.y <= 373)
							{
								//点击返回选项
								ChangeChooseUI(510, 347, 588, 373, 6);
								FlushBatchDraw();//立即批量绘制
								Sleep(500);

								backMainUI = true;
								break;
							}
						}
					}
				}
				break;
			}

			//如果要回到主UI
			if (backMainUI)
			{
				FlushMouseMsgBuffer();//清空鼠标消息缓冲区。
				break;		//跳出主循环
			}
		}

		//更新显示数据
		if (changeShowData)
		{
			ClearRegion(505, 115, 640, 200);
			DrawScore();
			DrawSnakeLen();
			DrawSpeed();
		}

		//移动
		m_snake->Move();

		//吃食物
		if (m_snake->ColideFood(m_food->getPos()))
		{
			m_snake->EatFood();

			this->m_food->setState(false);

			//分数增加，长度增加
			this->m_msg.score += 10;

			//更新数据
			ClearRegion(505, 115, 640, 200);
			DrawScore();
			DrawSnakeLen();
			DrawSpeed();
		}

		//碰撞检测
		if (m_snake->ColideWall(10, 10, 470, 470) || m_snake->ColideSnake())
		{
			////游戏结束
			m_snake->Dead();

			//清空游戏区，重绘死去的蛇
			ClearRegion(0, 0, 479, 480);

			m_snake->DrawSnake();

			//绘制重新开始
			DrawRebegin();

			//绘制GameOVer
			DrawGameOver();

			FlushBatchDraw();//批量绘制

			while (true)
			{
				if (_kbhit())
				{
					char key = _getch();
					if (key == 32)		//按空格继续
					{
						//点击再来一局选项
						ChangeChooseUI(510, 397, 588, 423, 7);
						FlushBatchDraw();//立即批量绘制
						Sleep(500);

						rePlayGame = true;
						break;
					}
					else if (key == 27) //esc键 退出
					{
						ChangeChooseUI(510, 347, 588, 373, 6);
						FlushBatchDraw();//立即批量绘制
						Sleep(500);

						backMainUI = true;
						break;
					}
				}

				if (MouseHit())
				{
					MOUSEMSG mouse = GetMouseMsg();//获取鼠标点击消息
					if (mouse.mkLButton)
					{
						if (mouse.x >= 510 && mouse.x <= 588 && mouse.y >= 347 && mouse.y <= 373)
						{
							//点击返回选项
							ChangeChooseUI(510, 347, 588, 373, 6);
							FlushBatchDraw();//立即批量绘制
							Sleep(500);

							backMainUI = true;
							break;
						}
						else if (mouse.x >= 510 && mouse.x <= 588 && mouse.y >= 397 && mouse.y <= 423)
						{

							//点击再来一局选项
							ChangeChooseUI(510, 397, 588, 423, 7);
							FlushBatchDraw();//立即批量绘制
							Sleep(500);

							rePlayGame = true;
							break;
						}
					}
				}
			}

			//如果要回到主UI
			if (backMainUI)
			{
				FlushMouseMsgBuffer();//清空鼠标消息缓冲区。
				break;		//跳出主循环
			}

			//如果要重新开始游戏
			if (rePlayGame)
			{
				FlushMouseMsgBuffer();//清空鼠标消息缓冲区。
				break;		//跳出主循环
			}
		}

		//清空屏幕
		setbkcolor(BG_COLOR);//设置背景色
		ClearRegion(0, 0, 479, 480);

		this->m_snake->DrawSnake();

		if (this->m_food->getState() == true)
		{
			m_food->DrawFood();
		}

		FlushBatchDraw();//立即批量绘制

		int sleep = 30 - m_snake->getSpeed();
		Sleep(sleep * 10);
	}

	EndBatchDraw();//结束批量绘制

	//如果要回到主UI
	if (backMainUI)
	{
		m_msg.len = m_snake->getLen();

		m_ranklist->SaveMsg(m_msg);
		m_ranklist->SaveToRank();

		m_GameState = 0;
		ShowMainUI();
	}

	//如果要重新开始游戏
	if (rePlayGame)
	{
		m_msg.len = m_snake->getLen();

		m_ranklist->SaveMsg(m_msg);
		m_ranklist->SaveToRank();

		PlayGame();
	}

}

void Game::ShowMainUI()
{
	//清空屏幕
	setbkcolor(BG_COLOR);//设置背景色
	cleardevice();

	/*--------------绘制标题----------*/
	settextstyle(80, 0, L"微软雅黑");
	settextcolor(0XFFFFFF);
	outtextxy(230, 60, L"贪吃蛇");

	settextstyle(18, 0, L"微软雅黑");
	outtextxy(380, 140, L"By Colourso");

	settextstyle(20, 0, L"微软雅黑");
	settextcolor(0XFF5555);
	outtextxy(365, 160, L"(www.colourso.top)");
	settextcolor(0XFFFFFF);

	/*--------------绘制选项----------*/

	setlinecolor(0xFF00FF);
	rectangle(240, 195, 400, 235);
	rectangle(240, 255, 400, 295);
	rectangle(240, 315, 400, 355);
	rectangle(240, 375, 400, 415);


	settextstyle(28, 0, L"微软雅黑");
	settextcolor(0x55FFFF);//黄色

	outtextxy(297, 200, L"开始");
	outtextxy(287, 260, L"排行榜");
	outtextxy(297, 320, L"帮助");
	outtextxy(297, 380, L"退出");

	settextstyle(24, 0, L"微软雅黑");
	settextcolor(0xAAAAAA);
	outtextxy(255, 450, L"(鼠标点击选项)");
}

void CharToTCHAR(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_UTF8, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, _char, strlen(_char) + 1, tchar, iLength);

}

void Game::ShowRank()
{
	//清空屏幕
	setbkcolor(BG_COLOR);//设置背景色
	cleardevice();

	/*--------------绘制规则----------*/
	settextstyle(60, 0, L"微软雅黑");
	settextcolor(0XFFFFFF);
	outtextxy(160, 40, L"贪吃蛇排行榜");

	/*--------------绘制返回键----------*/
	setlinecolor(0xFFFFFF);
	rectangle(20, 20, 63, 43);

	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"宋体");
	outtextxy(22, 22, L" ← ");

	/*---------------绘制排行榜信息---------------*/
	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"宋体");
	outtextxy(140, 120, L"排名");
	outtextxy(240, 120, L"分数");
	outtextxy(340, 120, L"长度");
	outtextxy(490, 120, L"日期");

	std::vector<PlayerMsg> msg = m_ranklist->getRankList();
	if (msg.size() == 0)
	{
		settextcolor(0x5555FF);
		settextstyle(40, 0, L"宋体");
		outtextxy(230, 200, L"暂无排名");
	}
	for (int i = 0; i < msg.size(); ++i)
	{
		if (i == 0)
		{
			settextcolor(0x5555FF);
			settextstyle(15, 0, L"宋体");
		}
		else
		{
			settextcolor(0XFFFFFF);
			settextstyle(15, 0, L"宋体");
		}
		TCHAR t[40];
		_stprintf_s(t, _T("%d"), msg[i].id);
		outtextxy(145, 150 + 30 * i, t);

		_stprintf_s(t, _T("%d"), msg[i].score);
		outtextxy(245, 150 + 30 * i, t);

		_stprintf_s(t, _T("%d"), msg[i].len);
		outtextxy(345, 150 + 30 * i, t);

		//_stprintf_s(t, _T("%s"), msg[i].r_time.c_str());
		CharToTCHAR(msg[i].r_time.c_str(), t);
		outtextxy(450, 150 + 30 * i, t);

		settextcolor(0XFFFFFF);
	}
}

void Game::ShowRule()
{
	//清空屏幕
	setbkcolor(BG_COLOR);//设置背景色黑色
	cleardevice();

	/*--------------绘制规则----------*/
	settextstyle(60, 0, L"微软雅黑");
	settextcolor(0XFFFFFF);
	outtextxy(160, 40, L"贪吃蛇按键介绍");

	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"宋体");
	outtextxy(230, 150, L"↑ ← ↓ → 控制方向");
	outtextxy(230, 180, L"w  a  s  d  控制方向");
	outtextxy(230, 210, L"速度等级1-25，默认12");
	outtextxy(240, 240, L"c键加速，x键减速");
	outtextxy(240, 270, L"z键恢复原始速度");
	outtextxy(240, 300, L"空格键暂停/继续");

	outtextxy(180, 350, L"请将输入法调至英文输入法状态下");

	/*--------------绘制返回键----------*/

	setlinecolor(0xFFFFFF);
	rectangle(20, 20, 63, 43);

	settextcolor(0XFFFFFF);
	settextstyle(20, 0, L"宋体");
	outtextxy(22, 22, L" ← ");
}

void Game::DrawGamePlay()
{
	//清空屏幕
	setbkcolor(BG_COLOR);//设置背景色
	cleardevice();

	//画宽度为2的棕色实线，分割游戏区
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(482, 0, 482, 480);

	//画蛇和食物
	this->m_snake->DrawSnake();

	//绘制分数
	DrawScore();

	//绘制蛇身长度
	DrawSnakeLen();

	//绘制速度
	DrawSpeed();

	//绘制游戏状态
	DrawRunning();
}

void Game::DrawScore()
{
	TCHAR s[] = _T("获得分数：");
	settextstyle(16, 0, _T("宋体"));
	settextcolor(0xFFFFFF);//白
	outtextxy(510, 120, s);

	settextcolor(0xFF5555);//亮蓝
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), m_msg.score); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(590, 120, t);
	settextcolor(0xFFFFFF);//白
}

void Game::DrawSnakeLen()
{
	settextstyle(16, 0, L"宋体");
	outtextxy(510, 150, L"蛇身长度：");

	settextcolor(0xFF55FF);//亮紫
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), m_snake->getLen()); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(590, 150, t);
	settextcolor(0xFFFFFF);//白
}

void Game::DrawSpeed()
{
	TCHAR s[] = _T("当前速度：");
	settextstyle(16, 0, _T("宋体"));
	outtextxy(510, 180, s);

	int speed = m_snake->getSpeed();//速度等级显示为1 - 25
	if (speed <= 9)
	{
		settextcolor(0x00AA00);//禄
	}
	else if (speed >= 18)
	{
		settextcolor(0x0000AA);//红
	}
	else
	{
		settextcolor(0x55FFFF);//黄
	}
	TCHAR t[5];
	_stprintf_s(t, _T("%d"), speed); // 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(590, 180, t);
	settextcolor(0xFFFFFF);//白
}

void Game::DrawRunning()
{
	settextcolor(0x55FF55);//亮绿
	settextstyle(16, 0, L"宋体");
	outtextxy(510, 250, L"游戏 运行");
	outtextxy(510, 280, L"祝玩的开心");
	settextcolor(0xFFFFFF);//白
}

void Game::DrawPause()
{
	settextcolor(0xFF55FF);//亮紫
	settextstyle(16, 0, L"宋体");
	outtextxy(510, 250, L"游戏 暂停");
	outtextxy(510, 280, L"空格键继续");
	outtextxy(510, 310, L"Esc键退出");
	settextcolor(0xFFFFFF);//白

	setlinecolor(0xFFFFFF);
	rectangle(510, 347, 588, 373);

	settextcolor(0XFFFFFF);
	settextstyle(16, 0, L"宋体");
	outtextxy(517, 352, L"退出游戏");
}

void Game::DrawRebegin()
{
	settextcolor(0x5555FF);//亮红
	settextstyle(16, 0, L"宋体");
	outtextxy(510, 250, L"游戏 结束");
	outtextxy(510, 280, L"空格键再来一局");
	outtextxy(510, 310, L"Esc键退出");
	settextcolor(0xFFFFFF);//白

	setlinecolor(0xFFFFFF);
	rectangle(510, 347, 588, 373);
	rectangle(510, 397, 588, 423);

	settextcolor(0XFFFFFF);
	settextstyle(16, 0, L"宋体");
	outtextxy(517, 352, L"退出游戏");
	outtextxy(517, 402, L"再来一局");
}

void Game::DrawGameOver()
{
	settextcolor(0xFFFFFF);//亮红
	settextstyle(48, 0, L"宋体");
	outtextxy(170, 210, L"GAMEOVER");
}

void Game::ChangeChooseUI(int left, int top, int right, int bottom, int kind)
{
	setfillcolor(0XFFFFFF);				//使用白色填充这一区域
	fillrectangle(left, top, right, bottom);

	setlinecolor(0x55FF55);				//画线边框设置为亮绿色
	rectangle(left, top, right, bottom);

	settextstyle(28, 0, L"微软雅黑");
	settextcolor(0xFF5555);//亮蓝色

	setbkcolor(0XFFFFFF);				//设置背景色为白色，文字的背景色就会变成白色
	switch (kind)
	{
	case 1:
		outtextxy(297, 200, L"开始");
		break;
	case 2:
		outtextxy(287, 260, L"排行榜");
		break;
	case 3:
		outtextxy(297, 320, L"帮助");
		break;
	case 4:
		outtextxy(297, 380, L"退出");
		break;
	case 5:
		settextcolor(0);
		settextstyle(20, 0, L"宋体");
		outtextxy(22, 22, L" ← ");
		break;
	case 6:
		settextcolor(0);
		settextstyle(16, 0, L"宋体");
		outtextxy(517, 352, L"退出游戏");
		break;
	case 7:
		settextcolor(0);
		settextstyle(16, 0, L"宋体");
		outtextxy(517, 402, L"再来一局");
		break;
	}
	setbkcolor(BG_COLOR);	//恢复背景色
}

void Game::ClearRegion(int left, int top, int right, int bottom)
{
	//黑色，全填充，无边框的矩形
	setfillcolor(BG_COLOR);
	setfillstyle(BS_SOLID);
	solidrectangle(left, top, right, bottom);
}
