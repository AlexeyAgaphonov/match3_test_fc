#pragma once
#include "Field.hpp"

namespace match3
{
	bool DoesChipPosExistsInField(const ChipsField& field, const ChipPos& pos);

	const ChipPos& SwipeDirectionConvertToOffset(SwipeDirection dir);

	bool CanChipBeSwipedByDirection(const ChipsField& field, const ChipPos& pos, SwipeDirection direction);

	const std::vector<SwipeDirection>& WhatDirectionCanChipBeSwiped(const ChipsField& field, const ChipPos& pos);

	bool DoesAnyMatchExist(const match3::ChipsField& field);
}
