#ifndef RENDER_H
#define RENDER_H

#include "BasePlayer.h"
#include "PlayManager.h"

class Render
{
private:
	static PlayManager &playManager;

public:
	static void drawBottom();											 // 绘制底牌
	static void drawPlayerCards(const BasePlayer *player);				 // 绘制玩家手牌
	static void drawPlayerHandOutCards(const BasePlayer *player);		 // 绘制玩家出的牌
	static void drawPlayerName(const BasePlayer *player);				 // 绘制玩家名称
	static void drawPlayerCount(const BasePlayer *player);				 // 绘制玩家手牌数量
	static void drawPlayerBid(const BasePlayer *player);				 // 绘制玩家叫分
	static void drawPlayerLandlord(const BasePlayer *player);			 // 绘制地主标志
	static void drawButton(const int x, const int y, const TCHAR *text); // 绘制按钮
	static void drawThinkingPromptOrWinner();							 // 绘制思考提示或胜利信息
	static void drawAll();												 // 绘制所有内容
};

#endif // RENDER_H