/** 
 *  @file   Command.cpp 
 *  @brief  Implementation of Command.hpp
 *  @author Mike and Dennis Ping 
 *  @date   2021-11-13
 ***********************************************/

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
// Project header files
#include "Command.hpp"

/*! \brief 	Command constructor.
*		
*/
Command::Command(sf::Image& image) : m_image(image) {}

/*! \brief 	Command destructor.
*		
*/
Command::~Command(){}