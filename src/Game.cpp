#include "Game.h"

Game::Game()
: m_evtMgr(EventManager::getInstance()) // instantiation of the EventManager singleton
, m_window(640u, 480u, "Test")
{ }

void Game::run()
{
	m_window.open();
	
	// Game loop
	while (isRunning())
	{
		processEvents();
		update();
		display();
	}
}

void Game::launch()
{
	m_evtMgr->load(); // load sfml events converts
}

void Game::processEvents()
{
	m_evtMgr->processRequests(); // update m_evtMgr with new events and callbacks

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		m_evtMgr->handle(event); // transfer work to m_evtMgr
	}
}

void Game::update()
{
	// Game tick
}

void Game::display()
{
	m_window.clear();

	m_window.display();
}
