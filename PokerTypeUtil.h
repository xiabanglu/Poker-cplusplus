#ifndef POKERTYPEUTIL_H
#define POKERTYPEUTIL_H

#include <vector>
#include <map>
#include <functional>
#include "Poker.h"
#include "PokerType.h"

class PokerTypeUtil
{
public:
    static PokerType getPokerType(std::vector<Poker> &pokers);         // 获取牌型
    static bool compare(std::vector<Poker> &a, std::vector<Poker> &b); // 比较牌型大小

    // 牌型判断函数
    static bool isRoyalBomb(std::vector<Poker> &pokers);
    static bool isBomb(std::vector<Poker> &pokers);
    static bool isSingle(std::vector<Poker> &pokers);
    static bool isPair(std::vector<Poker> &pokers);
    static bool isTriple(std::vector<Poker> &pokers);
    static bool isStraight(std::vector<Poker> &pokers);
    static bool isContinuousPair(std::vector<Poker> &pokers);
    static bool isTripleWithSingle(std::vector<Poker> &pokers);
    static bool isTripleWithPair(std::vector<Poker> &pokers);
    static bool isPlane(std::vector<Poker> &pokers);
    static bool isPlaneWithWings(std::vector<Poker> &pokers);
    static bool isFourWithTwo(std::vector<Poker> &pokers);
    static std::map<Point, int, std::function<bool(Point, Point)>> getPointCountMap(std::vector<Poker> &pokers); // 获取点数计数映射
    static bool isPointsContinuous(std::vector<Point> &points);                                                  // 判断点数是否连续
};

#endif // POKERTYPEUTIL_H
