/** 
 *  @file   App.cpp 
 *  @brief  Main class for program
 *  @author Mike and Dennis Ping
 *  @date   2021-11-13
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
// Include standard library C++ libraries.
#include <cassert>
#include <iostream>
#include <string>
#include <queue>
// Project header files
#include "App.hpp"
// #include "Draw.hpp"
#include "MathUtility.hpp"

/*! \brief  App constructor
*		
*/
App::App() {
    // Canvas variables
    m_window = nullptr;
    m_image = new sf::Image;
    m_sprite = new sf::Sprite;
    m_texture = new sf::Texture;
    m_current_color = new sf::Color;
    m_paintbrush_radius = nullptr;
    m_cursor_sprite = new sf::Sprite;
    m_cursor_texture = new sf::RenderTexture;
    m_cursor_circle = new sf::CircleShape(5);
    m_circle_template = new std::vector<std::pair<int,int>>;
    m_prev_point = nullptr;
    m_render_texture = new sf::RenderTexture;
    m_render_sprite = new sf::Sprite;
    m_draw_vector = new std::vector<std::unique_ptr<RoundedLine>>;

    // Color code member variable
    color_codes = {
        {sf::Keyboard::Num1, sf::Color::Black},
        {sf::Keyboard::Num2, sf::Color::White},
        {sf::Keyboard::Num3, sf::Color::Red},
        {sf::Keyboard::Num4, sf::Color::Green},
        {sf::Keyboard::Num5, sf::Color::Blue},
        {sf::Keyboard::Num6, sf::Color::Yellow},
        {sf::Keyboard::Num7, sf::Color::Magenta},
        {sf::Keyboard::Num8, sf::Color::Cyan}
    };
    
    
}

App::~App(){}

/*! \brief  Clear the redo stack.
*		
*/
void App::ClearRedo() {
    int clearCount = 0;
    while(!m_redo.empty()) {
        clearCount++;
        m_redo.pop();
    }
    while (!m_redo_count.empty()) {
        m_redo_count.pop();
    }
    if (clearCount > 0) {
        std::cout << "Cleared " << clearCount << " from the redo stack" << std::endl;
    }
}

/*! \brief 	Add new commands to queue for execution.
*		
*/
// void App::AddCommand(std::unique_ptr<Command> c) {
//     m_commands.push(std::move(c));
// }
void App::AddCommand(std::unique_ptr<RoundedLine> c) {
    m_commands.push(std::move(c));
}

/*! \brief  Execute commands from the m_command stack.
*           Push executed commands to the undo stack.
*           Clear the redo stack.
*/
int App::ExecuteCommand() {
    int successCount = 0;
    while (!m_commands.empty()) {
        // bool success = m_commands.front() -> execute(*m_render_texture);
        // if (success) {
        //     m_undo.push(std::move(m_commands.front()));
        //     successCount++;
        // } 
        m_draw_vector->push_back(std::move(m_commands.front()));
        m_commands.pop();
        successCount++;
    }
    ClearRedo();
    return successCount;
}

/*! \brief  Look at the m_undo_count stack to determine how many Draw commands
*           to undo. The opposite logic of the RedoCommand().
*/
int App::UndoCommand() {
    // Need this if statement so we don't undo "nothing"
    int numUndo = 0;
    if (!m_undo_count.empty()) {
        numUndo = m_undo_count.top();
        std::cout << "Undoing: " << numUndo << " lines" << std::endl;
        // for (int i = 0; i < m_undo_count.top(); i++) {
        //     m_undo.top() -> undo(*m_render_texture);
        //     m_redo.push(std::move(m_undo.top()));
        //     m_undo.pop();
        // }
        for (int i = 0; i < numUndo; i++) {
            m_redo.push(std::move(m_draw_vector->back()));
            m_draw_vector->pop_back();
        }
        m_redo_count.push(m_undo_count.top());
        m_undo_count.pop();
    }
    else {
        std::cout << "There is nothing to undo" << std::endl;
    }
    return numUndo;
}

/*! \brief  Look at the m_redo_count stack to determine how many Draw commands
*           to redo. The opposite logic of the UndoCommand().
*/
int App::RedoCommand() {
    // Need this if statement so we don't redo "nothing"
    int numRedo = 0;
    if (!m_redo_count.empty()) {
        numRedo = m_redo_count.top();
        std::cout << "Redoing: " << numRedo << " lines" << std::endl;
        // for (int i = 0; i < m_redo_count.top(); i++) {
        //     m_redo.top() -> redo(*m_render_texture);
        //     m_undo.push(std::move(m_redo.top()));
        //     m_redo.pop();
        // }
        for (int i = 0; i < numRedo; i++) {
            m_draw_vector->push_back(std::move(m_redo.top()));
            m_redo.pop();
        }
        m_undo_count.push(m_redo_count.top());
        m_redo_count.pop();
    }
    else {
        std::cout << "There is nothing to redo" << std::endl;
    }
    return numRedo;
}

/*! \brief  Return a reference to our m_image, so that
*           we do not have to publicly expose it.
*		
*/
sf::Image& App::GetImage(){
    return *m_image;
}

/*! \brief  Return a reference to our m_Texture so that
*           we do not have to publicly expose it.
*		
*/
sf::Texture& App::GetTexture(){
    return *m_texture;
}

/*! \brief  Return a reference to our m_window
*		
*/
sf::RenderWindow& App::GetWindow(){
    return *m_window;
}

/*! \brief  Return a reference to our m_sprite
*
*/
sf::Sprite& App::GetSprite(){
    return *m_sprite;
}

sf::RenderTexture& App::GetRenderTexture(){
    return *m_render_texture;
}

sf::Sprite& App::GetRenderSprite(){
    return *m_render_sprite;
}

/*! \brief  Return a reference to our m_current_color
*/
sf::Color& App::GetPaintbrushColor() {
    return *m_current_color;
}

/*! \brief  Set the m_current_color to the newColor
*
*/
void App::SetPaintbrushColor(sf::Keyboard::Key numKey) {
    *m_current_color = color_codes[numKey];
}

/*! \brief  Return a reference to our m_paintbrush_radius
*
*/
int& App::GetPaintbrushRadius(){
    return *m_paintbrush_radius;
}

/*! \brief  Set the m_paintbrush_radius to the new radius.
*
*/
void App::SetPaintbrushRadius(int radius){
    *m_paintbrush_radius = radius;
}

/*! \brief  Set the sprite cursor position on the window and apply an offset because
*           the pointer tip is not exactly in the center of the cursor.
*/
void App::SetCursorPosition(const int &x, const int &y) {
    m_cursor_sprite->setPosition(x - *m_paintbrush_radius, y - *m_paintbrush_radius);
}

/*! \brief  Generate a new cursor if the paintbrush radius or color is changed.
*		
*/
void App::GenerateCursor(int radius, sf::Color paintbrush_color) {
    radius += 1;
    // Build our circle shape
    m_cursor_circle -> setRadius(radius);
    m_cursor_circle -> setFillColor(paintbrush_color);
    m_cursor_circle -> setPointCount(4*radius);
    if (paintbrush_color == sf::Color::Black || paintbrush_color == sf::Color::Blue) {
        m_cursor_circle -> setOutlineColor(sf::Color::White);
    }
    else {
        m_cursor_circle -> setOutlineColor(sf::Color::Black);
    }
    m_cursor_circle -> setOutlineThickness(-1);

    // Create the cursor texture and draw our circle shape on it
    m_cursor_texture -> create((radius)*2, (radius)*2);
    m_cursor_texture -> clear(sf::Color::Transparent);
    //m_cursor_texture -> setSmooth(true);
    m_cursor_texture -> draw(*m_cursor_circle);
    m_cursor_sprite -> setTexture(m_cursor_texture -> getTexture(), true);
}

/*! \brief  Generate and cache a circle template for drawing.
*
*/
void App::GenerateCircleTemplate(int radius) {
    *m_circle_template = MathUtility::BresenhamCircleAlgo(radius);
}

/*! \brief  A cached circle template generated at (0,0) which is shifted by (x,y) 
*           rather than constantly computing the same circle.
*/
std::vector<std::pair<int,int>> App::UseCircleTemplate(int x, int y) {
	std::vector<std::pair<int,int>> transformedCircle;
    // Reserve memory in vector to get 4x performance: https://github.com/facontidavide/CPP_Optimizations_Diary/blob/master/docs/reserve.md 
    transformedCircle.reserve((m_circle_template->size()*8));
    // Use std::transform to shift the circle template by (x,y)
    std::transform(m_circle_template->begin(), m_circle_template->end(), std::back_inserter(transformedCircle),
    	[x,y](std::pair<int,int> p) {
    		return std::make_pair(p.first + x, p.second + y);
    	});
    return transformedCircle;
}

/*! \brief 	Destroy all raw pointers before ending the program.
*		
*/
void App::Destroy(){
    delete m_prev_point;
    delete m_render_texture;
    delete m_render_sprite;
    delete m_current_color;
    delete m_paintbrush_radius;
    delete m_cursor_sprite;
    delete m_cursor_texture;
    delete m_cursor_circle;
    delete m_circle_template;
    delete m_image;
    delete m_sprite;
    delete m_texture;
    delete m_window;
}

/*! \brief  Initializes the App and sets up the main
*           rendering window(i.e. our canvas.)
*/
void App::Init(void (*initFunction)(void)){
    // Create our window
    int width = 1280;
    int height = 720;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    m_window = new sf::RenderWindow(sf::VideoMode(width,height),"Mini-Paint alpha 0.0.3",sf::Style::Titlebar | sf::Style::Close, settings);
    //m_window -> clear(sf::Color::White);
    //m_window -> setFramerateLimit(60);
    m_window -> setVerticalSyncEnabled(true);
    // Set the mouse cursor to be invisible because we are going to draw our own cursor
    m_window->setMouseCursorVisible(false);
    // Create an image which stores the pixels we will update
    m_image->create(width, height, sf::Color::White);
    assert(m_image != nullptr && "m_image != nullptr");
    // Create a texture which lives in the GPU and will render our image
    m_texture->loadFromImage(*m_image);
    assert(m_texture != nullptr && "m_texture != nullptr");
    // // Create a sprite which is the entity that can be textured
    m_sprite->setTexture(*m_texture);
    assert(m_sprite != nullptr && "m_sprite != nullptr");
    m_render_texture->create(width, height);
    m_render_texture->clear(sf::Color::White);
    m_render_sprite->setTextureRect(sf::IntRect(0, height, width, -height));
    // Initialize current color = black
    SetPaintbrushColor(sf::Keyboard::Num0);
    // Initialize the cursor radius
    m_paintbrush_radius = new int(5);
    // Generate the cursor with current color = black
    GenerateCursor(*m_paintbrush_radius, sf::Color::Black);
    // Set the cursor initial position to off screen so it doesn't momentarily appear
    int x = -10;
    int y = -10;
    SetCursorPosition(x, y);
    GenerateCircleTemplate(5);
    m_initFunc = initFunction;
}

/*! \brief  Set a callback function which will be called
*           each iteration of the main loop before drawing.
*		
*/
void App::UpdateCallback(void (*updateFunction)(App& myApp)){
    m_updateFunc = updateFunction;
}

/*! \brief  Set a callback function which will be called
*           each iteration of the main loop after update.
*		
*/
void App::DrawCallback(void (*drawFunction)(App& myApp)){
    m_drawFunc = drawFunction;
}

/*! \brief 	The main loop function which handles initialization
            and will be executed until the main window is closed.
            Within the loop function the update and draw callback
            functions will be called.
*		
*/
void App::Loop(App& myApp){
    // Call the init function
    m_initFunc();

    // Start the main rendering loop
    while(m_window->isOpen()){
        // Clear the window
        m_window->clear(sf::Color::White);
        // Updates specified by the user
        m_updateFunc(myApp);
        // Additional drawing specified by user
        m_drawFunc(myApp);
        // Update the texture
        // Note: This can be done in the 'draw call'
        // Draw to the canvas
        // m_window->draw(*m_render_sprite);
        //m_window->draw(*m_sprite);
        
        // Iterate through m_draw_vector and draw each line
        for (auto& line : *m_draw_vector) {
            m_window->draw(*line);
        }

        m_window->draw(*m_cursor_sprite);
        // Display the canvas
        m_window->display();
    }
}


