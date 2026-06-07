#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "GameState.h"
#include <easyx.h>
#include "PlayManager.h"

class InputManager
{
private:
	GameState &gameState = GameState::getInstance();
	PlayManager &playManager = PlayManager::getInstance();

	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager &) = delete;
	InputManager &operator=(const InputManager &) = delete;
	InputManager(InputManager &&) = delete;
	InputManager &operator=(InputManager &&) = delete;

public:
	static InputManager &getInstance();

	int clickWhichBidButton(const ExMessage &msg);	// 返回点击了哪个叫分按钮
	int clickWhichPlayButton(const ExMessage &msg); // 返回点击了哪个出牌按钮
	int clickSelectWhichCard(const ExMessage &msg); // 返回点击了哪张牌的索引
};

#endif // INPUTMANAGER_H