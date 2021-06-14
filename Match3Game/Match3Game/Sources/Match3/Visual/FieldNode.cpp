#include "FieldNode.hpp"

using namespace match3;

FieldNode::FieldNode(std::shared_ptr<Field> field)
	: Node("FieldNode"), _field(field)
{
	for (uint8_t i = 0; i < static_cast<uint8_t>(ChipType::ColorSize); ++i)
	{
		_chipDrawers.push_back(sf::CircleShape(ChipRadius, i + 3));
	}
	_chipDrawers[static_cast<size_t>(ChipType::Blue)].setFillColor(sf::Color(50, 50, 255));
	_chipDrawers[static_cast<size_t>(ChipType::Yellow)].setFillColor(sf::Color(255, 255, 50));
	_chipDrawers[static_cast<size_t>(ChipType::Green)].setFillColor(sf::Color(50, 255, 50));
	_chipDrawers[static_cast<size_t>(ChipType::Red)].setFillColor(sf::Color(255, 50, 50));
	_chipDrawers[static_cast<size_t>(ChipType::Purple)].setFillColor(sf::Color(255, 50, 255));

	float fieldWidth = field->GetChipsField().size() * ChipDistance;
	float fieldHeight = field->GetChipsField()[0].size() * ChipDistance;

	setPosition((1024.f - fieldWidth) / 4, (768 - fieldHeight) / 4);

	_font.loadFromFile("Resources/MontserratAlternates-Medium.ttf");
}

void FieldNode::Update(float dt)
{
	
}



void FieldNode::InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform)
{
	if (_field) 
	{
		auto& chipsField = _field->GetChipsField();
		int x = 0;
		for (const auto& horizontalLine: chipsField)
		{
			int y = 0;
			for (const auto& chip: horizontalLine)
			{
				auto currTransform = parentTransform * getTransform();
				currTransform = currTransform.translate(sf::Vector2f(x * ChipDistance, y * ChipDistance));
				sf::CircleShape sf(ChipRadius);
				

				ChipType chipType = chip.GetType();
				target.draw(_chipDrawers[static_cast<size_t>(chipType)], currTransform);

				if (true)
				{
					sf::Text text;
					text.setCharacterSize(10);
					text.setFont(_font);
					text.setString(std::to_string(x) + "-" + std::to_string(y));
					text.setFillColor(sf::Color::White);
					target.draw(text, currTransform);
				}
				++y;
			}
			++x;
		}
	}
}

