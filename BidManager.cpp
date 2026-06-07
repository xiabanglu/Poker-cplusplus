#include "BidManager.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "PokerUtil.h"
#include <easyx.h>
#include "Render.h"

BidManager &BidManager::getInstance()
{
    static BidManager instance;
    return instance;
}
bool BidManager::humanBid(const ExMessage &msg)
{
    if (isBidFinished)
        return false;
    if (gameState.getCurrentBidderIndex() != 0)
        return false;
    gameState.setThinkingPlayerIndex(gameState.getCurrentBidderIndex());
    Render::drawAll();
    FlushBatchDraw();
    if (auto *human = dynamic_cast<HumanPlayer *>(gameState.getPlayers()[gameState.getCurrentBidderIndex()].get()))
    {
        int bid = human->bid(msg);
        if (bid != -1)
        {
            processBid(bid);
            Render::drawAll();
            FlushBatchDraw();
            return true;
        }
    }
    return false;
}
bool BidManager::aiBid()
{
    if (isBidFinished)
        return false;
    if (gameState.getCurrentBidderIndex() == 0)
        return false;
    gameState.setThinkingPlayerIndex(gameState.getCurrentBidderIndex());
    Render::drawAll();
    FlushBatchDraw();
    if (auto *ai = dynamic_cast<AIPlayer *>(gameState.getPlayers()[gameState.getCurrentBidderIndex()].get()))
    {
        int bid = ai->bid();
        processBid(bid);
        Sleep(2000);
        Render::drawAll();
        FlushBatchDraw();
        return true;
    }
    return false;
}
void BidManager::processBid(int bid)
{
    logger.logPlayerBid(gameState.getPlayers()[gameState.getCurrentBidderIndex()]->getName(), bid);
    gameState.getPlayers()[gameState.getCurrentBidderIndex()]->setBid(bid);
    if (bid > 0 && bid > highestBid)
    {
        highestBid = bid;
        highestBidPlayerIndex = gameState.getCurrentBidderIndex();
    }
    if (bid == 3)
    {
        highestBid = 3;
        highestBidPlayerIndex = gameState.getCurrentBidderIndex();
        isBidFinished = true;
        determineLandlord();
        return;
    }
    nextBidder();
    bool allBidded = true;
    for (int i = 0; i < 3; i++)
    {
        if (gameState.getPlayers()[i]->getBid() == -1)
        {
            allBidded = false;
            break;
        }
    }
    if (allBidded)
    {
        isBidFinished = true;
        determineLandlord();
    }
}
void BidManager::nextBidder()
{
    gameState.setCurrentBidderIndex((gameState.getCurrentBidderIndex() + 1) % 3);
    gameState.setThinkingPlayerIndex(gameState.getCurrentBidderIndex());
}
void BidManager::determineLandlord()
{
    if (highestBidPlayerIndex == -1)
    {
        highestBidPlayerIndex = 0;
    }
    logger.logLandlordDetermined(gameState.getPlayers()[highestBidPlayerIndex]->getName());
    gameState.setBiddingPhase(false);
    gameState.setPlayingPhase(true);
    gameState.setCurrentBidderIndex(-1);
    gameState.setCurrentPlayerIndex(highestBidPlayerIndex);
    gameState.setLandlordIndex(highestBidPlayerIndex);
    gameState.setThinkingPlayerIndex(gameState.getLandlordIndex());
    auto &landlord = gameState.getPlayers()[gameState.getLandlordIndex()];
    for (const auto &poker : gameState.getBottom())
    {
        landlord->addPoker(poker);
    }
    PokerUtil::sort(landlord->getPokers());
    if (gameState.getLandlordIndex() == 0)
    {
        dynamic_cast<HumanPlayer *>(landlord.get())->initSelectedState();
    }
}
void BidManager::setHighestBid(int bid) { highestBid = bid; }
int BidManager::getHighestBid() const { return highestBid; }
void BidManager::setHighestBidPlayerIndex(int index) { highestBidPlayerIndex = index; }
int BidManager::getHighestBidPlayerIndex() const { return highestBidPlayerIndex; }
bool BidManager::isBidDone() const { return isBidFinished; }