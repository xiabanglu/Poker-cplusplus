#include "Poker.h"
#include <string>

Suit Poker::getSuit() const { return suit; }
Point Poker::getPoint() const { return point; }
std::basic_string<TCHAR> Poker::getImgUrl() const { return imgUrl; }