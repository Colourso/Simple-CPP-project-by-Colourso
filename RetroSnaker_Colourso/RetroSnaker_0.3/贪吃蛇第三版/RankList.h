#pragma once

#include "common.h"
#include <vector>

class RankList
{
private:
	std::vector<PlayerMsg> m_msg;
	const std::string m_rankfile = "retro";
	const int MAX_RANK = 10;
public:
	RankList();

	void SaveMsg(PlayerMsg msg);
	std::vector<PlayerMsg> getRankList();
	void SaveToRank();

private:
	void WriteTime(PlayerMsg &msg);
	void ReadFile();
	void WriteFile();
};

