#include <iostream>
using namespace std;
#include "game.h"

int main() {
    cout << "Добро пожаловать в консольные шашки!" << endl;
    Game game;
    game.run();
    cout << "Игра завершена. Спасибо за игру!" << endl;
    return 0;
}
