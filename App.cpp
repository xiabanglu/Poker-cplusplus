#include <easyx.h>
#include "GameManager.h"
#include "Constants.h"

int main()
{
    initgraph(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    setbkcolor(WHITE);
    cleardevice();

    GameManager &game = GameManager::getInstance();
    game.init();
    game.run();

    Sleep(5000);

    closegraph();
    return 0;
}