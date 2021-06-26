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
			chipNode->setPosition(ConvertChipPosToPosition({x, y}));
			AddChild(std::move(chipNode));
			++y;
		}
		++x;
	}
	
}

void FieldNode::InnerUpdate(float dt)
{
	if (_blockerTimer > 0.f)
	{
		_blockerTimer -= dt;
		if (_blockerTimer < 0.f)
		{
			_blockerTimer = 0.f;
		}
	}

	if (_checkerMatchField.activated)
	{
		_checkerMatchField.timer += dt;
		if (_checkerMatchField.timer > _checkerMatchField.duration)
		{
			_checkerMatchField.timer = _checkerMatchField.duration;
			_checkerMatchField.activated = false;

			if (_field)
			{
				std::vector<ChipPos> wereDestroyed;
				std::vector<std::pair<ChipPos, Chip>> newChips;
				_field->MatchChips();
				_field->RemoveDestroyedAndGen(wereDestroyed, newChips);
				for (auto& chipPos: wereDestroyed)
				{
					RemoveChildByName(ChipNode::ConvertChipPosToName(chipPos));
				}
			}
		}
	}
}



void FieldNode::InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform)
{

}

bool FieldNode::InnerMouseDown(const sf::Vector2f& pos)
{
	if (IsBlocked())
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
			SendMessageToChild(ChipNode::ConvertChipPosToName(_selectedChip), "Select");
		}
	}
	else
	{
		if (DoesChipPosNextToAnother(_selectedChip, tempChipPos))
		{
			StartSwiping(_selectedChip, CalcDirectionFromChipPoses(_selectedChip, tempChipPos));
			SendMessageToChild(ChipNode::ConvertChipPosToName(_selectedChip), "Unselect");
			_selectedChip = EMPTY_CHIP_POS;
		}
		else
		{
			SendMessageToChild(ChipNode::ConvertChipPosToName(_selectedChip), "Unselect");
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
	if (_field)
	{
		_blockerTimer += ChipSwipeTime;
		if (_field->TryToSwipeChip(_selectedChip, dir))
		{
			const auto newPos = _selectedChip + SwipeDirectionConvertToOffset(dir);
			const auto selectedPosName = ChipNode::ConvertChipPosToName(_selectedChip);
			const auto newPosName = ChipNode::ConvertChipPosToName(newPos);
			
			SendMessageToChild(selectedPosName, "SwipeAnim", ConvertSwipeDirectionToStr(dir));
			SendMessageToChild(newPosName, "SwipeAnim", ConvertSwipeDirectionToStr(OppositeOfSwipeDirection(dir)));

			SendMessageToChild(selectedPosName, "ChangeName", "tempName");
			SendMessageToChild(newPosName, "ChangeName", selectedPosName);
			SendMessageToChild("tempName", "ChangeName", newPosName);
			
			
			_checkerMatchField.activated = true;
			_checkerMatchField.duration = ChipSwipeTime;
		}
		else
		{
			SendMessageToChild(ChipNode::ConvertChipPosToName(_selectedChip), "SwipeAnimBadly", ConvertSwipeDirectionToStr(dir));
		}
	}
}


sf::Vector2f FieldNode::ConvertChipPosToPosition(const ChipPos& chipPos)
{
	return { chipPos.x * ChipDistance, _height - chipPos.y * ChipDistance };
}