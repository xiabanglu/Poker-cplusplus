#include "PokerTypeUtil.h"
#include <map>
#include "Poker.h"
#include "PokerUtil.h"
#include <functional>

const std::map<PokerType, int> TYPE_PRIORITY = {
	{PokerType::INVALID, 0},
	{PokerType::SINGLE, 1},
	{PokerType::PAIR, 2},
	{PokerType::TRIPLE, 3},
	{PokerType::STRAIGHT, 4},
	{PokerType::CONTINUOUS_PAIR, 5},
	{PokerType::TRIPLE_WITH_SINGLE, 6},
	{PokerType::TRIPLE_WITH_PAIR, 7},
	{PokerType::PLANE, 8},
	{PokerType::PLANE_WITH_WINGS, 9},
	{PokerType::FOUR_WITH_TWO, 10},
	{PokerType::BOMB, 11},
	{PokerType::ROYAL_BOMB, 12}};
PokerType PokerTypeUtil::getPokerType(std::vector<Poker> &pokers)
{
	if (pokers.empty())
		return PokerType::INVALID;
	std::vector<Poker> &sortedPokers = pokers;
	PokerUtil::sort(sortedPokers);
	// 按优先级判定牌型（王炸>炸弹>其他）
	if (isRoyalBomb(sortedPokers))
		return PokerType::ROYAL_BOMB;
	if (isBomb(sortedPokers))
		return PokerType::BOMB;
	if (isSingle(sortedPokers))
		return PokerType::SINGLE;
	if (isPair(sortedPokers))
		return PokerType::PAIR;
	if (isTriple(sortedPokers))
		return PokerType::TRIPLE;
	if (isStraight(sortedPokers))
		return PokerType::STRAIGHT;
	if (isContinuousPair(sortedPokers))
		return PokerType::CONTINUOUS_PAIR;
	if (isTripleWithSingle(sortedPokers))
		return PokerType::TRIPLE_WITH_SINGLE;
	if (isTripleWithPair(sortedPokers))
		return PokerType::TRIPLE_WITH_PAIR;
	if (isFourWithTwo(sortedPokers))
		return PokerType::FOUR_WITH_TWO;
	if (isPlane(sortedPokers))
		return PokerType::PLANE;
	if (isPlaneWithWings(sortedPokers))
		return PokerType::PLANE_WITH_WINGS;
	return PokerType::INVALID;
}
bool PokerTypeUtil::compare(std::vector<Poker> &a, std::vector<Poker> &b)
{
	PokerType typeA = getPokerType(a);
	PokerType typeB = getPokerType(b);
	if (typeA == PokerType::INVALID || typeB == PokerType::INVALID)
		return false;
	// 王炸压制所有
	if (typeA == PokerType::ROYAL_BOMB)
	{
		return typeB != PokerType::ROYAL_BOMB;
	}
	if (typeB == PokerType::ROYAL_BOMB)
	{
		return false;
	}
	// 炸弹压制非炸弹
	if (typeA == PokerType::BOMB)
	{
		if (typeB == PokerType::BOMB)
		{
			std::vector<Poker> sortedA = a;
			std::vector<Poker> sortedB = b;
			PokerUtil::sort(sortedA);
			PokerUtil::sort(sortedB);
			return PokerUtil::getPointPriority(sortedA[0].getPoint()) > PokerUtil::getPointPriority(sortedB[0].getPoint());
		}
		return true;
	}
	if (typeB == PokerType::BOMB)
	{
		return false;
	}
	if (typeA != typeB)
	{
		return false;
	}
	// 同牌型比较点数
	std::vector<Poker> sortedA = a;
	std::vector<Poker> sortedB = b;
	PokerUtil::sort(sortedA);
	PokerUtil::sort(sortedB);
	switch (typeA)
	{
	case PokerType::ROYAL_BOMB:
		return false;
	case PokerType::BOMB:
		return PokerUtil::getPointPriority(sortedA[0].getPoint()) > PokerUtil::getPointPriority(sortedB[0].getPoint());
	case PokerType::TRIPLE_WITH_SINGLE:
	case PokerType::TRIPLE_WITH_PAIR:
	{
		auto countA = getPointCountMap(sortedA);
		auto countB = getPointCountMap(sortedB);
		Point tripleA = {}, tripleB = {};
		for (auto &[p, c] : countA)
			if (c == 3)
				tripleA = p;
		for (auto &[p, c] : countB)
			if (c == 3)
				tripleB = p;
		return PokerUtil::getPointPriority(tripleA) > PokerUtil::getPointPriority(tripleB);
	}
	case PokerType::STRAIGHT:
	case PokerType::CONTINUOUS_PAIR:
	{
		size_t sizeA = sortedA.size();
		size_t sizeB = sortedB.size();
		if (sizeA != sizeB)
			return false;
		return PokerUtil::getPointPriority(sortedA[sizeA - 1].getPoint()) > PokerUtil::getPointPriority(sortedB[sizeA - 1].getPoint());
	}
	case PokerType::PLANE:
	case PokerType::PLANE_WITH_WINGS:
	{
		if (sortedA.size() != sortedB.size())
			return false;
		auto countA = getPointCountMap(sortedA);
		auto countB = getPointCountMap(sortedB);
		std::vector<Point> planesA, planesB;
		for (auto &[p, c] : countA)
			if (c >= 3)
				planesA.push_back(p);
		for (auto &[p, c] : countB)
			if (c >= 3)
				planesB.push_back(p);
		return PokerUtil::getPointPriority(planesA[0]) > PokerUtil::getPointPriority(planesB[0]);
	}
	case PokerType::FOUR_WITH_TWO:
	{
		auto countA = getPointCountMap(sortedA);
		auto countB = getPointCountMap(sortedB);
		Point fourA = {}, fourB = {};
		for (auto &[p, c] : countA)
			if (c == 4)
				fourA = p;
		for (auto &[p, c] : countB)
			if (c == 4)
				fourB = p;
		return PokerUtil::getPointPriority(fourA) > PokerUtil::getPointPriority(fourB);
	}
	case PokerType::PAIR:
	case PokerType::TRIPLE:
	case PokerType::SINGLE:
		return PokerUtil::getPointPriority(sortedA[0].getPoint()) > PokerUtil::getPointPriority(sortedB[0].getPoint());
	default:
		return false;
	}
}
bool PokerTypeUtil::isRoyalBomb(std::vector<Poker> &pokers)
{
	if (pokers.size() != 2)
		return false;
	bool hasGiant = false, hasTiny = false;
	for (const auto &p : pokers)
	{
		if (p.getPoint() == Point::GIANT_JOKER)
			hasGiant = true;
		if (p.getPoint() == Point::TINY_JOKER)
			hasTiny = true;
	}
	return hasGiant && hasTiny;
}
bool PokerTypeUtil::isBomb(std::vector<Poker> &pokers)
{
	if (pokers.size() != 4)
		return false;
	Point firstPoint = pokers[0].getPoint();
	if (firstPoint == Point::GIANT_JOKER || firstPoint == Point::TINY_JOKER)
		return false;
	for (const auto &p : pokers)
	{
		if (p.getPoint() != firstPoint)
			return false;
	}
	return true;
}
bool PokerTypeUtil::isSingle(std::vector<Poker> &pokers)
{
	return pokers.size() == 1;
}
bool PokerTypeUtil::isPair(std::vector<Poker> &pokers)
{
	if (pokers.size() != 2)
		return false;
	Point p1 = pokers[0].getPoint();
	Point p2 = pokers[1].getPoint();
	if (p1 == Point::GIANT_JOKER || p1 == Point::TINY_JOKER)
		return false;
	return p1 == p2;
}
bool PokerTypeUtil::isTriple(std::vector<Poker> &pokers)
{
	if (pokers.size() != 3)
		return false;
	Point firstPoint = pokers[0].getPoint();
	if (firstPoint == Point::GIANT_JOKER || firstPoint == Point::TINY_JOKER)
		return false;
	for (const auto &p : pokers)
	{
		if (p.getPoint() != firstPoint)
			return false;
	}
	return true;
}
bool PokerTypeUtil::isStraight(std::vector<Poker> &pokers)
{
	size_t size = pokers.size();
	if (size < 5)
		return false;
	for (const auto &p : pokers)
	{
		Point pt = p.getPoint();
		if (pt == Point::TWO || pt == Point::GIANT_JOKER || pt == Point::TINY_JOKER)
			return false;
	}
	std::vector<Point> points;
	for (const auto &p : pokers)
		points.push_back(p.getPoint());
	return isPointsContinuous(points);
}
bool PokerTypeUtil::isContinuousPair(std::vector<Poker> &pokers)
{
	size_t size = pokers.size();
	if (size < 6 || size % 2 != 0)
		return false;
	auto countMap = getPointCountMap(pokers);
	for (auto &[p, c] : countMap)
	{
		if (c != 2)
			return false;
		if (p == Point::TWO || p == Point::GIANT_JOKER || p == Point::TINY_JOKER)
			return false;
	}
	std::vector<Point> points;
	for (auto &[p, c] : countMap)
		points.push_back(p);
	return points.size() == size / 2 && isPointsContinuous(points);
}
bool PokerTypeUtil::isTripleWithSingle(std::vector<Poker> &pokers)
{
	if (pokers.size() != 4)
		return false;
	auto countMap = getPointCountMap(pokers);
	if (countMap.size() != 2)
		return false;
	bool hasTriple = false, hasSingle = false;
	for (auto &[p, c] : countMap)
	{
		if (c == 3)
			hasTriple = true;
		else if (c == 1)
			hasSingle = true;
		else
			return false;
	}
	return hasTriple && hasSingle;
}
bool PokerTypeUtil::isTripleWithPair(std::vector<Poker> &pokers)
{
	if (pokers.size() != 5)
		return false;
	auto countMap = getPointCountMap(pokers);
	if (countMap.size() != 2)
		return false;
	bool hasTriple = false, hasPair = false;
	for (auto &[p, c] : countMap)
	{
		if (c == 3)
			hasTriple = true;
		else if (c == 2)
			hasPair = true;
		else
			return false;
	}
	return hasTriple && hasPair;
}
bool PokerTypeUtil::isPlane(std::vector<Poker> &pokers)
{
	size_t size = pokers.size();
	if (size < 6 || size % 3 != 0)
		return false;
	auto countMap = getPointCountMap(pokers);
	for (auto &[p, c] : countMap)
	{
		if (c != 3)
			return false;
		if (p == Point::TWO || p == Point::GIANT_JOKER || p == Point::TINY_JOKER)
			return false;
	}
	std::vector<Point> points;
	for (auto &[p, c] : countMap)
		points.push_back(p);
	return points.size() == size / 3 && isPointsContinuous(points);
}
bool PokerTypeUtil::isPlaneWithWings(std::vector<Poker> &pokers)
{
	size_t size = pokers.size();
	if (size % 4 != 0 && size % 5 != 0)
		return false;
	size_t n = (size % 4 == 0) ? size / 4 : size / 5;
	if (n < 2)
		return false;
	auto countMap = getPointCountMap(pokers);
	std::vector<Point> planePoints;
	std::vector<Point> wingPoints;
	for (auto &[p, c] : countMap)
	{
		if (c == 3)
			planePoints.push_back(p);
		else if (size % 4 == 0 && c == 1)
			wingPoints.push_back(p);
		else if (size % 5 == 0 && c == 2)
			wingPoints.push_back(p);
		else
			return false;
	}
	if (planePoints.size() != n || wingPoints.size() != n)
		return false;
	return isPointsContinuous(planePoints);
}
bool PokerTypeUtil::isFourWithTwo(std::vector<Poker> &pokers)
{
	if (pokers.size() != 6)
		return false;
	auto countMap = getPointCountMap(pokers);
	if (countMap.size() != 3 && countMap.size() != 2)
		return false;
	bool hasFour = false;
	int singleCount = 0;
	for (auto &[p, c] : countMap)
	{
		if (c == 4)
			hasFour = true;
		else if (c == 1)
			singleCount++;
		else
			return false;
	}
	return hasFour && (singleCount == 2);
}
std::map<Point, int, std::function<bool(Point, Point)>> PokerTypeUtil::getPointCountMap(std::vector<Poker> &pokers)
{
	auto cmp = std::function<bool(Point, Point)>(
		[](Point a, Point b)
		{
			return PokerUtil::getPointPriority(a) < PokerUtil::getPointPriority(b);
		});
	std::map<Point, int, std::function<bool(Point, Point)>> countMap(cmp);
	for (const auto &p : pokers)
	{
		countMap[p.getPoint()]++;
	}
	return countMap;
}
bool PokerTypeUtil::isPointsContinuous(std::vector<Point> &points)
{
	for (size_t i = 1; i < points.size(); ++i)
	{
		int prev = PokerUtil::getPointPriority(points[i - 1]);
		int curr = PokerUtil::getPointPriority(points[i]);
		if (curr - prev != 1)
			return false;
	}
	return true;
}