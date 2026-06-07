#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameState.h"
#include "BidManager.h"
#include "PlayManager.h"
#include "InputManager.h"
#include "Logger.h"

class GameManager
{
private:
	GameState &gameState = GameState::getInstance();
	BidManager &bidManager = BidManager::getInstance();
	PlayManager &playManager = PlayManager::getInstance();
	InputManager &inputManager = InputManager::getInstance();
	Logger &logger = Logger::getInstance();

	bool isRunning = true; // 游戏是否运行

	void distribute(std::vector<Poker> &deck); // 发牌

	bool handleBiddingPhase(ExMessage &msg);
	bool handlePlayingPhase(ExMessage &msg);
	bool handleHumanBidding(ExMessage &msg);
	bool handleAIBidding();
	bool handleHumanPlaying(ExMessage &msg);
	bool handleAIPlaying();

	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager &) = delete;
	GameManager &operator=(const GameManager &) = delete;
	GameManager(GameManager &&) = delete;
	GameManager &operator=(GameManager &&) = delete;

public:
	static GameManager &getInstance();

	void init(); // 初始化游戏
	void run();	 // 游戏主循环
};

#endif // GAMEMANAGER_H