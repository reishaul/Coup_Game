// #include "ActionBar.h"
// #include "ActionButton.h"
// #include <vector>
// #include <SFML/Graphics.hpp>
// #include <string>
// #include <iostream>
// ActionBar::ActionBar(float x, float y, const std::string& role)
//     : startX(x), startY(y) {
//     font.loadFromFile("arial.ttf");  // ודא שהפונט קיים

//     std::vector<std::string> commonActions = {
//         "gather", "tax", "bribe", "arrest", "sanction", "coup"
//     };

//     std::vector<std::string> uniqueActions;

//     if (role == "Governor") uniqueActions.push_back("undo");
//     else if (role == "Spy") uniqueActions.push_back("steal");
//     else if (role == "Baron") uniqueActions.push_back("duel");
//     else if (role == "General") uniqueActions.push_back("draft");
//     else if (role == "Judge") uniqueActions.push_back("pardon");
//     else if (role == "Merchant") uniqueActions.push_back("trade");

//     int i = 0;
//     for (const auto& action : commonActions) {
//         ActionButton btn(action, font);
//         btn.setPosition(x + i * 100, y);
//         buttons.push_back(btn);
//         ++i;
//     }

//     for (const auto& action : uniqueActions) {
//         ActionButton btn(action, font);
//         btn.setPosition(x + i * 100, y);
//         buttons.push_back(btn);
//         ++i;
//     }
// }
