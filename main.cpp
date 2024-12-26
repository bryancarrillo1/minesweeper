#include <iostream>
#include "welcomeLeaderboard.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "gameBoard.h"

int main() {
     std::string playerName = setWelcomeWindow();
    if (!playerName.empty()) {
        setGameWindow(playerName);
    }
    return 0;
}
