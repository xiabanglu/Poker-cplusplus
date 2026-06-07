#ifndef PLAYMANAGER_H
#define PLAYMANAGER_H

#include "GameState.h"
#include <easyx.h>
#include "Logger.h"

class PlayManager
{
private:
	GameState &gameState = GameState::getInstance();
	Logger &logger = Logger::getInstance();

	std::vector<Poker> lastPlayedCards; // 上一轮出的牌
	int lastPlayerIndex = -1;			// 上一轮出牌的玩家索引
	int passCount = 0;					// 连续不出的次数（用于判断回合结束）
	bool isPlayFinished = false;		// 出牌阶段是否结束

	PlayManager() = default;
	~PlayManager() = default;
	PlayManager(const PlayManager &) = delete;
	PlayManager &operator=(const PlayManager &) = delete;
	PlayManager(PlayManager &&) = delete;
	PlayManager &operator=(PlayManager &&) = delete;

public:
	static PlayManager &getInstance();

	bool humanPlay(const ExMessage &msg);			   // 处理人类出牌
	bool humanChangeSelected(const ExMessage &msg);	   // 处理人类玩家选牌
	bool aiPlay();									   // AI出牌
	void nextPlayer();								   // 切换到下一个玩家
	bool isValidPlay(std::vector<Poker> &cards) const; // 验证出牌是否合法
	// 寻找最小合法出牌和最小能打过上一手牌的出牌
	std::vector<Poker> findSmallestValidPlay(std::vector<Poker> &cards);
	std::vector<Poker> findSmallestWinningPlay(std::vector<Poker> &cards, std::vector<Poker> &lastPlay);

	std::vector<Poker> findSmallestSingle(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestPair(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestTriple(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestStraight(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestContinuousPair(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestTripleWithSingle(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestTripleWithPair(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestPlane(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestPlaneWithWings(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestFourWithTwo(std::vector<Poker> &sortedCards);
	std::vector<Poker> findSmallestBomb(std::vector<Poker> &sortedCards);
	std::vector<Poker> findRoyalBomb(std::vector<Poker> &sortedCards);

	std::vector<Poker> findWinningSingle(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningPair(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningTriple(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningStraight(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningContinuousPair(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningTripleWithSingle(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningTripleWithPair(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningPlane(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningPlaneWithWings(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningFourWithTwo(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	std::vector<Poker> findWinningBomb(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay);
	bool isGameOver(); // 检查游戏是否结束

	// Getter和Setter
	std::vector<Poker> getLastPlayedCards() const;
	void setLastPlayedCards(const std::vector<Poker> &cards);
	int getLastPlayerIndex() const;
	void setLastPlayerIndex(int index);
	int getPassCount() const;
	void setPassCount(int count);
	void incrementPassCount();
	void resetPassCount();
	bool isPlayDone() const;
};

#endif // PLAYMANAGER_H