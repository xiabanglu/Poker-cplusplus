#include "Render.h"
#include "Constants.h"
#include "BasePlayer.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include <array>
#include "BidManager.h"

PlayManager &Render::playManager = PlayManager::getInstance();
void Render::drawBottom()
{
	GameState &gameState = GameState::getInstance();
	settextcolor(RGB(30, 30, 30));
	settextstyle(20, 0, _T("宋体"));
	outtextxy(Constants::NAME_BOTTOM_X, Constants::NAME_BOTTOM_Y, _T("底牌"));
	int x = Constants::BOTTOM_CARD_START_X, y = Constants::BOTTOM_CARD_START_Y;
	for (const auto &poker : gameState.getBottom())
	{
		IMAGE img;
		if (gameState.isInBiddingPhase())
		{
			loadimage(&img, _T("img/poker/rear.png"));
		}
		else
		{
			loadimage(&img, poker.getImgUrl().c_str());
		}
		putimage(x, y, &img);
		x += Constants::CARD_SPACING_X;
	}
}
void Render::drawPlayerCards(const BasePlayer *player)
{
	const HumanPlayer *humanPlayer = dynamic_cast<const HumanPlayer *>(player);
	const AIPlayer *aiPlayer = dynamic_cast<const AIPlayer *>(player);
	if (humanPlayer)
	{
		int x = humanPlayer->cardStartX;
		int y = humanPlayer->cardStartY;
		for (size_t i = 0; i < humanPlayer->getPokers().size(); ++i)
		{
			if (humanPlayer->getPokers().empty())
			{
				return;
			}
			const auto &poker = humanPlayer->getPokers()[i];
			IMAGE img;
			loadimage(&img, poker.getImgUrl().c_str());
			bool isSelected = (i < humanPlayer->getPokerSelectedStates().size())
								  ? humanPlayer->getPokerSelectedStates()[i]
								  : false;
			y = isSelected ? Constants::PLAYER_ME_CARD_SELECTED_Y : Constants::PLAYER_ME_CARD_START_Y;
			putimage(x, y, &img);
			x += Constants::CARD_SPACING_X;
		}
	}
	else if (aiPlayer)
	{
		int x = aiPlayer->cardStartX;
		int y = aiPlayer->cardStartY;
		for (size_t i = 0; i < aiPlayer->getPokers().size(); ++i)
		{
			IMAGE img;
			loadimage(&img, _T("img/poker/rear.png"));
			putimage(x, y, &img);
			y += Constants::CARD_SPACING_Y;
		}
	}
}
void Render::drawPlayerHandOutCards(const BasePlayer *player)
{
	const HumanPlayer *humanPlayer = dynamic_cast<const HumanPlayer *>(player);
	const AIPlayer *aiPlayer = dynamic_cast<const AIPlayer *>(player);
	int x = 0, y = 0;
	if (humanPlayer)
	{
		x = humanPlayer->handOutCardStartX;
		y = humanPlayer->handOutCardStartY;
		const auto &handOutPokers = humanPlayer->getHandOutPokers();
		if (humanPlayer->getPassed())
		{
			settextcolor(RGB(30, 30, 30));
			settextstyle(20, 0, _T("宋体"));
			outtextxy(x + 50, y + 50, _T("不出"));
			return;
		}
		for (const Poker &poker : handOutPokers)
		{
			IMAGE img;
			loadimage(&img, poker.getImgUrl().c_str());
			putimage(x, y, &img);
			x += Constants::CARD_SPACING_X;
		}
	}
	else if (aiPlayer)
	{
		x = aiPlayer->handOutCardStartX;
		y = aiPlayer->handOutCardStartY;
		const auto &handOutPokers = aiPlayer->getHandOutPokers();
		if (aiPlayer->getPassed())
		{
			settextcolor(RGB(30, 30, 30));
			settextstyle(20, 0, _T("宋体"));
			outtextxy(x + 50, y + 50, _T("不出"));
			return;
		}
		for (const Poker &poker : handOutPokers)
		{
			IMAGE img;
			loadimage(&img, poker.getImgUrl().c_str());
			putimage(x, y, &img);
			x += Constants::CARD_SPACING_X;
		}
	}
}
void Render::drawPlayerName(const BasePlayer *player)
{
	const HumanPlayer *humanPlayer = dynamic_cast<const HumanPlayer *>(player);
	const AIPlayer *aiPlayer = dynamic_cast<const AIPlayer *>(player);
	int x = 0, y = 0;
	if (humanPlayer)
	{
		x = humanPlayer->nameStartX;
		y = humanPlayer->nameStartY;
		settextcolor(RGB(30, 30, 30));
		settextstyle(20, 0, _T("宋体"));
		outtextxy(x, y, humanPlayer->getName());
	}
	else if (aiPlayer)
	{
		x = aiPlayer->nameStartX;
		y = aiPlayer->nameStartY;
		settextcolor(RGB(30, 30, 30));
		settextstyle(20, 0, _T("宋体"));
		outtextxy(x, y, aiPlayer->getName());
	}
}
void Render::drawPlayerCount(const BasePlayer *player)
{
	const HumanPlayer *humanPlayer = dynamic_cast<const HumanPlayer *>(player);
	const AIPlayer *aiPlayer = dynamic_cast<const AIPlayer *>(player);
	int x = 0, y = 0;
	if (humanPlayer)
	{
		x = humanPlayer->countStartX;
		y = humanPlayer->countStartY;
		settextcolor(RGB(30, 30, 30));
		settextstyle(20, 0, _T("宋体"));
		std::array<TCHAR, 20> countText = {};
		_stprintf_s(countText.data(), countText.size(), _T("（%d张）"), static_cast<int>(humanPlayer->getPokers().size()));
		outtextxy(x, y, countText.data());
	}
	else if (aiPlayer)
	{
		x = aiPlayer->countStartX;
		y = aiPlayer->countStartY;
		settextcolor(RGB(30, 30, 30));
		settextstyle(20, 0, _T("宋体"));
		std::array<TCHAR, 20> countText = {};
		_stprintf_s(countText.data(), countText.size(), _T("（%d张）"), static_cast<int>(aiPlayer->getPokers().size()));
		outtextxy(x, y, countText.data());
	}
}
void Render::drawPlayerBid(const BasePlayer *player)
{
	const HumanPlayer *humanPlayer = dynamic_cast<const HumanPlayer *>(player);
	const AIPlayer *aiPlayer = dynamic_cast<const AIPlayer *>(player);
	int x = 0, y = 0;
	if (humanPlayer)
	{
		x = humanPlayer->bidStartX;
		y = humanPlayer->bidStartY;
		if (humanPlayer->getBid() == -1)
			return;
		settextcolor(RED);
		settextstyle(20, 0, _T("宋体"));
		setbkmode(TRANSPARENT);
		std::array<TCHAR, 20> text = {};
		if (humanPlayer->getBid() == 0)
			_stprintf_s(text.data(), text.size(), _T("不叫"));
		else
		{
			_stprintf_s(text.data(), text.size(), _T("%d分"), humanPlayer->getBid());
		}
		outtextxy(x, y, text.data());
		setbkmode(OPAQUE);
	}
	else if (aiPlayer)
	{
		x = aiPlayer->bidStartX;
		y = aiPlayer->bidStartY;
		if (aiPlayer->getBid() == -1)
			return;
		settextcolor(RED);
		settextstyle(20, 0, _T("宋体"));
		setbkmode(TRANSPARENT);
		std::array<TCHAR, 20> text = {};
		if (aiPlayer->getBid() == 0)
			_stprintf_s(text.data(), text.size(), _T("不叫"));
		else
		{
			_stprintf_s(text.data(), text.size(), _T("%d分"), aiPlayer->getBid());
		}
		outtextxy(x, y, text.data());
		setbkmode(OPAQUE);
	}
}
void Render::drawPlayerLandlord(const BasePlayer *player)
{
	const HumanPlayer *humanPlayer = dynamic_cast<const HumanPlayer *>(player);
	const AIPlayer *aiPlayer = dynamic_cast<const AIPlayer *>(player);
	int x = 0, y = 0;
	if (humanPlayer)
	{
		x = humanPlayer->landlordStartX;
		y = humanPlayer->landlordStartY;
		IMAGE img;
		loadimage(&img, _T("img/landlord.png"));
		putimage(x, y, &img);
	}
	else if (aiPlayer)
	{
		x = aiPlayer->landlordStartX;
		y = aiPlayer->landlordStartY;
		IMAGE img;
		loadimage(&img, _T("img/landlord.png"));
		putimage(x, y, &img);
	}
}
void Render::drawButton(const int x, const int y, const TCHAR *text)
{
	int width = Constants::BUTTON_WIDTH;
	int height = Constants::BUTTON_HEIGHT;
	COLORREF btnBgColor = RGB(230, 230, 230);
	COLORREF btnBorderColor = RGB(100, 100, 100);
	COLORREF textColor = RGB(30, 30, 30);
	setfillcolor(btnBgColor);
	solidrectangle(x, y, x + width, y + height);
	setlinecolor(btnBorderColor);
	setlinestyle(PS_SOLID, 1);
	rectangle(x, y, x + width, y + height);
	setbkmode(TRANSPARENT);
	settextstyle(18, 0, _T("微软雅黑"));
	settextcolor(textColor);
	RECT textRect = {x + 8, y + 8, x + width - 8, y + height - 8};
	drawtext(text, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkmode(OPAQUE);
}
void Render::drawThinkingPromptOrWinner()
{
	GameState &gameState = GameState::getInstance();
	BidManager &bidManager = BidManager::getInstance();
	PlayManager &playManager = PlayManager::getInstance();
	if (!bidManager.isBidDone() || !playManager.isPlayDone())
	{
		int playerIndex = gameState.getThinkingPlayerIndex();
		if (playerIndex < 0 || playerIndex >= gameState.getPlayers().size())
			return;
		const TCHAR *playerName = gameState.getPlayers()[playerIndex]->getName();
		std::array<TCHAR, 20> text = {};
		_stprintf_s(text.data(), text.size(), _T("%s思考中..."), playerName);
		settextcolor(RGB(0, 0, 0));
		settextstyle(30, 0, _T("宋体"));
		setbkmode(TRANSPARENT);
		RECT rect = {};
		rect.left = 0;
		rect.top = Constants::WINDOW_HEIGHT / 2 - 15;
		rect.right = Constants::WINDOW_WIDTH;
		rect.bottom = Constants::WINDOW_HEIGHT / 2 + 15;
		drawtext(text.data(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setbkmode(OPAQUE);
	}
	else
	{
		settextcolor(RGB(0, 0, 0));
		settextstyle(30, 0, _T("宋体"));
		if (gameState.getWinnerIndex() == gameState.getLandlordIndex())
		{

			outtextxy(Constants::WINDOW_WIDTH / 2 - 40, Constants::WINDOW_HEIGHT / 2 - 20, _T("地主胜利!"));
		}
		else
		{
			outtextxy(Constants::WINDOW_WIDTH / 2 - 40, Constants::WINDOW_HEIGHT / 2 - 20, _T("农民胜利!"));
		}
	}
}
void Render::drawAll()
{
	GameState &gameState = GameState::getInstance();
	setfillcolor(WHITE);
	solidrectangle(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
	drawBottom();
	drawThinkingPromptOrWinner();
	for (auto &player : gameState.getPlayers())
	{
		player->drawCards();
		player->drawHandOutCards();
		player->drawNameAndCount();
		player->drawBid();
	}
	if (gameState.isInBiddingPhase() && gameState.getCurrentBidderIndex() == 0)
	{
		drawButton(Constants::BUTTON_BID1_X, Constants::BUTTON_BID1_Y, _T("1分"));
		drawButton(Constants::BUTTON_BID2_X, Constants::BUTTON_BID2_Y, _T("2分"));
		drawButton(Constants::BUTTON_BID3_X, Constants::BUTTON_BID3_Y, _T("3分"));
		drawButton(Constants::BUTTON_BID_PASS_X, Constants::BUTTON_BID_PASS_Y, _T("不叫"));
	}
	else if (gameState.isInPlayingPhase() && gameState.getCurrentPlayerIndex() == 0)
	{
		drawButton(Constants::BUTTON_PLAY_X, Constants::BUTTON_PLAY_Y, _T("出牌"));
		if (!playManager.getLastPlayedCards().empty())
		{
			drawButton(Constants::BUTTON_PASS_X, Constants::BUTTON_PASS_Y, _T("不出"));
		}
	}
	if (gameState.isInPlayingPhase())
	{
		gameState.getPlayers()[gameState.getLandlordIndex()]->drawLandlord();
	}
}