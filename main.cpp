#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>

// Bad file read exception
class file_read_exception : public std::exception {
private:
    std::string message;
public:
    explicit file_read_exception(const char *msg) : message(msg) {}

    // Override the what() method
    const char *what() const noexcept override {
        return message.c_str();
    }
};

// todo Perhaps this data structure isn't quite the right thing
std::vector<int> readConfig();

sf::Text initializeWelcomeText(const sf::RenderWindow &window, const sf::Font &font);

sf::Text initializeNameEntryText(const sf::RenderWindow &window, const sf::Font &font);

sf::Text fillNameEntryField(const sf::RenderWindow &window, const sf::Font &font, std::string name);

int main() {
    std::vector<int> gameParameters = readConfig();
    int colCount = gameParameters[0];
    int rowCount = gameParameters[1];
    int mineCount = gameParameters[2];

    // Render the welcome menu
    // Window object
    sf::RenderWindow window(sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Minesweeper");
    // Font text
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        // todo should load an error window
        throw file_read_exception("Failed to load font!");
    }

    sf::Text welcomeText = initializeWelcomeText(window, font);
    sf::Text nameEntryText = initializeNameEntryText(window, font);
    sf::Text nameEntryField = fillNameEntryField(window, font, "");

    std::string name;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (!std::isalpha(static_cast<char>(event.text.unicode)) || name.size() == 10) {
                    continue;
                }
                name += static_cast<char>(event.text.unicode);
                for (int i = 1; i < name.size(); i++) {
                    name[i] = static_cast<char>(std::tolower(name[i]));
                }
                name[0] = static_cast<char>(std::toupper(name[0]));
                nameEntryField = fillNameEntryField(window, font, name);
            }
        }

        // FIXME Change back to blue before submission!
        window.clear(sf::Color(120, 120, 120));
        window.draw(welcomeText);
        window.draw(nameEntryText);
        window.draw(nameEntryField);
        window.display();
        printf("The name of the player is: %s\n", name.c_str());
    }
    return 0;
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
    } catch (std::invalid_argument &e) {
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

sf::Text initializeWelcomeText(const sf::RenderWindow &window, const sf::Font &font) {
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

sf::Text initializeNameEntryText(const sf::RenderWindow &window, const sf::Font &font) {
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

sf::Text fillNameEntryField(const sf::RenderWindow &window, const sf::Font &font, std::string name) {
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