#ifndef POKER_H
#define POKER_H

#include <tchar.h>
#include <string>

enum class Suit
{
    SPADE,
    HEART,
    CLUB,
    DIAMOND,
    NONE
}; // 花色枚举
enum class Point
{
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    TINY_JOKER,
    GIANT_JOKER
}; // 点数枚举

class Poker
{
private:
    Suit suit = {};                  // 花色
    Point point = {};                // 点数
    std::basic_string<TCHAR> imgUrl; // 图片地址
public:
    Poker() = default;
    Poker(Suit suit, Point point, std::basic_string<TCHAR> imgUrl) : suit(suit), point(point), imgUrl(imgUrl) {};
    Poker(const Poker &other) = default;
    Poker(Poker &&other) = default;
    Poker &operator=(const Poker &other) = default;
    Poker &operator=(Poker &&other) = default;
    ~Poker() = default;

    // Getter和Setter
    Suit getSuit() const;
    Point getPoint() const;
    std::basic_string<TCHAR> getImgUrl() const;
};

#endif // POKER_H