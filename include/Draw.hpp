/** 
 *  @file   Draw.hpp 
 *  @brief  Drawing actions interface. 
 *  @author Mike and Dennis Ping 
 *  @date   2021-11-13
 ***********************************************/
#ifndef DRAW_H
#define DRAW_H

// Include our Third-Party SFML header
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
// Include standard library C++ libraries.
#include <string>
// Project header files
#include "Command.hpp"

// The Draw class inherits from the Command class so that the functions
// 'execute', 'undo', and 'redo' are required to be implemented. 
class Draw : public Command {
    private:
        int xCoord;
        int yCoord;
        sf::Color prevColor;
        sf::Color currColor;

    public:
        // The constructor for a Draw requires an (x,y), an image ref, 
        // and the current paintbrush color.
        Draw(int x, int y, sf::Image& image, sf::Color paintbrushColor);
        ~Draw();
        bool execute() override;
        bool undo() override;
        bool redo() override;
        std::pair<int, int> getCoords() override;
        std::string getDescription() override;
};

#endif
