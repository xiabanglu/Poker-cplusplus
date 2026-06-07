#include "InputManager.h"
#include "Constants.h"
#include "GameState.h"
#include "HumanPlayer.h"
#include <easyx.h>

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}
int InputManager::clickWhichBidButton(const ExMessage &msg)
{
    if (msg.x >= Constants::BUTTON_BID1_X && msg.x <= Constants::BUTTON_BID1_X + Constants::BUTTON_WIDTH &&
        msg.y >= Constants::BUTTON_BID1_Y && msg.y <= Constants::BUTTON_BID1_Y + Constants::BUTTON_HEIGHT)
    {
        return 1;
    }
    if (msg.x >= Constants::BUTTON_BID2_X && msg.x <= Constants::BUTTON_BID2_X + Constants::BUTTON_WIDTH &&
        msg.y >= Constants::BUTTON_BID2_Y && msg.y <= Constants::BUTTON_BID2_Y + Constants::BUTTON_HEIGHT)
    {
        return 2;
    }
    if (msg.x >= Constants::BUTTON_BID3_X && msg.x <= Constants::BUTTON_BID3_X + Constants::BUTTON_WIDTH &&
        msg.y >= Constants::BUTTON_BID3_Y && msg.y <= Constants::BUTTON_BID3_Y + Constants::BUTTON_HEIGHT)
    {
        return 3;
    }
    if (msg.x >= Constants::BUTTON_BID_PASS_X && msg.x <= Constants::BUTTON_BID_PASS_X + Constants::BUTTON_WIDTH &&
        msg.y >= Constants::BUTTON_BID_PASS_Y && msg.y <= Constants::BUTTON_BID_PASS_Y + Constants::BUTTON_HEIGHT)
    {
        return 0;
    }
    return -1;
}
int InputManager::clickWhichPlayButton(const ExMessage &msg)
{
    if (msg.x >= Constants::BUTTON_PLAY_X && msg.x <= Constants::BUTTON_PLAY_X + Constants::BUTTON_WIDTH &&
        msg.y >= Constants::BUTTON_PLAY_Y && msg.y <= Constants::BUTTON_PLAY_Y + Constants::BUTTON_HEIGHT)
    {
        return 1;
    }
    if (msg.x >= Constants::BUTTON_PASS_X && msg.x <= Constants::BUTTON_PASS_X + Constants::BUTTON_WIDTH &&
        msg.y >= Constants::BUTTON_PASS_Y && msg.y <= Constants::BUTTON_PASS_Y + Constants::BUTTON_HEIGHT && !playManager.getLastPlayedCards().empty())
    {
        return 0;
    }
    return -1;
}
int InputManager::clickSelectWhichCard(const ExMessage &msg)
{
    auto *humanPlayer = dynamic_cast<HumanPlayer *>(gameState.getPlayers()[0].get());
    std::vector<Poker> &pokers = humanPlayer->getPokers();
    int x = Constants::PLAYER_ME_CARD_START_X;
    for (int i = 0; i < static_cast<int>(pokers.size()); ++i)
    {
        int cardX = x + i * Constants::CARD_SPACING_X;
        int cardY = humanPlayer->isPokerSelected(i) ? Constants::PLAYER_ME_CARD_SELECTED_Y : Constants::PLAYER_ME_CARD_START_Y;
        int width = 0;
        if (i == pokers.size() - 1)
        {
            width = (pokers[i].getPoint() == Point::TINY_JOKER || pokers[i].getPoint() == Point::GIANT_JOKER) ? Constants::CARD_JOKER_WIDTH : Constants::CARD_NORMAL_WIDTH;
        }
        else
        {
            width = Constants::CARD_SPACING_X;
        }
        int height = (pokers[i].getPoint() == Point::TINY_JOKER || pokers[i].getPoint() == Point::GIANT_JOKER) ? Constants::CARD_JOKER_HEIGHT : Constants::CARD_NORMAL_HEIGHT;
        if (msg.x >= cardX && msg.x <= cardX + width &&
            msg.y >= cardY && msg.y <= cardY + height)
        {
            return i;
        }
    }
    return -1;
}