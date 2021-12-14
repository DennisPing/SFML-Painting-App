/** 
 *  @file   main.cpp 
 *  @brief  Entry point into the program.
 *  @author Mike and Dennis Ping
 *  @date   2021-11-13
 ***********************************************/

// Compile: Use a CMakeLists.txt or you can try compiling with:
// g++ -std=c++17 ./src/*.cpp -I./include/ -o App -lsfml-graphics -lsfml-window -lsfml-system

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
#include <catch_amalgamated.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <string>
// Project header files
#include "App.hpp"
// #include "Command.hpp"
// #include "Draw.hpp"
#include "MathUtility.hpp"
#include "RoundedLine.hpp"


/*! \brief 	Initializes the App and sets up the main
*		    rendering window (i.e. our canvas.)
*		
*/
void initialization(void){
    std::cout << "\nStarting the App" << std::endl;
    std::string instructions = "How to use:\n"
                                "\tPress numbers [1, 2, 3, 4, 5, 6, 7, 8] to change paintbrush color\n"
                                "\tPress Z to undo\n"
                                "\tPress Y to redo\n"
                                "\tPress , to decrease paintbrush size\n"
                                "\tPress . to increase paintbrush size\n";
    std::cout << instructions << std::endl;
}

/*! \brief 	The update window function.
*		
*/
void update(App& myApp){
    // Update our canvas
    sf::Event event;
    while(myApp.GetWindow().pollEvent(event)){
        sf::Vector2i mousePos = sf::Mouse::getPosition(myApp.GetWindow());
        int mouseX = mousePos.x;
        int mouseY = mousePos.y;
        myApp.SetCursorPosition(mouseX, mouseY);

        if(event.type == sf::Event::KeyReleased) {
            // Undo command
            if(event.key.code == sf::Keyboard::Z) {
                myApp.UndoCommand();
            }
            // Redo command
            if(event.key.code == sf::Keyboard::Y) {
                myApp.RedoCommand();
            }
            // Check for change paintbrush color keypress
            if(myApp.color_codes.find(event.key.code) != myApp.color_codes.end()) {
                myApp.SetPaintbrushColor(event.key.code);
                myApp.GenerateCursor(myApp.GetPaintbrushRadius(), myApp.GetPaintbrushColor());
            }
            // Check for change paintbrush size keypress
            if(event.key.code == sf::Keyboard::Comma) {
                if (myApp.GetPaintbrushRadius() > 3) {
                    myApp.SetPaintbrushRadius(myApp.GetPaintbrushRadius() - 2);
                    int& radius = myApp.GetPaintbrushRadius();
                    myApp.GenerateCircleTemplate(radius);
                    myApp.GenerateCursor(radius, myApp.GetPaintbrushColor());
                    myApp.SetCursorPosition(mouseX, mouseY);
                    std::cout << "Paintbrush radius is now: " << radius << std::endl;
                }
            }
            if(event.key.code == sf::Keyboard::Period) {
                if (myApp.GetPaintbrushRadius() < 15) {
                    myApp.SetPaintbrushRadius(myApp.GetPaintbrushRadius() + 2);
                    int& radius = myApp.GetPaintbrushRadius();
                    myApp.GenerateCircleTemplate(radius);
                    myApp.GenerateCursor(radius, myApp.GetPaintbrushColor());
                    myApp.SetCursorPosition(mouseX, mouseY);
                    std::cout << "Paintbrush radius is now: " << radius << std::endl;
                }
            }

        }
        // Close the window when you press the 'close' button.
        if(event.type == sf::Event::Closed) {
            myApp.GetWindow().close();
            exit(EXIT_SUCCESS);
        }
        // When the mouse is released, push the count to the m_undo_count stack. Reset the count.
        if(event.type == sf::Event::MouseButtonReleased) {
            if (myApp.cmdCount == 0) {
                break;
            }
            myApp.m_undo_count.push(myApp.cmdCount);
            std::cout << "Pushed " << myApp.cmdCount << " to the undo stack" << std::endl;
            myApp.cmdCount = 0;
            myApp.m_prev_point = nullptr;
        }
        // Draw with the paintbrush
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            
            if ((mouseX < 0) || (mouseY < 0) || (mouseX >= (int)myApp.GetWindow().getSize().x || (mouseY > (int)myApp.GetWindow().getSize().y))) {
                return;
            }
            // Use a dummy port for now because we're not on a network
            short port = 1234;
            // If you have already started drawing...
            if (myApp.m_prev_point != nullptr) {
                std::unique_ptr<RoundedLine> line (new RoundedLine(*myApp.m_prev_point, sf::Vector2f(mouseX, mouseY), myApp.GetPaintbrushRadius()*2, myApp.GetPaintbrushColor(), port));
                myApp.m_prev_point = new sf::Vector2f(mouseX, mouseY);
                myApp.AddCommand(std::move(line));
                
            } else {
                //Set the m_prev_pointer to the current mouse position.
                std::unique_ptr<RoundedLine> line (new RoundedLine(sf::Vector2f(mouseX, mouseY), sf::Vector2f(mouseX, mouseY), myApp.GetPaintbrushRadius()*2, myApp.GetPaintbrushColor(), port));
                myApp.m_prev_point = new sf::Vector2f(mouseX, mouseY);
                myApp.AddCommand(std::move(line));
            }
            myApp.cmdCount += myApp.ExecuteCommand();

            // Create a vector of all the points to be drawn.
            //std::vector<std::pair<int, int>> allCoords = myApp.UseCircleTemplate(mouseX, mouseY);
            //myApp.prevCmds.resize(allCoords.size());

            // For each coordinate in allCoords, make a new Draw drawCmd pointer.
            // for (int i=0; i < (int)allCoords.size(); i++) {
            //     std::unique_ptr<Command> drawCmd1 (new Draw(allCoords[i].first, allCoords[i].second, myApp.GetImage(), myApp.GetPaintbrushColor()));
            //     std::unique_ptr<Command> drawCmd2 (new Draw(allCoords[i].first, allCoords[i].second, myApp.GetImage(), myApp.GetPaintbrushColor()));
            //     // If we have already started drawing...
            //     if (myApp.prevCmds[i] != nullptr) {
            //         // Use the EFLA algorithm to get a vector of intermediate pixels between prevCmd and drawCmd.
            //         std::vector<std::pair<int, int>> pixels = MathUtility::ExtremelyFastLineAlgo(myApp.prevCmds[i]->getCoords().first, myApp.prevCmds[i]->getCoords().second, drawCmd1->getCoords().first, drawCmd1->getCoords().second);
            //         // Iterate through the pixels and construct a new draw command for each pixel.
            //         for (auto& pixel : pixels) {
            //             std::unique_ptr<Command> intermDraw1(new Draw(pixel.first, pixel.second, myApp.GetImage(), myApp.GetPaintbrushColor()));
            //             std::unique_ptr<Command> intermDraw2(new Draw(pixel.first, pixel.second, myApp.GetImage(), myApp.GetPaintbrushColor()));
            //             // Remember to update the prevCmd with the new intermediateCmd.
            //             myApp.prevCmds[i] = std::move(intermDraw1);
            //             myApp.AddCommand(std::move(intermDraw2));
            //         }
            //     }
            //     // Else if this is the first command, just re-assign the pointer to drawCmd and do nothing.
            //     else {
            //         myApp.prevCmds[i] = std::move(drawCmd1);
            //         myApp.AddCommand(std::move(drawCmd2));
            //     }
            //     myApp.cmdCount += myApp.ExecuteCommand();
            // }
        }
    }

    // Check for ESC key press
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
        myApp.GetWindow().close();
        exit(EXIT_SUCCESS);
    }
}


/*! \brief 	The draw call 
*		
*/
void draw(App& myApp){
    // Make the clock variable static so we don't lose it.
    static sf::Clock clock;

    // Load the modified pixels into our texture every 1/60 of a second.
    // So every 1 frame if your monitor's refresh rate is 60 hz.
    if(clock.getElapsedTime().asSeconds() > 0.0111f) {
        // myApp.GetTexture().loadFromImage(myApp.GetImage());
        // myApp.GetRenderSprite().setTexture(myApp.GetRenderTexture().getTexture());
        //myApp.cmdCount += myApp.ExecuteCommand();
        clock.restart();
    }
}

 
/*! \brief 	The entry point into our program.
*		
*/
int main(){
    // Call any setup function
    // Passing a function pointer into the 'init' function.
    // of our application.
    App myApp = App();
    myApp.Init(&initialization);
    // Setup your keyboard
    myApp.UpdateCallback(&update);
    // Setup the Draw Function
    myApp.DrawCallback(&draw);
    // Call the main loop function
    myApp.Loop(myApp);
    // Destroy our app
    myApp.Destroy();

    return 0;
}