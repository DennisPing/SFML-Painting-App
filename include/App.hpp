/** 
 *  @file   App.hpp 
 *  @brief  App class interface
 *  @author Mike and Dennis Ping
 *  @date   2021-11-13
 ***********************************************/
#ifndef APP_HPP 
#define APP_HPP

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System.hpp>
// Include standard library C++ libraries.
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
// Project header files
#include "Command.hpp"
#include "Draw.hpp"
#include "RoundedLine.hpp"

// The main application that contains Minipaint functionality
class App{
private:

    std::queue <std::unique_ptr<RoundedLine>> m_commands;
    std::stack <std::unique_ptr<RoundedLine>> m_undo;
    std::stack <std::unique_ptr<RoundedLine>> m_redo;
    
    // Main image	
    sf::Image* m_image;
    // Create a sprite that we overaly
    // on top of the texture.
    sf::Sprite* m_sprite;
    // Texture sent to the GPU for rendering
    sf::Texture* m_texture;
    // Our rendering window
    sf::RenderWindow* m_window;
    // Current paintbrush color
    sf::Color* m_current_color;
    // Paintbrush radius
    int* m_paintbrush_radius;
    // Mouse cursor sprite
    sf::Sprite* m_cursor_sprite;
    // Mouse cursor texture
    sf::RenderTexture* m_cursor_texture;
    // Mouse cursor CircleShape template
    sf::CircleShape* m_cursor_circle;
    // Circle template for the paintbrush to use
    std::vector<std::pair<int,int>>* m_circle_template;

    sf::RenderTexture* m_render_texture;
    sf::Sprite* m_render_sprite;

    std::vector<std::unique_ptr<RoundedLine>>* m_draw_vector;

    // Helper method to clear redo stack 
    void ClearRedo();

public:
    App();
    ~App();
    // Public color codes map
    std::unordered_map<sf::Keyboard::Key, sf::Color> color_codes;
    // Stack that stores the number of undo commands.
    std::stack<int> m_undo_count;
    // Stack that stores the number of redo commands.
    std::stack<int> m_redo_count;
    sf::Vector2f* m_prev_point;

    // Count the number of commands from mouse press to mouse release.
    int cmdCount = 0;
    // A vector of draw commands set to nullptr.
    // std::vector<std::unique_ptr<Command>> prevCmds;

    // Function pointer of init callback.
	void (*m_initFunc)(void);
    // Function pointer of update callback.
	void (*m_updateFunc)(App& myApp);
    // Function pointer of draw callback.
	void (*m_drawFunc)(App& myApp);

    // void 	AddCommand(std::unique_ptr<Command> c);
    void    AddCommand(std::unique_ptr<RoundedLine> c);
    int 	ExecuteCommand();
    int 	UndoCommand();
    int	    RedoCommand();

    // Delete the copy, copy assignment, move, and copy move assignment
    App(const App& other) = delete;
    App(App&& other) = delete;
    App& operator=(const App& other) = delete;
    App& operator=(App& other) = delete;

    sf::Image&          GetImage();
    sf::Texture&        GetTexture();
    sf::RenderWindow&   GetWindow();
    sf::Sprite&         GetSprite();
    sf::RenderTexture&  GetRenderTexture();
    sf::Sprite&         GetRenderSprite();
    
    sf::Color&  GetPaintbrushColor();
    void        SetPaintbrushColor(sf::Keyboard::Key numKey);
    int&        GetPaintbrushRadius();
    void        SetPaintbrushRadius(int radius);

    void SetCursorPosition(const int &x, const int &y);
    void GenerateCursor(int radius, sf::Color color);

    void GenerateCircleTemplate(int radius);
    std::vector<std::pair<int,int>> UseCircleTemplate(int x, int y);
    
	void Init(void (*initFunction)(void));
	void UpdateCallback(void (*updateFunction)(App& myApp));
	void DrawCallback(void (*drawFunction)(App& myApp));
	void Loop(App& myApp);
    void Destroy();

};


#endif
