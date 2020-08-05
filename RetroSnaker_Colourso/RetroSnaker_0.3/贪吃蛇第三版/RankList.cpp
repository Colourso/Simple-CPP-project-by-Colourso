#include "RankList.h"
#include <ctime>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>

RankList::RankList()
{
	if (!this->m_msg.empty())
	{
		this->m_msg.clear();
	}

	ReadFile();
}

void RankList::SaveMsg(PlayerMsg msg)
{
	WriteTime(msg);		//写入时间

	m_msg.push_back(msg);
	std::sort(m_msg.begin(), m_msg.end(), SortPlayerMsg());

	if (m_msg.size() > this->MAX_RANK)
	{
		m_msg.pop_back();
	}

	std::vector<PlayerMsg>::iterator it = m_msg.begin();

	//修改id
	for (int i = 0; i < m_msg.size(); i++, it++)
	{
		it->id = i + 1;
	}
}

std::vector<PlayerMsg> RankList::getRankList()
{
	return this->m_msg;
}

void RankList::SaveToRank()
{
	WriteFile();
}

void RankList::WriteTime(PlayerMsg & msg)
{
	// 基于当前系统的当前日期/时间
	time_t now = time(0);		//1970 到目前经过秒数

	tm *ltm = localtime(&now);

	std::stringstream sstream;

	// 输出 tm 结构的各个组成部分
	sstream << ltm->tm_year + 1900 - 2000 << "-";	//年
	sstream << ((ltm->tm_mon + 1 >= 10) ? "" : "0") << ltm->tm_mon + 1 << "-";	//月
	sstream << ((ltm->tm_mday >= 10) ? "" : "0") << ltm->tm_mday << "_";	//日
	sstream << ((ltm->tm_hour >= 10) ? "" : "0") << ltm->tm_hour << ":";	//时
	sstream << ((ltm->tm_min >= 10) ? "" : "0") << ltm->tm_min;	//分

	msg.r_time = sstream.str();
}

void RankList::ReadFile()
{
	std::ifstream infile;

	infile.open(m_rankfile, std::ios::in | std::ios::binary);

	if (!infile)
	{
		//如果文件不存在，则创建
		std::ofstream os;
		os.open(m_rankfile);		//默认会创建

		if (!os)
		{
			//如果创建失败，只能结束程序
			exit(0);
		}

		os.close();
	}
	else 
	{
		std::string line;
		std::stringstream stream;
		int index = 0;
		PlayerMsg msg;

		while (std::getline(infile, line))
		{
			stream.clear();
			stream.str(line);

			stream >> msg.id >> msg.score >> msg.len >> msg.r_time;

			m_msg.push_back(msg);

			index++;
		}

		std::sort(m_msg.begin(), m_msg.end(), SortPlayerMsg());
	}

	infile.close();
}

void RankList::WriteFile()
{
	std::ofstream outfile;
	outfile.open(m_rankfile, std::ios::out | std::ios::binary); //每次写文件都重新写一遍

	if (!outfile)
	{
		return;
	}

	for (int i = 0; i < m_msg.size(); ++i)
	{
		outfile << m_msg[i].id << " " << m_msg[i].score << " " << m_msg[i].len
			<< " " << m_msg[i].r_time << std::endl;
	}

	outfile.close();
}