#include "Application.hpp"

#include <iostream>
#include "Match3/Logic/Balance.hpp"


core::Application::Application()
	: _window(sf::VideoMode(1024, 768), "Match3 Game")
{
	match3::balance::InitSeed();
	_rootNode.reset(new Node("Root"));
	
	NewField();
}

void core::Application::NewField()
{
	_field.reset(new match3::Field());
	_field->Init(_lastFieldSize, _lastFieldSize);
	auto fieldNode = new match3::FieldNode(_field);
	fieldNode->SubscribeOnEvents([this](match3::FieldEvent event)
		{
			this->EventFromField(event);
		});
	_rootNode->AddChild(core::Node::Ptr(fieldNode));
}


void core::Application::EventFromField(match3::FieldEvent event)
{
	if (event == match3::FieldEvent::DoNotHaveSwipes)
	{
		_recreateField = true;
	}
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

		if (_recreateField)
		{
			_recreateField = false;
			_rootNode->RemoveChildByName("FieldNode");
			_lastFieldSize += 2;
			NewField();
		}
		
		//_window.draw(sprite);
		

		if (deltaTime.asMilliseconds() < TicksDelay)
		{
			sf::sleep(sf::milliseconds(TicksDelay) - deltaTime);
		}
	}
}
