/** 
 *  @file   Command.hpp 
 *  @brief  Represents an actionable command by the user.
 *  @author Mike and Dennis Ping
 *  @date   2021-11-13
 ***********************************************/
#ifndef COMMAND_HPP
#define COMMAND_HPP

// Include our Third-Party SFML header
#include <SFML/Graphics/Image.hpp>
// Include standard library C++ libraries.
#include <string>

// The command class
class Command{
    protected:
        sf::Image& m_image;

    public:
        // The Constructor for a command requires an image reference that it uses to
        // apply the command
        Command(sf::Image& image);
        // The Destructor for a command
        virtual ~Command();

        // Returns true or false if the command was able to succssfully
        // execute.
        // If it executes successfully, then that command is added to the
        // undo stack.
        virtual bool execute() = 0;
        virtual bool undo() = 0;
        virtual bool redo() = 0;
        virtual std::pair<int, int> getCoords() = 0;
        //friend bool operator!= (const Command &left, const Command &right);
        virtual std::string getDescription() = 0;
};

#endif
