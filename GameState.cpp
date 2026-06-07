#include "GameState.h"

GameState &GameState::getInstance()
{
    static GameState instance;
    return instance;
}
void GameState::setBottom(const std::vector<Poker> &newBottom) { bottom = newBottom; }
const std::vector<Poker> &GameState::getBottom() const { return bottom; }
std::vector<Poker> &GameState::getBottom() { return bottom; }
std::vector<std::unique_ptr<IPlayer>> &GameState::getPlayers() { return players; }
void GameState::setBiddingPhase(bool isBidding) { isBiddingPhase = isBidding; }
bool GameState::isInBiddingPhase() const { return isBiddingPhase; }
void GameState::setPlayingPhase(bool isPlaying) { isPlayingPhase = isPlaying; }
bool GameState::isInPlayingPhase() const { return isPlayingPhase; }
void GameState::setLandlordIndex(int index) { landlordIndex = index; }
int GameState::getLandlordIndex() const { return landlordIndex; }
void GameState::setCurrentBidderIndex(int index) { currentBidderIndex = index; }
int GameState::getCurrentBidderIndex() const { return currentBidderIndex; }
void GameState::setCurrentPlayerIndex(int index) { currentPlayerIndex = index; }
int GameState::getCurrentPlayerIndex() const { return currentPlayerIndex; }
void GameState::setThinkingPlayerIndex(int index) { thinkingPlayerIndex = index; }
int GameState::getThinkingPlayerIndex() const { return thinkingPlayerIndex; }
int GameState::getWinnerIndex() const { return winnerIndex; }
void GameState::setWinnerIndex(int index) { winnerIndex = index; }