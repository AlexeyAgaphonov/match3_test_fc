#include "Balance.hpp"


#include <ctime>

using namespace match3;
using namespace match3::balance;

int balance::Random(int to)
{
	return std::rand() % to;
}

void balance::InitSeed()
{
	std::srand(std::time(nullptr));
}


Chip balance::GenerateNewChip()
{
	const auto color = static_cast<ChipType>(Random(static_cast<int>(ChipType::ColorSize)));
	return Chip(color, CF_Matchable | CF_Movable);
}

void balance::GenerateNewField(int width, int height, ChipsField& field)
{
	field.clear();
	field.reserve(width);
	for (int i = 0; i < width; ++i)
	{
		field.push_back(std::vector<Chip>());
		field[i].reserve(height);
		for (int j = 0; j < height; ++j)
		{
			field[i].emplace_back(GenerateNewChip());
		}
	}
}