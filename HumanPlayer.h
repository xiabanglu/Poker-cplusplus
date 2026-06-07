#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "BasePlayer.h"
#include <vector>
#include "InputManager.h"
#include "PlayManager.h"
#include "Constants.h"
#include <easyx.h>
#include "GameState.h"
#include "Logger.h"

class HumanPlayer : public BasePlayer
{
private:
	GameState &gameState = GameState::getInstance();
	PlayManager &playManager = PlayManager::getInstance();
	InputManager &inputManager = InputManager::getInstance();
	Logger &logger = Logger::getInstance();

	std::vector<bool> pokerSelected; // 记录每张牌的选中状态

public:
	HumanPlayer(const TCHAR *playerName) : BasePlayer(playerName, true, -1) {}
	HumanPlayer(const HumanPlayer &other) = default;
	HumanPlayer(HumanPlayer &&other) noexcept = default;
	HumanPlayer &operator=(const HumanPlayer &other) = default;
	HumanPlayer &operator=(HumanPlayer &&other) noexcept = default;
	~HumanPlayer() = default;

	// 绘制坐标
	const int cardStartX = Constants::PLAYER_ME_CARD_START_X;
	const int cardStartY = Constants::PLAYER_ME_CARD_START_Y;
	const int nameStartX = Constants::NAME_PLAYER_ME_X;
	const int nameStartY = Constants::NAME_PLAYER_ME_Y;
	const int countStartX = Constants::PLAYER_CARDS_COUNT_X;
	const int countStartY = Constants::PLAYER_CARDS_COUNT_Y;
	const int bidStartX = Constants::BID_PLAYER_ME_X;
	const int bidStartY = Constants::BID_PLAYER_ME_Y;
	const int landlordStartX = Constants::PLAYER_LANDLORD_START_X;
	const int landlordStartY = Constants::PLAYER_LANDLORD_START_Y;
	const int handOutCardStartX = Constants::PLAYER_HAND_OUT_CARD_START_X;
	const int handOutCardStartY = Constants::PLAYER_HAND_OUT_CARD_START_Y;

	void drawCards() const override;
	void drawHandOutCards() const override;
	void drawNameAndCount() const override;
	void drawBid() const override;
	void drawLandlord() const override;

	int bid(const ExMessage &msg);			   // 叫分
	bool play(const ExMessage &msg);		   // 出牌
	bool changeSelected(const ExMessage &msg); // 选中手牌

	void removePoker(size_t index) override;

	// Getter和Setter
	std::vector<bool> getPokerSelectedStates() const;
	void initSelectedState();
	void setPokerSelected(int index, bool isSelected);
	bool isPokerSelected(int index) const;
	void clearPokerSelected();
};

#endif // HUMANPLAYER_H