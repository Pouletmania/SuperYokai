#ifndef EVENT_UTILITIES_H
#define EVENT_UTILITIES_H

#include <SFML/Window/Event.hpp>

#include <string>

inline
std::string makeIdentifier(void* object, const std::string& name)
{
	return name + ":" + std::to_string(reinterpret_cast<std::uintptr_t>(object)); // store memory adress after name (to reduce search time)
}

inline
bool equals(const sf::Event::KeyEvent& key1, const sf::Event::KeyEvent& key2) // tests if key and controls are the same
{
        return (key1.code == key2.code && key1.alt == key2.alt
                                        && key1.control == key2.control
                                        && key1.shift == key2.shift
                                        && key1.system == key2.system);
}

#endif
