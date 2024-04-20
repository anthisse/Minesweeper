#include "TrayGui.h"

// Default constructor
TrayGui::TrayGui(std::pair<int, int>& boardDimensions, const int& mines) {
    this->boardDimensions = boardDimensions;
    startTime = std::chrono::high_resolution_clock::now();
    endTime = std::chrono::high_resolution_clock::now();
    paused = false;
    this->mines = mines;
    this->flags = 0;
}

// TODO does not account for time paused
long long TrayGui::getElapsedSeconds() {
    endTime = std::chrono::high_resolution_clock::now();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    return elapsedSeconds.count();
}

bool TrayGui::isPaused() const {
    return paused;
}

void TrayGui::setPaused(bool p) {
    this->paused = p;
}

void TrayGui::incrementFlags() {
    this->flags++;
}

void TrayGui::decrementFlags() {
    this->flags--;
}

void TrayGui::render(sf::RenderWindow& window, std::vector<sf::Texture>& textures) const {
    enum guiTextures {
        debug, digits, happy, lose, win, lb, pause, play
    };
    sf::Texture digitTexture = textures[digits];

    renderMinesRemaining(window, digitTexture);

    // Face button
    // TODO if (!gameOver) .... track game state with private member
    sf::Sprite gameStateSprite(textures[happy]);
    gameStateSprite.setPosition(static_cast<float>(boardDimensions.first * 16 - 32),
                                static_cast<float>(32 * (boardDimensions.second + 0.5)));
    window.draw(gameStateSprite);

    // Pause button
    sf::Sprite pauseButtonSprite;
    paused ? pauseButtonSprite.setTexture(textures[pause]) : pauseButtonSprite.setTexture(textures[play]);
    pauseButtonSprite.setPosition(static_cast<float>(boardDimensions.first * 32) - 240,
                                  static_cast<float>(32 * (boardDimensions.second + 0.5)));
    window.draw(pauseButtonSprite);

    // Leaderboard button
    sf::Sprite leaderboardSprite(textures[lb]);
    leaderboardSprite.setPosition(static_cast<float>(boardDimensions.first * 32 - 176),
                                  static_cast<float>(32 * (boardDimensions.second + 0.5)));

    window.draw(leaderboardSprite);

    // Draw debug sprite
    sf::Sprite debugSprite(textures[debug]);
    debugSprite.setPosition(static_cast<float>(boardDimensions.first * 32 - 304),
                            static_cast<float>(32 * (boardDimensions.second + 0.5)));
    window.draw(debugSprite);

    renderTimer(window, digitTexture);
}

void TrayGui::renderMinesRemaining(sf::RenderWindow& window, const sf::Texture& texture) const {
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
                          const sf::Texture& texture) const {
    // Get minutes and seconds elapsed
    long long elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(endTime - startTime).count();
    long long elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    // Int divide by ten to get ten's digit and mod by ten to get one's digit
    int topMinutesDigit = static_cast<int>(elapsedMinutes) / 10;
    int bottomMinutesDigit = static_cast<int>(elapsedMinutes) % 10;
    int topSecondsDigit = static_cast<int>(elapsedSeconds) / 10;
    int bottomSecondsDigit = static_cast<int>(elapsedSeconds) % 10;

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
