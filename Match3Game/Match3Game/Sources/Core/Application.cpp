#include "Application.hpp"


Core::Application::Application()
{
	_window.reset(new sf::RenderWindow(sf::VideoMode(1024, 768), "Match3 Game"));
	texture1.loadFromFile("bg.png");
	sprite.setTexture(texture1);
}

void Core::Application::HandleUpdate()
{
	sf::Event ev;

	sf::Clock deltaClock;
	while (_window->isOpen())
	{
		const auto deltaTime = deltaClock.restart();
		while (_window->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				_window->close();
			}
		}

		_window->clear();
		_window->draw(sprite);
		_window->display();

		if (deltaTime.asMilliseconds() < TicksDelay)
		{
			sf::sleep(sf::milliseconds(TicksDelay) - deltaTime);
		}
	}
}
