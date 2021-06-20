#include "Field.hpp"

#include "Balance.hpp"

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


