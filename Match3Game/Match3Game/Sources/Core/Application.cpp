#include "Application.hpp"

#include <iostream>


core::Application::Application()
	: _window(sf::VideoMode(1024, 768), "Match3 Game")
{
	//texture1.loadFromFile("bg.png");
	//sprite.setTexture(texture1);
	_rootNode.reset(new Node("Root"));

	_field = std::make_shared<match3::Field>();
	_field->Init(10, 10);
}

void core::Application::HandleUpdate()
{
	sf::Event event;

	sf::Clock deltaClock;

	while (_window.isOpen())
	{
		const auto deltaTime = deltaClock.restart();
		while (_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					_rootNode->MouseDown(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					_rootNode->MouseUp(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
					_rootNode->MouseCancel();
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				_rootNode->MouseMove(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
			}
		}

		_window.clear(sf::Color::Black);
		
		_rootNode->Update(deltaTime.asSeconds());
		_rootNode->Draw(_window, _renderStates, sf::Transform());
		_window.display();
		
		//_window.draw(sprite);
		

		if (deltaTime.asMilliseconds() < TicksDelay)
		{
			sf::sleep(sf::milliseconds(TicksDelay) - deltaTime);
		}
	}
}

std::shared_ptr<match3::Field> core::Application::GetFieldPtr()
{
	return _field;
}
