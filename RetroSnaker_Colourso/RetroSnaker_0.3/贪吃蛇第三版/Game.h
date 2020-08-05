#pragma once

#include "common.h"
#include "Snake.h"
#include "Food.h"
#include "RankList.h"

class Game
{
private:
	int m_GameState;			//游戏状态,0在主UI，1在游戏中，2在排行榜，3在游戏规则中
	PlayerMsg m_msg;			//游玩数据
	Snake *m_snake;				//蛇
	Food *m_food;				//食物
	RankList *m_ranklist;		//排行榜

public:
	Game();

	void Init();			//初始化
	void Run();				//控制程序
	void Close();			//关闭程序，释放资源

private:
	void InitData();		//初始化数据

	void PlayGame();		//开始游戏

	void ShowMainUI();		//展示主UI
	void ShowRank();		//排行榜展示
	void ShowRule();		//展示规则界面

	void DrawGamePlay();	//绘制初始游戏界面
	void DrawScore();		//绘制分数
	void DrawSnakeLen();	//绘制长度
	void DrawSpeed();		//绘制速度
	void DrawRunning();		//绘制正在运行
	void DrawPause();		//绘制暂停提示
	void DrawRebegin();		//绘制重新开始
	void DrawGameOver();	//绘制游戏结束

	void ChangeChooseUI(int left, int top, int right, int bottom, int kind);//修改选中的选项颜色
	void ClearRegion(int left, int top, int right, int bottom);		//使用黑色清除指定区域
};

