#include "Field.hpp"

#include "Balance.hpp"
#include "FieldFunctions.hpp"

#include <array>
using namespace match3;

static const std::array<std::string, 4> arrDirStr{ "Up", "Left", "Right", "Down" };

std::string match3::ConvertSwipeDirectionToStr(SwipeDirection dir)
{
	std::string dirStr = "";
	if (dir > SwipeDirection::None && dir < SwipeDirection::Size)
	{
		dirStr = arrDirStr[static_cast<uint8_t>(dir) - 1];
	}
	return dirStr;
}

SwipeDirection match3::ConvertStrToSwipeDirection(const std::string& dirStr)
{
	SwipeDirection dir = SwipeDirection::None;
	int i = 0;
	for (const auto& tempDirStr: arrDirStr)
	{
		if (tempDirStr == dirStr)
		{
			dir = static_cast<SwipeDirection>(i + 1);
			break;
		}
		++i;
	}
	return dir;
}


Field::Field()
{
	
}

void Field::Init(int width, int height)
{
	balance::GenerateNewField(width, height, _chipsField);
}

bool Field::TryToSwipeChip(const ChipPos& chipPos, SwipeDirection dir)
{
	bool successfulSwipe = false;
	if (WillChipHaveMatchAfterSwipe(_chipsField, chipPos, dir))
	{
		auto chipPos2 = chipPos + SwipeDirectionConvertToOffset(dir);
		std::swap(_chipsField[chipPos.x][chipPos.y], _chipsField[chipPos2.x][chipPos2.y]);
		successfulSwipe = true;
	}
	return successfulSwipe;
}



