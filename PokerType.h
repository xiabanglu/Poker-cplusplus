#ifndef POKERTYPE_H
#define POKERTYPE_H

enum class PokerType
{
    INVALID,            // 无效牌型
    SINGLE,             // 单牌：任意一张单独的牌
    PAIR,               // 对子：两张点数相同的牌
    TRIPLE,             // 三个：三张点数相同的牌
    STRAIGHT,           // 顺子：五张及以上连续点数的单牌
    CONTINUOUS_PAIR,    // 连对：三对及以上连续点数的对子
    BOMB,               // 炸弹：四张点数相同的牌
    TRIPLE_WITH_SINGLE, // 三带一：三张+一张单牌
    TRIPLE_WITH_PAIR,   // 三带二：三张+一对
    PLANE,              // 飞机：二组及以上连续的三张
    PLANE_WITH_WINGS,   // 飞机带翅膀：飞机+对应数量的单牌/对子
    FOUR_WITH_TWO,      // 四带二：四张+两张单牌
    ROYAL_BOMB          // 王炸：大小王组合
};

#endif // POKERTYPE_H