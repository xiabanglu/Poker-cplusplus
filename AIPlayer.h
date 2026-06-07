#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "BasePlayer.h"
#include "Constants.h"
#include "GameState.h"
#include "PlayManager.h"
#include "Logger.h"

class AIPlayer : public BasePlayer
{
private:
	GameState &gameState = GameState::getInstance();
	PlayManager &playManager = PlayManager::getInstance();
	Logger &logger = Logger::getInstance();

public:
	AIPlayer(const TCHAR *playerName) : BasePlayer(playerName, true, -1) {}
	AIPlayer(const AIPlayer &other) = default;
	AIPlayer(AIPlayer &&other) = default;
	AIPlayer &operator=(const AIPlayer &other) = default;
	AIPlayer &operator=(AIPlayer &&other) noexcept = default;
	~AIPlayer() = default;

	// 绘制坐标
	const int cardStartX = name == _T("AI1") ? Constants::AI1_CARD_START_X : Constants::AI2_CARD_START_X;
	const int cardStartY = name == _T("AI1") ? Constants::AI1_CARD_START_Y : Constants::AI2_CARD_START_Y;
	const int nameStartX = name == _T("AI1") ? Constants::NAME_AI1_X : Constants::NAME_AI2_X;
	const int nameStartY = name == _T("AI1") ? Constants::NAME_AI1_Y : Constants::NAME_AI2_Y;
	const int countStartX = name == _T("AI1") ? Constants::AI1_CARDS_COUNT_X : Constants::AI2_CARDS_COUNT_X;
	const int countStartY = name == _T("AI1") ? Constants::AI1_CARDS_COUNT_Y : Constants::AI2_CARDS_COUNT_Y;
	const int bidStartX = name == _T("AI1") ? Constants::BID_AI1_X : Constants::BID_AI2_X;
	const int bidStartY = name == _T("AI1") ? Constants::BID_AI1_Y : Constants::BID_AI2_Y;
	const int landlordStartX = name == _T("AI1") ? Constants::AI1_LANDLORD_START_X : Constants::AI2_LANDLORD_START_X;
	const int landlordStartY = name == _T("AI1") ? Constants::AI1_LANDLORD_START_Y : Constants::AI2_LANDLORD_START_Y;
	const int handOutCardStartX = name == _T("AI1") ? Constants::AI1_HAND_OUT_CARD_START_X : Constants::AI2_HAND_OUT_CARD_START_X;
	const int handOutCardStartY = name == _T("AI1") ? Constants::AI1_HAND_OUT_CARD_START_Y : Constants::AI2_HAND_OUT_CARD_START_Y;

	void drawCards() const override;
	void drawHandOutCards() const override;
	void drawNameAndCount() const override;
	void drawBid() const override;
	void drawLandlord() const override;

	int bid();	 // 自动叫分
	bool play(); // 自动出牌
};

#endif // AIPLAYER_H