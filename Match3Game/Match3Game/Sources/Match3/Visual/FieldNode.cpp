#include "FieldNode.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace match3;

#include "Match3/Logic/FieldFunctions.hpp"

FieldNode::FieldNode(std::shared_ptr<Field> field)
	: Node("FieldNode"), _field(field)
	, _cursor(5)
{
	
	for (uint8_t i = 0; i < static_cast<uint8_t>(ChipType::ColorSize); ++i)
	{
		_chipDrawers.push_back(sf::CircleShape(ChipRadius, i + 3));
		_chipDrawers.back().setOutlineColor(sf::Color::White);
		_chipDrawers.back().setOutlineThickness(2);
		_chipSelectedDrawers.push_back(sf::CircleShape(ChipRadius, i + 3));
		_chipSelectedDrawers.back().setOutlineColor(sf::Color::Magenta);
		_chipSelectedDrawers.back().setOutlineThickness(3);
	}

	
	_chipDrawers[static_cast<size_t>(ChipType::Blue)].setFillColor(sf::Color(50, 50, 255));
	_chipDrawers[static_cast<size_t>(ChipType::Yellow)].setFillColor(sf::Color(255, 255, 50));
	_chipDrawers[static_cast<size_t>(ChipType::Green)].setFillColor(sf::Color(50, 255, 50));
	_chipDrawers[static_cast<size_t>(ChipType::Red)].setFillColor(sf::Color(255, 50, 50));
	_chipDrawers[static_cast<size_t>(ChipType::Purple)].setFillColor(sf::Color(255, 50, 255));

	_chipSelectedDrawers[static_cast<size_t>(ChipType::Blue)].setFillColor(sf::Color(50, 50, 255));
	_chipSelectedDrawers[static_cast<size_t>(ChipType::Yellow)].setFillColor(sf::Color(255, 255, 50));
	_chipSelectedDrawers[static_cast<size_t>(ChipType::Green)].setFillColor(sf::Color(50, 255, 50));
	_chipSelectedDrawers[static_cast<size_t>(ChipType::Red)].setFillColor(sf::Color(255, 50, 50));
	_chipSelectedDrawers[static_cast<size_t>(ChipType::Purple)].setFillColor(sf::Color(255, 50, 255));

	_width = field->GetChipsField().size() * ChipDistance;
	_height = field->GetChipsField()[0].size() * ChipDistance;

	sf::Vector2f pos((1024.f - _width) / 4, -(768 - _height) / 4 + ChipDistance / 2);
	setPosition(pos);
	auto scale = getScale();

	_font.loadFromFile("Resources/MontserratAlternates-Medium.ttf");

	_cursor.setFillColor(sf::Color::White);
	_cursor.setOutlineColor(sf::Color::Black);

	_selectedChip = EMPTY_CHIP_POS;
}

void FieldNode::Update(float dt)
{
	if (_swipingAnimData.activated)
	{
		_swipingAnimData.timer += dt;
		if (_swipingAnimData.timer > _swipingAnimData.duration)
		{
			_swipingAnimData.timer = 0.f;
			_swipingAnimData.activated = false;
			_selectedChip = EMPTY_CHIP_POS;
			_mouseBlocked = false;
		}
		auto dirVec = sf::Vector2f(SwipeDirectionConvertToOffset(_swipingAnimData.dir).x, 
			SwipeDirectionConvertToOffset(_swipingAnimData.dir).y);
		dirVec *= ChipDistance * 0.5f;
		dirVec *= static_cast<float>(sin((_swipingAnimData.timer / _swipingAnimData.duration) * M_PI));
		_swipingAnimData.offset = dirVec;
	}
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
				currTransform = currTransform.translate(sf::Vector2f(x * ChipDistance, _height - y * ChipDistance));
				sf::CircleShape sf(ChipRadius);
				

				ChipType chipType = chip.GetType();
				if (_selectedChip.x == x && _selectedChip.y == y)
				{
					if (_swipingAnimData.activated)
					{
						currTransform = currTransform.translate(_swipingAnimData.offset);
					}
					target.draw(_chipSelectedDrawers[static_cast<size_t>(chipType)], currTransform);
				}
				else
				{
					target.draw(_chipDrawers[static_cast<size_t>(chipType)], currTransform);
				}
				
#if _DEBUG 
				{
					sf::Text text;
					text.setCharacterSize(10);
					text.setFont(_font);
					text.setString(std::to_string(x) + "-" + std::to_string(y));
					text.setFillColor(sf::Color::White);
					target.draw(text, currTransform);
				}
#endif
				++y;
			}
			++x;
		}
	}

	target.draw(_cursor, parentTransform * getTransform());



}

bool FieldNode::InnerMouseDown(const sf::Vector2f& pos)
{
	if (_mouseBlocked)
	{
		return false;
	}
	// TODO: Debug
	_cursor.setPosition(pos);

	auto tempChipPos = GetSelectedChipPosByRenderPos(pos);
	if (_selectedChip == EMPTY_CHIP_POS)
	{
		if (DoesChipPosExistsInField(_field->GetChipsField(), tempChipPos))
		{
			_selectedChip = tempChipPos;
			_mousePressed = true;
		}
	}
	else
	{
		if (DoesChipPosNextToAnother(_selectedChip, tempChipPos))
		{
			StartSwiping(_selectedChip, CalcDirectionFromChipPoses(_selectedChip, tempChipPos));
		}
		else
		{
			_selectedChip = EMPTY_CHIP_POS;
		}
	}
	
	sf::FloatRect rect(getPosition(), sf::Vector2f(_width, _height));

	return rect.contains(pos);
}

void FieldNode::InnerMouseCancel()
{
	_mousePressed = true;
}

ChipPos FieldNode::GetSelectedChipPosByRenderPos(const sf::Vector2f& pos)
{
	auto selectedChipPos = ChipPos(-1, -1);
	selectedChipPos.x = pos.x / ChipDistance;
	selectedChipPos.y = (_height - pos.y + ChipDistance) / ChipDistance;
	return selectedChipPos;
}

void FieldNode::StartSwiping(const ChipPos& from, SwipeDirection dir)
{
	_mouseBlocked = true;
	_swipingAnimData.activated = true;
	_swipingAnimData.dir = dir;
}
