#ifndef IPLAYER_H
#define IPLAYER_H

#include <vector>
#include "Poker.h"

class IPlayer
{
protected:
    IPlayer() = default;

public:
    virtual ~IPlayer() = default;
    IPlayer(const IPlayer &) = delete;
    IPlayer &operator=(const IPlayer &) = delete;
    IPlayer(IPlayer &&) = delete;
    IPlayer &operator=(IPlayer &&) = delete;

    virtual void drawCards() const = 0;        // 绘制手牌
    virtual void drawHandOutCards() const = 0; // 绘制出的牌
    virtual void drawNameAndCount() const = 0; // 绘制名称和手牌数
    virtual void drawBid() const = 0;          // 绘制叫分
    virtual void drawLandlord() const = 0;     // 绘制地主标志

    // Getter和Setter
    virtual std::vector<Poker> &getPokers() = 0;
    virtual const std::vector<Poker> &getPokers() const = 0;
    virtual std::vector<Poker> &getHandOutPokers() = 0;
    virtual const std::vector<Poker> &getHandOutPokers() const = 0;
    virtual const TCHAR *getName() const = 0;
    virtual void setBid(int bidValue) = 0;
    virtual int getBid() const = 0;
    virtual bool isAIPlayer() const = 0;

    virtual void addPoker(const Poker &poker) = 0; // 添加手牌
    virtual void removePoker(size_t index) = 0;    // 删除手牌
};

#endif // IPLAYER_H