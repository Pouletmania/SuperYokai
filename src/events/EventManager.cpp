#include "EventManager.h"

/* Handling events
 * 
 * We search if an identifier is bound to the parameter event
 * If there is, we call the bound callback
 */

void EventManager::handle(const sf::Event& event)
{
	for (const auto& order : m_orders) // for each order
	{
		if (m_ordersToDelete.find(order.first) != m_ordersToDelete.end()) // if order is about to be deleted
			continue;

		const sf::Event& evt = order.second; // for each event binded to this order (because m_orders is a multimap)
		// test if events match
		if (event.type == evt.type) // if type matches
		{
			bool call = false;
			switch (evt.type)
			{
				// no test events
				case sf::Event::Closed:
					call = true;
					break;

				//keyboard events
				case sf::Event::KeyPressed:
				case sf::Event::KeyReleased:
					call = equals(event.key, evt.key); // test if keys and controls are the same
					break;

				default: // other events
					// not handled yet
					break;

			}
			if (call)
			{
				m_callbacks.at(order.first)(event); // call binded function and give event as parameter
			}
		}
	}
}

/* Process requests
 * Used to update events bindings
 *
 * Delete events bindings first, read new files, and free the two buffers
 * This order is used to avoid bugs in case a new GameState has the same identifier than one that has just been deleted
 */

void EventManager::processRequests()
{
        // delete orders first
        while (!m_ordersToDelete.empty())
        {
                auto order = m_ordersToDelete.begin();

                forgetOrder(*order);
                m_ordersToDelete.erase(order);
        }

        // load orders from files
        while (!m_filesToLoad.empty())
        {
		std::string filename = m_filesToLoad.begin()->second;
		std::string identifier = m_filesToLoad.begin()->first;

                loadOrders(filename, identifier);
                m_filesToLoad.erase(identifier);
        }
}

/* Read events files 
 * Done in response of requests, and once a game tick
 * Used to read stored events bindings
 * Usually used by GameStates when they're loading
 * These files should be stored in resources/conf/events/ directory
 * 
 * Each line begins with the name chosen for the binding, then keys in SFML format
 * All possible values are stored in resources/conf/build/events/ directory
 */

void EventManager::loadOrders(const std::string& filename, const std::string& identifier)
{
	std::ifstream file (filename); // opening file to read
	if (!file)
	{
		throw std::runtime_error("Error opening file : " + filename);
	}

	std::string line;
	while (std::getline(file, line)) // for each line
	{
		if (line.empty() || line[0] == '/') // ignore empty lines, and those beginning with '/',  they're for comments
		{
			continue;
		}

		std::istringstream lineIs (line);
		sf::Event event; // to insert in m_orders at block end

		// read order and event type
		std::string order, sEventType;
		lineIs >> order >> sEventType;

		// compute sf::Event::EventType
		auto found = std::find(m_typeConverts.cbegin(), m_typeConverts.cend(), sEventType); // find sEventType in m_typeConverts
		if (found == m_typeConverts.cend())
		{
			throw std::runtime_error(filename + " : Error of conversion : " + sEventType);
		}
		decltype(m_typeConverts.size()) index = found - m_typeConverts.cbegin(); // process index
		event.type = static_cast<sf::Event::EventType>(index); // convert to sf::Event::EventType

		/** Keyboard events **/
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// read key
			std::string sKey;
			lineIs >> sKey;

			// compute sf::Keyboard::Key
			auto found = std::find(m_keyboardConverts.cbegin(), m_keyboardConverts.cend(), sKey); // find sKey in m_keyboardConverts
			if (found == m_keyboardConverts.cend())
			{
				throw std::runtime_error(filename + " : Error of conversion : " + sKey);
			}
			decltype(m_keyboardConverts.size()) index = found - m_keyboardConverts.cbegin(); // process index
			event.key.code = static_cast<sf::Keyboard::Key>(index); // convert to sf::Keyboard::Key

			// read controls
			event.key.alt = event.key.control = event.key.shift = event.key.system = false; // initialize
			std::string ctrls;
			while (lineIs >> ctrls)
			{
				if (ctrls == "alt")
					event.key.alt = true;
				else if (ctrls == "ctrl")
					event.key.control = true;
				else if (ctrls == "shift")
					event.key.shift = true;
				else if (ctrls == "system")
					event.key.system = true;
				else
					throw std::runtime_error(filename + " : Unrecognized control : " + ctrls);
			}
		}

		// add order
		m_orders.emplace(order + identifier, event); // add unique identifier
	}
}

/* Loading SFML Enums as vectors
 * Read them from resources/conf/build/events/ directory
 *
 * These vectors allow us to convert Enum position to std::string, corresponding to the name of the element in the Enum
 * Thanks to this convert, orders files (resources/conf/events/) can be easily written with SFML Enums identifiers
 */

void EventManager::loadEnum(std::vector<std::string>& cont, const std::string& filename)
{
	cont.clear(); // make sure cont is empty

        std::ifstream file (filename); // open file to read
        if (!file)
        {
                throw std::runtime_error("Error openening file : " + filename);
        }

        std::string line;
        while (std::getline(file, line)) // for each line
        {
                if (!line.empty() && line[0] != '/') // ignore empty lines, and those beginning with '/',  they're for comments
                {
                        cont.push_back(line); // each line has an index in cont equals to its position in file, ignoring comments and empty lines
                }
        }
}
