#ifndef GRAPHIC_WINDOW_H
#define GRAPHIC_WINDOW_H

#include <SFML/Graphics.hpp>

#include "../events/EventManager.h"

/* GraphicWindow
 * Used in Game
 * 
 * This class stores a sf::RenderWindow, and is used in the same way
 * This gives us a better control on it
 * e.g about resizing or callbacks bindings
 */

class GraphicWindow
{
	public:
		
		GraphicWindow(const sf::Vector2u&, const std::string&, bool = false);
		inline GraphicWindow(unsigned, unsigned, const std::string&, bool = false);
		~GraphicWindow();

		// Control methods
		inline void open();
		inline void close(const sf::Event&);

		// SFML methods
		inline bool pollEvent(sf::Event&); // store event in parameter if there's one, return true if there are still events in the buffer

		inline void clear(); // reset every pixel
		inline void draw(const sf::Drawable&, const sf::RenderStates& = sf::RenderStates::Default); // draw an SFML object on screen
		inline void display(); // update screen display

		// Accessors
		void toggleFullscreen();
		inline bool isFullscreen() const;

		void setSize(const sf::Vector2u&);
		inline void setSize(unsigned, unsigned);
		inline sf::Vector2u getSize() const;

		inline void setPosition(const sf::Vector2i&);
		inline sf::Vector2i getPosition() const;

		inline std::string getName() const;
		inline bool isOpen() const;

	private:

		// Do the real job of opening and closing window
		void create();
		void destroy();

		// Attributes

		sf::Vector2u m_size;
		std::string m_name;
		bool m_isFullscreen;

		sf::RenderWindow m_window;

		std::shared_ptr<EventManager> l_evtMgr; // singleton, already initialized in game
};

// Second constructor, calls the first one
inline
GraphicWindow::GraphicWindow(unsigned width, unsigned height, const std::string& name, bool isFullscreen)
: GraphicWindow(sf::Vector2u(width, height), name, isFullscreen)
{ }

/***********
 * Control *
 ***********/

inline
void GraphicWindow::open()
{
	if (!isOpen()) // tests to avoid reseting an opened window
		create();
}

inline
void GraphicWindow::close(const sf::Event&) // ignoring everything about event
{
	if (isOpen())
		destroy();
}

/****************
 * SFML methods *
 ****************/

inline
bool GraphicWindow::pollEvent(sf::Event& event)
{
	return m_window.pollEvent(event);
}

inline
void GraphicWindow::clear()
{
	m_window.clear();
}

inline
void GraphicWindow::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	m_window.draw(drawable, states);
}

inline
void GraphicWindow::display()
{
	m_window.display();
}

/*************
 * Accessors *
 *************/

// Fullscreen

inline
bool GraphicWindow::isFullscreen() const
{
	return m_isFullscreen;
}

// Size

inline
void GraphicWindow::setSize(unsigned width, unsigned height)
{
	setSize(sf::Vector2u(width, height));
}

inline
sf::Vector2u GraphicWindow::getSize() const
{
	return m_size;
}

// Position

inline
void GraphicWindow::setPosition(const sf::Vector2i& pos)
{
	m_window.setPosition(pos);
}

inline
sf::Vector2i GraphicWindow::getPosition() const
{
	return m_window.getPosition();
}

// Name

inline
std::string GraphicWindow::getName() const
{
	return m_name;
}

// Window state : isOpen

inline
bool GraphicWindow::isOpen() const
{
	return m_window.isOpen();
}

#endif
