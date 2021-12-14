#define CATCH_CONFIG_MAIN

#include "catch_amalgamated.hpp"
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"
#include "MathUtility.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

// Our custom print coordinates function for testing and debugging purposes
void _printCoordinates(std::vector<std::pair<int, int>> coordinates) {
    for (auto coordinate : coordinates) {
        std::cout << "(" << coordinate.first << ", " << coordinate.second << ")" << std::endl;
    }
}

// Our custom initializer function for testing purposes
void _initialization(void) {}

// Our custom update function for testing purposes with mouse position at (100, 200)
void _update(App& app) {
    int mouseX = 100;
    int mouseY = 200;
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(mouseX, mouseY);
    app.prevCmds.resize(allCoords.size());
    // For each coordinate in allCoords, make a new Draw drawCmd pointer.
    for (int i=0; i < (int)allCoords.size(); i++) {
        std::unique_ptr<Command> drawCmd1 (new Draw(allCoords[i].first, allCoords[i].second, app.GetImage(), app.GetPaintbrushColor()));
        std::unique_ptr<Command> drawCmd2 (new Draw(allCoords[i].first, allCoords[i].second, app.GetImage(), app.GetPaintbrushColor()));
        // If we have already started drawing...
        if (app.prevCmds[i] != nullptr) {
            // Use the EFLA algorithm to get a vector of intermediate pixels between prevCmd and drawCmd.
            std::vector<std::pair<int, int>> pixels = MathUtility::ExtremelyFastLineAlgo(app.prevCmds[i]->getCoords().first, app.prevCmds[i]->getCoords().second, drawCmd1->getCoords().first, drawCmd1->getCoords().second);
            // Iterate through the pixels and construct a new draw command for each pixel.
            for (auto& pixel : pixels) {
                std::unique_ptr<Command> intermDraw1(new Draw(pixel.first, pixel.second, app.GetImage(), app.GetPaintbrushColor()));
                std::unique_ptr<Command> intermDraw2(new Draw(pixel.first, pixel.second, app.GetImage(), app.GetPaintbrushColor()));
                // Remember to update the prevCmd with the new intermediateCmd.
                app.prevCmds[i] = std::move(intermDraw1);
                app.AddCommand(std::move(intermDraw2));
            }
        }
        // Else if this is the first command, just re-assign the pointer to drawCmd and do nothing.
        else {
            app.prevCmds[i] = std::move(drawCmd1);
            app.AddCommand(std::move(drawCmd2));
        }
        app.cmdCount += app.ExecuteCommand();
    }
}

// Our custom update function for testing purposes with mouse position at (120, 220)
void _update2(App& app) {
    int mouseX = 120;
    int mouseY = 220;
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(mouseX, mouseY);
    app.prevCmds.resize(allCoords.size());
    // For each coordinate in allCoords, make a new Draw drawCmd pointer.
    for (int i=0; i < (int)allCoords.size(); i++) {
        std::unique_ptr<Command> drawCmd1 (new Draw(allCoords[i].first, allCoords[i].second, app.GetImage(), app.GetPaintbrushColor()));
        std::unique_ptr<Command> drawCmd2 (new Draw(allCoords[i].first, allCoords[i].second, app.GetImage(), app.GetPaintbrushColor()));
        // If we have already started drawing...
        if (app.prevCmds[i] != nullptr) {
            // Use the EFLA algorithm to get a vector of intermediate pixels between prevCmd and drawCmd.
            std::vector<std::pair<int, int>> pixels = MathUtility::ExtremelyFastLineAlgo(app.prevCmds[i]->getCoords().first, app.prevCmds[i]->getCoords().second, drawCmd1->getCoords().first, drawCmd1->getCoords().second);
            // Iterate through the pixels and construct a new draw command for each pixel.
            for (auto& pixel : pixels) {
                std::unique_ptr<Command> intermDraw1(new Draw(pixel.first, pixel.second, app.GetImage(), app.GetPaintbrushColor()));
                std::unique_ptr<Command> intermDraw2(new Draw(pixel.first, pixel.second, app.GetImage(), app.GetPaintbrushColor()));
                // Remember to update the prevCmd with the new intermediateCmd.
                app.prevCmds[i] = std::move(intermDraw1);
                app.AddCommand(std::move(intermDraw2));
            }
        }
        // Else if this is the first command, just re-assign the pointer to drawCmd and do nothing.
        else {
            app.prevCmds[i] = std::move(drawCmd1);
            app.AddCommand(std::move(drawCmd2));
        }
        app.cmdCount += app.ExecuteCommand();
    }
}

// Our custom draw function for testing purposes
void _draw(App& myApp){
	static sf::Clock clock;
	if(clock.getElapsedTime().asSeconds() > 0.0166f) {
		myApp.GetTexture().loadFromImage(myApp.GetImage());
		clock.restart();
	}
}

// A replacement function for the App::Loop() function that only runs 1 time at point (100,200).
void manualUpdateAndDraw1(App& app) {
    app.UpdateCallback(&_update);
    app.DrawCallback(&_draw);
    app.GetWindow().clear();
    app.m_updateFunc(app);
    app.m_drawFunc(app);
    app.GetWindow().draw(app.GetSprite());
}

// A replacement function for the App::Loop() function that only runs 1 time at point (120,220).
void manualUpdateAndDraw2(App& app) {
    app.UpdateCallback(&_update2);
    app.DrawCallback(&_draw);
    app.GetWindow().clear();
    app.m_updateFunc(app);
    app.m_drawFunc(app);
    app.GetWindow().draw(app.GetSprite());
}

/*! \brief Test app init and destroy.
*/
TEST_CASE("App Inititialization and Destroy", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    app.Destroy();
}

/*! \brief Test drawing 1 pixel from white to black
*/
TEST_CASE("Draw 1 point from White to Black", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    REQUIRE(app.GetImage().getPixel(100, 200) == sf::Color::White);
    manualUpdateAndDraw1(app);
    REQUIRE(app.GetImage().getPixel(100, 200) == sf::Color::Black);
    app.Destroy();
}

/*! \brief Test drawing an entire circle (97 pixels) from white to black using default radius of 5.
*/
TEST_CASE("Draw entire circle (97) from White to Black", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);

    // Manually extract the coordinates to be drawn and check the vector size.
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(100, 200);
    REQUIRE(allCoords.size() == 97);

    // Test the color before drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::White);
    }
    manualUpdateAndDraw1(app);
    // Test the color after drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::Black);
    }
    // Check that the outer circle boundary (N,S,E,W) is still white
    REQUIRE(app.GetImage().getPixel(106, 200) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(94, 200) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(100, 206) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(100, 194) == sf::Color::White);
    app.Destroy();
}

/*! \brief Test changing paintbrush radius.
*/
TEST_CASE("Change paintbrush radius", "[App] [Features]") {
    App app = App();
    app.Init(&_initialization);
    SECTION("Change paintbrush radius from 5 to 7") {
        REQUIRE(app.GetPaintbrushRadius() == 5);
        app.SetPaintbrushRadius(7);
        REQUIRE(app.GetPaintbrushRadius() == 7);
    }
    SECTION("Change paintbrush radius from 5 to 3") {
        REQUIRE(app.GetPaintbrushRadius() == 5);
        app.SetPaintbrushRadius(3);
        REQUIRE(app.GetPaintbrushRadius() == 3);
    }
    app.Destroy();
}

/*! \brief Test drawing an entire circle (97 pixels) using paintbrush radius of 7.
*/
TEST_CASE("Draw after changing paintbrush radius from 5 to 7", "[App] [Features]") {
    App app = App();
    app.Init(&_initialization);
    REQUIRE(app.GetPaintbrushRadius() == 5);
    app.SetPaintbrushRadius(7);
    app.GenerateCircleTemplate(7);

    // Change the paintbrush radius to 7
    REQUIRE(app.GetPaintbrushRadius() == 7);

    // Manually extract the coordinates to be drawn and check the vector size.
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(100, 200);
    REQUIRE(allCoords.size() == 177);

    // Test the color before drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::White);
    }
    manualUpdateAndDraw1(app);
    // Test the color after drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::Black);
    }
    // Check that the outer circle boundary (N,S,E,W) is still white
    REQUIRE(app.GetImage().getPixel(108, 200) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(92, 200) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(100, 208) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(100, 192) == sf::Color::White);
    app.Destroy();
}

/*! \brief Test drawing an entire circle (177 pixels) using paintbrush radius of 7 and color red.
*/
TEST_CASE("Draw using paintbrush radius = 7 and color = Red", "[App] [Features]") {
    App app = App();
    app.Init(&_initialization);
    app.SetPaintbrushRadius(7);
    app.GenerateCircleTemplate(7);
    REQUIRE(app.GetPaintbrushRadius() == 7);
    // Change the paintbrush color from Black to Red
    REQUIRE(app.GetPaintbrushColor() == sf::Color::Black);
    app.SetPaintbrushColor(sf::Keyboard::Key::Num3);
    REQUIRE(app.GetPaintbrushColor() == sf::Color::Red);

    // Manually extract the coordinates to be drawn and check the vector size.
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(100, 200);

    REQUIRE(allCoords.size() == 177);
    // Test the color before drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::White);
    }
    manualUpdateAndDraw1(app);
    // Test the color after drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::Red);
    }
    // Check that the outer circle boundary (N,S,E,W) is still white
    REQUIRE(app.GetImage().getPixel(108, 200) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(92, 200) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(100, 208) == sf::Color::White);
    REQUIRE(app.GetImage().getPixel(100, 192) == sf::Color::White);
    app.Destroy();
}

/*! \brief Test drawing a thick line with paintbrush radius of 5 and color black.
*/
TEST_CASE("Draw a line from (100,200) to (120,220) using radius = 5 and color = Black", "[App] [Features]") {
    App app = App();
    app.Init(&_initialization);
    // Manually extract the coordinates to be drawn and check the vector size.
    std::vector<std::pair<int, int>> allCoords1 = app.UseCircleTemplate(100, 200);
    // Test the color before drawing
    for (auto& coord : allCoords1) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::White);
    }
    manualUpdateAndDraw1(app);
    // Test the color after drawing
    for (auto& coord : allCoords1) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::Black);
    }

    // **************************** MOVE THE MOUSE **************************************

    // Manually extract the coordinates to be drawn and check the vector size.
    std::vector<std::pair<int, int>> allCoords2 = app.UseCircleTemplate(120, 220);
    // Test the color before drawing
    for (auto& coord : allCoords2) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::White);
    }
    manualUpdateAndDraw2(app);
    // Test the color after drawing
    for (auto& coord : allCoords2) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::Black);
    }
    // Test all interpolated points between the original mouse position and the new mouse position.
    for (std::pair<int,int> coord1 : allCoords1) {
        for (std::pair<int,int> coord2 : allCoords2) {
            std::vector<std::pair<int, int>> intermCoords = MathUtility::ExtremelyFastLineAlgo(coord1.first, coord1.second, coord2.first, coord2.second);
            for (auto& interm : intermCoords) {
                REQUIRE(app.GetImage().getPixel(interm.first, interm.second) == sf::Color::Black);
            }
        }
    }
    app.Destroy();
}

/*! \brief Test undo when there is nothing to undo.
*/
TEST_CASE("Test undo when there is nothing to undo", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    int numUndo = app.UndoCommand();
    REQUIRE(numUndo == 0);
    app.Destroy();
}

/*! \brief Test redo when there is nothing to redo.
*/
TEST_CASE("Test redo when there is nothing to redo", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    int numRedo = app.RedoCommand();
    REQUIRE(numRedo == 0);
    app.Destroy();
}

/*! \brief Test undo after drawing.
*/
TEST_CASE("Test undo after drawing", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    manualUpdateAndDraw1(app);
    // Manually extract the coordinates that were drawn and check the vector size.
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(100, 200);
    REQUIRE(allCoords.size() == 97);
    // Manually push in the m_undo_count because we don't have a mouse to release mouse button.
    app.m_undo_count.push((int)allCoords.size());
    // Test the color after drawing
    for (auto& coord : allCoords) {
        REQUIRE(app.GetImage().getPixel(coord.first, coord.second) == sf::Color::Black);
    }
    int numUndo = app.UndoCommand();
    REQUIRE(numUndo == 97);
    app.Destroy();
}

/*! \brief Test redo after drawing.
*/
TEST_CASE("Test redo after undoing", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    manualUpdateAndDraw1(app);
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(100, 200);
    REQUIRE(allCoords.size() == 97);
    // Manually push in the m_undo_count because we don't have a mouse to release mouse button.
    app.m_undo_count.push((int)allCoords.size());
    app.UndoCommand();
    // Call undo again and verify that 0 is returned.
    int numUndo = app.UndoCommand();
    REQUIRE(numUndo == 0);
    int numRedo = app.RedoCommand();
    REQUIRE(numRedo == 97);
    app.Destroy();
}

/*! \brief Test that m_redo is automatically cleared if user draws after undoing.
*/
TEST_CASE("Test that m_redo is automatically cleared if user draws after undoing.", "[App] [Core]") {
    App app = App();
    app.Init(&_initialization);
    manualUpdateAndDraw1(app);
    std::vector<std::pair<int, int>> allCoords = app.UseCircleTemplate(100, 200);
    REQUIRE(allCoords.size() == 97);
    // Manually push in the m_undo_count because we don't have a mouse to release mouse button.
    app.m_undo_count.push((int)allCoords.size());
    app.UndoCommand();
    // Draw again
    manualUpdateAndDraw2(app);
    // Verify that m_redo is empty
    int numRedo = app.RedoCommand();
    REQUIRE(numRedo == 0);
    app.Destroy();
}