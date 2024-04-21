#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "Board.h"
#include "TrayGui.h"
#include "file_read_exception.h"

// todo Perhaps this data structure isn't quite the right thing
std::vector<int> readConfig();

sf::Text initializeWelcomeText(const sf::RenderWindow& window, const sf::Font& font);

sf::Text initializeNameEntryText(const sf::RenderWindow& window, const sf::Font& font);

sf::Text fillNameEntryField(const sf::RenderWindow& window, const sf::Font& font, std::string name);

bool renderWelcomeWindow(sf::RenderWindow& window, std::string& name);

void renderGameWindow(sf::RenderWindow& window, Board& board, TrayGui& gui);

std::vector<sf::Texture> loadTextures();

int main() {
    std::vector<int> gameParameters = readConfig();
    int colCount = gameParameters[0];
    int rowCount = gameParameters[1];
    int mineCount = gameParameters[2];

    std::pair<int, int> dimensions = {colCount, rowCount};

    printf("dimensions: %d, %d\n", dimensions.first, dimensions.second);

    // welcomeWindow object
    sf::RenderWindow welcomeWindow(sf::VideoMode(
            colCount * 32, rowCount * 32 + 100), "Minesweeper");

    std::string name;
    // Render the welcome menu
    if (renderWelcomeWindow(welcomeWindow, name)) {
        return EXIT_SUCCESS;
    }

    // If no name was entered, then the close button must have been pressed; exit now
    if (name.empty()) {
        printf("no name entered, must have clicked the X\n");
        return EXIT_SUCCESS;
    }

    // After closing the welcome menu create the game window
    welcomeWindow.close();
    sf::RenderWindow gameWindow(sf::VideoMode(
            colCount * 32, rowCount * 32 + 100), "Minesweeper");

    // Create a board and a TrayGui
    Board board = Board(dimensions, mineCount);
    TrayGui gui = TrayGui(dimensions);

    renderGameWindow(gameWindow, board, gui);
    // Load the board
    return EXIT_SUCCESS;
}

sf::Texture loadTexture(const std::string& fileName) {
    sf::Texture texture;
    if (!texture.loadFromFile(fileName)) {
        std::cerr << "Game texture " << fileName << " is missing!\n";
    }
    return texture;
}

// Load textures for sprites
std::vector<sf::Texture> loadTextures() {
    std::vector<sf::Texture> textures;
    textures.push_back(loadTexture("files/images/flag.png"));
    textures.push_back(loadTexture("files/images/number_1.png"));
    textures.push_back(loadTexture("files/images/number_2.png"));
    textures.push_back(loadTexture("files/images/number_3.png"));
    textures.push_back(loadTexture("files/images/number_4.png"));
    textures.push_back(loadTexture("files/images/number_5.png"));
    textures.push_back(loadTexture("files/images/number_6.png"));
    textures.push_back(loadTexture("files/images/number_7.png"));
    textures.push_back(loadTexture("files/images/number_8.png"));
    textures.push_back(loadTexture("files/images/debug.png"));
    textures.push_back(loadTexture("files/images/digits.png"));
    textures.push_back(loadTexture("files/images/face_happy.png"));
    textures.push_back(loadTexture("files/images/face_lose.png"));
    textures.push_back(loadTexture("files/images/face_win.png"));
    textures.push_back(loadTexture("files/images/leaderboard.png"));
    textures.push_back(loadTexture("files/images/mine.png"));
    textures.push_back(loadTexture("files/images/pause.png"));
    textures.push_back(loadTexture("files/images/play.png"));
    textures.push_back(loadTexture("files/images/tile_hidden.png"));
    textures.push_back(loadTexture("files/images/tile_revealed.png"));
    return textures;
}

// Main game window
// TODO create the board inside this function!
void renderGameWindow(sf::RenderWindow& window, Board& board, TrayGui& gui) {

    std::vector<sf::Texture> textures = loadTextures();
    enum textureIndices {
        flag, num1, num2, num3, num4, num5, num6, num7, num8, debug, digits, happy, lose, win, lb, mine,
        pause, play, hidden, revealed
    };

    std::vector<sf::Texture> tileTextures = {textures[flag], textures[num1], textures[num2], textures[num3],
                                             textures[num4], textures[num5], textures[num6], textures[num7],
                                             textures[num8], textures[mine], textures[hidden], textures[revealed]};

    std::vector<sf::Texture> guiTextures = {textures[debug], textures[digits], textures[happy], textures[lose],
                                            textures[win], textures[lb], textures[pause], textures[play]};

    board.print();
    while (window.isOpen()) {
        sf::Event event{};
        if (board.isGameOver()) {
            gui.setGameOver(true);
            gui.setGameWon(board.isGameWon());

        }
        while (window.pollEvent(event)) {
            // Close the window if closed by the OS
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                bool isLeftMouseButton;
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isLeftMouseButton = true;
                    if (!board.isGameOver()) {
                        board.click(window, mousePosition, isLeftMouseButton);
                    }
                    // TODO maybe there should be a function in Board that counts the number of flags
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    isLeftMouseButton = false;
                    if (!board.isGameOver()) {
                        board.click(window, mousePosition, isLeftMouseButton);
                    }
                }
                gui.click(window, mousePosition, tileTextures, board);
            }
        }
        window.clear(sf::Color::White);
        board.render(window, tileTextures);
        gui.render(window, guiTextures, board.getMines(), board.getFlags());
        window.display();
        // Save on processing power so the user doesn't think the program is mining bitcoin
        sf::sleep(sf::seconds(1.0f/60));
    }
}

bool renderWelcomeWindow(sf::RenderWindow& window, std::string& name) {
    // Font text
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        // todo should load an error window
        throw file_read_exception("Failed to load font!");
    }

    // Text fields
    sf::Text welcomeText = initializeWelcomeText(window, font);
    sf::Text nameEntryText = initializeNameEntryText(window, font);
    sf::Text nameEntryField = fillNameEntryField(window, font, "");

    // Event loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Close the window if closed by the OS
            if (event.type == sf::Event::Closed) {
                window.close();
                return true;
            }
            // Return once a name is submitted
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !name.empty()) {
                return false;
            }
            // Get name input
            if (event.type == sf::Event::TextEntered) {
                // Escape sequence for backspace is \b
                if (event.text.unicode == '\b' && !name.empty()) {
                    name.pop_back();
                    nameEntryField = fillNameEntryField(window, font, name);
                    continue;
                }
                // Don't add the character if it's not alphanumeric or if the name's length is >= 10
                if (!isalpha(static_cast<char>(event.text.unicode)) || name.size() >= 10) {
                    continue;
                }
                // Append the character to the name
                name += static_cast<char>(event.text.unicode);
                // Force title-case
                name[0] = static_cast<char>(toupper(name[0]));
                for (int i = 1; i < name.size(); i++) {
                    name[i] = static_cast<char>(tolower(name[i]));
                }
                // Render the name
                nameEntryField = fillNameEntryField(window, font, name);
            }
        }

        // Draw window elements
        // FIXME Change back to blue before submission!
        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(nameEntryText);
        window.draw(nameEntryField);
        window.display();
    }
    return true;
}

std::vector<int> readConfig() {
    // Open the config file
    std::ifstream configFile = std::ifstream("files/config.cfg");
    if (!configFile.good()) {
        // TODO maybe this should open a window instead of going to standard output
        throw file_read_exception("Failed to open config file config.cfg!");
    }

    std::string colCountString;
    std::string rowCountString;
    std::string mineCountString;

    int colCount;
    int rowCount;
    int mineCount;

    // Read the contents of the config file
    std::getline(configFile, colCountString, '\n');
    std::getline(configFile, rowCountString, '\n');
    std::getline(configFile, mineCountString, '\n');

    try {
        colCount = std::stoi(colCountString);
        rowCount = std::stoi(rowCountString);
        mineCount = std::stoi(mineCountString);
    } catch (std::invalid_argument& e) {
        configFile.close();
        throw file_read_exception("File config.cfg has invalid contents!");
    }
    // TODO remove, debugging
    printf("colCount: %d\n", colCount);
    printf("rowCount: %d\n", rowCount);
    printf("mineCount: %d\n", mineCount);

    std::vector<int> vec = {colCount, rowCount, mineCount};
    return vec;

}

sf::Text initializeWelcomeText(const sf::RenderWindow& window, const sf::Font& font) {
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setStyle(sf::Text::Underlined | sf::Text::Bold);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setCharacterSize(24);

    sf::FloatRect welcomeTextRect = welcomeText.getLocalBounds();
    welcomeText.setOrigin(welcomeTextRect.left + welcomeTextRect.width / 2.0f,
                          welcomeTextRect.top + welcomeTextRect.height / 2.0f);
    welcomeText.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                            static_cast<float>(window.getSize().y) / 2.0f - 150);
    return welcomeText;
}

sf::Text initializeNameEntryText(const sf::RenderWindow& window, const sf::Font& font) {
    sf::Text nameEntryText;
    nameEntryText.setFont(font);
    nameEntryText.setString("Enter your name:");
    nameEntryText.setStyle(sf::Text::Bold);
    nameEntryText.setFillColor(sf::Color::White);
    nameEntryText.setCharacterSize(20);

    sf::FloatRect nameEntryTextRect = nameEntryText.getLocalBounds();
    nameEntryText.setOrigin(nameEntryTextRect.left + nameEntryTextRect.width / 2.0f,
                            nameEntryTextRect.top + nameEntryTextRect.height / 2.0f);

    nameEntryText.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                              static_cast<float>(window.getSize().y) / 2.0f - 75);

    return nameEntryText;
}

sf::Text fillNameEntryField(const sf::RenderWindow& window, const sf::Font& font, std::string name) {
    sf::Text nameEntryField;
    nameEntryField.setFont(font);
    name += '|';
    nameEntryField.setString(name);
    nameEntryField.setStyle(sf::Text::Bold);
    nameEntryField.setFillColor(sf::Color::White);
    nameEntryField.setCharacterSize(20);

    sf::FloatRect nameEntryFieldRect = nameEntryField.getLocalBounds();
    nameEntryField.setOrigin(nameEntryFieldRect.left + nameEntryFieldRect.width / 2.0f,
                             nameEntryFieldRect.top + nameEntryFieldRect.height / 2.0f);

    nameEntryField.setPosition(static_cast<float>(window.getSize().x) / 2.0f,
                               static_cast<float>(window.getSize().y) / 2.0f - 45);

    return nameEntryField;
}