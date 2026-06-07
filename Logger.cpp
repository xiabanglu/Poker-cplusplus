#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <windows.h>
#include "PokerTypeUtil.h"

Logger::Logger()
{
	logFile.open("game_record.txt", std::ios::app);
	if (!logFile.is_open())
	{
		std::cerr << "无法打开日志文件 game_record.txt" << std::endl;
	}
}
Logger::~Logger()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}
Logger &Logger::getInstance()
{
	static Logger instance;
	return instance;
}
std::string Logger::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::tm tm = {};
	int m = localtime_s(&tm, &time);
	std::stringstream ss;
	ss << std::put_time(&tm, "%Y.%m.%d %H:%M:%S");
	return ss.str();
}
std::string Logger::tcharToString(const TCHAR *tstr)
{
	if (tstr == nullptr)
	{
		return "";
	}
#ifdef _UNICODE
	int wideLen = lstrlenW(tstr);
	int byteLen = WideCharToMultiByte(
		CP_UTF8,
		0,
		tstr,
		wideLen,
		nullptr,
		0,
		nullptr,
		nullptr);
	if (byteLen <= 0)
	{
		return "";
	}
	std::string result(byteLen, 0);
	WideCharToMultiByte(
		CP_UTF8,
		0,
		tstr,
		wideLen,
		&result[0],
		byteLen,
		nullptr,
		nullptr);
	return result;
#else
	return std::string(tstr);
#endif
}
std::string Logger::pokerToString(const Poker &poker)
{
	std::string pointStr;
	std::string suitStr;
	if (poker.getPoint() == Point::TINY_JOKER)
	{
		return "小王";
	}
	if (poker.getPoint() == Point::GIANT_JOKER)
	{
		return "大王";
	}
	switch (poker.getPoint())
	{
	case Point::ACE:
		pointStr = "A";
		break;
	case Point::TWO:
		pointStr = "2";
		break;
	case Point::THREE:
		pointStr = "3";
		break;
	case Point::FOUR:
		pointStr = "4";
		break;
	case Point::FIVE:
		pointStr = "5";
		break;
	case Point::SIX:
		pointStr = "6";
		break;
	case Point::SEVEN:
		pointStr = "7";
		break;
	case Point::EIGHT:
		pointStr = "8";
		break;
	case Point::NINE:
		pointStr = "9";
		break;
	case Point::TEN:
		pointStr = "10";
		break;
	case Point::JACK:
		pointStr = "J";
		break;
	case Point::QUEEN:
		pointStr = "Q";
		break;
	case Point::KING:
		pointStr = "K";
		break;
	default:
		pointStr = "未知";
	}
	switch (poker.getSuit())
	{
	case Suit::SPADE:
		suitStr = "♠";
		break;
	case Suit::HEART:
		suitStr = "♥";
		break;
	case Suit::CLUB:
		suitStr = "♣";
		break;
	case Suit::DIAMOND:
		suitStr = "♦";
		break;
	default:
		suitStr = "未知";
	}
	return suitStr + pointStr;
}
std::string Logger::pokersToString(const std::vector<Poker> &pokers)
{
	if (pokers.empty())
	{
		return "无";
	}
	std::string str;
	for (size_t i = 0; i < pokers.size(); ++i)
	{
		if (i > 0)
		{
			str += ", ";
		}
		str += pokerToString(pokers[i]);
	}
	return str;
}
std::string Logger::pokerTypeToString(PokerType type)
{
	switch (type)
	{
	case PokerType::INVALID:
		return "无效牌型";
	case PokerType::SINGLE:
		return "单牌";
	case PokerType::PAIR:
		return "对子";
	case PokerType::TRIPLE:
		return "三个";
	case PokerType::STRAIGHT:
		return "顺子";
	case PokerType::CONTINUOUS_PAIR:
		return "连对";
	case PokerType::BOMB:
		return "炸弹";
	case PokerType::TRIPLE_WITH_SINGLE:
		return "三带一";
	case PokerType::TRIPLE_WITH_PAIR:
		return "三带二";
	case PokerType::PLANE:
		return "飞机";
	case PokerType::PLANE_WITH_WINGS:
		return "飞机带翅膀";
	case PokerType::FOUR_WITH_TWO:
		return "四带二";
	case PokerType::ROYAL_BOMB:
		return "王炸";
	default:
		return "未知牌型";
	}
}
void Logger::log(const std::string &message)
{
	std::lock_guard<std::mutex> lock(logMutex);
	std::string logMessage = "[" + getCurrentTime() + "] " + message;
	std::cout << logMessage << std::endl;
	if (logFile.is_open())
	{
		logFile << logMessage << std::endl;
	}
}
void Logger::logInit()
{
	log("游戏初始化");
}
void Logger::logPlayerJoined(const TCHAR *playerName)
{
	std::string Name = tcharToString(playerName);
	log(Name + "加入了游戏");
}
void Logger::logPlayerReceivedCards(const TCHAR *playerName, const std::vector<Poker> &cards)
{
	std::string Name = tcharToString(playerName);
	log(Name + "获得了手牌: " + pokersToString(cards));
}
void Logger::logBottomCards(const std::vector<Poker> &bottomCards)
{
	log("底牌为: " + pokersToString(bottomCards));
}
void Logger::logPlayerBid(const TCHAR *playerName, int bid)
{
	if (bid == 0)
	{
		std::string Name = tcharToString(playerName);
		log(Name + "选择不叫");
		return;
	}
	std::string Name = tcharToString(playerName);
	log(Name + "叫了" + std::to_string(bid) + "分");
}
void Logger::logLandlordDetermined(const TCHAR *landlordName)
{
	std::string Name = tcharToString(landlordName);
	log(Name + "成为地主");
}
void Logger::logPlayerPlayedCards(const TCHAR *playerName, std::vector<Poker> &cards)
{
	if (cards.empty())
	{
		std::string Name = tcharToString(playerName);
		log(Name + "选择不出");
		return;
	}
	PokerType type = PokerTypeUtil::getPokerType(cards);
	std::string Name = tcharToString(playerName);
	log(Name + "出了" + pokerTypeToString(type) + pokersToString(cards));
}
void Logger::logPlayerWon()
{
	if (gameState.getWinnerIndex() == gameState.getLandlordIndex())
	{
		log("地主获取了胜利");
		return;
	}
	log("农民获取了胜利");
}