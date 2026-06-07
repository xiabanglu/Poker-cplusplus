#include "PlayManager.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"
#include "GameState.h"
#include "Render.h"
#include <easyx.h>
#include "PokerTypeUtil.h"
#include "PokerType.h"
#include "PokerUtil.h"
#include <algorithm>
#include <random>

PlayManager &PlayManager::getInstance()
{
	static PlayManager instance;
	return instance;
}
bool PlayManager::humanPlay(const ExMessage &msg)
{
	if (isPlayFinished)
		return false;
	if (gameState.getCurrentPlayerIndex() != 0)
		return false;
	if (auto *human = dynamic_cast<HumanPlayer *>(gameState.getPlayers()[gameState.getCurrentPlayerIndex()].get()))
	{
		gameState.setThinkingPlayerIndex(gameState.getCurrentPlayerIndex());
		human->getHandOutPokers().clear();
		human->setPassed(false);
		Render::drawAll();
		FlushBatchDraw();
		if (human->play(msg))
		{
			if (isGameOver())
			{
				isPlayFinished = true;
			}
			return true;
		}
	}
	return false;
}
bool PlayManager::humanChangeSelected(const ExMessage &msg)
{
	if (isPlayFinished)
		return false;
	if (gameState.getCurrentPlayerIndex() != 0)
		return false;
	if (auto *human = dynamic_cast<HumanPlayer *>(gameState.getPlayers()[gameState.getCurrentPlayerIndex()].get()))
	{
		gameState.setThinkingPlayerIndex(gameState.getCurrentPlayerIndex());
		human->getHandOutPokers().clear();
		human->setPassed(false);
		Render::drawAll();
		FlushBatchDraw();
		if (human->changeSelected(msg))
		{
			if (isGameOver())
			{
				isPlayFinished = true;
			}
			return true;
		}
	}
	return false;
}
bool PlayManager::aiPlay()
{
	if (isPlayFinished)
		return false;
	if (gameState.getCurrentPlayerIndex() == 0)
		return false;
	if (auto *ai = dynamic_cast<AIPlayer *>(gameState.getPlayers()[gameState.getCurrentPlayerIndex()].get()))
	{
		gameState.setThinkingPlayerIndex(gameState.getCurrentPlayerIndex());
		ai->getHandOutPokers().clear();
		ai->setPassed(false);
		Render::drawAll();
		FlushBatchDraw();
		if (ai->play())
		{
			if (isGameOver())
			{
				isPlayFinished = true;
			}
			return true;
		}
	}
	return false;
}
void PlayManager::nextPlayer()
{
	gameState.setCurrentPlayerIndex((gameState.getCurrentPlayerIndex() + 1) % 3);
	gameState.setThinkingPlayerIndex(gameState.getCurrentPlayerIndex());
}
bool PlayManager::isValidPlay(std::vector<Poker> &cards) const
{
	if (cards.empty())
		return false;
	PokerType type = PokerTypeUtil::getPokerType(cards);
	if (type == PokerType::INVALID)
		return false;
	auto lastPlay = getLastPlayedCards();
	if (lastPlay.empty())
		return true;
	return PokerTypeUtil::compare(cards, lastPlay);
}
std::vector<Poker> PlayManager::findSmallestValidPlay(std::vector<Poker> &cards)
{
	if (cards.empty())
		return {};
	std::vector<Poker> sortedCards = cards;
	PokerUtil::sort(sortedCards);
	std::vector<std::vector<Poker>> validNonBombPlays;
	auto single = findSmallestSingle(sortedCards);
	if (!single.empty())
		validNonBombPlays.push_back(single);
	auto pair = findSmallestPair(sortedCards);
	if (!pair.empty())
		validNonBombPlays.push_back(pair);
	auto triple = findSmallestTriple(sortedCards);
	if (!triple.empty())
		validNonBombPlays.push_back(triple);
	auto straight = findSmallestStraight(sortedCards);
	if (!straight.empty())
		validNonBombPlays.push_back(straight);
	auto continuousPair = findSmallestContinuousPair(sortedCards);
	if (!continuousPair.empty())
		validNonBombPlays.push_back(continuousPair);
	auto tripleWithSingle = findSmallestTripleWithSingle(sortedCards);
	if (!tripleWithSingle.empty())
		validNonBombPlays.push_back(tripleWithSingle);
	auto tripleWithPair = findSmallestTripleWithPair(sortedCards);
	if (!tripleWithPair.empty())
		validNonBombPlays.push_back(tripleWithPair);
	auto plane = findSmallestPlane(sortedCards);
	if (!plane.empty())
		validNonBombPlays.push_back(plane);
	auto planeWithWings = findSmallestPlaneWithWings(sortedCards);
	if (!planeWithWings.empty())
		validNonBombPlays.push_back(planeWithWings);
	auto fourWithTwo = findSmallestFourWithTwo(sortedCards);
	if (!fourWithTwo.empty())
		validNonBombPlays.push_back(fourWithTwo);
	if (!validNonBombPlays.empty())
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, static_cast<int>(validNonBombPlays.size() - 1));
		int randomIdx = distr(gen);
		return validNonBombPlays[randomIdx];
	}
	auto bomb = findSmallestBomb(sortedCards);
	if (!bomb.empty())
		return bomb;
	auto royalBomb = findRoyalBomb(sortedCards);
	if (!royalBomb.empty())
		return royalBomb;
	return {};
}
std::vector<Poker> PlayManager::findSmallestWinningPlay(std::vector<Poker> &cards, std::vector<Poker> &lastPlay)
{
	if (lastPlay.empty())
		return findSmallestValidPlay(cards);
	std::vector<Poker> sortedCards = cards;
	PokerUtil::sort(sortedCards);
	PokerType targetType = PokerTypeUtil::getPokerType(lastPlay);
	if (targetType == PokerType::INVALID)
		return {};
	std::vector<Poker> sameTypePlay;
	switch (targetType)
	{
	case PokerType::SINGLE:
		sameTypePlay = findWinningSingle(sortedCards, lastPlay);
		break;
	case PokerType::PAIR:
		sameTypePlay = findWinningPair(sortedCards, lastPlay);
		break;
	case PokerType::TRIPLE:
		sameTypePlay = findWinningTriple(sortedCards, lastPlay);
		break;
	case PokerType::STRAIGHT:
		sameTypePlay = findWinningStraight(sortedCards, lastPlay);
		break;
	case PokerType::CONTINUOUS_PAIR:
		sameTypePlay = findWinningContinuousPair(sortedCards, lastPlay);
		break;
	case PokerType::TRIPLE_WITH_SINGLE:
		sameTypePlay = findWinningTripleWithSingle(sortedCards, lastPlay);
		break;
	case PokerType::TRIPLE_WITH_PAIR:
		sameTypePlay = findWinningTripleWithPair(sortedCards, lastPlay);
		break;
	case PokerType::PLANE:
		sameTypePlay = findWinningPlane(sortedCards, lastPlay);
		break;
	case PokerType::PLANE_WITH_WINGS:
		sameTypePlay = findWinningPlaneWithWings(sortedCards, lastPlay);
		break;
	case PokerType::FOUR_WITH_TWO:
		sameTypePlay = findWinningFourWithTwo(sortedCards, lastPlay);
		break;
	case PokerType::BOMB:
		sameTypePlay = findWinningBomb(sortedCards, lastPlay);
		break;
	case PokerType::ROYAL_BOMB:
		return {};
	default:
		return {};
	}
	if (!sameTypePlay.empty())
	{
		return sameTypePlay;
	}
	if (targetType != PokerType::BOMB && targetType != PokerType::ROYAL_BOMB)
	{
		auto bomb = findSmallestBomb(sortedCards);
		if (!bomb.empty())
			return bomb;
		auto royalBomb = findRoyalBomb(sortedCards);
		if (!royalBomb.empty())
			return royalBomb;
	}
	else if (targetType == PokerType::BOMB)
	{
		auto biggerBomb = findWinningBomb(sortedCards, lastPlay);
		if (!biggerBomb.empty())
			return biggerBomb;
		auto royalBomb = findRoyalBomb(sortedCards);
		if (!royalBomb.empty())
			return royalBomb;
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestSingle(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 1)
		return {};
	if (!sortedCards.empty())
		return {sortedCards[0]};
	return {};
}
std::vector<Poker> PlayManager::findSmallestPair(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 2)
		return {};
	for (size_t i = 0; i < sortedCards.size() - 1; ++i)
	{
		if (sortedCards[i].getPoint() == sortedCards[i + 1].getPoint())
		{
			return {sortedCards[i], sortedCards[i + 1]};
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestTriple(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 3)
		return {};
	for (size_t i = 0; i < sortedCards.size() - 2; ++i)
	{
		if (sortedCards[i].getPoint() == sortedCards[i + 1].getPoint() &&
			sortedCards[i].getPoint() == sortedCards[i + 2].getPoint())
		{
			return {sortedCards[i], sortedCards[i + 1], sortedCards[i + 2]};
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestStraight(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 5)
		return {};
	std::vector<Point> uniquePoints;
	for (const auto &card : sortedCards)
	{
		Point p = card.getPoint();
		if (p == Point::TWO || p == Point::TINY_JOKER || p == Point::GIANT_JOKER)
			continue;
		if (uniquePoints.empty() || uniquePoints.back() != p)
		{
			uniquePoints.push_back(p);
		}
	}
	for (size_t i = 0; i + 4 < uniquePoints.size(); ++i)
	{
		bool continuous = true;
		for (size_t j = i; j < i + 4; ++j)
		{
			if (PokerUtil::getPointPriority(uniquePoints[j + 1]) -
					PokerUtil::getPointPriority(uniquePoints[j]) !=
				1)
			{
				continuous = false;
				break;
			}
		}
		if (continuous)
		{
			std::vector<Poker> straight;
			for (size_t j = i; j < i + 5; ++j)
			{
				for (const auto &card : sortedCards)
				{
					if (card.getPoint() == uniquePoints[j])
					{
						straight.push_back(card);
						break;
					}
				}
			}
			return straight;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestContinuousPair(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 6)
		return {};
	std::map<Point, int> pointCount;
	for (const auto &card : sortedCards)
	{
		pointCount[card.getPoint()]++;
	}
	std::vector<Point> pairPoints;
	for (const auto &[p, cnt] : pointCount)
	{
		if (cnt >= 2 && p != Point::TWO && p != Point::TINY_JOKER && p != Point::GIANT_JOKER)
		{
			pairPoints.push_back(p);
		}
	}
	for (size_t i = 0; i + 2 < pairPoints.size(); ++i)
	{
		bool continuous = true;
		for (size_t j = i; j < i + 2; ++j)
		{
			if (PokerUtil::getPointPriority(pairPoints[j + 1]) -
					PokerUtil::getPointPriority(pairPoints[j]) !=
				1)
			{
				continuous = false;
				break;
			}
		}
		if (continuous)
		{
			std::vector<Poker> result;
			for (size_t j = i; j < i + 3; ++j)
			{
				int added = 0;
				for (const auto &card : sortedCards)
				{
					if (card.getPoint() == pairPoints[j] && added < 2)
					{
						result.push_back(card);
						added++;
					}
				}
			}
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestTripleWithSingle(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 4)
		return {};
	std::map<Point, std::vector<Poker>> pointMap;
	for (const auto &card : sortedCards)
	{
		pointMap[card.getPoint()].push_back(card);
	}
	std::vector<Point> triplePoints;
	for (const auto &[p, cards] : pointMap)
	{
		if (cards.size() >= 3)
			triplePoints.push_back(p);
	}
	if (triplePoints.empty())
		return {};
	std::sort(triplePoints.begin(), triplePoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point targetTriple = triplePoints[0];
	std::vector<Poker> triple(pointMap[targetTriple].begin(), pointMap[targetTriple].begin() + 3);
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetTriple && !cards.empty())
		{
			std::vector<Poker> result = triple;
			result.push_back(cards[0]);
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestTripleWithPair(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 5)
		return {};
	if (sortedCards.size() < 6)
		return {};
	std::map<Point, std::vector<Poker>> pointMap;
	for (const auto &card : sortedCards)
	{
		pointMap[card.getPoint()].push_back(card);
	}
	std::vector<Point> triplePoints;
	for (const auto &[p, cards] : pointMap)
	{
		if (cards.size() >= 3)
			triplePoints.push_back(p);
	}
	if (triplePoints.empty())
		return {};
	std::sort(triplePoints.begin(), triplePoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point targetTriple = triplePoints[0];
	std::vector<Poker> triple(pointMap[targetTriple].begin(), pointMap[targetTriple].begin() + 3);
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetTriple && cards.size() >= 2)
		{
			std::vector<Poker> result = triple;
			result.push_back(cards[0]);
			result.push_back(cards[1]);
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestPlane(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 6)
		return {};
	std::map<Point, int> pointCount;
	for (const auto &card : sortedCards)
	{
		pointCount[card.getPoint()]++;
	}
	std::vector<Point> triplePoints;
	for (const auto &[p, cnt] : pointCount)
	{
		if (cnt >= 3 && p != Point::TWO && p != Point::TINY_JOKER && p != Point::GIANT_JOKER)
		{
			triplePoints.push_back(p);
		}
	}
	for (size_t i = 0; i + 1 < triplePoints.size(); ++i)
	{
		if (PokerUtil::getPointPriority(triplePoints[i + 1]) -
				PokerUtil::getPointPriority(triplePoints[i]) ==
			1)
		{
			std::vector<Poker> result;
			int added = 0;
			for (const auto &card : sortedCards)
			{
				if (card.getPoint() == triplePoints[i] && added < 3)
				{
					result.push_back(card);
					added++;
				}
			}
			added = 0;
			for (const auto &card : sortedCards)
			{
				if (card.getPoint() == triplePoints[i + 1] && added < 3)
				{
					result.push_back(card);
					added++;
				}
			}
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestPlaneWithWings(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 8)
		return {};
	auto plane = findSmallestPlane(sortedCards);
	if (plane.empty())
		return {};
	std::vector<Poker> remaining = sortedCards;
	std::map<Point, int> planePoints;
	for (const auto &card : plane)
		planePoints[card.getPoint()]++;
	for (auto it = remaining.begin(); it != remaining.end();)
	{
		if (planePoints[it->getPoint()] > 0)
		{
			planePoints[it->getPoint()]--;
			it = remaining.erase(it);
		}
		else
		{
			++it;
		}
	}
	if (remaining.size() >= 2)
	{
		std::vector<Poker> wings;
		wings.push_back(remaining[0]);
		wings.push_back(remaining[1]);
		plane.insert(plane.end(), wings.begin(), wings.end());
		return plane;
	}
	auto pair = findSmallestPair(remaining);
	if (!pair.empty() && remaining.size() >= 4)
	{
		std::vector<Poker> wings = pair;
		std::vector<Poker> remaining2 = remaining;
		for (auto it = remaining2.begin(); it != remaining2.end();)
		{
			if (it->getPoint() == pair[0].getPoint())
			{
				it = remaining2.erase(it);
			}
			else
			{
				++it;
			}
		}
		auto pair2 = findSmallestPair(remaining2);
		if (!pair2.empty())
		{
			wings.insert(wings.end(), pair2.begin(), pair2.end());
			plane.insert(plane.end(), wings.begin(), wings.end());
			return plane;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestFourWithTwo(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 6)
		return {};
	std::map<Point, std::vector<Poker>> pointMap;
	for (const auto &card : sortedCards)
	{
		pointMap[card.getPoint()].push_back(card);
	}
	std::vector<Point> fourPoints;
	for (const auto &[p, cards] : pointMap)
	{
		if (cards.size() >= 4)
			fourPoints.push_back(p);
	}
	if (fourPoints.empty())
		return {};
	std::sort(fourPoints.begin(), fourPoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point targetFour = fourPoints[0];
	std::vector<Poker> four(pointMap[targetFour].begin(), pointMap[targetFour].begin() + 4);
	std::vector<Poker> singles;
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetFour && !cards.empty())
		{
			singles.push_back(cards[0]);
			if (singles.size() >= 2)
				break;
		}
	}
	if (singles.size() >= 2)
	{
		four.insert(four.end(), singles.begin(), singles.begin() + 2);
		return four;
	}
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetFour && cards.size() >= 2)
		{
			four.push_back(cards[0]);
			four.push_back(cards[1]);
			return four;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findSmallestBomb(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 4)
		return {};
	for (size_t i = 0; i < sortedCards.size() - 3; ++i)
	{
		if (sortedCards[i].getPoint() == sortedCards[i + 1].getPoint() &&
			sortedCards[i].getPoint() == sortedCards[i + 2].getPoint() &&
			sortedCards[i].getPoint() == sortedCards[i + 3].getPoint())
		{
			return {sortedCards[i], sortedCards[i + 1], sortedCards[i + 2], sortedCards[i + 3]};
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findRoyalBomb(std::vector<Poker> &sortedCards)
{
	if (sortedCards.size() < 2)
		return {};
	bool hasTiny = false, hasGiant = false;
	Poker tiny, giant;
	for (const auto &card : sortedCards)
	{
		if (card.getPoint() == Point::TINY_JOKER)
		{
			hasTiny = true;
			tiny = card;
		}
		if (card.getPoint() == Point::GIANT_JOKER)
		{
			hasGiant = true;
			giant = card;
		}
	}
	if (hasTiny && hasGiant)
		return {tiny, giant};
	return {};
}
std::vector<Poker> PlayManager::findWinningSingle(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	Point lastPoint = lastPlay[0].getPoint();
	for (const auto &card : sortedCards)
	{
		if (PokerUtil::getPointPriority(card.getPoint()) > PokerUtil::getPointPriority(lastPoint))
		{
			return {card};
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningPair(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	Point lastPoint = lastPlay[0].getPoint();
	for (size_t i = 0; i < sortedCards.size() - 1; ++i)
	{
		if (sortedCards[i].getPoint() == sortedCards[i + 1].getPoint() &&
			PokerUtil::getPointPriority(sortedCards[i].getPoint()) > PokerUtil::getPointPriority(lastPoint))
		{
			return {sortedCards[i], sortedCards[i + 1]};
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningTriple(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	Point lastPoint = lastPlay[0].getPoint();
	for (size_t i = 0; i < sortedCards.size() - 2; ++i)
	{
		if (sortedCards[i].getPoint() == sortedCards[i + 1].getPoint() &&
			sortedCards[i].getPoint() == sortedCards[i + 2].getPoint() &&
			PokerUtil::getPointPriority(sortedCards[i].getPoint()) > PokerUtil::getPointPriority(lastPoint))
		{
			return {sortedCards[i], sortedCards[i + 1], sortedCards[i + 2]};
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningStraight(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	size_t targetLen = lastPlay.size();
	if (targetLen < 5)
		return {};
	std::vector<Poker> sortedLast = lastPlay;
	PokerUtil::sort(sortedLast);
	Point lastMaxPoint = sortedLast.back().getPoint();
	std::vector<Point> uniquePoints;
	for (const auto &card : sortedCards)
	{
		Point p = card.getPoint();
		if (p == Point::TWO || p == Point::TINY_JOKER || p == Point::GIANT_JOKER)
			continue;
		if (uniquePoints.empty() || uniquePoints.back() != p)
		{
			uniquePoints.push_back(p);
		}
	}
	for (size_t i = 0; i + targetLen - 1 < uniquePoints.size(); ++i)
	{
		bool continuous = true;
		for (size_t j = i; j < i + targetLen - 1; ++j)
		{
			if (PokerUtil::getPointPriority(uniquePoints[j + 1]) -
					PokerUtil::getPointPriority(uniquePoints[j]) !=
				1)
			{
				continuous = false;
				break;
			}
		}
		if (continuous && PokerUtil::getPointPriority(uniquePoints[i + targetLen - 1]) >
							  PokerUtil::getPointPriority(lastMaxPoint))
		{
			std::vector<Poker> straight;
			for (size_t j = i; j < i + targetLen; ++j)
			{
				for (const auto &card : sortedCards)
				{
					if (card.getPoint() == uniquePoints[j])
					{
						straight.push_back(card);
						break;
					}
				}
			}
			return straight;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningContinuousPair(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	size_t targetPairCount = lastPlay.size() / 2;
	if (targetPairCount < 3)
		return {};
	std::vector<Poker> sortedLast = lastPlay;
	PokerUtil::sort(sortedLast);
	Point lastMaxPoint = sortedLast.back().getPoint();
	std::map<Point, int> pointCount;
	for (const auto &card : sortedCards)
	{
		pointCount[card.getPoint()]++;
	}
	std::vector<Point> pairPoints;
	for (const auto &[p, cnt] : pointCount)
	{
		if (cnt >= 2 && p != Point::TWO && p != Point::TINY_JOKER && p != Point::GIANT_JOKER)
		{
			pairPoints.push_back(p);
		}
	}
	for (size_t i = 0; i + targetPairCount - 1 < pairPoints.size(); ++i)
	{
		bool continuous = true;
		for (size_t j = i; j < i + targetPairCount - 1; ++j)
		{
			if (PokerUtil::getPointPriority(pairPoints[j + 1]) -
					PokerUtil::getPointPriority(pairPoints[j]) !=
				1)
			{
				continuous = false;
				break;
			}
		}
		if (continuous && PokerUtil::getPointPriority(pairPoints[i + targetPairCount - 1]) >
							  PokerUtil::getPointPriority(lastMaxPoint))
		{
			std::vector<Poker> result;
			for (size_t j = i; j < i + targetPairCount; ++j)
			{
				int added = 0;
				for (const auto &card : sortedCards)
				{
					if (card.getPoint() == pairPoints[j] && added < 2)
					{
						result.push_back(card);
						added++;
					}
				}
			}
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningTripleWithSingle(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	auto lastCountMap = PokerTypeUtil::getPointCountMap(lastPlay);
	Point lastTriplePoint = {};
	for (const auto &[p, cnt] : lastCountMap)
	{
		if (cnt == 3)
			lastTriplePoint = p;
	}
	std::map<Point, std::vector<Poker>> pointMap;
	for (const auto &card : sortedCards)
	{
		pointMap[card.getPoint()].push_back(card);
	}
	std::vector<Point> triplePoints;
	for (const auto &[p, cards] : pointMap)
	{
		if (cards.size() >= 3 && PokerUtil::getPointPriority(p) > PokerUtil::getPointPriority(lastTriplePoint))
		{
			triplePoints.push_back(p);
		}
	}
	if (triplePoints.empty())
		return {};
	std::sort(triplePoints.begin(), triplePoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point targetTriple = triplePoints[0];
	std::vector<Poker> triple(pointMap[targetTriple].begin(), pointMap[targetTriple].begin() + 3);
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetTriple && !cards.empty())
		{
			std::vector<Poker> result = triple;
			result.push_back(cards[0]);
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningTripleWithPair(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	auto lastCountMap = PokerTypeUtil::getPointCountMap(lastPlay);
	Point lastTriplePoint = {};
	for (const auto &[p, cnt] : lastCountMap)
	{
		if (cnt == 3)
			lastTriplePoint = p;
	}
	std::map<Point, std::vector<Poker>> pointMap;
	for (const auto &card : sortedCards)
	{
		pointMap[card.getPoint()].push_back(card);
	}
	std::vector<Point> triplePoints;
	for (const auto &[p, cards] : pointMap)
	{
		if (cards.size() >= 3 && PokerUtil::getPointPriority(p) > PokerUtil::getPointPriority(lastTriplePoint))
		{
			triplePoints.push_back(p);
		}
	}
	if (triplePoints.empty())
		return {};
	std::sort(triplePoints.begin(), triplePoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point targetTriple = triplePoints[0];
	std::vector<Poker> triple(pointMap[targetTriple].begin(), pointMap[targetTriple].begin() + 3);
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetTriple && cards.size() >= 2)
		{
			std::vector<Poker> result = triple;
			result.push_back(cards[0]);
			result.push_back(cards[1]);
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningPlane(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	size_t targetTripleCount = lastPlay.size() / 3;
	if (targetTripleCount < 2)
		return {};
	auto lastCountMap = PokerTypeUtil::getPointCountMap(lastPlay);
	std::vector<Point> lastPlanePoints;
	for (const auto &[p, cnt] : lastCountMap)
	{
		if (cnt >= 3)
			lastPlanePoints.push_back(p);
	}
	std::sort(lastPlanePoints.begin(), lastPlanePoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point lastMinPlanePoint = lastPlanePoints[0];
	std::map<Point, int> pointCount;
	for (const auto &card : sortedCards)
	{
		pointCount[card.getPoint()]++;
	}
	std::vector<Point> triplePoints;
	for (const auto &[p, cnt] : pointCount)
	{
		if (cnt >= 3 && p != Point::TWO && p != Point::TINY_JOKER && p != Point::GIANT_JOKER)
		{
			triplePoints.push_back(p);
		}
	}
	for (size_t i = 0; i + targetTripleCount - 1 < triplePoints.size(); ++i)
	{
		bool continuous = true;
		for (size_t j = i; j < i + targetTripleCount - 1; ++j)
		{
			if (PokerUtil::getPointPriority(triplePoints[j + 1]) -
					PokerUtil::getPointPriority(triplePoints[j]) !=
				1)
			{
				continuous = false;
				break;
			}
		}
		if (continuous && PokerUtil::getPointPriority(triplePoints[i]) >
							  PokerUtil::getPointPriority(lastMinPlanePoint))
		{
			std::vector<Poker> result;
			for (size_t j = i; j < i + targetTripleCount; ++j)
			{
				int added = 0;
				for (const auto &card : sortedCards)
				{
					if (card.getPoint() == triplePoints[j] && added < 3)
					{
						result.push_back(card);
						added++;
					}
				}
			}
			return result;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningPlaneWithWings(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	auto plane = findWinningPlane(sortedCards, lastPlay);
	if (plane.empty())
		return {};
	std::vector<Poker> remaining = sortedCards;
	std::map<Point, int> planePoints;
	for (const auto &card : plane)
		planePoints[card.getPoint()]++;
	for (auto it = remaining.begin(); it != remaining.end();)
	{
		if (planePoints[it->getPoint()] > 0)
		{
			planePoints[it->getPoint()]--;
			it = remaining.erase(it);
		}
		else
		{
			++it;
		}
	}
	size_t lastWingCount = lastPlay.size() % 4 == 0 ? 1 : 2;
	size_t targetWingCount = plane.size() / 3;
	if (lastWingCount == 1)
	{
		if (remaining.size() >= targetWingCount)
		{
			std::vector<Poker> wings(remaining.begin(), remaining.begin() + static_cast<int>(targetWingCount));
			plane.insert(plane.end(), wings.begin(), wings.end());
			return plane;
		}
	}
	else
	{
		std::vector<Poker> wings;
		std::vector<Poker> temp = remaining;
		for (size_t i = 0; i < targetWingCount; ++i)
		{
			auto pair = findSmallestPair(temp);
			if (pair.empty())
				return {};
			wings.insert(wings.end(), pair.begin(), pair.end());
			for (auto it = temp.begin(); it != temp.end();)
			{
				if (it->getPoint() == pair[0].getPoint())
				{
					it = temp.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		plane.insert(plane.end(), wings.begin(), wings.end());
		return plane;
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningFourWithTwo(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	auto lastCountMap = PokerTypeUtil::getPointCountMap(lastPlay);
	Point lastFourPoint = {};
	for (const auto &[p, cnt] : lastCountMap)
	{
		if (cnt == 4)
			lastFourPoint = p;
	}
	std::map<Point, std::vector<Poker>> pointMap;
	for (const auto &card : sortedCards)
	{
		pointMap[card.getPoint()].push_back(card);
	}
	std::vector<Point> fourPoints;
	for (const auto &[p, cards] : pointMap)
	{
		if (cards.size() >= 4 && PokerUtil::getPointPriority(p) > PokerUtil::getPointPriority(lastFourPoint))
		{
			fourPoints.push_back(p);
		}
	}
	if (fourPoints.empty())
		return {};
	std::sort(fourPoints.begin(), fourPoints.end(), [](Point a, Point b)
			  { return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b); });
	Point targetFour = fourPoints[0];
	std::vector<Poker> four(pointMap[targetFour].begin(), pointMap[targetFour].begin() + 4);
	std::vector<Poker> singles;
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetFour && !cards.empty())
		{
			singles.push_back(cards[0]);
			if (singles.size() >= 2)
				break;
		}
	}
	if (singles.size() >= 2)
	{
		four.insert(four.end(), singles.begin(), singles.begin() + 2);
		return four;
	}
	for (const auto &[p, cards] : pointMap)
	{
		if (p != targetFour && cards.size() >= 2)
		{
			four.push_back(cards[0]);
			four.push_back(cards[1]);
			return four;
		}
	}
	return {};
}
std::vector<Poker> PlayManager::findWinningBomb(std::vector<Poker> &sortedCards, std::vector<Poker> &lastPlay)
{
	Point lastBombPoint = lastPlay[0].getPoint();
	for (size_t i = 0; i < sortedCards.size() - 3; ++i)
	{
		if (sortedCards[i].getPoint() == sortedCards[i + 1].getPoint() &&
			sortedCards[i].getPoint() == sortedCards[i + 2].getPoint() &&
			sortedCards[i].getPoint() == sortedCards[i + 3].getPoint() &&
			PokerUtil::getPointPriority(sortedCards[i].getPoint()) > PokerUtil::getPointPriority(lastBombPoint))
		{
			return {sortedCards[i], sortedCards[i + 1], sortedCards[i + 2], sortedCards[i + 3]};
		}
	}
	return {};
}
bool PlayManager::isGameOver()
{
	for (int i = 0; i < 3; i++)
	{
		const auto &player = gameState.getPlayers()[i];
		if (player->getPokers().empty())
		{
			gameState.setWinnerIndex(i);
			logger.logPlayerWon();
			isPlayFinished = true;
			return true;
		}
	}
	return false;
}
std::vector<Poker> PlayManager::getLastPlayedCards() const { return lastPlayedCards; }
void PlayManager::setLastPlayedCards(const std::vector<Poker> &cards) { lastPlayedCards = cards; }
int PlayManager::getLastPlayerIndex() const { return lastPlayerIndex; }
void PlayManager::setLastPlayerIndex(int index) { lastPlayerIndex = index; }
int PlayManager::getPassCount() const { return passCount; }
void PlayManager::setPassCount(int count) { passCount = count; }
void PlayManager::incrementPassCount()
{
	passCount++;
	if (passCount >= 2)
	{
		for (auto &player : gameState.getPlayers())
		{
			BasePlayer *basePlayer = dynamic_cast<BasePlayer *>(player.get());
			basePlayer->getHandOutPokers().clear();
			basePlayer->setPassed(false);
		}
		resetPassCount();
		lastPlayedCards = {};
		lastPlayerIndex = -1;
	}
}
void PlayManager::resetPassCount() { passCount = 0; }
bool PlayManager::isPlayDone() const { return isPlayFinished; }