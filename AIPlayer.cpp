#include "AIPlayer.h"
#include <random>
#include "Render.h"
#include "playManager.h"

int AIPlayer::bid()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 3);
	int aiBid = dis(gen);
	return aiBid;
}
bool AIPlayer::play()
{
	int currentIndex = gameState.getCurrentPlayerIndex();
	auto &aiPlayer = dynamic_cast<AIPlayer &>(*gameState.getPlayers()[currentIndex]);
	std::vector<Poker> &currentCards = aiPlayer.getPokers();
	std::vector<Poker> bestPlay;
	auto lastPlay = playManager.getLastPlayedCards();
	// 策略选择：首家出牌或2人连续不出时，出最小合法牌；否则出能压制的最小牌
	if (lastPlay.empty() || playManager.getPassCount() >= 2)
	{
		bestPlay = playManager.findSmallestValidPlay(currentCards);
	}
	else
	{
		bestPlay = playManager.findSmallestWinningPlay(currentCards, lastPlay);
	}
	if (!bestPlay.empty())
	{
		// 移除手牌中已出的牌，添加到出牌记录
		for (const auto &card : bestPlay)
		{
			auto it = std::find_if(pokers.begin(), pokers.end(), [&](const Poker &p)
								   { return p.getPoint() == card.getPoint() && p.getSuit() == card.getSuit(); });
			if (it != pokers.end())
			{
				size_t idx = std::distance(pokers.begin(), it);
				removePoker(idx);
				handOutPokers.push_back(card);
			}
		}
		// 延迟1秒模拟思考，更新界面
		Sleep(1000);
		Render::drawAll();
		FlushBatchDraw();
		logger.logPlayerPlayedCards(aiPlayer.getName(), handOutPokers);
		playManager.setLastPlayedCards(bestPlay);
		playManager.setLastPlayerIndex(currentIndex);
		playManager.resetPassCount();
		playManager.nextPlayer();
		Sleep(1000);
		return true;
	}
	// 无合法牌可出，选择不出
	setPassed(true);
	handOutPokers = {};
	Sleep(1000);
	Render::drawAll();
	FlushBatchDraw();
	logger.logPlayerPlayedCards(aiPlayer.getName(), handOutPokers);
	playManager.incrementPassCount();
	playManager.nextPlayer();
	Sleep(1000);
	return true;
}
void AIPlayer::drawCards() const
{
	Render::drawPlayerCards(this);
}
void AIPlayer::drawHandOutCards() const
{
	Render::drawPlayerHandOutCards(this);
}
void AIPlayer::drawNameAndCount() const
{
	Render::drawPlayerName(this);
	Render::drawPlayerCount(this);
}
void AIPlayer::drawBid() const
{
	Render::drawPlayerBid(this);
}
void AIPlayer::drawLandlord() const
{
	Render::drawPlayerLandlord(this);
}