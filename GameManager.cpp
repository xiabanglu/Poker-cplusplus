#include "GameManager.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "PokerUtil.h"
#include <graphics.h>
#include <vector>
#include "GameState.h"
#include "Render.h"
#include "Logger.h"

GameManager &GameManager::getInstance()
{
	static GameManager instance;
	return instance;
}
void GameManager::init()
{
	logger.logInit();
	gameState.getPlayers().push_back(std::make_unique<HumanPlayer>(_T("我")));
	gameState.getPlayers().push_back(std::make_unique<AIPlayer>(_T("AI1")));
	gameState.getPlayers().push_back(std::make_unique<AIPlayer>(_T("AI2")));
	for (auto &player : gameState.getPlayers())
	{
		logger.logPlayerJoined(player->getName());
	}
	std::vector<Poker> deck = PokerUtil::generateDeck();
	PokerUtil::shuffle(deck);
	distribute(deck);
	PokerUtil::sort(gameState.getBottom());
	for (auto &player : gameState.getPlayers())
	{
		PokerUtil::sort(player->getPokers());
	}
	logger.logBottomCards(gameState.getBottom());
	for (auto &player : gameState.getPlayers())
	{
		logger.logPlayerReceivedCards(player->getName(), player->getPokers());
	}
	dynamic_cast<HumanPlayer *>(gameState.getPlayers()[0].get())->initSelectedState();
}
void GameManager::run()
{
	ExMessage msg = {};
	BeginBatchDraw();// 开启批量绘制，提升界面流畅度
	bool needRedraw = false;
	Render::drawAll();
	FlushBatchDraw();

	while (isRunning)
	{
		// 叫分阶段处理
		if (gameState.isInBiddingPhase() && !bidManager.isBidDone())
		{
			needRedraw = handleBiddingPhase(msg);
		}
		// 出牌阶段处理
		else if (gameState.isInPlayingPhase() && !playManager.isPlayDone())
		{
			needRedraw = handlePlayingPhase(msg);
		}
		// 游戏结束
		else if (bidManager.isBidDone() && playManager.isPlayDone())
		{
			Render::drawAll();
			FlushBatchDraw();
			isRunning = false;
		}
		// 必要时重绘界面
		if (needRedraw)
		{
			Render::drawAll();
			FlushBatchDraw();
			needRedraw = false;
		}
	}
	EndBatchDraw();
}
void GameManager::distribute(std::vector<Poker> &deck)
{
	gameState.setBottom(std::vector<Poker>(deck.end() - 3, deck.end()));
	deck.erase(deck.end() - 3, deck.end());
	for (int i = 0; i < static_cast<int>(deck.size()); ++i)
	{
		gameState.getPlayers()[i % 3]->addPoker(deck[i]);
	}
}
// 处理叫分阶段
bool GameManager::handleBiddingPhase(ExMessage &msg)
{
	int currentBidder = gameState.getCurrentBidderIndex();
	if (currentBidder == 0)
	{
		return handleHumanBidding(msg); // 处理人类叫分
	}
	return handleAIBidding(); // 处理AI叫分
}
// 处理出牌阶段
bool GameManager::handlePlayingPhase(ExMessage &msg)
{
	int currentPlayer = gameState.getCurrentPlayerIndex();
	if (currentPlayer == 0)
	{
		return handleHumanPlaying(msg); // 处理人类出牌
	}
	return handleAIPlaying(); // 处理AI出牌
}
// 处理人类叫分
bool GameManager::handleHumanBidding(ExMessage &msg)
{
	if (peekmessage(&msg, EM_MOUSE) && msg.message == WM_LBUTTONDOWN)
	{
		return bidManager.humanBid(msg); // 处理人类叫分
	}
	return false; 
}
// 处理AI叫分
bool GameManager::handleAIBidding()
{
	return bidManager.aiBid(); // AI叫分
}
// 处理人类出牌
bool GameManager::handleHumanPlaying(ExMessage &msg)
{
	if (peekmessage(&msg, EM_MOUSE) && msg.message == WM_LBUTTONDOWN)
	{
		if (playManager.humanPlay(msg))
			return true; // 处理人类出牌
		if (playManager.humanChangeSelected(msg))
			return true; // 处理人类选牌
	}
	return false;
}
// 处理AI出牌
bool GameManager::handleAIPlaying()
{
	return playManager.aiPlay(); // AI出牌
}