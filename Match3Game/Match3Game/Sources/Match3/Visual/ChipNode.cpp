#include "ChipNode.hpp"

#include <corecrt_math_defines.h>


#include "FieldNode.hpp"
#include "Match3/Logic/FieldFunctions.hpp"

using namespace match3;

namespace 
{
	const sf::CircleShape& GetSelectedChipDrawer(ChipType type)
	{
		static std::vector<sf::CircleShape> chipSelectedDrawers;
		if (chipSelectedDrawers.empty())
		{
			for (uint8_t i = 0; i < static_cast<uint8_t>(ChipType::ColorSize); ++i)
			{
				chipSelectedDrawers.push_back(sf::CircleShape(ChipRadius, i + 3));
				chipSelectedDrawers.back().setOutlineColor(sf::Color::Magenta);
				chipSelectedDrawers.back().setOutlineThickness(3);
			}

			chipSelectedDrawers[static_cast<size_t>(ChipType::Blue)].setFillColor(sf::Color(50, 50, 255));
			chipSelectedDrawers[static_cast<size_t>(ChipType::Yellow)].setFillColor(sf::Color(255, 255, 50));
			chipSelectedDrawers[static_cast<size_t>(ChipType::Green)].setFillColor(sf::Color(50, 255, 50));
			chipSelectedDrawers[static_cast<size_t>(ChipType::Red)].setFillColor(sf::Color(255, 50, 50));
			chipSelectedDrawers[static_cast<size_t>(ChipType::Purple)].setFillColor(sf::Color(255, 50, 255));
		}
		return chipSelectedDrawers[static_cast<size_t>(type)];
	}
	
	const sf::CircleShape& GetChipDrawer(ChipType type)
	{
		static std::vector<sf::CircleShape> chipDrawers;
		if (chipDrawers.empty())
		{
			for (uint8_t i = 0; i < static_cast<uint8_t>(ChipType::ColorSize); ++i)
			{
				chipDrawers.push_back(sf::CircleShape(ChipRadius, i + 3));
				chipDrawers.back().setOutlineColor(sf::Color::White);
				chipDrawers.back().setOutlineThickness(2);
			}


			chipDrawers[static_cast<size_t>(ChipType::Blue)].setFillColor(sf::Color(50, 50, 255));
			chipDrawers[static_cast<size_t>(ChipType::Yellow)].setFillColor(sf::Color(255, 255, 50));
			chipDrawers[static_cast<size_t>(ChipType::Green)].setFillColor(sf::Color(50, 255, 50));
			chipDrawers[static_cast<size_t>(ChipType::Red)].setFillColor(sf::Color(255, 50, 50));
			chipDrawers[static_cast<size_t>(ChipType::Purple)].setFillColor(sf::Color(255, 50, 255));
		}
		return chipDrawers[static_cast<size_t>(type)];
	}
	
}

std::string ChipNode::ConvertChipPosToName(const ChipPos& pos)
{
	return "C:" + std::to_string(pos.x) + ";" + std::to_string(pos.y);
}



ChipNode::ChipNode(ChipType type, const std::string& name)
	: Node(name)
	, _type(type)
	, _selected(false)
{
	ChangeState(core::NodeState::Updated | core::NodeState::Drawable);
}

ChipNode::~ChipNode() { }

void ChipNode::InnerUpdate(float dt)
{
	if (_animData.activated)
	{
		_animData.timer += dt;
		if (_animData.timer > _animData.duration)
		{
			_animData.timer = 0.f;
			_animData.activated = false;
			if (_animData.type == AnimDataType::SuccessfulSwipe)
			{
				setPosition(getPosition() + _animData.offset);
			}
		}
		sf::Vector2f vec;
		if (_animData.type == AnimDataType::SuccessfulSwipe)
		{
			vec = sf::Vector2f(SwipeDirectionConvertToOffset(_animData.dir).x,
				-SwipeDirectionConvertToOffset(_animData.dir).y);
			vec *= ChipDistance * 1.0f;
			vec *= static_cast<float>(sin((_animData.timer / _animData.duration) * M_PI_2));
		}
		else if (_animData.type == AnimDataType::BadSwipe)
		{
			vec = sf::Vector2f(SwipeDirectionConvertToOffset(_animData.dir).x,
				-SwipeDirectionConvertToOffset(_animData.dir).y);
			vec *= ChipDistance * 0.5f;
			vec *= static_cast<float>(sin((_animData.timer / _animData.duration) * M_PI));
		}

		_animData.offset = vec;
	}
}

void ChipNode::InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform)
{
	auto currTransform = parentTransform;
	currTransform = currTransform.translate(_animData.offset);
	currTransform = currTransform.translate(0.f, -ChipDistance);
	auto figure = _selected ? GetSelectedChipDrawer(_type) : GetChipDrawer(_type);
	target.draw(figure, currTransform);

}

void ChipNode::AcceptMessage(const std::string& message, const std::string& data)
{
	if (message == "SwipeAnimBadly")
	{
		_animData.activated = true;
		_animData.dir = ConvertStrToSwipeDirection(data);
		_animData.type = AnimDataType::BadSwipe;
	}
	else if (message == "SwipeAnim")
	{
		_animData.activated = true;
		_animData.dir = ConvertStrToSwipeDirection(data);
		_animData.type = AnimDataType::SuccessfulSwipe;
	}
	else if (message == "Select")
	{
		_selected = true;
	}
	else if (message == "Unselect")
	{
		_selected = false;
	}
	else
	{
		Node::AcceptMessage(message, data);
	}
}
