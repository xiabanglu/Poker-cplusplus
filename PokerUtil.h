#ifndef POKERUTIL_H
#define POKERUTIL_H

#include "PokerUtil.h"
#include "Poker.h"
#include <unordered_map>

class PokerUtil
{
public:
    static std::vector<Poker> generateDeck();                  // 生成一副完整的扑克牌
    static void shuffle(std::vector<Poker> &deck);             // 洗牌
    static void sort(std::vector<Poker> &pokers);              // 对牌进行排序
    static const std::unordered_map<Suit, int> suitPriority;   // 花色优先级映射表（从小到大）
    static const std::unordered_map<Point, int> pointPriority; // 点数优先级映射表（从小到大）
    static int getSuitPriority(const Suit &suit);              // 获取牌的花色优先级
    static int getPointPriority(const Point &point);           // 获取牌的点数优先级
    static bool compareCards(const Poker &a, const Poker &b);  // 排序比较函数（用于排序手牌/底牌）
};

#endif // POKERUTIL_H