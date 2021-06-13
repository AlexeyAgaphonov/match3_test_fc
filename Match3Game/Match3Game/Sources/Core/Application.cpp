#include "Application.hpp"
#include "Node.hpp"


core::Application::Application()
{
	_window.reset(new sf::RenderWindow(sf::VideoMode(1024, 768), "Match3 Game"));
	texture1.loadFromFile("bg.png");
	sprite.setTexture(texture1);
	_rootNode.reset(new Node("Root"));
}

void core::Application::HandleUpdate()
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

		_rootNode->Update(deltaTime.asSeconds());
		_rootNode->FullRender();
		
		_window->clear();
		_window->draw(sprite);
		_window->display();

		if (deltaTime.asMilliseconds() < TicksDelay)
		{
			sf::sleep(sf::milliseconds(TicksDelay) - deltaTime);
		}
	}
}
