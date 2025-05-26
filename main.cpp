
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

    //ערך הכפתורים לצורך מיפוי לפי פעולות
    std::vector<std::tuple<sf::RectangleShape, size_t, std::string>> actionButtons;


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
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        //sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                        // בדיקת לחיצות על כפתורים
                        if (state == GAME_SCREEN) {
                            for (const auto& ab : actionButtons) {
                                if (std::get<0>(ab).getGlobalBounds().contains(mouse)) {
                                    size_t playerIndex = std::get<1>(ab);
                                    std::string action = std::get<2>(ab);
                                    Player* currentPlayer = game.getPlayers()[playerIndex];
                                    if (action == "gather") {currentPlayer->gather(); }
                                    else if (action == "tax") { currentPlayer->tax();}
                                    else if (action == "bribe") {currentPlayer->bribe();}
                                    // else if (ab.action == "foreign_aid") {
                                    //     currentPlayer->foreign_aid();
                                    // }
                                    // הוסף עוד פעולות אם צריך
                                }

                            }
                        }
                    }

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
                        sf::RectangleShape box(sf::Vector2f(300, 30));
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
    
                        state = GAME_SCREEN;
                        std::cout << "Total players in game after adding: " << game.getPlayers().size() << std::endl;
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
            sf::Text gameTitle("GAME IN PROGRESS", font, 20);
            gameTitle.setFillColor(sf::Color::White);
            sf::FloatRect textRect = gameTitle.getLocalBounds();
            gameTitle.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            gameTitle.setPosition(window.getSize().x / 2.0f, 20);
            window.draw(gameTitle);

            const auto& gamePlayers = game.getPlayers();
            std::vector<std::string> actions = {"gather", "tax", "bribe", "arrest", "sanction", "coup"};

            float startY = 70.f; // מיקום תחילת רשימת השחקנים לאורך ציר y
            float playerBoxHeight = 25.f;
            //float playerBoxWidth = 60.f;
            float actionButtonWidth = 55.f;
            float actionButtonHeight = 18.f;
            float spacingY = 45.f;
            float spacingX = 3.f;

            ///
            //const auto& gamePlayers = game.getPlayers();

            actionButtons.clear();

            for (size_t i = 0; i < gamePlayers.size(); ++i) {
                float baseYPos =startY + i* spacingY ;
                
                // תיבת שחקן
                sf::RectangleShape playerBox(sf::Vector2f(90, playerBoxHeight));
                playerBox.setFillColor(sf::Color::White);
                playerBox.setPosition(10, baseYPos);
                
                // שם השחקן ותפקידו
                std::string playerName = gamePlayers[i]->getName();
                std::string role = gamePlayers[i]->getRole();
                sf::Text playerNameText(playerName + " (" + role + ")", font, 12);
                playerNameText.setFillColor(sf::Color::Black);
                playerNameText.setPosition(15, baseYPos + 8);

                window.draw(playerBox);
                window.draw(playerNameText);
                
                // בניית רשימת פעולות לשחקן הנוכחי
                std::vector<std::string> basicActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup"};
                std::vector<std::string> specialActions;

                // הוספת פעולות מיוחדות לפי תפקיד
                if (role == "Governor") {
                    specialActions.push_back("UNDO");
                }
                else if (role == "Spy") {
                    specialActions.push_back("view coins");
                    specialActions.push_back("block arrest");
                }
                else if (role == "Baron") {
                    specialActions.push_back("INVEST");
                }
                else if (role == "General") {
                    specialActions.push_back("cancel coup");
                }
                else if (role == "Judge") {
                    specialActions.push_back("UNDO");
                }

                float actionStartX = 110.f;

                for (size_t j = 0; j < basicActions.size(); ++j) {
                float x = actionStartX + j * (actionButtonWidth + spacingX);

                sf::RectangleShape button(sf::Vector2f(actionButtonWidth, actionButtonHeight));
                button.setFillColor(sf::Color::Red);
                button.setPosition(x, baseYPos + 2.f);

                sf::Text text(basicActions[j], font, 10); // פונט קטן יותר
                text.setFillColor(sf::Color::Black);
                text.setPosition(
                    x + actionButtonWidth / 2 - text.getGlobalBounds().width / 2,
                    baseYPos + 2.f + actionButtonHeight / 2 - text.getGlobalBounds().height / 2
                );
                window.draw(button);
                window.draw(text);
                // actionButtons.push_back({button, i, basicActions[j]});
                }

             // ציור פעולות מיוחדות - שורה שנייה (אם קיימות)
            if (!specialActions.empty()) {
                for (size_t j = 0; j < specialActions.size(); ++j) {
                    float x = actionStartX + (basicActions.size() + j) * (actionButtonWidth + spacingX);

                    // float x = actionStartX + j * (actionButtonWidth + spacingX);
                    // float specialYPos = baseYPos + actionButtonHeight + 4.f; // שורה למטה
                    
                    sf::RectangleShape button(sf::Vector2f(actionButtonWidth, actionButtonHeight));
                    button.setFillColor(sf::Color::Blue); // צבע שונה לפעולות מיוחדות
                    button.setPosition(x, baseYPos);//specialYpos
                    
                    sf::Text text(specialActions[j], font, 9); // פונט עוד יותר קטן
                    text.setFillColor(sf::Color::White);
                    text.setPosition(
                        x + actionButtonWidth / 2 - text.getGlobalBounds().width / 2,
                        baseYPos + actionButtonHeight / 2 - text.getGlobalBounds().height / 2
                    );
                    
                    window.draw(button);
                    window.draw(text);
                    actionButtons.push_back({button, i, basicActions[j]});
                }
            }
           


            if (!gamePlayers.empty()) {
                std::string currentPlayerName = game.turn();
                //std::cout << "Name length: " << currentPlayerName.length() << std::endl;//new
                sf::Text turnText("Turn: " + currentPlayerName, font, 20);
                turnText.setFillColor(sf::Color::Yellow);
                
                sf::FloatRect turnBounds = turnText.getLocalBounds();
                float xPos = window.getSize().x - turnBounds.width - 10; // 10 פיקסלים מהקצה הימני
                float yPos = 10; // 10 פיקסלים מהחלק העליון
                turnText.setPosition(xPos, yPos);
                //turnText.setPosition(50, 200);
                window.draw(turnText);
            }
        }
    }

    window.display();
    }
    
    return 0;
}
