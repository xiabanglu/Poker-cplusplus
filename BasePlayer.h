#ifndef BASEPLAYER_H
#define BASEPLAYER_H

#include "IPlayer.h"
#include <vector>
#include "Poker.h"

class BasePlayer : public IPlayer
{
protected:
    std::vector<Poker> pokers;        // 手牌
    std::vector<Poker> handOutPokers; // 想要打出的牌
    const TCHAR *name;                // 名称
    bool isAI;                        // 是否是AI
    int bid;                          // 叫分
    bool isPassed = false;            // 标记是否主动选择不出

public:
    BasePlayer(const TCHAR *playerName, bool isAIFlag, int bid) : name(playerName), isAI(isAIFlag), bid(bid) {}
    BasePlayer(const BasePlayer &other) = default;
    BasePlayer(BasePlayer &&other) = default;
    BasePlayer &operator=(const BasePlayer &other) = default;
    BasePlayer &operator=(BasePlayer &&other) = default;
    ~BasePlayer() = default;

    void addPoker(const Poker &poker) override;
    void removePoker(size_t index) override;

    // Getter和Setter
    void setPassed(bool passed);
    bool getPassed() const;
    std::vector<Poker> &getPokers() override;
    const std::vector<Poker> &getPokers() const override;
    std::vector<Poker> &getHandOutPokers() override;
    const std::vector<Poker> &getHandOutPokers() const override;
    const TCHAR *getName() const override;
    void setBid(int bidValue) override;
    int getBid() const override;
    bool isAIPlayer() const override;
};

#endif // BASEPLAYER_H