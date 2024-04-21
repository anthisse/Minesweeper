#include <sstream>
#include "TrayGui.h"

// Constructor
TrayGui::TrayGui(std::pair<int, int>& boardDimensions, const std::string& n) {
    this->boardDimensions = boardDimensions;
    startTime = std::chrono::high_resolution_clock::now();
    endTime = std::chrono::high_resolution_clock::now();
    pausedStartTime = std::chrono::high_resolution_clock::now();
    pausedEndTime = std::chrono::high_resolution_clock::now();
    totalPausedTime = pausedEndTime - pausedStartTime;
    paused = false;
    gameOver = false;
    gameWon = false;
    leaderboardDisplayed = false;
    name = n;
    buttonSprites = {};
    hasSpace = hasSpaces();
}

std::chrono::duration<double, std::milli> TrayGui::updateGameTime() {
    std::chrono::duration<double, std::milli> elapsedTotalTime = {};
    elapsedTotalTime = endTime - startTime;
    auto elapsedPauseTime = pausedEndTime - pausedStartTime;
    if (!paused && !gameOver) {
        endTime = std::chrono::high_resolution_clock::now();
        totalPausedTime += elapsedPauseTime;
        pausedEndTime = std::chrono::high_resolution_clock::now();
        pausedStartTime = std::chrono::high_resolution_clock::now();
    }
    auto gameTime = elapsedTotalTime - totalPausedTime;
    return gameTime;
}

void TrayGui::setGameOver(bool g) {
    this->gameOver = g;
}

void TrayGui::setGameWon(bool w) {
    this->gameWon = w;
}

void TrayGui::render(sf::RenderWindow& window, std::vector<sf::Texture>& textures,
                     const int& numMines, const int& numFlags) {
    enum guiTextures {
        debug, digits, happy, lose, win, lb, pause, play
    };
    sf::Texture digitTexture = textures[digits];

    renderMinesRemaining(window, digitTexture, numMines, numFlags);

    // Face button
    sf::Sprite gameStateSprite;
    if (!gameOver) {
        gameStateSprite.setTexture(textures[happy]);
    } else {
        gameWon ? gameStateSprite.setTexture(textures[win]) : gameStateSprite.setTexture(textures[lose]);
    }

    gameStateSprite.setPosition(static_cast<float>(boardDimensions.first * 16 - 32),
                                static_cast<float>(32 * (boardDimensions.second + 0.5)));
    buttonSprites.emplace_back(gameStateSprite);
    window.draw(gameStateSprite);

    // Pause button, clickable
    sf::Sprite pauseButtonSprite;
    paused ? pauseButtonSprite.setTexture(textures[play]) : pauseButtonSprite.setTexture(textures[pause]);
    pauseButtonSprite.setPosition(static_cast<float>(boardDimensions.first * 32) - 240,
                                  static_cast<float>(32 * (boardDimensions.second + 0.5)));
    buttonSprites.emplace_back(pauseButtonSprite);
    window.draw(pauseButtonSprite);

    // Leaderboard button, clickable
    sf::Sprite leaderboardSprite(textures[lb]);
    leaderboardSprite.setPosition(static_cast<float>(boardDimensions.first * 32 - 176),
                                  static_cast<float>(32 * (boardDimensions.second + 0.5)));
    buttonSprites.emplace_back(leaderboardSprite);
    window.draw(leaderboardSprite);

    // Debug button
    sf::Sprite debugSprite(textures[debug]);
    debugSprite.setPosition(static_cast<float>(boardDimensions.first * 32 - 304), // 25 x 16 board
                            static_cast<float>(32 * (boardDimensions.second + 0.5)));
    buttonSprites.emplace_back(debugSprite);
    window.draw(debugSprite);

    // Timer
    renderTimer(window, digitTexture);

    // Render leaderboard if game was won
    if (gameWon && !leaderboardDisplayed) {
        std::chrono::duration<double, std::milli> elapsedGameTimeSeconds = updateGameTime();
        long long elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(elapsedGameTimeSeconds).count();
        long long elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedGameTimeSeconds).count();
        if (elapsedMinutes >= 99) {
            elapsedMinutes = 99;
        }
        // Convert to time format
        std::stringstream formattedTime;
        formattedTime << std::setfill('0') << std::setw(2) << elapsedMinutes << ":" << std::setw(2) << elapsedSeconds;
        std::pair<std::string, std::string> entry = {formattedTime.str(), name};
        writeScore(entry);
        displayLeaderboard();
        leaderboardDisplayed = true;
    }
}

void TrayGui::renderMinesRemaining(sf::RenderWindow& window, const sf::Texture& texture,
                                   const int& mines, const int& flags) const {
    int minesRemaining = mines - flags;
    bool negative;
    if (minesRemaining < 0) {
        negative = true;
    } else {
        negative = false;
    }

    int hundredsPlace = abs(minesRemaining / 100);
    int tensPlace = abs(minesRemaining % 100 / 10);
    int onesPlace = abs(minesRemaining % 100 % 10);
    sf::Sprite hundredsPlaceSprite(texture);
    sf::Sprite tensPlaceSprite(texture);
    sf::Sprite onesPlaceSprite(texture);
    sf::Sprite negativeSprite(texture);

    hundredsPlaceSprite.setTextureRect(sf::IntRect(21 * hundredsPlace, 0, 21, 32));
    tensPlaceSprite.setTextureRect(sf::IntRect(21 * tensPlace, 0, 21, 32));
    onesPlaceSprite.setTextureRect(sf::IntRect(21 * onesPlace, 0, 21, 32));
    negativeSprite.setTextureRect(sf::IntRect((21 * 10), 0, 21, 32));

    hundredsPlaceSprite.setPosition(static_cast<float>(33),
                                    static_cast<float>(32 * (boardDimensions.second + 0.5) + 16));
    tensPlaceSprite.setPosition(static_cast<float>(33 + 21),
                                static_cast<float>(32 * (boardDimensions.second + 0.5) + 16));
    onesPlaceSprite.setPosition(static_cast<float>(33 + 21 + 21),
                                static_cast<float>(32 * (boardDimensions.second + 0.5) + 16));
    negativeSprite.setPosition(static_cast<float>(12),
                               static_cast<float>(32 * (boardDimensions.second + 0.5) + 16));
    if (negative) {
        window.draw(negativeSprite);
    }
    window.draw(hundredsPlaceSprite);
    window.draw(tensPlaceSprite);
    window.draw(onesPlaceSprite);
}

// Render the timer
void TrayGui::renderTimer(sf::RenderWindow& window,
                          const sf::Texture& texture) {
    // Get minutes and seconds elapsed
    auto elapsedGameTimeSeconds = updateGameTime();
    long long elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(elapsedGameTimeSeconds).count();
    long long elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedGameTimeSeconds).count();
    if (elapsedMinutes >= 99) {
        elapsedMinutes = 99;
    }

    // Int divide by ten to get ten's digit and mod by ten to get one's digit
    int topMinutesDigit = static_cast<int>(elapsedMinutes) / 10;
    int bottomMinutesDigit = static_cast<int>(elapsedMinutes) % 10;
    int topSecondsDigit = static_cast<int>(elapsedSeconds) % 60 / 10;
    int bottomSecondsDigit = static_cast<int>(elapsedSeconds) % 60 % 10;

    // Set textures
    sf::Sprite topMinutesSprite(texture);
    sf::Sprite bottomMinutesSprite(texture);
    sf::Sprite topSecondsSprite(texture);
    sf::Sprite bottomSecondsSprite(texture);
    // Each digit offset by 21 pixels in width
    topMinutesSprite.setTextureRect(sf::IntRect(21 * topMinutesDigit, 0, 21, 32));
    bottomMinutesSprite.setTextureRect(sf::IntRect(21 * bottomMinutesDigit, 0, 21, 32));
    topSecondsSprite.setTextureRect(sf::IntRect(21 * topSecondsDigit, 0, 21, 32));
    bottomSecondsSprite.setTextureRect(sf::IntRect(21 * bottomSecondsDigit, 0, 21, 32));

    topMinutesSprite.setPosition(static_cast<float>((boardDimensions.first * 32)) - 97,
                                 static_cast<float>((32 * (boardDimensions.second + 0.5))) + 16);
    bottomMinutesSprite.setPosition(static_cast<float>((boardDimensions.first * 32)) - 97 + 21,
                                    static_cast<float>((32 * (boardDimensions.second + 0.5))) + 16);
    topSecondsSprite.setPosition(static_cast<float>((boardDimensions.first * 32)) - 54,
                                 static_cast<float>((32 * (boardDimensions.second + 0.5))) + 16);
    bottomSecondsSprite.setPosition(static_cast<float>((boardDimensions.first * 32)) - 54 + 21,
                                    static_cast<float>((32 * (boardDimensions.second + 0.5))) + 16);


    window.draw(topMinutesSprite);
    window.draw(bottomMinutesSprite);
    window.draw(topSecondsSprite);
    window.draw(bottomSecondsSprite);
}

void TrayGui::click(sf::RenderWindow& window, const sf::Vector2i& mousePosition,
                    const std::vector<sf::Texture>& tileTextures, Board& board) {
    enum buttonSpritesIndices {
        face, pause, lb, debug
    };
    enum tileTextureIndices {
        flag, mine, num1, num2, num3, num4, num5, num6, num7, num8, hidden, revealed
    };

    // Figure out which button was clicked
    sf::Vector2f translatedPosition = window.mapPixelToCoords(mousePosition);
    for (unsigned i = 0; i < buttonSprites.size(); i++) {
        if (buttonSprites[i].getGlobalBounds().contains(translatedPosition)) {
            // No need for a default label, it's okay to do nothing
            switch (i) {
                case face:
                    // Reset time and the board
                    board.reset();
                    this->gameOver = false;
                    this->gameWon = false;
                    this->leaderboardDisplayed = false;
                    startTime = std::chrono::high_resolution_clock::now();
                    endTime = std::chrono::high_resolution_clock::now();
                    pausedStartTime = std::chrono::high_resolution_clock::now();
                    pausedEndTime = std::chrono::high_resolution_clock::now();
                    totalPausedTime = pausedStartTime - pausedEndTime;
                    break;
                case pause:
                    // Unpause the game
                    if (board.paused()) {
                        this->paused = false;
                        board.setPaused(paused);
                        pausedEndTime = std::chrono::high_resolution_clock::now();
                        break;
                    }
                    // Pause the game
                    this->paused = true;
                    board.setPaused(paused);
                    pausedStartTime = std::chrono::high_resolution_clock::now();
                    break;
                case lb:
                    // Paused before opening the leaderboard?
                    bool wasPaused;
                    wasPaused = paused;
                    // Pause the game
                    if (!paused) {
                        this->paused = true;
                        board.setPaused(true);
                        pausedStartTime = std::chrono::high_resolution_clock::now();
                        board.render(window, tileTextures);
                        window.display();
                    }
                    // Open leaderboard window
                    displayLeaderboard();

                    // Once closed, check previous pause state and apply it
                    paused = wasPaused;
                    board.setPaused(wasPaused);
                    if (!paused) {
                        board.render(window, tileTextures);
                    }
                    pausedEndTime = std::chrono::high_resolution_clock::now();
                    window.display();
                    break;
                case debug:
                    // Toggle debug mode
                    board.setDebug(!board.isDebugMode());
                    break;
            }
            // Stop after first button click
            return;
        }
    }
}

void TrayGui::displayLeaderboard() {
    sf::RenderWindow lbWindow(sf::VideoMode(boardDimensions.first * 16,
                                            (boardDimensions.second * 16) + 50), "Minesweeper");

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        throw file_read_exception("Failed to load font!");
    }
    sf::Text leaderboardText = initializeLeaderboardHeaderText(lbWindow, font);
    sf::Text leaderboardContentText = initializeLeaderboardContentText(lbWindow, font);

    while (lbWindow.isOpen()) {
        sf::Event event{};
        while (lbWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                lbWindow.close();
                return;
            }
            // Keep leaderboard window on top
            if (event.type == sf::Event::LostFocus) {
                lbWindow.requestFocus();
                // Sleep to prevent window from flickering
                sf::sleep(sf::milliseconds(10));
            }
        }
        lbWindow.clear(sf::Color::Blue);
        lbWindow.draw(leaderboardText);
        lbWindow.draw(leaderboardContentText);
        lbWindow.display();
        // Sleep to avoid hogging system resources
        sf::sleep(sf::seconds(1.0f / 60));
    }
}

void TrayGui::writeScore(std::pair<std::string, std::string>& newEntry) const {
    std::fstream leaderboardFile("files/leaderboard.txt", std::ios::in);
    if (!leaderboardFile.good()) {
        throw file_read_exception("Failed to open files/leaderboard.txt!");
    }
    std::vector<std::pair<std::string, std::string>> lbEntries = {newEntry};
    std::string contentString;
    while (!leaderboardFile.eof()) {
        std::string time;
        std::string leaderboardName;
        std::getline(leaderboardFile, time, ',');
        // Skip the space
        if (hasSpace) {
            std::getline(leaderboardFile, leaderboardName, ' ');
        }
        std::getline(leaderboardFile, leaderboardName, '\n');
        std::pair<std::string, std::string> lbEntry = {time, leaderboardName};
        lbEntries.push_back(lbEntry);
    }
    std::sort(lbEntries.begin(), lbEntries.end());
    lbEntries.pop_back();
    // Done reading, time to write
    leaderboardFile.close();
    leaderboardFile.open("files/leaderboard.txt", std::ios::out);
    for (const auto& entry: lbEntries) {
        if (hasSpace) {
            leaderboardFile << entry.first << ", " << entry.second;
        } else {
            leaderboardFile << entry.first << "," << entry.second;

        }
        if (entry != lbEntries.back()) {
            leaderboardFile << std::endl;
        }
    }
}

sf::Text TrayGui::initializeLeaderboardHeaderText(const sf::RenderWindow& window, const sf::Font& font) {
    sf::Text leaderboardText;
    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setStyle(sf::Text::Underlined | sf::Text::Bold);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setCharacterSize(20);

    sf::FloatRect lbTextRect = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin(lbTextRect.left + lbTextRect.width / 2.0f,
                              lbTextRect.top + lbTextRect.height / 2.0f);
    leaderboardText.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                                static_cast<float>(window.getSize().y) / 2.0f - 120);
    return leaderboardText;
}

sf::Text TrayGui::initializeLeaderboardContentText(const sf::RenderWindow& window, const sf::Font& font) {
    std::string contentString;
    std::chrono::duration<double, std::milli> elapsedGameTimeSeconds = updateGameTime();
    long long elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(elapsedGameTimeSeconds).count();
    long long elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedGameTimeSeconds).count();
    if (elapsedMinutes >= 99) {
        elapsedMinutes = 99;
    }
//    std::string timeString = std::to_string(elapsedMinutes) + ":" + std::to_string(elapsedSeconds);
    std::stringstream formattedTime;
    formattedTime << std::setfill('0') << std::setw(2) << elapsedMinutes << ":" << std::setw(2) << elapsedSeconds;
    gameOver && gameWon ? contentString = getLeaderboardString(name, formattedTime.str()) : contentString = getLeaderboardString();
    sf::Text leaderboardContentText;
    leaderboardContentText.setFont(font);
    leaderboardContentText.setString(contentString);
    leaderboardContentText.setStyle(sf::Text::Bold);
    leaderboardContentText.setFillColor(sf::Color::White);
    leaderboardContentText.setCharacterSize(18);

    sf::FloatRect welcomeTextRect = leaderboardContentText.getLocalBounds();
    leaderboardContentText.setOrigin(welcomeTextRect.left + welcomeTextRect.width / 2.0f,
                                     welcomeTextRect.top + welcomeTextRect.height / 2.0f);
    leaderboardContentText.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                                       static_cast<float>(window.getSize().y) / 2.0f + 20);
    return leaderboardContentText;
}

std::string TrayGui::getLeaderboardString(const std::string& lbName, const std::string& lbTime) const {
    std::ifstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.good()) {
        throw file_read_exception("Failed to open files/leaderboard.txt!");
    }
    std::string contentString;
    int iteration = 1;
    while (!leaderboardFile.eof()) {
        std::string time;
        std::string leaderboardName;
        std::getline(leaderboardFile, time, ',');
        if (hasSpace) {
            std::getline(leaderboardFile, leaderboardName, ' ');
        }
        std::getline(leaderboardFile, leaderboardName, '\n');

        // Build content string
        contentString.append(std::to_string(iteration)).append(std::string(".")).append
        ( std::string("\t")).append(time).append(std::string("\t")).append(leaderboardName);

        if (lbName == leaderboardName && lbTime == time) {
            contentString += "*";
        }
        contentString.append("\n\n");
        iteration++;

    }
    return contentString;
}

std::string TrayGui::getLeaderboardString() const {
    std::ifstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.good()) {
        throw file_read_exception("Failed to open files/leaderboard.txt!");
    }
    std::string contentString;
    std::stringstream buffer;
    int iteration = 1;
    // Jump to beginning of file and clear the eof bit
    leaderboardFile.clear();
    leaderboardFile.seekg(0, std::ios::beg);
    while (!leaderboardFile.eof()) {
        std::string time;
        std::string leaderboardName;
        std::getline(leaderboardFile, time, ',');
        // Skip the space if the original file has spaces
        if (hasSpace) {
            std::getline(leaderboardFile, leaderboardName, ' ');
        }
        std::getline(leaderboardFile, leaderboardName, '\n');

        // Build content string
        contentString += std::to_string(iteration) += std::string(".") += std::string("\t") +=
        time += std::string("\t") += leaderboardName += "\n\n";
        iteration++;
    }
    return contentString;
}

bool TrayGui::hasSpaces() {
    std::ifstream leaderboardFile("files/leaderboard.txt");
    if (!leaderboardFile.good()) {
        throw file_read_exception("Failed to open files/leaderboard.txt!");
    }
    char c;
    while (leaderboardFile >> std::noskipws >> c) {
        if (c == ' ') {
            leaderboardFile.close();
            return true;
        }
    }
    leaderboardFile.close();
    return false;
}

