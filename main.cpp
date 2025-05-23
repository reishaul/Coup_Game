
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Game.hpp"
#include "Baron.hpp"
#include "Spy.hpp"
#include "Governor.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <memory>
#include <random>
//QT_QUICK_BACKEND=software ./coup_game


using namespace coup;


int main() {
    Game game;
    std::vector<std::unique_ptr<Player>> players;
    std::string currentInput;
    sf::Text inputText;
    std::vector<std::pair<sf::RectangleShape, sf::Text>> playerBoxes;

    sf::RenderWindow window(sf::VideoMode(600, 400), "Coup Game GUI");

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return -1;
    }

    enum State { START_SCREEN, ADD_PLAYERS, GAME_SCREEN };
    State state = START_SCREEN;

    // START SCREEN
    sf::Color brown(139, 69, 19);
    sf::Text title("COUP GAME", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(600 / 2 - title.getGlobalBounds().width / 2, 50);

    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(600 / 2 - 100, 300);

    sf::Text startText("START", font, 24);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(
        startButton.getPosition().x + 100 - startText.getGlobalBounds().width / 2,
        startButton.getPosition().y + 10
    );

    // ADD PLAYER SCREEN
    sf::RectangleShape addPlayerButton(sf::Vector2f(150, 40));
    addPlayerButton.setFillColor(sf::Color::White);
    addPlayerButton.setPosition(225, 300);

    sf::Text addText("ADD PLAYER", font, 20);
    addText.setFillColor(sf::Color::Black);
    addText.setPosition(
        addPlayerButton.getPosition().x + 75 - addText.getGlobalBounds().width / 2,
        addPlayerButton.getPosition().y + 8
    );

    sf::RectangleShape playButton(sf::Vector2f(100, 40));
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(250, 350);

    sf::Text playText("PLAY", font, 20);
    playText.setFillColor(sf::Color::Black);
    playText.setPosition(
        playButton.getPosition().x + 50 - playText.getGlobalBounds().width / 2,
        playButton.getPosition().y + 8
    );


    std::vector<std::string> playerNames;
    std::vector<sf::Text> playerTexts;

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(50, 260);
    inputText.setString("");

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(18);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50, 220);
    std::string errorMessage = "";


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered && state == ADD_PLAYERS) {
                if (event.text.unicode == '\b') {
                    if (!currentInput.empty())
                        currentInput.pop_back();
                } else if (event.text.unicode == '\r') {
                    // Enter - נא לא לפעול כאן
                } else if (event.text.unicode < 128) {
                    currentInput += static_cast<char>(event.text.unicode);
                }
                inputText.setString(currentInput);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

                if (state == START_SCREEN && startButton.getGlobalBounds().contains(mouse)) {
                    state = ADD_PLAYERS;
                }
                
                if (state == ADD_PLAYERS && addPlayerButton.getGlobalBounds().contains(mouse)) {
                    if (!currentInput.empty() && playerNames.size() < 6) {
                        std::string name = currentInput;

                        // מניעת שמות כפולים
                        bool exists = false;
                        for (const auto& n : playerNames) {
                            if (n == name) {
                                exists = true;
                                break;
                            }
                        }
                        if (exists) {
                            errorMessage = "Name already exists!";
                            errorText.setString(errorMessage);
                            continue;
                        }
                        // ניקוי הודעת שגיאה קודמת
                        errorMessage = "";
                        errorText.setString("");

                        playerNames.push_back(name);

                        // תצוגת השם
                        sf::Text newText(name, font, 20);
                        newText.setFillColor(sf::Color::White);
                        newText.setPosition(50, 50 + playerTexts.size() * 30);
                        playerTexts.push_back(newText);

                        // הגרלת תפקיד
                        std::vector<std::string> roles = {"Governor", "Spy", "Baron", "General", "Judge", "Merchant"};
                        static std::random_device rd;
                        static std::mt19937 gen(rd());
                        std::uniform_int_distribution<> dis(0, roles.size() - 1);
                        std::string role = roles[dis(gen)];

                        if (role == "Baron") {
                            players.push_back(std::make_unique<Baron>(game, name));
                        } else if (role == "Spy") {
                            players.push_back(std::make_unique<Spy>(game, name));
                        } else if (role == "Governor") {
                            players.push_back(std::make_unique<Governor>(game, name));
                        } else if (role == "General") {
                            players.push_back(std::make_unique<General>(game, name));
                        } else if (role == "Judge") {
                            players.push_back(std::make_unique<Judge>(game, name));
                        } else if (role == "Merchant") {
                            players.push_back(std::make_unique<Merchant>(game, name));
                        }

                        currentInput.clear();
                        inputText.setString("");

                        // יצירת תיבה עם שם ותפקיד
                        sf::RectangleShape box(sf::Vector2f(500, 40));
                        box.setFillColor(sf::Color::White);
                        box.setPosition(50, 50 + playerBoxes.size() * 50);

                        sf::Text playerInfo(name + " - " + role, font, 18);
                        playerInfo.setFillColor(sf::Color::Black);
                        playerInfo.setPosition(box.getPosition().x + 10, box.getPosition().y + 8);

                        playerBoxes.push_back({box, playerInfo});
                    }
                }
                if (state == ADD_PLAYERS && playButton.getGlobalBounds().contains(mouse)) {
                    if (playerNames.size() < 2) {
                        errorMessage = "Need at least 2 players!";
                        errorText.setString(errorMessage);
                    } else if (playerNames.size() > 6) {
                        errorMessage = "Maximum 6 players allowed!";
                        errorText.setString(errorMessage);
                    } else {
                        // הכל בסדר - מתחילים את המשחק
                        errorMessage = "";
                        errorText.setString("");
                        
                        // הוסף את השחקנים למשחק
                        for (auto& player : players) {
                            game.add_player(*player);
                        }
                        players.clear(); // מנקה את הוקטור המקומי
                        
                        // התחל את המשחק
    
                        state = GAME_SCREEN;
                    }
                }
            }
        }
        

        if (state == START_SCREEN) {
            window.clear(brown);
            window.draw(title);
            window.draw(startButton);
            window.draw(startText);
        }
        else if (state == ADD_PLAYERS) {
            window.clear(brown);

            for (const auto& box : playerBoxes) {
                window.draw(box.first);
                window.draw(box.second);
            }
            window.draw(addPlayerButton);
            window.draw(addText);
            window.draw(inputText);

            window.draw(playButton);
            window.draw(playText);
                // הצגת הודעות שגיאה
            if (!errorMessage.empty()) {
                window.draw(errorText);
            }
        }
        else if (state == GAME_SCREEN) {
            window.clear(brown);
            
            // כאן תוסיף את ממשק המשחק
            sf::Text gameTitle("GAME IN PROGRESS", font, 30);
            gameTitle.setFillColor(sf::Color::White);
            gameTitle.setPosition(150, 150);
            window.draw(gameTitle);
        }

        window.display();
    }

    return 0;
}
