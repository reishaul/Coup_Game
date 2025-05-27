//reishaul1@gmail.com
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

//global variables to manage the current action state and the action initiator
enum class ActionState {
    NONE,
    SELECTING_TARGET_FOR_ARREST,
    SELECTING_TARGET_FOR_SANCTION,
    SELECTING_TARGET_FOR_COUP,
    SELECTING_TARGET_FOR_CANCEL_COUP,
    SELECTING_TARGET_FOR_UNDO,
    SELECTING_TARGET_FOR_BLOCK_ARREST,
    SELECTING_TARGET_FOR_VIEW_COINS
};
ActionState currentActionState = ActionState::NONE;
Player* actionInitiator = nullptr;

//function to handle target selection based on mouse position
void handleTargetSelection(sf::Vector2f mousePos, Game& game) {
    const auto& players = game.getPlayers();
    
    // Check if clicked on one of the players
    for (size_t i = 0; i < players.size(); ++i) {
        // calculate the position of the player box
        float baseYPos = 70.f + i * 45.f;
        
        // check if the mouse position is within the bounds of the player box
        if (mousePos.x >= 10 && mousePos.x <= 100 && 
            mousePos.y >= baseYPos && mousePos.y <= baseYPos + 25) {
            
            Player* target = players[i];
            
            //check if the target is the action initiator
            if (target == actionInitiator) {
                //std::cout << "Cannot target yourself!" << std::endl;
                //return;
            }

            // Perform the action with the selected target
            try {
                switch (currentActionState) {
                        
                    case ActionState::SELECTING_TARGET_FOR_ARREST:
                        actionInitiator->arrest(*target);
                        break;
                        
                    case ActionState::SELECTING_TARGET_FOR_SANCTION:
                        actionInitiator->sanction(*target);
                        break;
                        
                    case ActionState::SELECTING_TARGET_FOR_COUP:
                        actionInitiator->coup(*target);
                        break;
                    //cancel coup is performed by General only
                    case ActionState::SELECTING_TARGET_FOR_CANCEL_COUP:{
                        General* general = dynamic_cast<General*>(actionInitiator);//using dynamic cast to check if the actionInitiator is a General
                        if (general) {
                            general->cancelCoup(*target);
                        } else {
                            std::cout << "Only General can cancel coup!" << std::endl;
                        }
                        break;
                    }
                    //undo is performed by Governor or Judge only
                    case ActionState::SELECTING_TARGET_FOR_UNDO:{
                        Governor* governor = dynamic_cast<Governor*>(actionInitiator);
                        Judge* judge = dynamic_cast<Judge*>(actionInitiator);

                        if (governor) {
                            governor->undo(*target);
                        } else if (judge) {
                            judge->undo(*target);
                        } else {
                            std::cout << "Only Governor or Judge can undo!" << std::endl;
                        }
                        break;
                    }
                    //block arrest is performed by Spy only
                    case ActionState::SELECTING_TARGET_FOR_BLOCK_ARREST:{
                        Spy* spy = dynamic_cast<Spy*>(actionInitiator);
                        if (spy) {
                            spy->blockArrest(*target);
                        } else {
                            std::cout << "Only Spy can block arrest!" << std::endl;
                        }
                        break;
                    }
                    //view coins is performed by Spy only
                    case ActionState::SELECTING_TARGET_FOR_VIEW_COINS:{
                        Spy* spy = dynamic_cast<Spy*>(actionInitiator);
                        if (spy) {
                            spy->viewCoins(*target);
                        } else {
                            std::cout << "Only Spy can view coins!" << std::endl;
                        }
                        break;
                    }
                    default:
                        std::cout << "No valid action selected." << std::endl;
                        break;
                }
                
            } catch (const std::exception& e) {
                std::cout << "Action failed: " << e.what() << std::endl;
            }
            
            // reset the action state and initiator
            currentActionState = ActionState::NONE;
            actionInitiator = nullptr;
            break;
        }
    }
}

int main() {
    //initate the game object and data structure to be used in the game
    Game game;
    std::vector<std::unique_ptr<Player>> players;
    std::string currentInput;
    sf::Text inputText;
    std::vector<std::pair<sf::RectangleShape, sf::Text>> playerBoxes;

    //design the main window
    sf::RenderWindow window(sf::VideoMode(600, 400), "Coup Game GUI");
    // Load a font for text rendering
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {//
        return -1;
    }

    //enum State to manage the different states of the game
    enum State { START_SCREEN, ADD_PLAYERS, GAME_SCREEN };
    State state = START_SCREEN;

    // Initialize action buttons
    std::vector<std::tuple<sf::RectangleShape, size_t, std::string>> actionButtons;


    // START SCREEN
    // Set background color and other UI elements
    sf::Color brown(139, 69, 19);
    sf::Text title("COUP GAME", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(600 / 2 - title.getGlobalBounds().width / 2, 50);

    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(600 / 2 - 100, 300);
    //adding text to the start button
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
    // Adding text to the add player button
    sf::Text addText("ADD PLAYER", font, 20);
    addText.setFillColor(sf::Color::Black);
    addText.setPosition(
        addPlayerButton.getPosition().x + 75 - addText.getGlobalBounds().width / 2,
        addPlayerButton.getPosition().y + 8
    );
    //design the play button
    sf::RectangleShape playButton(sf::Vector2f(100, 40));
    playButton.setFillColor(sf::Color::White);
    playButton.setPosition(250, 350);

    sf::Text playText("PLAY", font, 20);
    playText.setFillColor(sf::Color::Black);
    playText.setPosition(
        playButton.getPosition().x + 50 - playText.getGlobalBounds().width / 2,
        playButton.getPosition().y + 8
    );
    //vectors to store player names and texts 
    std::vector<std::string> playerNames;
    std::vector<sf::Text> playerTexts;

    // Text input for player names 
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(50, 260);
    inputText.setString("");
    //design error message text
    sf::Text errorText;
    errorText.setFont(font);
    errorText.setCharacterSize(18);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(50, 220);
    std::string errorMessage = "";

    
    while (window.isOpen()) {
        sf::Event event;
        /*
        Main Event Loop:
        Handles window close, text input, and mouse clicks.
        - Closes window when "X" is clicked.
        - In ADD_PLAYERS state:
            * Collects text input for player names.
            * Adds a new player when "Add Player" is clicked (if valid).
            * Starts the game when "Play" is clicked (if enough players).
        - In GAME_SCREEN state:
            * Handles action button clicks (gather, tax, bribe, etc.).
            * For actions that need a target (e.g., coup, arrest), waits for target selection.
            * Executes special role actions (e.g., invest for Baron).
        */ 
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            // Handle text input for player names
            if (event.type == sf::Event::TextEntered && state == ADD_PLAYERS) {
                if (event.text.unicode == '\b') {
                    if (!currentInput.empty())
                        currentInput.pop_back();
                } else if (event.text.unicode == '\r') {
                } else if (event.text.unicode < 128) {
                    currentInput += static_cast<char>(event.text.unicode);
                }
                inputText.setString(currentInput);
            }
            // Handle mouse clicks 
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
                    if (event.mouseButton.button == sf::Mouse::Left) {

                        // check if we are in the game screen
                        if (state == GAME_SCREEN) {
                            // If we are in the target selection state
                            if (currentActionState != ActionState::NONE) {
                                handleTargetSelection(mouse, game);
                            }
                            // Otherwise, check clicks on action buttons
                            else {
                                for (const auto& ab : actionButtons) {
                                    if (std::get<0>(ab).getGlobalBounds().contains(mouse)) {
                                        /*
                                        Get the player index and action from the button.
                                        std::get<0>(ab) is the button, std::get<1>(ab) is the player index, std::get<2>(ab) is the action.
                                        */ 
                                        size_t playerIndex = std::get<1>(ab);
                                        std::string action = std::get<2>(ab);
                                        Player* currentPlayer = game.getPlayers()[playerIndex];

                                        // Actions that do not require a target
                                        if (action == "gather") {//check if the action on the button is gather 
                                            try {
                                                currentPlayer->gather();//perform gather action
                                            } catch (const std::exception& e) {//if an error occurs, catch it and print the error message
                                                std::cout << "Gather failed: " << e.what() << std::endl;
                                            }
                                        }
                                        else if (action == "tax") {
                                            try {
                                                currentPlayer->tax();//perform tax action
                                            } catch (const std::exception& e) {
                                                std::cout << "Tax failed: " << e.what() << std::endl;
                                            }
                                        }
                                        else if (action == "bribe") {
                                            try{
                                                currentPlayer->bribe();//perform bribe action
                                            }catch (const std::exception& e) {
                                                std::cout << "Bribe failed: " << e.what() << std::endl;
                                            }
                                        }
                                        // Actions that require target selection
                                        else if (action == "arrest") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_ARREST;// set the current action state to selecting target for arrest
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for arrest by " << currentPlayer->getName() << std::endl;// print the message to the console
                                        }

                                        else if (action == "sanction") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_SANCTION;
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for sanction by " << currentPlayer->getName() << std::endl;
                                        }
                                        else if (action == "coup") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_COUP;
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for coup by " << currentPlayer->getName() << std::endl;
                                        }

                                        //special operation by roles
                                        else if (action == "INVEST") {
                                            if(currentPlayer->getRole() == "Baron") {// check if the current player is a Baron
                                                Baron* baron = dynamic_cast<Baron*>(currentPlayer);
                                                if(baron){
                                                    try{
                                                        baron->invest();// perform invest action by Baron
                                                    } catch (const std::exception& e) {
                                                        std::cout << "Invest failed: " << e.what() << std::endl;
                                                    }
                                                }else{
                                                    std::cout << "Error: Failed to cast to Baron" << std::endl;
                                                }
                                            } else {
                                                std::cout << "Only Baron can perform invest action!" << std::endl;
                                            }
                                        }
                                        else if (action == "cancel coup") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_CANCEL_COUP;// set the current action state to selecting target for cancel coup
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for cancel coup by " << currentPlayer->getName() << std::endl;
                                        }
                                        else if (action == "UNDO") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_UNDO;// set the current action state to selecting target for undo
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for undo by " << currentPlayer->getName() << std::endl;
                                        }
                                        else if (action == "block arrest") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_BLOCK_ARREST;// set the current action state to selecting target for block arrest
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for block arrest by " << currentPlayer->getName() << std::endl;
                                        }
                                        else if (action == "view coins") {
                                            currentActionState = ActionState::SELECTING_TARGET_FOR_VIEW_COINS;// set the current action state to selecting target for view coins
                                            actionInitiator = currentPlayer;
                                            std::cout << "Select target for view coins by " << currentPlayer->getName() << std::endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                
                // Check if the mouse is on the start button and change the stat accordingly 
                if (state == START_SCREEN && startButton.getGlobalBounds().contains(mouse)) {
                    state = ADD_PLAYERS;
                }
                // Check if the mouse is on the add player button
                if (state == ADD_PLAYERS && addPlayerButton.getGlobalBounds().contains(mouse)) {
                    if (!currentInput.empty() && playerNames.size() < 6) {
                        std::string name = currentInput;

                        // avoid adding same name twice
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
                        // Clear previous error message
                        errorMessage = "";
                        errorText.setString("");

                        playerNames.push_back(name);

                        // Display the name
                        sf::Text newText(name, font, 20);
                        newText.setFillColor(sf::Color::White);
                        newText.setPosition(50, 50 + playerTexts.size() * 30);
                        playerTexts.push_back(newText);

                        // Create a new player with a random role
                        std::vector<std::string> roles = {"Governor", "Spy", "Baron", "General", "Judge", "Merchant"};
                        //random and possibility things
                        static std::random_device rd;
                        static std::mt19937 gen(rd());
                        std::uniform_int_distribution<> dis(0, roles.size() - 1);
                        std::string role = roles[dis(gen)];
                        // Create the player based on the role
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

                        // Create a box with name and role
                        sf::RectangleShape box(sf::Vector2f(300, 30));
                        box.setFillColor(sf::Color::White);
                        box.setPosition(50, 50 + playerBoxes.size() * 50);
                        // design elements
                        sf::Text playerInfo(name + " - " + role, font, 18);
                        playerInfo.setFillColor(sf::Color::Black);
                        playerInfo.setPosition(box.getPosition().x + 10, box.getPosition().y + 8);

                        playerBoxes.push_back({box, playerInfo});
                    }
                }
                // Check if the mouse is on the play button
                if (state == ADD_PLAYERS && playButton.getGlobalBounds().contains(mouse)) {
                    // Check if we have enough players to start the game and if there is too many players
                    if (playerNames.size() < 2) {
                        errorMessage = "Need at least 2 players!";
                        errorText.setString(errorMessage);
                    } else if (playerNames.size() > 6) {
                        errorMessage = "Maximum 6 players allowed!";
                        errorText.setString(errorMessage);
                    } else {
                        // start the game
                        errorMessage = "";
                        errorText.setString("");
    
                        state = GAME_SCREEN;//change the state to GAME_SCREEN
                        std::cout << "Total players in game after adding: " << game.getPlayers().size() << std::endl;
                    }
                }
            }
        }
        //design the action buttons for each player
        if (state == START_SCREEN) {
            window.clear(brown);
            window.draw(title);
            window.draw(startButton);
            window.draw(startText);
        }
        // ADD PLAYERS SCREEN
        else if (state == ADD_PLAYERS) {
            window.clear(brown);
            //designing the title of the screen
            sf::Text promptText("Enter name of player to add...", font, 20);
            promptText.setFillColor(sf::Color::Yellow);

            sf::FloatRect textBounds = promptText.getLocalBounds();
            promptText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            promptText.setPosition(window.getSize().x / 2, 30); 

            window.draw(promptText);
            //
            // Display the input text
            for (const auto& box : playerBoxes) {
                window.draw(box.first);
                window.draw(box.second);
            }
            window.draw(addPlayerButton);
            window.draw(addText);
            window.draw(inputText);

            window.draw(playButton);
            window.draw(playText);
                // Display error messages
            if (!errorMessage.empty()) {
                window.draw(errorText);
            }
        }
        // GAME SCREEN
        else if (state == GAME_SCREEN) {
            window.clear(brown);
            
            // Draw the action buttons for each player
            sf::Text gameTitle("GAME IN PROGRESS", font, 20);
            gameTitle.setFillColor(sf::Color::White);
            sf::FloatRect textRect = gameTitle.getLocalBounds();
            gameTitle.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            gameTitle.setPosition(window.getSize().x / 2.0f, 20);
            window.draw(gameTitle);

            const auto& gamePlayers = game.getPlayers();
            std::vector<std::string> actions = {"gather", "tax", "bribe", "arrest", "sanction", "coup"};
            // Define positions and sizes for player boxes and action buttons
            float startY = 70.f; // Starting position of the player list along the y-axis
            float playerBoxHeight = 25.f;
            float actionButtonWidth = 55.f;
            float actionButtonHeight = 18.f;
            float spacingY = 45.f;
            float spacingX = 3.f;

            actionButtons.clear();
            // Draw player boxes and action buttons
            for (size_t i = 0; i < gamePlayers.size(); ++i) {
                float baseYPos =startY + i* spacingY ;

                // Player box
                sf::RectangleShape playerBox(sf::Vector2f(90, playerBoxHeight));
                playerBox.setFillColor(sf::Color::White);
                playerBox.setPosition(10, baseYPos);

                // Player name and role
                std::string playerName = gamePlayers[i]->getName();
                std::string role = gamePlayers[i]->getRole();
                sf::Text playerNameText(playerName + " (" + role + ")", font, 12);
                playerNameText.setFillColor(sf::Color::Black);
                playerNameText.setPosition(15, baseYPos + 8);

                window.draw(playerBox);
                window.draw(playerNameText);

                // Build action list for current player
                std::vector<std::string> basicActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup"};
                std::vector<std::string> specialActions;

                // Add special actions based on role
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

                float actionStartX = 110.f;// Starting position of action buttons along the x-axis
                // Draw basic actions - first row
                for (size_t j = 0; j < basicActions.size(); ++j) {
                float x = actionStartX + j * (actionButtonWidth + spacingX);

                sf::RectangleShape button(sf::Vector2f(actionButtonWidth, actionButtonHeight));
                button.setFillColor(sf::Color::Red);
                button.setPosition(x, baseYPos + 2.f);

                sf::Text text(basicActions[j], font, 10); // smaller font size for basic actions
                text.setFillColor(sf::Color::Black);
                text.setPosition(
                    x + actionButtonWidth / 2 - text.getGlobalBounds().width / 2,
                    baseYPos + 2.f + actionButtonHeight / 2 - text.getGlobalBounds().height / 2
                );
                window.draw(button);
                window.draw(text);
                actionButtons.push_back({button, i, basicActions[j]});//to store the button, player index and action name
                }

             // Draw special actions - second row (if any)
            if (!specialActions.empty()) {
                for (size_t j = 0; j < specialActions.size(); ++j) {
                    float x = actionStartX + (basicActions.size() + j) * (actionButtonWidth + spacingX);
                    
                    sf::RectangleShape button(sf::Vector2f(actionButtonWidth, actionButtonHeight));
                    button.setFillColor(sf::Color::Blue); // Different color for special actions
                    button.setPosition(x, baseYPos);//specialYpos

                    sf::Text text(specialActions[j], font, 9); // choose a font size that fits well
                    text.setFillColor(sf::Color::White);
                    text.setPosition(
                        x + actionButtonWidth / 2 - text.getGlobalBounds().width / 2,
                        baseYPos + actionButtonHeight / 2 - text.getGlobalBounds().height / 2
                    );
                    // Draw the button and text
                    window.draw(button);
                    window.draw(text);
                    actionButtons.push_back({button, i, specialActions[j]});
                }
            }
            /*
            THIS LINES is to check if there is a winner in the game and display the winner.
            as long as there is more than one active player, the game continues.
            When there is only one active player, that player is the winner and the game ends.
            */
            int activePlayersCount = 0;
            Player* lastActivePlayer = nullptr;
            for (const auto& player : gamePlayers) {
                if (player->isActive()) {  // או איך שקוראים לפונקציה שבודקת אם השחקן פעיל
                    activePlayersCount++;
                    lastActivePlayer = player;
                }
            }
            if (activePlayersCount == 1 && lastActivePlayer != nullptr) {//print the winner name and congratulations message with some design elements
                std::string winner = lastActivePlayer->getName();
                // Display the winner
                sf::Text winnerText("Winner: " + winner, font, 20);
                winnerText.setFillColor(sf::Color::Yellow);

                sf::FloatRect winnerBounds = winnerText.getLocalBounds();
                float winnerXPos = window.getSize().x - winnerBounds.width - 250; // 250 פיקסלים מהקצה הימני
                float winnerYPos = 50; // 50 פיקסלים מהחלק העליון
                winnerText.setPosition(winnerXPos, winnerYPos);
                window.draw(winnerText);

                sf::Text congratsText("Congratulations!", font, 16);
                congratsText.setFillColor(sf::Color::White);
                sf::FloatRect congratsBounds = congratsText.getLocalBounds();
                congratsText.setPosition(
                    (window.getSize().x - congratsBounds.width) / 2,
                    winnerYPos + 47
                );
                window.draw(congratsText);
            }
            else if(activePlayersCount>1){//if so- the game continues and we display the current turn, coins and last arrest with some design elements
                std::string currentPlayerName = game.turn();
             
                sf::Text turnText("Turn: " + currentPlayerName, font, 20);
                turnText.setFillColor(sf::Color::Yellow);
                
                sf::FloatRect turnBounds = turnText.getLocalBounds();
                float xPos = window.getSize().x - turnBounds.width - 10; // 10 pixels from the right edge
                float yPos = 10; // 10 pixsels from the top
                turnText.setPosition(xPos, yPos);
                window.draw(turnText);

                // for displaying the current player's coins
                int coins = 0;
                for (const auto& player : gamePlayers) {
                    if (player->getName() == currentPlayerName) {
                        coins = player->coins();
                        break;
                    }
                }
                // Display the coins of the current player
                sf::Text coinsText("Coins: " + std::to_string(coins), font, 20);
                coinsText.setFillColor(sf::Color::Yellow);

                sf::FloatRect coinsBounds = coinsText.getLocalBounds();
                float xXPos = window.getSize().x - coinsBounds.width - 10; // 10 pixels from the right edge
                float yYPos = 30; // 30 pixels from the top
                coinsText.setPosition(xXPos, yYPos);
                window.draw(coinsText);
                // Display last arrest information
                std::string lastArrest = game.getLastArrest();

                sf::Text lastArrestText("Last Arrest: " + lastArrest, font, 20);
                lastArrestText.setFillColor(sf::Color::Yellow);

                sf::FloatRect lastArrestBounds = lastArrestText.getLocalBounds();
                float lastArrestXPos = window.getSize().x - lastArrestBounds.width - 10; // 10 pixels from the right edge
                float lastArrestYPos = 50; // 50 pixels from the top
                lastArrestText.setPosition(lastArrestXPos, lastArrestYPos);
                window.draw(lastArrestText);
            }

            // Draw information about the game - turn, coins, last arrest and winner
            // if (!gamePlayers.empty()) {
            //     std::string currentPlayerName = game.turn();
             
            //     sf::Text turnText("Turn: " + currentPlayerName, font, 20);
            //     turnText.setFillColor(sf::Color::Yellow);
                
            //     sf::FloatRect turnBounds = turnText.getLocalBounds();
            //     float xPos = window.getSize().x - turnBounds.width - 10; // 10 פיקסלים מהקצה הימני
            //     float yPos = 10; // 10 פיקסלים מהחלק העליון
            //     turnText.setPosition(xPos, yPos);
            //     window.draw(turnText);

            //     //
            //     int coins = 0;
            //     for (const auto& player : gamePlayers) {
            //         if (player->getName() == currentPlayerName) {
            //             coins = player->coins();
            //             break;
            //         }
            //     }

            //     sf::Text coinsText("Coins: " + std::to_string(coins), font, 20);
            //     coinsText.setFillColor(sf::Color::Yellow);

            //     sf::FloatRect coinsBounds = coinsText.getLocalBounds();
            //     float xXPos = window.getSize().x - coinsBounds.width - 10; // 10 פיקסלים מהקצה הימני
            //     float yYPos = 30; // 30 פיקסלים מהחלק העליון
            //     coinsText.setPosition(xXPos, yYPos);
            //     window.draw(coinsText);
            //     //
            //     std::string lastArrest = game.getLastArrest();

            //     sf::Text lastArrestText("Last Arrest: " + lastArrest, font, 20);
            //     lastArrestText.setFillColor(sf::Color::Yellow);

            //     sf::FloatRect lastArrestBounds = lastArrestText.getLocalBounds();
            //     float lastArrestXPos = window.getSize().x - lastArrestBounds.width - 10; // 10 פיקסלים מהקצה הימני
            //     float lastArrestYPos = 50; // 50 פיקסלים מהחלק העליון
            //     lastArrestText.setPosition(lastArrestXPos, lastArrestYPos);
            //     window.draw(lastArrestText);
            //     //
                
            // }
        }
    }

    window.display();
    }
    
    return 0;
}
