#include "ChipNode.hpp"

#include "FieldNode.hpp"

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

ChipNode::ChipNode(ChipType type)
	: Node("ChipNode")
	, _type(type)
	, _selected(false)
{
	
}

ChipNode::~ChipNode() { }

void ChipNode::InnerUpdate(float dt)
{
	
}

void ChipNode::InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform)
{
	auto currTransform = parentTransform;
	if (_selected)
	{
		target.draw(GetSelectedChipDrawer(_type), currTransform);
	}
	else
	{
		target.draw(GetChipDrawer(_type), currTransform);
	}
}
