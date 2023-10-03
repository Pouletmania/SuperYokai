#ifndef GAME_H
#define GAME_H

#include "windows/GraphicWindow.h"
#include "events/EventManager.h"

/* Game class
 * Used once in main function
 * 
 * This is the main class of the program
 * It coordinates all actions, and controls the game loop
 */

class Game
{
	public:

		Game();

		// no copies
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		void launch();
		void run();

	private:

		inline bool isRunning() const;

		void processEvents();
		void update();
		void display();

		// Attributes

		std::shared_ptr<EventManager> m_evtMgr; // singleton
		GraphicWindow m_window;
};

inline
bool Game::isRunning() const
{
	return m_window.isOpen();
}

#endif
