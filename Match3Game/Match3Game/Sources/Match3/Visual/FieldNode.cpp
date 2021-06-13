#include "FieldNode.hpp"

using namespace match3;

FieldNode::FieldNode(std::weak_ptr<Field>&& field)
	: Node("FieldNode"), _field(field)
{
	for (uint8_t i = 0; i < static_cast<uint8_t>(ChipType::ColorSize); ++i)
	{
		_chipDrawers[static_cast<ChipType>(i)] = sf::CircleShape(ChipRadius, i + 3);
		_chipDrawers[ChipType::Blue].setFillColor(sf::Color(50, 50, 255));
		_chipDrawers[ChipType::Yellow].setFillColor(sf::Color(255, 255, 50));
		_chipDrawers[ChipType::Green].setFillColor(sf::Color(50, 255, 50));
		_chipDrawers[ChipType::Red].setFillColor(sf::Color(255, 50, 50));
		_chipDrawers[ChipType::Purple].setFillColor(sf::Color(255, 50, 255));
	}
}

void FieldNode::Render()
{
	if (auto field = _field.lock()) 
	{
		auto& chipsField = field->GetChipsField();
		int x = 0;
		for (const auto& horizontalLine: chipsField)
		{
			int y = 0;
			for (const auto& chip: horizontalLine)
			{
				//_chipDrawers[chip.GetType()]
				++y;
			}
			++x;
		}
	}
}

