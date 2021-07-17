#include "Application.hpp"


core::Application::Application(const sf::VideoMode& videoMode, const std::string& appName)
	: _window(videoMode, appName)
{
}



int core::Application::RunCycle()
{
	sf::Event event;

	sf::Clock deltaClock;

	while (_window.isOpen())
	{
		const auto deltaTime = deltaClock.restart();
		while (_window.pollEvent(event))
		{
			HandleEvent(event);
		}

		_window.clear(sf::Color::Black);

		HandleUpdate(deltaTime.asSeconds());
		HandleDraw(_window, _renderStates);
		_window.display();


		if (deltaTime.asMilliseconds() < TicksDelay)
		{
			sf::sleep(sf::milliseconds(TicksDelay) - deltaTime);
		}
	}
	return 0;
}
