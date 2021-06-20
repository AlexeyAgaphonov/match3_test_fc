#include "FieldFunctions.hpp"

#include <array>

using namespace match3;

static std::array<ChipPos, 5> StaticCache = { ChipPos(0, 0), ChipPos(0, 1), ChipPos(-1, 0), ChipPos(1, 0), ChipPos(0, -1) };
static std::array<SwipeDirection, 4> DirectionsCache = { SwipeDirection::Up, SwipeDirection::Left, SwipeDirection::Right, SwipeDirection::Down };


bool match3::DoesChipPosExistsInField(const ChipsField& field, const ChipPos& pos)
{
	return pos.x < field.size() && pos.y < field[pos.x].size();
}


const ChipPos& match3::SwipeDirectionConvertToOffset(SwipeDirection dir)
{
	// without "if-else", because branch-prediction
	// interface of "enum class" gives us a some protection for "out from range"
	return StaticCache[static_cast<uint8_t>(dir)];

}


bool match3::CanChipBeSwipedByDirection(const ChipsField& field, const ChipPos& pos, SwipeDirection direction)
{
	bool canSwipeChip = false;
	if (DoesChipPosExistsInField(field, pos) && field[pos.x][pos.y].IsMovable())
	{
		auto swipedPos = pos + SwipeDirectionConvertToOffset(direction);
		if (DoesChipPosExistsInField(field, swipedPos) && field[swipedPos.x][swipedPos.y].IsMovable())
		{
			canSwipeChip = true;
		}
	}
	return canSwipeChip;
}


const std::vector<SwipeDirection>& match3::WhatDirectionCanChipBeSwiped(const ChipsField& field, const ChipPos& pos)
{
	std::vector<SwipeDirection> vecOfDis;
	for (const auto dir : DirectionsCache)
	{
		if (CanChipBeSwipedByDirection(field, pos, dir))
		{
			vecOfDis.emplace_back(dir);
		}
	}
	return vecOfDis;
}


bool match3::DoesAnyMatchExist(const ChipsField& field)
{
	for (int i_x = 0; i_x < field.size(); ++i_x)
	{
		for (int i_y = 0; i_y < field[i_x].size(); ++i_y)
		{
			if (WhatDirectionCanChipBeSwiped(field, ChipPos(i_x, i_y)).size() > 0)
			{
				return true;
			}
		}
	}
	return false;
}

int match3::ToCountVerticalMatchedLine(const std::vector<Chip>& verticalLine, const ChipPos& pos, ChipType type)
{
	const auto height = verticalLine.size();
	auto checkRanges = [height](int pos)->bool { return pos > 0 && pos < height; };
	bool shouldCheckUpper = true;
	bool shouldCheckBottom = true;
	int howMuchChipsInMatch = 1;
	
	for (size_t i = 1; i < height / 2; ++i)
	{
		if (int newPosY = pos.y + i; shouldCheckUpper 
			&& checkRanges(newPosY) 
			&& verticalLine[newPosY].GetType() == type)
		{
			++howMuchChipsInMatch;
		}
		else
		{
			shouldCheckUpper = false;
		}
		if (int newPosY = pos.y - i; shouldCheckBottom && checkRanges(newPosY) && verticalLine[newPosY].GetType() == type)
		{
			++howMuchChipsInMatch;
		}
		else
		{
			shouldCheckBottom = false;
		}
	}
	return howMuchChipsInMatch;
}


int match3::ToCountHorizontalMatchedLine(const ChipsField& field, const ChipPos& pos)
{
	return 0;
}


bool match3::WillChipHaveMatchAfterSwipe(const ChipsField& field, const ChipPos& pos, SwipeDirection direction)
{
	if (!DoesChipPosExistsInField(field, pos) || !field[pos.x][pos.y].IsMatchable())
	{
		return false;
	}

	bool willHave = false;
	const auto newPos = pos + SwipeDirectionConvertToOffset(direction);
	if (CanChipBeSwipedByDirection(field, pos, direction))
	{
		const auto checkedTpe = field[pos.x][pos.y].GetType();
		if (ToCountVerticalMatchedLine(field[newPos.x], pos, field[pos.x][pos.y].GetType()) >= 3
			|| ToCountHorizontalMatchedLine(field, pos + SwipeDirectionConvertToOffset(direction)) >= 3)
		{
			willHave = true;
		}
	}
	return willHave;
}

bool match3::DoesChipPosNextToAnother(const ChipPos& first, const ChipPos& second)
{
	return ((first.x == second.x - 1 || first.x == second.x + 1) && first.y == second.y)
		|| ((first.y == second.y - 1 || first.y == second.y + 1) && first.x == second.x);
}

SwipeDirection match3::CalcDirectionFromChipPoses(const ChipPos& first, const ChipPos& second)
{
	SwipeDirection dir = SwipeDirection::None;

	if (first.x == second.x)
	{
		if (first.y + 1 == second.y)
		{
			dir = SwipeDirection::Up;
		}
		else if (first.y - 1 == second.y)
		{
			dir = SwipeDirection::Down;
		}
	}
	else if (first.y == second.y)
	{
		if (first.x + 1 == second.x)
		{
			dir = SwipeDirection::Right;
		}
		else if (first.x - 1 == second.x)
		{
			dir = SwipeDirection::Left;
		}
	}

	return dir;
}
