#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include "Poker.h"
#include "IPlayer.h"
#include <memory>

class GameState
{
private:
    std::vector<Poker> bottom;                     // 底牌
    std::vector<std::unique_ptr<IPlayer>> players; // 玩家列表
    bool isBiddingPhase = true;                    // 是否处于叫分阶段
    bool isPlayingPhase = false;                   // 是否处于出牌阶段
    int landlordIndex = -1;                        // 地主索引
    int currentBidderIndex = 0;                    // 当前叫分玩家（0=玩家，1=AI1，2=AI2）
    int currentPlayerIndex = -1;                   // 当前出牌玩家（0=玩家，1=AI1，2=AI2）
    int thinkingPlayerIndex = 0;                   // 正在思考的玩家索引
    int winnerIndex = -1;                          // 胜利玩家索引

    GameState() = default;
    ~GameState() = default;
    GameState(const GameState &) = delete;
    GameState &operator=(const GameState &) = delete;
    GameState(GameState &&) = delete;
    GameState &operator=(GameState &&) = delete;

public:
    static GameState &getInstance();

    // Getter和Setter
    void setBottom(const std::vector<Poker> &newBottom);
    const std::vector<Poker> &getBottom() const;
    std::vector<Poker> &getBottom();
    std::vector<std::unique_ptr<IPlayer>> &getPlayers();
    void setBiddingPhase(bool isBidding);
    bool isInBiddingPhase() const;
    void setPlayingPhase(bool isPlaying);
    bool isInPlayingPhase() const;
    void setLandlordIndex(int index);
    int getLandlordIndex() const;
    void setCurrentBidderIndex(int index);
    int getCurrentBidderIndex() const;
    void setCurrentPlayerIndex(int index);
    int getCurrentPlayerIndex() const;
    void setThinkingPlayerIndex(int index);
    int getThinkingPlayerIndex() const;
    int getWinnerIndex() const;
    void setWinnerIndex(int index);
};

#endif // GAMESTATE_H