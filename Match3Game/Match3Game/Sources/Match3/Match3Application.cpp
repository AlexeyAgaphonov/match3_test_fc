#include "Match3Application.hpp"

#include "Match3/Logic/Balance.hpp"
#include "Visual/FieldNode.hpp"

Match3Application::Match3Application()
	: Application(sf::VideoMode(1024, 768), "Match3 Game")
{
	match3::balance::InitSeed();
	_rootNode.reset(new core::Node("Root"));

	NewField();
}

Match3Application::~Match3Application()
{
	
}

void Match3Application::HandleUpdate(float dt)
{
	if (_recreateField)
	{
		_recreateField = false;
		_rootNode->RemoveChildByName("FieldNode");
		_lastFieldSize += 2;
		NewField();
	}
	
	_rootNode->Update(dt);
}


void Match3Application::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		CloseWindow();
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

void Match3Application::HandleDraw(sf::RenderTarget& target, sf::RenderStates states)
{
	_rootNode->Draw(target, states, sf::Transform());
}


void Match3Application::NewField()
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

void Match3Application::EventFromField(match3::FieldEvent event)
{
	if (event == match3::FieldEvent::DoNotHaveSwipes)
	{
		_recreateField = true;
	}
}
