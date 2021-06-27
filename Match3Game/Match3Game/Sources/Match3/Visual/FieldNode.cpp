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
		_chipNodes.push_back({});
		int y = 0;
		for (const auto& chip : horizontalLine)
		{
			AddChipNode(chip.GetType(), {x, y});
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
				RemoveChipsFromField(wereDestroyed);
				MoveChips();
				for (auto& newChipsPair : newChips)
				{
					const auto& pos = newChipsPair.first;
					const auto& chip = newChipsPair.second;
					AddChipNode(chip.GetType(), pos);
				}

				if (!wereDestroyed.empty())
				{
					CheckFieldAfterTime(ChipSwipeTime);
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
			_chipNodes[_selectedChip.x][_selectedChip.y]->AcceptMessage("Select", "");
		}
	}
	else
	{
		if (DoesChipPosNextToAnother(_selectedChip, tempChipPos))
		{
			StartSwiping(_selectedChip, CalcDirectionFromChipPoses(_selectedChip, tempChipPos));
			_chipNodes[_selectedChip.x][_selectedChip.y]->AcceptMessage("Unselect", "");
			_selectedChip = EMPTY_CHIP_POS;
		}
		else
		{
			_chipNodes[_selectedChip.x][_selectedChip.y]->AcceptMessage("Unselect", "");
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
		if (_field->TryToSwipeChip(_selectedChip, dir))
		{
			const auto newPos = _selectedChip + SwipeDirectionConvertToOffset(dir);
			const auto selectedPosName = ChipNode::ConvertChipPosToName(_selectedChip);
			const auto newPosName = ChipNode::ConvertChipPosToName(newPos);

			auto& selectedChip = _chipNodes[_selectedChip.x][_selectedChip.y];
			auto& secondChip = _chipNodes[newPos.x][newPos.y];

			selectedChip->AcceptMessage("SwipeAnim", ConvertSwipeDirectionToStr(dir));
			secondChip->AcceptMessage("SwipeAnim", ConvertSwipeDirectionToStr(OppositeOfSwipeDirection(dir)));

			selectedChip->AcceptMessage("ChangeName", newPosName);
			secondChip->AcceptMessage("ChangeName", selectedPosName);

			CheckFieldAfterTime(ChipSwipeTime);
			
			_chipNodes[_selectedChip.x][_selectedChip.y] = secondChip;
			_chipNodes[newPos.x][newPos.y] = selectedChip;
		}
		else
		{
			_blockerTimer += ChipSwipeTime;
			auto& selectedChip = _chipNodes[_selectedChip.x][_selectedChip.y];
			selectedChip->AcceptMessage("SwipeAnimBadly", ConvertSwipeDirectionToStr(dir));
		}
	}
}


sf::Vector2f FieldNode::ConvertChipPosToPosition(const ChipPos& chipPos)
{
	return { chipPos.x * ChipDistance, _height - chipPos.y * ChipDistance };
}

void FieldNode::RemoveChipsFromField(const std::vector<ChipPos>& chipPoses)
{
	for (auto& chipPos : chipPoses)
	{
		RemoveChildByName(ChipNode::ConvertChipPosToName(chipPos));
	}
	int x = 0;
	for (auto& column : _chipNodes)
	{
		int y = 0;
		auto it = std::remove_if(column.begin(), column.end(), [&chipPoses, &y, x](auto& chipNode) -> bool
			{
				bool result = std::find(chipPoses.begin(), chipPoses.end(), ChipPos(x, y)) != chipPoses.end();
				++y;
				return result;
			});
		column.erase(it, column.end());
		++x;
	}
}

void FieldNode::MoveChips()
{
	int x = 0;
	for (auto& column: _chipNodes)
	{
		int y = 0;
		for (auto& chipNode: column)
		{
			chipNode->AcceptMessage("ChangeName", ChipNode::ConvertChipPosToName({ x, y }));
			chipNode->setPosition(ConvertChipPosToPosition({x, y}));
			++y;
		}
		++x;
	}
}

void FieldNode::AddChipNode(ChipType type, const ChipPos& chipPos)
{
	Node::Ptr chipNode = new ChipNode(type, ChipNode::ConvertChipPosToName(chipPos));
	chipNode->setPosition(ConvertChipPosToPosition(chipPos));
	_chipNodes[chipPos.x].push_back(chipNode);
	AddChild(std::move(chipNode));
}

void FieldNode::CheckFieldAfterTime(float time)
{
	_blockerTimer += time;

	_checkerMatchField.activated = true;
	_checkerMatchField.timer = 0.f;
	_checkerMatchField.duration = time;
}
