#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <tchar.h>
#include "Poker.h"
#include "PokerType.h"
#include <vector>
#include "GameState.h"

class Logger
{
private:
	Logger();
	~Logger();
	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;
	Logger(Logger &&) = delete;
	Logger &operator=(Logger &&) = delete;

	GameState &gameState = GameState::getInstance();

	std::ofstream logFile; // 日志文件流
	std::mutex logMutex;   // 互斥锁，确保线程安全

	std::string getCurrentTime();								  // 获取当前时间字符串
	std::string tcharToString(const TCHAR *tstr);				  // 将TCHAR字符串转换为std::string
	std::string pokerToString(const Poker &poker);				  // 将单张牌转换为字符串
	std::string pokersToString(const std::vector<Poker> &pokers); // 将多张牌转换为字符串
	std::string pokerTypeToString(PokerType type);				  // 将牌型枚举转换为字符串

public:
	static Logger &getInstance();

	void logInit();																		   // 记录初始化日志
	void logPlayerJoined(const TCHAR *playerName);										   // 记录玩家加入日志
	void logPlayerReceivedCards(const TCHAR *playerName, const std::vector<Poker> &cards); // 记录玩家收到牌日志
	void logBottomCards(const std::vector<Poker> &bottomCards);							   // 记录底牌日志
	void logPlayerBid(const TCHAR *playerName, int bid);								   // 记录玩家叫分日志
	void logLandlordDetermined(const TCHAR *landlordName);								   // 记录地主确定日志
	void logPlayerPlayedCards(const TCHAR *playerName, std::vector<Poker> &cards);		   // 记录玩家出牌日志
	void logPlayerWon();																   // 记录玩家获胜日志
	void log(const std::string &message);												   // 通用日志记录函数
};

#endif // LOGGER_H