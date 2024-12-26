//
// Created by carrb on 4/11/2024.
//

#ifndef MINESWEEPER_WELCOMELEADERBOARD_H
#define MINESWEEPER_WELCOMELEADERBOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

struct configSettings {
    int colCount;
    int rowCount;
    int numMines;

    configSettings(int col, int row, int mines) {
        colCount = col;
        rowCount = row;
        numMines = mines;
    }
};


configSettings readConfigfile (const std::string& filename) {
    std::ifstream file(filename);
    std::string col, row, mines;
    getline(file, col);
    getline(file, row);
    getline(file, mines);
    configSettings temp(stoi(col), stoi(row), stoi(mines) +  30);
    return temp;
}

sf::Font loadFont(const std::string& filename) {
    sf::Font font;
    font.loadFromFile(filename);
    return font;
}

sf::Text loadText(sf::Font &font,  int size = 24, std::string string = "") {
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(size);
    return text;
}

void setText(sf::Text &text, float x, float y){
     sf::FloatRect textRect = text.getLocalBounds();
     text.setOrigin(textRect.left + textRect.width/2.0f,
                    textRect.top + textRect.height/2.0f);
     text.setPosition(sf::Vector2f(x, y));
}

bool openGameWindow(sf::Event event, sf::String text) {
    if (event.text.unicode == 13 && text.getSize()> 0) {
        return true;
    }
    else {
        return false;
    }
}

struct leaderboardEntry {
    std::string time;
    std::string name;
};

std::vector<leaderboardEntry> readLeaderboard (std::string filename) {
    std::vector<leaderboardEntry> leaderboard;
    std::ifstream file(filename);

    std::string name;
    std::string time;


    while (std::getline(file,time, ',')) {
        std::getline(file,name);
        leaderboardEntry temp;
        temp.time = time;
        temp.name = name;
        leaderboard.push_back(temp);
    }
    return leaderboard;
};

void writeLeaderboard(std::vector<leaderboardEntry> leaderboard, std::string filename) {
    std::ofstream file(filename);
    for (int i=0; i < leaderboard.size(); i++) {
        file << leaderboard[i].time << "," << leaderboard[i].name << "\n";
    }
    file.close();
}

std::string formatTime(int mins, int secs) {
    std::ostringstream formattedTime;
    formattedTime << std::setw(2) << std::setfill('0') << mins << ":"
                  << std::setw(2) << std::setfill('0') << secs;
    return formattedTime.str();
}

void setLeaderboardWindow() {
    configSettings cfg = readConfigfile("files/config.cfg");

    int width = cfg.colCount*16;
    int height = cfg.rowCount*16 + 50;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window", sf::Style::Close);

    std::vector<leaderboardEntry> leaderboard = readLeaderboard("files/leaderboard.txt");
    sf::Font font = loadFont("files/font.ttf");

    sf::Text titleText("LEADERBOARD", font, 20);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);
    setText(titleText,(width/2),(height/2) -120);

    sf::Text leaderboardText("", font, 18);
    leaderboardText.setStyle(sf::Text::Bold);
    leaderboardText.setFillColor(sf::Color::White);


    std::ostringstream leaderboardString;
    for (int i=0; i < leaderboard.size();i++ ) {
        leaderboardString << leaderboard[i].time << "\t" << leaderboard[i].name << "\n\n";
    }

    leaderboardText.setString(leaderboardString.str());
    setText(leaderboardText,(width/2),(height/2)+20);
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(titleText);
        window.draw(leaderboardText);
        window.display();
}
}

int convertTimeToInt(const std::string& timeStr) {
    auto colonPos = timeStr.find(':');
    std::string minutesStr = timeStr.substr(0, colonPos);
    std::string secondsStr = timeStr.substr(colonPos + 1);

    int minutes = std::stoi(minutesStr);
    int seconds = std::stoi(secondsStr);

    return minutes * 60 + seconds;
}

std::string setWelcomeWindow() {
    configSettings cfg = readConfigfile("files/config.cfg");

    int width = cfg.colCount * 32;
    int height = cfg.rowCount * 32 + 100;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window", sf::Style::Close);
    //width = (float) width;
    //height = (float) height;

    sf::Font font = loadFont("files/font.ttf");

    std::string playerName;
    // sf::String name, title, enterName;
    sf::Text nameText, titleText, enterNameText;
    bool startGame = false;

    titleText = loadText(font, 24, "WELCOME TO MINESWEEPER!");
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(titleText, (width / 2), (height / 2) - 150); //can set text statically

    enterNameText = loadText(font, 20, "Enter your name:");
    enterNameText.setStyle(sf::Text::Bold);
    setText(enterNameText, (width / 2), (height / 2) - 75);

    nameText = loadText(font, 18, "|");
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, (width / 2), (height / 2) - 45);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    return playerName;
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    playerName.pop_back();
                } else if (event.text.unicode < 128 && std::isalpha(static_cast<char>(event.text.unicode))) {
                    if (playerName.size() < 10) {

                        if (playerName.empty()) {
                            playerName += std::toupper(static_cast<char> (event.text.unicode));
                        } else {
                            playerName += std::tolower(static_cast<char> (event.text.unicode));
                        }
                    }

                }
                nameText.setString(playerName + "|");
                setText(nameText, (width / 2), (height / 2) - 45);
            }

        }
        window.clear(sf::Color::Blue);
        window.draw(nameText);
        window.draw(titleText);
        window.draw(enterNameText);
        window.display();

    }
    return "";
}

#endif //MINESWEEPER_WELCOMELEADERBOARD_H
