/** 
 *  @file   Draw.cpp 
 *  @brief  Draw implementation, all drawing actions are commands. 
 *  @author Mike and Dennis Ping 
 *  @date   2021-11-13
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics/Color.hpp>
// Include standard library C++ libraries.
#include <string>
#include <map>
// Project header files
#include "Draw.hpp"
#include "Command.hpp"

/*! \brief  Constructor for a Draw command.
*           Stores the current (x,y) mouse coordinates upon creation.
*
*/
Draw::Draw(int x, int y, sf::Image& image, sf::Color paintbrushColor) : Command(image), 
    xCoord(x), 
    yCoord(y), 
    currColor(paintbrushColor) {
}

Draw::~Draw(){}

/*! \brief  Store the previous pixel color and set the new pixel color at (x,y).
*		
*/
bool Draw::execute() {
    if (xCoord < 0 || xCoord >= (int)m_image.getSize().x || yCoord < 0 || yCoord >= (int)m_image.getSize().y) {
        return false;
    }
	prevColor = m_image.getPixel(xCoord, yCoord);
	if (m_image.getPixel(xCoord, yCoord) == currColor) {
		return false;
	}
	m_image.setPixel(xCoord, yCoord, currColor);
	return true;
}

/*! \brief  Set the pixel color at (x,y) to the previous pixel color.
*		
*/
bool Draw::undo(){
	m_image.setPixel(xCoord, yCoord, prevColor);
	return true;
}

/*! \brief  Set the pixel color at (x,y) to the original pixel color.
*		
*/
bool Draw::redo() {
	m_image.setPixel(xCoord, yCoord, currColor);
	return true;
}

/*! \brief  Return the (x,y) coordinates of this Draw command.
*
*/
std::pair<int, int> Draw::getCoords() {
	return std::make_pair(xCoord, yCoord);
}

/*! \brief  Get a string representation of this Draw command in the form (x, y, color).
*
*/
std::string Draw::getDescription() {
    // C++ does not know how to hash an sf::Color object, so we must use the literal integer value.
    const std::map<int, std::string> colorMap {
        {255, "Black"},
        {4294967295, "White"},
        {4278190335, "Red"},
        {16711935, "Green"},
        {65535, "Blue"},
        {4294902015, "Yellow"},
        {4278255615, "Magenta"},
        {16777215, "Cyan"},
        {0, "Transparent"}
    };
	return "Draw (" + std::to_string(xCoord) + ", " + std::to_string(yCoord) + ", " + colorMap.at(currColor.toInteger()) + ")";
}