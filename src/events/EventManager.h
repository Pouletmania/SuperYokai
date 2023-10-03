#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <SFML/Window/Event.hpp>

#include "utilities.h"

#include <memory>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

/* EventManager class
 * Uses singleton pattern
 * Used by Game and by all the classes whose methods we want to store as callbacks
 * 
 * This class is used to bind events (here SFML events) to callbacks, which are usually GameStates methods
 * Several events could be bound to the same callback
 * To do this, we use a std::string identifier, made up of the memory adress of the GameState and the name chosen for the binding
 * Identifier is bound with both events (called orders) and callbacks.
 *
 * We read events bindings in files, stored in resources/conf/event/ directory
 * To unload events or callbacks bindings, or to load callbacks bindings, we need to specify the chosen name of this binding
 * Each event loading or unloading is just a request. We store the identifier (and the filename) in a buffer, and requests are processed at the next game tick
 */

class EventManager
{
	/*********************
	 * Singleton pattern *
	 *********************/

	public:

		static std::shared_ptr<EventManager> getInstance()
		{
			static std::shared_ptr<EventManager> instance(new EventManager()); // initialized once
			return instance; // return a copy of the static shared_ptr
		}

		// forbid copies
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		// destructed when program ends

	private:

		EventManager() = default; // private constructor

	/*****************
	 * Event manager *
	 *****************/

	public:
	
		inline void load(); // to load SFML event types
		void handle(const sf::Event&); // functions called here

		// callbacks
		template<class T>
		void bindCall(const std::string& name, void (T::*f)(const sf::Event&), T* state) // immediatly done
		{
			std::string identifier = makeIdentifier(state, name);
			m_callbacks.emplace(identifier, std::bind(f, state, std::placeholders::_1)); // convert function type to ignore instance type
		}
		inline void forgetCall(const std::string&, void*); // immediatly done

		// events bindings
		inline void requestOrders(const std::string& file, void*);
		inline void requestForget(const std::string&, void*);

		void processRequests();

	private:

		// requests processings
		void loadOrders(const std::string& file, const std::string& identifier);
		inline void forgetOrder(const std::string&);
		
		// Loading
		void loadEnum(std::vector<std::string>&, const std::string& file);
		inline void loadEventTypes(); // load m_typeConverts in the same order as sf::Event::EventType enum, to make converts easier
		inline void loadKeyboardKeys(); // load m_keyboardConverts in the same order as sf::Keyboard::Key enum, to make converts easier

		/**************
		 * Attributes *
		 **************/

		std::multimap<std::string, sf::Event> m_orders; // links one-to-many event to each identifier
		std::map<std::string, std::function<void(const sf::Event&)>> m_callbacks; // links identifiers to function pointers

		// Requests buffers
		std::set<std::string> m_ordersToDelete;
		std::multimap<std::string, std::string> m_filesToLoad; // potentially several files for one identifier

		// Event types conversions
		const std::string M_TYPE_URL = "resources/conf/build/events/event_types";
		const std::string M_KEYS_URL = "resources/conf/build/events/keyboard_keys";

		std::vector<std::string> m_typeConverts; // to convert sf::Event::EventType from / to std::string
		std::vector<std::string> m_keyboardConverts; // to convert sf::Keyboard::Key from / to std::string
};

/******************
 * Inline methods *
 ******************/

inline
void EventManager::load()
{
	loadEventTypes();
	loadKeyboardKeys();
}

inline
void EventManager::forgetCall(const std::string& name, void* object)
{
	std::string identifier = makeIdentifier(object, name);
	m_callbacks.erase(m_callbacks.lower_bound(identifier), m_callbacks.upper_bound(identifier));
}

/**********
 * Orders *
 **********/

inline
void EventManager::requestOrders(const std::string& filename, void* object)
{
	std::string identifier = makeIdentifier(object, "");
	m_filesToLoad.emplace(identifier, filename);
}

inline
void EventManager::requestForget(const std::string& name, void* object)
{
	std::string identifier = makeIdentifier(object, name);
	m_ordersToDelete.emplace(identifier);
}

inline
void EventManager::forgetOrder(const std::string& identifier)
{
	m_orders.erase(m_orders.lower_bound(identifier), m_orders.upper_bound(identifier));
}

/***********
 * Loading *
 ***********/

inline
void EventManager::loadEventTypes()
{
	loadEnum(m_typeConverts, M_TYPE_URL);
}

inline
void EventManager::loadKeyboardKeys()
{
	loadEnum(m_keyboardConverts, M_KEYS_URL);
}

#endif
