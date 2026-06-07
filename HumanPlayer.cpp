#include "HumanPlayer.h"
#include "Render.h"
#include <easyx.h>
#include <algorithm>
#include "GameState.h"
#include "PlayManager.h"

int HumanPlayer::bid(const ExMessage &msg)
{
	return inputManager.clickWhichBidButton(msg);
}
bool HumanPlayer::play(const ExMessage &msg)
{
	int choice = inputManager.clickWhichPlayButton(msg);
	if (choice == 1)
	{
		auto *human = dynamic_cast<HumanPlayer *>(gameState.getPlayers()[0].get());
		std::vector<size_t> selectedIndices;
		for (size_t i = 0; i < human->getPokerSelectedStates().size(); ++i)
		{
			if (human->getPokerSelectedStates()[i])
				selectedIndices.push_back(i);
		}
		if (selectedIndices.empty())
			return false;
		std::vector<Poker> selectedCards;
		for (size_t idx : selectedIndices)
		{
			selectedCards.push_back(human->getPokers()[idx]);
		}
		if (playManager.isValidPlay(selectedCards))
		{
			std::sort(selectedIndices.rbegin(), selectedIndices.rend());
			for (size_t idx : selectedIndices)
			{
				human->removePoker(idx);
			}
			handOutPokers = selectedCards;
			human->clearPokerSelected();
			Render::drawAll();
			FlushBatchDraw();
			logger.logPlayerPlayedCards(human->getName(), handOutPokers);
			playManager.setLastPlayedCards(selectedCards);
			playManager.setLastPlayerIndex(0);
			playManager.resetPassCount();
			playManager.nextPlayer();
			Sleep(1000);
			return true;
		}
	}
	if (choice == 0)
	{
		auto *human = dynamic_cast<HumanPlayer *>(gameState.getPlayers()[0].get());
		setPassed(true);
		handOutPokers = {};
		human->clearPokerSelected();
		Render::drawAll();
		FlushBatchDraw();
		logger.logPlayerPlayedCards(human->getName(), handOutPokers);
		playManager.setLastPlayerIndex(0);
		playManager.incrementPassCount();
		playManager.nextPlayer();
		Sleep(1000);
		return true;
	}
	return false;
}
bool HumanPlayer::changeSelected(const ExMessage &msg)
{
	int selectedIndex = inputManager.clickSelectWhichCard(msg);
	if (selectedIndex != -1)
	{
		setPokerSelected(selectedIndex, !isPokerSelected(selectedIndex));
		return true;
	}
	return false;
}
void HumanPlayer::drawCards() const
{
	Render::drawPlayerCards(this);
}
void HumanPlayer::drawHandOutCards() const
{
	Render::drawPlayerHandOutCards(this);
}
void HumanPlayer::drawNameAndCount() const
{
	Render::drawPlayerName(this);
	Render::drawPlayerCount(this);
}
void HumanPlayer::drawBid() const
{
	Render::drawPlayerBid(this);
}
void HumanPlayer::drawLandlord() const
{
	Render::drawPlayerLandlord(this);
}
void HumanPlayer::removePoker(size_t index)
{
	BasePlayer::removePoker(index);
	if (index < pokerSelected.size())
	{
		pokerSelected.erase(pokerSelected.begin() + static_cast<int>(index));
	}
}
std::vector<bool> HumanPlayer::getPokerSelectedStates() const { return pokerSelected; }
void HumanPlayer::initSelectedState() { pokerSelected.assign(pokers.size(), false); }
void HumanPlayer::setPokerSelected(int index, bool isSelected)
{
	if (index >= 0 && index < static_cast<int>(pokerSelected.size()))
	{
		pokerSelected[index] = isSelected;
	}
}
bool HumanPlayer::isPokerSelected(int index) const
{
	if (index >= 0 && index < static_cast<int>(pokerSelected.size()))
	{
		return pokerSelected[index];
	}
	return false;
}
void HumanPlayer::clearPokerSelected() { pokerSelected.assign(pokers.size(), false); }