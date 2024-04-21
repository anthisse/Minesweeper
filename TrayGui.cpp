#include "TrayGui.h"

// Default constructor
TrayGui::TrayGui(std::pair<int, int>& boardDimensions) {
    this->boardDimensions = boardDimensions;
    startTime = std::chrono::high_resolution_clock::now();
    endTime = std::chrono::high_resolution_clock::now();
    pausedStartTime = std::chrono::high_resolution_clock::now();
    pausedEndTime = std::chrono::high_resolution_clock::now();
    totalPausedTime = pausedEndTime - pausedStartTime;
    paused = false;
    this->gameOver = false;
    this->gameWon = false;
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

    renderTimer(window, digitTexture);
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
                    printf("face button clicked!\n");
                    // Reset time and the board
                    board.reset();
                    this->gameOver = false;
                    this->gameWon = false;
                    startTime = std::chrono::high_resolution_clock::now();
                    endTime = std::chrono::high_resolution_clock::now();
                    pausedStartTime = std::chrono::high_resolution_clock::now();
                    pausedEndTime = std::chrono::high_resolution_clock::now();
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
                    openLeaderboard();

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

void TrayGui::openLeaderboard() {
    sf::RenderWindow lbWindow(sf::VideoMode(boardDimensions.first * 16,
                                            (boardDimensions.second * 16) + 50), "Minesweeper");

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        // todo should load an error window
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
    }
}

sf::Text TrayGui::initializeLeaderboardHeaderText(const sf::RenderWindow& window, const sf::Font& font) {
    sf::Text leaderboardText;
    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setStyle(sf::Text::Underlined | sf::Text::Bold);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setCharacterSize(20);

    sf::FloatRect welcomeTextRect = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin(welcomeTextRect.left + welcomeTextRect.width / 2.0f,
                              welcomeTextRect.top + welcomeTextRect.height / 2.0f);
    leaderboardText.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                                static_cast<float>(window.getSize().y) / 2.0f - 120);
    return leaderboardText;
}

sf::Text TrayGui::initializeLeaderboardContentText(const sf::RenderWindow& window, const sf::Font& font) {
    std::ifstream leaderboardFile("files/leaderboard.txt");

    if (!leaderboardFile.good()) {
        // TODO should open error window
        throw file_read_exception("Failed to open files/leaderboard.txt!");
    }
    std::string contentString;
    while (!leaderboardFile.eof()) {
        int iteration;
        std::string time;
        std::string name;
        std::getline(leaderboardFile, time, ',');
        std::getline(leaderboardFile, name, ' '); // Skip the space
        std::getline(leaderboardFile, name, '\n');

        // Build content string
        contentString += std::to_string(iteration) += std::string(".") += std::string("\t") +=
        time += std::string("\t") += name += std::string("\n\n");
        iteration++;
    }
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