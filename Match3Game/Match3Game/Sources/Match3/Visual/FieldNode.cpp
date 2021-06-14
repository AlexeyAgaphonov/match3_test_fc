#include "FieldNode.hpp"

using namespace match3;

FieldNode::FieldNode(std::shared_ptr<Field> field)
	: Node("FieldNode"), _field(field)
	, _cursor(5)
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

	_width = field->GetChipsField().size() * ChipDistance;
	_height = field->GetChipsField()[0].size() * ChipDistance;

	sf::Vector2f pos((1024.f - _width) / 4, -(768 - _height) / 4);
	setPosition(pos);
	auto scale = getScale();

	_font.loadFromFile("Resources/MontserratAlternates-Medium.ttf");

	_cursor.setFillColor(sf::Color::White);
	_cursor.setOutlineColor(sf::Color::Black);
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
				currTransform = currTransform.translate(sf::Vector2f(x * ChipDistance, _height - y * ChipDistance + ChipDistance / 2));
				sf::CircleShape sf(ChipRadius);
				

				ChipType chipType = chip.GetType();
				target.draw(_chipDrawers[static_cast<size_t>(chipType)], currTransform);

				// Debug
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

	target.draw(_cursor, parentTransform * getTransform());



}

bool FieldNode::InnerMouseDown(const sf::Vector2f& pos)
{
	_cursorPos = pos;
	_cursor.setPosition(pos);
	
	sf::FloatRect rect(getPosition(), sf::Vector2f(_width, _height));

	return rect.contains(pos);
}
