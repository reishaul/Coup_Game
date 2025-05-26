// ActionButton.hpp
#include <SFML/Graphics.hpp>
class ActionButton {
public:
    sf::RectangleShape shape;
    sf::Text label;
    std::string action_name;

    ActionButton(const std::string& text, const sf::Font& font);
    void setPosition(float x, float y);
    bool isClicked(sf::Vector2f mousePos) const;
    void draw(sf::RenderWindow& window) const;
};
