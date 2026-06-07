#include "BasePlayer.h"

void BasePlayer::addPoker(const Poker &poker)
{
	pokers.push_back(poker);
}
void BasePlayer::removePoker(size_t index)
{
	if (index >= 0 && index < pokers.size())
	{
		pokers.erase(pokers.begin() + static_cast<int>(index));
	}
}
void BasePlayer::setPassed(bool passed) { isPassed = passed; }
bool BasePlayer::getPassed() const { return isPassed; }
std::vector<Poker> &BasePlayer::getPokers() { return pokers; }
const std::vector<Poker> &BasePlayer::getPokers() const { return pokers; }
std::vector<Poker> &BasePlayer::getHandOutPokers() { return handOutPokers; }
const std::vector<Poker> &BasePlayer::getHandOutPokers() const { return handOutPokers; }
const TCHAR *BasePlayer::getName() const { return name; }
void BasePlayer::setBid(int bidValue) { bid = bidValue; }
int BasePlayer::getBid() const { return bid; }
bool BasePlayer::isAIPlayer() const { return isAI; }