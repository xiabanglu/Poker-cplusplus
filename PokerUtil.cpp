#include "PokerUtil.h"
#include <sstream>
#include <random>
#include <array>

std::vector<Poker> PokerUtil::generateDeck()
{
    std::vector<Poker> deck;
    std::array<Suit, 4> suits = {Suit::SPADE, Suit::HEART, Suit::CLUB, Suit::DIAMOND};
    std::array<Point, 13> points = {
        Point::ACE, Point::TWO, Point::THREE, Point::FOUR, Point::FIVE, Point::SIX,
        Point::SEVEN, Point::EIGHT, Point::NINE, Point::TEN, Point::JACK, Point::QUEEN, Point::KING};
    // 生成4种花色的13张普通牌
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            std::basic_stringstream<TCHAR> tss;
            tss << _T("img/poker/") << (i + 1) << _T("-") << (j + 1) << _T(".png");
            deck.emplace_back(suits[i], points[j], tss.str().c_str());
        }
    }
    // 添加大小王
    deck.emplace_back(Suit::NONE, Point::TINY_JOKER, _T("img/poker/5-1.png"));
    deck.emplace_back(Suit::NONE, Point::GIANT_JOKER, _T("img/poker/5-2.png"));
    return deck;
}
void PokerUtil::shuffle(std::vector<Poker> &deck)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}
void PokerUtil::sort(std::vector<Poker> &pokers)
{
    std::sort(pokers.begin(), pokers.end(), PokerUtil::compareCards);
}
const std::unordered_map<Suit, int> PokerUtil::suitPriority = {
    {Suit::SPADE, 0}, {Suit::HEART, 1}, {Suit::CLUB, 2}, {Suit::DIAMOND, 3}, {Suit::NONE, 4}};
const std::unordered_map<Point, int> PokerUtil::pointPriority = {
    {Point::THREE, 1}, {Point::FOUR, 2}, {Point::FIVE, 3}, {Point::SIX, 4}, {Point::SEVEN, 5}, {Point::EIGHT, 6}, {Point::NINE, 7}, {Point::TEN, 8}, {Point::JACK, 9}, {Point::QUEEN, 10}, {Point::KING, 11}, {Point::ACE, 12}, {Point::TWO, 13}, {Point::TINY_JOKER, 14}, {Point::GIANT_JOKER, 15}};
int PokerUtil::getSuitPriority(const Suit &suit)
{
    return suitPriority.at(suit);
}
int PokerUtil::getPointPriority(const Point &point)
{
    return pointPriority.at(point);
}
bool PokerUtil::compareCards(const Poker &a, const Poker &b)
{
    int aPoint = getPointPriority(a.getPoint());
    int bPoint = getPointPriority(b.getPoint());
    if (aPoint != bPoint)
    {
        return aPoint < bPoint;
    }
    return getSuitPriority(a.getSuit()) < getSuitPriority(b.getSuit());
}