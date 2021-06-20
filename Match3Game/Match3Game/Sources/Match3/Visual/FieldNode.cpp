#include "FieldNode.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace match3;

#include "Match3/Logic/FieldFunctions.hpp"
#include "ChipNode.hpp"

FieldNode::FieldNode(std::shared_ptr<Field> field)
	: Node("FieldNode"), _field(field)
	, _cursor(5)
{

	_width = field->GetChipsField().size() * ChipDistance;
	_height = field->GetChipsField()[0].size() * ChipDistance;

	sf::Vector2f pos((1024.f - _width) / 2, 768 / 2 - _height / 2);
	setPosition(pos);
	auto scale = getScale();

	_cursor.setFillColor(sf::Color::White);
	_cursor.setOutlineColor(sf::Color::Black);

	_selectedChip = EMPTY_CHIP_POS;

	auto& chipsField = _field->GetChipsField();
	int x = 0;
	for (const auto& horizontalLine : chipsField)
	{
		int y = 0;
		for (const auto& chip : horizontalLine)
		{
			auto chipNode = std::make_unique<ChipNode>(chip.GetType(), ChipNode::ConvertChipPosToName(ChipPos(x, y)));
			chipNode->setPosition(x * ChipDistance, y * ChipDistance);
			AddChild(std::move(chipNode));
			++y;
		}
		++x;
	}
	
}

void FieldNode::InnerUpdate(float dt)
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
		sf::Vector2f vec;
		if (_swipingAnimData.match)
		{
			vec = sf::Vector2f(SwipeDirectionConvertToOffset(_swipingAnimData.dir).x,
				SwipeDirectionConvertToOffset(_swipingAnimData.dir).y);
			vec *= ChipDistance * 1.0f;
			vec *= static_cast<float>(sin((_swipingAnimData.timer / _swipingAnimData.duration) * M_PI_2));
		}
		else
		{
			vec = sf::Vector2f(SwipeDirectionConvertToOffset(_swipingAnimData.dir).x,
				SwipeDirectionConvertToOffset(_swipingAnimData.dir).y);
			vec *= ChipDistance * 0.5f;
			vec *= static_cast<float>(sin((_swipingAnimData.timer / _swipingAnimData.duration) * M_PI));
		}
		
		_swipingAnimData.offset = vec;
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
	selectedChipPos.y = (_height - pos.y) / ChipDistance;
	return selectedChipPos;
}

void FieldNode::StartSwiping(const ChipPos& from, SwipeDirection dir)
{
	_mouseBlocked = true;
	_swipingAnimData.activated = true;
	_swipingAnimData.dir = dir;
	_swipingAnimData.match = WillChipHaveMatchAfterSwipe(_field->GetChipsField(), from, dir);
}
