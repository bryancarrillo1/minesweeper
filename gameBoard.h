//
// Created by carrb on 4/17/2024.
//

#ifndef MINESWEEPER_GAMEBOARD_H
#define MINESWEEPER_GAMEBOARD_H

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>
#include "welcomeLeaderboard.h"
#include <vector>
#include <map>
#include <chrono>

struct Textures {
    sf::Texture tileTexture;
    //sf::Texture mineTexture;
    sf::Texture flagTexture;
    std::vector<sf::Texture> numberTextures;
    sf::Texture numberText;
    sf::Texture tileRevealed;

    Textures() {
        tileTexture.loadFromFile("files/images/tile_hidden.png");
       // mineTexture.loadFromFile("files/images/mine.png");
        flagTexture.loadFromFile("files/images/flag.png");
        tileRevealed.loadFromFile("files/images/tile_revealed.png");
        for (int i = 0; i < 8; ++i) {
            numberTextures[i].loadFromFile("files/images/number_" + std::to_string(i+1) + ".png");
        }}

};
struct tile {
    Textures* textures;
    sf::Texture tileTexture;
   // sf::Texture mineTexture;
    sf::Texture flagTexture;
    sf::Texture numberText;

    sf::Sprite tileSprite;
    sf::Sprite mineSprite;
    sf::Sprite flagSprite;
    sf::Sprite numberSprite;


    sf::Texture tileRevealed;
    sf::Sprite hiddenSprite;

    std::vector<std::string> numberTextures = {"files/images/number_1.png", "files/images/number_2.png",
                                               "files/images/number_3.png",
                                               "files/images/number_4.png", "files/images/number_5.png",
                                               "files/images/number_6.png",
                                               "files/images/number_7.png", "files/images/number_8.png"};
    bool is_mine = false;
    bool tileClicked = false;
    bool flagClick = false;
    bool num_revealed = false;

    std::vector<int> adjacentTileIndex;
    int tile_num;
    int adjacentMines = 0;


    tile(float positionX, float positionY, int tile_num) {
        //tileTexture.loadFromFile("files/images/tile_hidden.png");
        tileSprite.setPosition(positionX, positionY);
        this->tile_num = tile_num;
    }
    void drawPaused(sf::RenderWindow &window) {
        tileRevealed.loadFromFile("files/images/tile_revealed.png");
        hiddenSprite.setTexture(tileRevealed);
        hiddenSprite.setPosition(tileSprite.getPosition());
        window.draw(hiddenSprite);
    }


    void drawFlag(sf::RenderWindow &window) const {
        window.draw(flagSprite);
    }

    void drawTile(sf::RenderWindow &window) const {
        window.draw(tileSprite);
    }

    void drawMine(sf::RenderWindow &window) const {
        window.draw(mineSprite);
    }

    void drawNumber(sf::RenderWindow &window) const {
        window.draw(numberSprite);
    }

    void setNum() {
        numberText.loadFromFile(numberTextures[adjacentMines - 1]);
        numberSprite.setTexture(numberText);
        numberSprite.setPosition(tileSprite.getPosition());
        num_revealed = true;

    }

    void clickTile(sf::Vector2i mousePos, std::vector<tile> &tiles) {
        if (tileSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && !flagClick) {
            revealTile(tiles);
            }
    }

    void revealTile(std::vector<tile> &tiles) {
        if (!tileClicked && !flagClick) {
            tileClicked = true;
            tileTexture.loadFromFile("files/images/tile_revealed.png");
            tileSprite.setTexture(tileTexture);
            if (tiles[tile_num].adjacentMines == 0 && !tiles[tile_num].is_mine) {
                for (int i = 0; i < tiles[tile_num].adjacentTileIndex.size(); i++) {
                    int index = tiles[tile_num].adjacentTileIndex[i];
                    if (!tiles[index].tileClicked) {
                        tiles[index].revealTile(tiles);
                    }
                }
            }
            if (tiles[tile_num].adjacentMines > 0 && !tiles[tile_num].is_mine) {
                setNum();
            }
        }
    }

    void clickFlag(sf::Vector2i mousePos) {
        if (tileSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            flagTexture.loadFromFile("files/images/flag.png");
            flagSprite.setTexture(flagTexture);
            flagSprite.setPosition(tileSprite.getPosition());
            if (!flagClick && !tileClicked) {
                flagClick = true;
            }
            else {
                flagClick = false;
            }
        }
    }

    void setMine(sf::Texture &mineTexture) {
        mineSprite.setTexture(mineTexture);
        mineSprite.setPosition(tileSprite.getPosition());
    }

    void checkAdjacentTiles(std::vector<tile> tiles, configSettings cfg) {
        int currentPos = tile_num;
        int column = currentPos % cfg.colCount;
        int row = currentPos / cfg.colCount;
        //std::cout << row << std::endl;
        // std::cout << column << std::endl;

        if (column < cfg.colCount - 1) {
            int right = currentPos + 1;
            adjacentTileIndex.push_back(right);
            if (tiles[right].is_mine) {
                adjacentMines += 1;
            }
        }

        if (column > 0) {
            int left = currentPos - 1;
            adjacentTileIndex.push_back(left);
            if (tiles[left].is_mine) {
                adjacentMines += 1;
            }
        }

        if (row < cfg.rowCount - 1) {
            int down = currentPos + cfg.colCount;
            adjacentTileIndex.push_back(down);
            if (tiles[down].is_mine) {
                adjacentMines += 1;
            }
        }
        if (row > 0) {
            int up = currentPos - cfg.colCount;
            adjacentTileIndex.push_back(up);
            if (tiles[up].is_mine) {
                adjacentMines += 1;
            }
        }

        if (row < cfg.rowCount - 1 && column < cfg.colCount - 1) {
            int downRight = currentPos + cfg.colCount + 1;
            adjacentTileIndex.push_back(downRight);
            if (tiles[downRight].is_mine) {
                adjacentMines += 1;
            }
        }

        if (row < cfg.rowCount - 1 && column > 0) {
            int downLeft = currentPos + cfg.colCount - 1;
            adjacentTileIndex.push_back(downLeft);
            if (tiles[downLeft].is_mine) {
                adjacentMines += 1;
            }
        }


        if (row > 0 && column < cfg.colCount - 1) {
            int upRight = currentPos - cfg.colCount + 1;
            adjacentTileIndex.push_back(upRight);
            if (tiles[upRight].is_mine) {
                adjacentMines += 1;
            }
        }


        if (row > 0 && column > 0) {
            int upLeft = currentPos - cfg.colCount - 1;
            adjacentTileIndex.push_back(upLeft);
            if (tiles[upLeft].is_mine) {
                adjacentMines += 1;
            }
        }
    }
    //std::cout << "adjacent mines: "<< adjacentMines << std::endl;
};



struct board {
    std::vector<tile> tiles;
    bool first_click = false;
    bool loss = false;
    bool winner = false;
    bool debugMode = false;
    int num_tiles = 0;
    bool is_paused = false;

    int flagsPlaced = 0;

    sf::Texture hiddenTexture;

    void initializeBoard(configSettings cfg, sf::Texture &texture, sf::Texture mineTexture) {
        num_tiles = cfg.colCount * cfg.rowCount;
        int positionY = 0;
        int count = 0;

        for (int i = 0; i < cfg.rowCount; i++) {
            int positionX = 0;
            for (int j = 0; j < cfg.colCount; j++) {
                tile temp(positionX, positionY, count);
                temp.tileSprite.setTexture(texture);
                tiles.push_back(temp);
                positionX += 32;
                count += 1;
            }
            positionY += 32;
        }
        assignMines(cfg.numMines, mineTexture);
        assignAdjacentTiles(cfg);
    }


    void drawBoard(sf::RenderWindow &window, sf::Texture mineTexture) {
        for (int i = 0; i < num_tiles; i++) {
            tiles[i].drawTile(window);
            if (tiles[i].tileClicked && tiles[i].is_mine) {
                is_paused = true;
                loss = true;
            }
            if (winner) {
                if (tiles[i].is_mine && !tiles[i].flagClick) {
                    tiles[i].clickFlag(sf::Vector2i (tiles[i].tileSprite.getPosition()));
                    tiles[i].drawFlag(window);
                    flagsPlaced += 1;
                }
            }
            if (loss) {
                if (tiles[i].num_revealed) {
                    tiles[i].drawNumber(window);
                }
                if (tiles[i].is_mine) {
                    tiles[i].drawPaused(window);
                    tiles[i].setMine(mineTexture);
                    tiles[i].drawMine(window);
                }
            }
            else if (is_paused && !loss) {
                tiles[i].drawPaused(window);
            }
            else {
                if (debugMode && tiles[i].is_mine) {
                    tiles[i].setMine(mineTexture);
                    tiles[i].drawMine(window);
                }
                if (tiles[i].flagClick) {
                    tiles[i].drawFlag(window);
                }
                if (tiles[i].num_revealed) {
                    tiles[i].drawNumber(window);
                }
            }

        }
    }

    void checkLeftClick(sf::Vector2i mousePos, configSettings cfg) {
        int column = mousePos.x / 32;
        int row = mousePos.y / 32;
        int index = row * cfg.colCount + column;
        if (index < tiles.size()) {
            tiles[index].clickTile(mousePos, tiles);
        }
    }


    void checkRightClick(sf::Vector2i mousePos, configSettings cfg) {
        int column = mousePos.x / 32;
        int row = mousePos.y / 32;
        int index = row * cfg.colCount + column;
        if (index < tiles.size()) {
            tiles[index].clickFlag(mousePos);
            if (tiles[index].flagClick) {
                flagsPlaced += 1;
            }
            else {
                flagsPlaced -=1;
            }

        }
    }

    void assignMines(int num_mines, sf::Texture mineTexture) {
        //int randNum[num_tiles];
        std::vector<int> randNum;
        randNum.reserve(num_tiles);
        for (int i = 0; i < num_tiles; i++) {
            //randNum[i] = i;
            randNum.push_back(i);
            //std::cout << randNum[i] << std::endl;
        }
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(randNum.begin(), randNum.end(), std::default_random_engine(seed));
        for (int i = 0; i < num_mines; i++) {
            tiles[randNum[i]].is_mine = true;
            //tiles[randNum[i]].setMine(mineTexture);
        }
    }

    void assignAdjacentTiles(configSettings cfg) {
        for (int i = 0; i < tiles.size(); i++) {
            tiles[i].checkAdjacentTiles(tiles, cfg);
        }
    }

    void toggleDebugMode() {
        if (!debugMode) {
            debugMode = true;
        } else {
            debugMode = false;
        }
    }
     void togglePause() {
        if (!is_paused) {
            is_paused = true;
        }
        else {
            is_paused = false;
        }
    }
    void checkWin(int num_mines) {
        int emptyTiles = 0;
        for (int i=0; i <tiles.size(); i++) {
            if (tiles[i].tileClicked && !tiles[i].is_mine) {
                emptyTiles += 1;
            }
        }
        if (num_tiles -num_mines == emptyTiles) {
            winner = true;
        }
    };
};

std::map <int, sf::Sprite> parseDigits(sf::Sprite digits) {
    std::map<int, sf::Sprite> digitsMap;

    for (int i =0;i <= 10; i++) {
        sf::IntRect rect(i*21, 0,21, 32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i,sprite);
    }
    return  digitsMap;
};
struct game_buttons {
    sf::Texture happyFace;
    sf::Texture faceWin;
    sf::Texture faceLose;
    sf::Texture debugIcon;
    sf::Texture pauseIcon;
    sf::Texture playIcon;
    sf::Texture leaderboardIcon;

    sf::Sprite happySprite;
    sf::Sprite faceWinSprite;
    sf::Sprite faceLoseSprite;
    sf::Sprite debugSprite;
    sf::Sprite pauseSprite;
    sf::Sprite playSprite;
    sf::Sprite leaderboardSprite;


    std::map<std::string, sf::Sprite> buttonMap;

    bool is_paused = false;

    game_buttons(configSettings cfg) {
        happyFace.loadFromFile("files/images/face_happy.png");
        faceWin.loadFromFile("files/images/face_win.png");
        faceLose.loadFromFile("files/images/face_lose.png");
        //digits.loadFromFile("files/images/digits.png");
        debugIcon.loadFromFile("files/images/debug.png");
        pauseIcon.loadFromFile("files/images/pause.png");
        playIcon.loadFromFile("files/images/play.png");
        leaderboardIcon.loadFromFile("files/images/leaderboard.png");

        happySprite.setTexture(happyFace);
        happySprite.setPosition((cfg.colCount/2.0*32)-32, 32*(cfg.rowCount+0.5));

        faceWinSprite.setTexture(faceWin);
        faceWinSprite.setPosition(happySprite.getPosition());

        faceLoseSprite.setTexture(faceLose);
        faceLoseSprite.setPosition(happySprite.getPosition());

       // counterSprite.setTexture(digits);
        //counterSprite.setPosition();

        //timerSprite.setTexture(digits);
        //timerSprite.setPosition();

        debugSprite.setTexture(debugIcon);
        debugSprite.setPosition((cfg.colCount*32)-304, 32*(cfg.rowCount+0.5));

        pauseSprite.setTexture(pauseIcon);
        pauseSprite.setPosition((cfg.colCount*32)-240, 32*(cfg.rowCount+0.5));

        playSprite.setTexture(playIcon);
        playSprite.setPosition((cfg.colCount*32)-240, 32*(cfg.rowCount+0.5));

        leaderboardSprite.setTexture(leaderboardIcon);
        leaderboardSprite.setPosition((cfg.colCount*32)-176, 32*(cfg.rowCount+0.5));


        buttonMap["faceButton"]  = happySprite;
        buttonMap["debugButton"] = debugSprite;
        buttonMap["pauseButton"] = pauseSprite;
        buttonMap["leaderboardButton"] = leaderboardSprite;
    }

    void drawButtons(sf::RenderWindow &window) {
        auto button = buttonMap.begin();
        for (; button != buttonMap.end(); button++ ){
            window.draw(button->second);
        }
    }
    void checkButtonClick (sf::Vector2i mousePos, board &Board ) {
        /*if (happySprite.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
            Board.initializeBoard();
        } */
        if (debugSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            Board.toggleDebugMode();
        }
        /*if (pauseSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            togglePause();
            //pauseSprite.setTexture(playIcon);
        } */
    }
    void togglePause() {
        if (!is_paused) {
            is_paused = true;
        }
        else {
            is_paused = false;
        }
    }
};

void setGameWindow(std::string playerName) {
    configSettings cfg = readConfigfile("files/config.cfg");

    int width = cfg.colCount * 32;
    int height = cfg.rowCount * 32 + 100;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window", sf::Style::Close);
    sf::Texture tiletext;
    tiletext.loadFromFile("files/images/tile_hidden.png");
    sf::Texture tileRevealed;
    tileRevealed.loadFromFile("files/images/tile_revealed.png");
    sf::Texture mineTexture;
    mineTexture.loadFromFile("files/images/mine.png");

    board Board;
    Board.initializeBoard(cfg, tiletext, mineTexture);
    game_buttons buttons(cfg);
    //starts timer
    auto start_time = std::chrono::steady_clock::now();
    //placeholder/temp values
    auto pauseTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-pauseTime).count();

    sf::Texture digitsTexture;
    sf::Sprite digitsSprite;
    digitsTexture.loadFromFile("files/images/digits.png");
    digitsSprite.setTexture(digitsTexture);

    sf::Sprite flagCounterSprite;
    flagCounterSprite.setTexture(digitsTexture);

    std::map<int, sf::Sprite> digitsMap = parseDigits(digitsSprite);
    std::map<int, sf::Sprite> flagMap = parseDigits(flagCounterSprite);
    bool leaderboardWindow = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (buttons.happySprite.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                        board Board2;
                        Board2.initializeBoard(cfg, tiletext, mineTexture);
                        Board = Board2;
                        start_time = std::chrono::steady_clock::now();
                        buttons.buttonMap["faceButton"] = buttons.happySprite;
                        buttons.buttonMap["pauseButton"] = buttons.pauseSprite;
                        buttons.is_paused = false;
                    }
                    if (buttons.leaderboardSprite.getGlobalBounds().contains(mousePos.x,mousePos.y)) {
                        setLeaderboardWindow();

                    }
                    if (buttons.pauseSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) & !Board.loss & !Board.winner) {
                        buttons.togglePause();
                        Board.togglePause();
                        if (buttons.is_paused||Board.is_paused) {
                            pauseTime = std::chrono::steady_clock::now();
                            buttons.buttonMap["pauseButton"] = buttons.playSprite;
                        }
                        else{
                            auto unPausedTime = std::chrono::steady_clock::now();
                            elapsedTime += std::chrono::duration_cast<std::chrono::seconds>(unPausedTime - pauseTime).count();
                            buttons.buttonMap["pauseButton"] = buttons.pauseSprite;
                        }

                    }
                    if(!buttons.is_paused && !Board.is_paused && !Board.loss && !Board.winner) {
                        buttons.checkButtonClick(mousePos, Board);
                        Board.checkLeftClick(mousePos, cfg);
                    }

                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    Board.checkRightClick(mousePos, cfg);
                }
            }
        }

        if (Board.loss) {
            buttons.buttonMap["faceButton"] = buttons.faceLoseSprite;
        }

        window.clear(sf::Color::White);
        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() -start_time);
        int total_time = game_duration.count();

        int minutes;
        int seconds;

        if (!buttons.is_paused && !Board.is_paused && !Board.winner) {
            total_time = total_time - elapsedTime;
            minutes = total_time / 60;
            seconds = total_time % 60;
        }

        int minutes0 = minutes/10%10;
        int minutes1 = minutes %10;
        int seconds0 = seconds/10%10;
        int seconds1 = seconds%10;

        int flagCounter = cfg.numMines - Board.flagsPlaced;
        if (flagCounter < 0) {
            flagMap[10].setPosition(12,32*(cfg.rowCount+0.5)+16);
            window.draw(flagMap[10]);
        }

        int flag0 = abs(flagCounter) / 100 %10;
        int flag1 = abs(flagCounter) / 10 %10;
        int flag2 = abs(flagCounter) % 10;
        flagMap[flag0].setPosition(33, 32*(cfg.rowCount+0.5) +16);
        window.draw(flagMap[flag0]);
        flagMap[flag1].setPosition(54,32*(cfg.rowCount+0.5) +16);
        window.draw(flagMap[flag1]);
        flagMap[flag2].setPosition(75,32*(cfg.rowCount+0.5) +16);
        window.draw(flagMap[flag2]);


        digitsMap[minutes0].setPosition((cfg.colCount*32)-97, 32*(cfg.rowCount+ 0.5)+16);
        window.draw(digitsMap[minutes0]);
        digitsMap[minutes1].setPosition((cfg.colCount*32)-76, 32*(cfg.rowCount+0.5)+16);
        window.draw(digitsMap[minutes1]);
        digitsMap[seconds0].setPosition((cfg.colCount*32)-54, 32*(cfg.rowCount+0.5)+16);
        window.draw(digitsMap[seconds0]);
        digitsMap[seconds1].setPosition((cfg.colCount*32)-33, 32*(cfg.rowCount + 0.5)+16);
        window.draw(digitsMap[seconds1]);
        bool wroteLeaderboard;
        Board.checkWin(cfg.numMines);
        if (Board.winner) {
            buttons.buttonMap["faceButton"] = buttons.faceWinSprite;
            std::vector<leaderboardEntry> leaderboard = readLeaderboard("files/leaderboard.txt");

            leaderboardEntry player;
            player.name = playerName + "*";
            player.time = formatTime(minutes,seconds); // Player's time in seconds
            //std::cout << player.time << std::endl;
            if (!wroteLeaderboard) {
                wroteLeaderboard = true;
                // Check if the player's time beats any other time in the list
                auto it = std::find_if(leaderboard.begin(), leaderboard.end(), [&](leaderboardEntry& existingPlayer) {
                    return convertTimeToInt(player.time) < convertTimeToInt(existingPlayer.time);
                });
                if (it != leaderboard.end()) {
                    it++;
                    // Insert the player's details into the leaderboard
                }
                leaderboard.insert(it-1, player);
                // Ensure there are only top 5 players in the leaderboard
                if (leaderboard.size() > 5) {
                    leaderboard.resize(5);
                }
                for (int i=0; i <leaderboard.size();i++) {
                    std::cout << leaderboard[i].time << " " << leaderboard[i].name << std::endl;
                }
                writeLeaderboard(leaderboard, "files/leaderboard.txt");
            }


        }

        Board.drawBoard(window, mineTexture);
        buttons.drawButtons(window);
        window.display();
    }
}

#endif //MINESWEEPER_GAMEBOARD_H