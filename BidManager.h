#ifndef BIDMANAGER_H
#define BIDMANAGER_H

#include "GameState.h"
#include <easyx.h>
#include "Logger.h"

class BidManager
{
private:
    GameState &gameState = GameState::getInstance();
    Logger &logger = Logger::getInstance();

    int highestBid = 0;             // 当前最高叫分
    int highestBidPlayerIndex = -1; // 当前最高叫分玩家索引
    bool isBidFinished = false;     // 叫分是否结束

    BidManager() = default;
    ~BidManager() = default;
    BidManager(const BidManager &) = delete;
    BidManager &operator=(const BidManager &) = delete;
    BidManager(BidManager &&) = delete;
    BidManager &operator=(BidManager &&) = delete;

public:
    static BidManager &getInstance();

    bool humanBid(const ExMessage &msg); // 人类叫分
    bool aiBid();                        // AI叫分
    void processBid(int bid);            // 处理叫分
    void nextBidder();                   // 切换到下一个叫分玩家
    void determineLandlord();            // 判定地主并切换到出牌阶段

    // Getter和Setter
    void setHighestBid(int bid);
    int getHighestBid() const;
    void setHighestBidPlayerIndex(int index);
    int getHighestBidPlayerIndex() const;
    bool isBidDone() const;
};

#endif // BIDMANAGER_H