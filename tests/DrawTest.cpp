#include "catch_amalgamated.hpp"
#include "Draw.hpp"

#include <string>
#include <SFML/Graphics.hpp>

TEST_CASE("Test GetDescription", "[Draw]") {
    sf::Image img = sf::Image();
    img.create(10, 10, sf::Color::White);
    Draw drawCmd = Draw(4, 5, img, sf::Color::Red);
    REQUIRE(drawCmd.getDescription() == "Draw (4, 5, Red)");
}