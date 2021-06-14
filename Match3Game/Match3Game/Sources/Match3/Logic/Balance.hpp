#pragma once

#include "Chip.hpp"
#include "Field.hpp"

namespace match3::balance
{
	void InitSeed();
	int Random(int to);
	Chip GenerateNewChip();
	void GenerateNewField(int width, int height, ChipsField& field);

}