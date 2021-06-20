#pragma once
#include "Field.hpp"

namespace match3
{
	bool DoesChipPosExistsInField(const ChipsField& field, const ChipPos& pos);

	const ChipPos& SwipeDirectionConvertToOffset(SwipeDirection dir);

	bool CanChipBeSwipedByDirection(const ChipsField& field, const ChipPos& pos, SwipeDirection direction);

	const std::vector<SwipeDirection>& WhatDirectionCanChipBeSwiped(const ChipsField& field, const ChipPos& pos);

	bool DoesAnyMatchExist(const match3::ChipsField& field);

	int ToCountVerticalMatchedLine(const std::vector<Chip>& verticalLine, const ChipPos& pos, ChipType type);
	
	int ToCountHorizontalMatchedLine(const ChipsField& field, const ChipPos& pos);
	
	bool WillChipHaveMatchAfterSwipe(const ChipsField& field, const ChipPos& pos, SwipeDirection direction);

	bool DoesChipPosNextToAnother(const ChipPos& first, const ChipPos& second);

	SwipeDirection CalcDirectionFromChipPoses(const ChipPos& first, const ChipPos& second);

	SwipeDirection OppositeOfSwipeDirection(SwipeDirection dir);
}
