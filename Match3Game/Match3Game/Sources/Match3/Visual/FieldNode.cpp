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
	const float FALLING_SPEED = 500.f;
	if (_chipsAreFalling)
	{
		_chipsAreFalling = false;
		for (auto& column: _chipNodes)
		{
			int indexY = 0;
			for (auto& chipNode: column)
			{
				auto currPos = chipNode->getPosition();
				const auto needPosY = ConvertChipPosToPosition({ 0, indexY }).y;
				if (currPos.y < needPosY)
				{
					currPos.y += FALLING_SPEED * dt;
					_chipsAreFalling = true;
				}
				else
				{
					currPos.y = needPosY;
				}
				chipNode->setPosition(currPos);
				
				++indexY;
			}
		}
		if (!_chipsAreFalling)
		{
			MoveChips();
		}
	}
	else
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

					std::vector<int> howManyChipsNeeds;
					howManyChipsNeeds.reserve(_chipNodes.size());
					for (auto i = 0; i < _chipNodes.size(); ++i)
					{
						howManyChipsNeeds.push_back(_field->GetChipsField()[i].size() - _chipNodes[i].size());
					}
					for (auto& newChipsPair : newChips)
					{
						const auto& pos = newChipsPair.first;
						const auto& chip = newChipsPair.second;
						AddChipNode(chip.GetType(), pos + ChipPos(0, howManyChipsNeeds[pos.x]));
					}

					StartFallingChips();
					
					if (!wereDestroyed.empty())
					{
						CheckFieldAfterTime(ChipSwipeTime);
					}
					else
					{
						// Perhaps should it be in another thread?
						if (!HasFieldSwipes(_field->GetChipsField()))
						{
							std::cout << "GAME OVER" << std::endl;
						}
					}
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
		if (TryToSwipe(_selectedChip, dir))
		{
			// SUCCESSFUL SWIPE
		}
		else
		{
			_blockerTimer += ChipSwipeTime;
			auto& selectedChip = _chipNodes[_selectedChip.x][_selectedChip.y];
			selectedChip->AcceptMessage("SwipeAnimBadly", ConvertSwipeDirectionToStr(dir));
		}
	}
}

bool FieldNode::TryToSwipe(const ChipPos& chipPos, SwipeDirection dir)
{
	if (_field->TryToSwipeChip(chipPos, dir))
	{
		const auto chipPos2 = chipPos + SwipeDirectionConvertToOffset(dir);
		const auto selectedPosName = ChipNode::ConvertChipPosToName(chipPos);
		const auto newPosName = ChipNode::ConvertChipPosToName(chipPos2);

		auto selectedChip = _chipNodes[chipPos.x][chipPos.y];
		auto secondChip = _chipNodes[chipPos2.x][chipPos2.y];

		selectedChip->AcceptMessage("SwipeAnim", ConvertSwipeDirectionToStr(dir));
		secondChip->AcceptMessage("SwipeAnim", ConvertSwipeDirectionToStr(OppositeOfSwipeDirection(dir)));

		selectedChip->AcceptMessage("ChangeName", newPosName);
		secondChip->AcceptMessage("ChangeName", selectedPosName);

		CheckFieldAfterTime(ChipSwipeTime);

		selectedChip->AcceptMessage("Unselect", "");
		secondChip->AcceptMessage("Unselect", "");

		_chipNodes[chipPos.x][chipPos.y] = secondChip;
		_chipNodes[chipPos2.x][chipPos2.y] = selectedChip;
		return true;
	}
	return false;
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

void FieldNode::StartFallingChips()
{
	_chipsAreFalling = true;
}
