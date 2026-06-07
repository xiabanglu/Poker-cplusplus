#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants
{
public:
    static constexpr auto WINDOW_WIDTH = 1200; // 游戏窗口宽度
    static constexpr auto WINDOW_HEIGHT = 800; // 游戏窗口高度

    static constexpr auto CARD_NORMAL_WIDTH = 71;  // 普通牌的宽度
    static constexpr auto CARD_NORMAL_HEIGHT = 96; // 普通牌的高度

    static constexpr auto CARD_REAR_WIDTH = 61;  // 反面牌的宽度
    static constexpr auto CARD_REAR_HEIGHT = 96; // 反面牌的高度

    static constexpr auto CARD_JOKER_WIDTH = 70;  // 大小王的宽度
    static constexpr auto CARD_JOKER_HEIGHT = 94; // 大小王的高度

    static constexpr auto NAME_BOTTOM_X = 500; // 底牌名称x坐标
    static constexpr auto NAME_BOTTOM_Y = 0;   // 底牌名称y坐标

    static constexpr auto NAME_PLAYER_ME_X = 390; // 玩家自己名称x坐标
    static constexpr auto NAME_PLAYER_ME_Y = 700; // 玩家自己名称y坐标

    static constexpr auto NAME_AI1_X = 50;  // AI1名称x坐标
    static constexpr auto NAME_AI1_Y = 160; // AI1名称y坐标

    static constexpr auto NAME_AI2_X = 1100; // AI2名称x坐标
    static constexpr auto NAME_AI2_Y = 160;  // AI2名称y坐标

    static constexpr auto PLAYER_CARDS_COUNT_X = 390; // 玩家自己手牌数量x坐标
    static constexpr auto PLAYER_CARDS_COUNT_Y = 730; // 玩家自己手牌数量y坐标

    static constexpr auto AI1_CARDS_COUNT_X = 80;  // AI1手牌数量x坐标
    static constexpr auto AI1_CARDS_COUNT_Y = 161; // AI1手牌数量y坐标

    static constexpr auto AI2_CARDS_COUNT_X = 1130; // AI2手牌数量x坐标
    static constexpr auto AI2_CARDS_COUNT_Y = 161;  // AI2手牌数量y坐标

    static constexpr auto BID_PLAYER_ME_X = 390; // 玩家自己叫分显示x坐标
    static constexpr auto BID_PLAYER_ME_Y = 760; // 玩家自己叫分显示y坐标

    static constexpr auto BID_AI1_X = 120; // AI1叫分显示x坐标
    static constexpr auto BID_AI1_Y = 200; // AI1叫分显示y坐标

    static constexpr auto BID_AI2_X = 1050; // AI2叫分显示x坐标
    static constexpr auto BID_AI2_Y = 200;  // AI2叫分显示y坐标

    static constexpr auto CARD_SPACING_X = 15; // 水平方向牌间距
    static constexpr auto CARD_SPACING_Y = 15; // 选中偏移间距

    static constexpr auto BOTTOM_CARD_START_X = 560; // 底牌起始x坐标
    static constexpr auto BOTTOM_CARD_START_Y = 0;   // 底牌起始y坐标

    static constexpr auto PLAYER_ME_CARD_START_X = 480;                                        // 玩家自己手牌起始x坐标
    static constexpr auto PLAYER_ME_CARD_START_Y = 700;                                        // 玩家自己手牌起始y坐标（未选中状态）
    static constexpr auto PLAYER_ME_CARD_SELECTED_Y = PLAYER_ME_CARD_START_Y - CARD_SPACING_Y; // 选中状态y偏移

    static constexpr auto AI1_CARD_START_X = 50;  // AI1手牌起始x坐标
    static constexpr auto AI1_CARD_START_Y = 200; // AI1手牌起始y坐标

    static constexpr auto AI2_CARD_START_X = 1100; // AI2手牌起始x坐标
    static constexpr auto AI2_CARD_START_Y = 200;  // AI2手牌起始y坐标

    static constexpr auto PLAYER_HAND_OUT_CARD_START_X = 560; // 玩家出牌起始x坐标
    static constexpr auto PLAYER_HAND_OUT_CARD_START_Y = 500; // 玩家出牌起始y坐标

    static constexpr auto AI1_HAND_OUT_CARD_START_X = 150; // AI1出牌起始x坐标
    static constexpr auto AI1_HAND_OUT_CARD_START_Y = 250; // AI1出牌起始y坐标

    static constexpr auto AI2_HAND_OUT_CARD_START_X = 900; // AI2出牌起始x坐标
    static constexpr auto AI2_HAND_OUT_CARD_START_Y = 250; // AI2出牌起始y坐标

    static constexpr auto PLAYER_LANDLORD_START_X = 340; // 玩家自己地主标志x坐标
    static constexpr auto PLAYER_LANDLORD_START_Y = 700; // 玩家自己地主标志y坐标

    static constexpr auto AI1_LANDLORD_START_X = 150; // AI1地主标志x坐标
    static constexpr auto AI1_LANDLORD_START_Y = 160; // AI1地主标志y坐标

    static constexpr auto AI2_LANDLORD_START_X = 1050; // AI2地主标志x坐标
    static constexpr auto AI2_LANDLORD_START_Y = 160;  // AI2地主标志y坐标

    static constexpr auto BUTTON_WIDTH = 100; // 按钮宽度
    static constexpr auto BUTTON_HEIGHT = 30; // 按钮高度

    static constexpr auto BUTTON_BID1_X = 400; // 叫1分按钮x坐标
    static constexpr auto BUTTON_BID1_Y = 650; // 叫1分按钮y坐标

    static constexpr auto BUTTON_BID2_X = 550; // 叫2分按钮x坐标
    static constexpr auto BUTTON_BID2_Y = 650; // 叫2分按钮y坐标

    static constexpr auto BUTTON_BID3_X = 700; // 叫3分按钮x坐标
    static constexpr auto BUTTON_BID3_Y = 650; // 叫3分按钮y坐标

    static constexpr auto BUTTON_BID_PASS_X = 850; // 不叫按钮x坐标
    static constexpr auto BUTTON_BID_PASS_Y = 650; // 不叫按钮y坐标

    static constexpr auto BUTTON_PLAY_X = 550; // 出牌按钮x坐标
    static constexpr auto BUTTON_PLAY_Y = 650; // 出牌按钮y坐标

    static constexpr auto BUTTON_PASS_X = 700; // 不出按钮x坐标
    static constexpr auto BUTTON_PASS_Y = 650; // 不出按钮y坐标

    static constexpr auto BUTTON_RESET_X = 1050; // 重新开始按钮x坐标
    static constexpr auto BUTTON_RESET_Y = 700;  // 重新开始按钮y坐标

    static constexpr auto BUTTON_EXIT_X = 1050; // 退出游戏按钮x坐标
    static constexpr auto BUTTON_EXIT_Y = 750;  // 退出游戏按钮y坐标
private:
    Constants() = delete; // 禁止实例化
};

#endif // CONSTANTS_H