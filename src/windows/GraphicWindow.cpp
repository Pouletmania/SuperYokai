#include "GraphicWindow.h"

/********************************
 * Constructors and destructors *
 ********************************/

GraphicWindow::GraphicWindow(const sf::Vector2u& size, const std::string& name, bool isFullscreen)
: m_size (size)
, m_name (name)
, m_isFullscreen (isFullscreen)
, l_evtMgr (EventManager::getInstance())
{ }

GraphicWindow::~GraphicWindow()
{
	if (isOpen())
		destroy();
}

/*****************
 * Changing size *
 *****************/

void GraphicWindow::toggleFullscreen()
{
	// TODO handle scenes and zoom

	m_isFullscreen = !m_isFullscreen;
	destroy();
	create();
}

void GraphicWindow::setSize(const sf::Vector2u& size)
{
	// TODO handle scenes and zoom

	m_size = size;
	destroy();
	create();
}

/******************************************
 * Opening or closing the internal window *
 ******************************************/

void GraphicWindow::create()
{
	m_window.create(sf::VideoMode(m_size.x, m_size.y), m_name, isFullscreen() ? sf::Style::Fullscreen : sf::Style::Default);

	// events/calls bindings update
	l_evtMgr->bindCall("window_close", &GraphicWindow::close, this);
	l_evtMgr->requestOrders("resources/conf/events/window", this);	
}

void GraphicWindow::destroy()
{
	m_window.close();

	// events/calls bindings update
	l_evtMgr->forgetCall("window_close", this);
	l_evtMgr->requestForget("window_close", this);
}
