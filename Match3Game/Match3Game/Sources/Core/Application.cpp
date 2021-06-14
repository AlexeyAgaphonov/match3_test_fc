#include "Application.hpp"


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
	sf::Event ev;

	sf::Clock deltaClock;

	while (_window.isOpen())
	{
		const auto deltaTime = deltaClock.restart();
		while (_window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				_window.close();
			}
		}

		_window.clear(sf::Color::Black);
		
		_rootNode->Update(deltaTime.asSeconds());

		if (_rootNode->IsDrawable())
		{
			_rootNode->Draw(_window, _renderStates, sf::Transform());
		}
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
