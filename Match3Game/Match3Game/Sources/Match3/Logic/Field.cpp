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

void Field::MatchChips()
{
	for (auto& column: _chipsField)
	{
		ChipType prevChipType = ChipType::ColorSize;
		int chipAmount = 1;
		int posY = 0;
		for (auto& chip: column)
		{
			if (chip.GetType() != prevChipType)
			{
				if (chipAmount >= balance::AmountForMatch)
				{
					for (int i = 0; i < chipAmount; ++i)
					{
						column[i + posY - chipAmount].Destroy();
					}
				}
				prevChipType = chip.GetType();
				chipAmount = 1;
			}
			else
			{
				++chipAmount;
			}
			++posY;
		}
	}
}

void Field::RemoveDestroyedAndGen(std::vector<ChipPos>& wereDestroyed, std::vector<std::pair<ChipPos, Chip>>& newChips)
{
	wereDestroyed.clear();
	newChips.clear();
	
	int x = 0;
	for (auto& column : _chipsField)
	{
		int y = 0;
		const int columnSize = column.size();

		auto it_erase = std::remove_if(column.begin(), column.end(), [&wereDestroyed, &y, x](auto& chip)->bool {
				bool ret = false;
				if (chip.IsDestroyed())
				{
					wereDestroyed.push_back(ChipPos(x, y));
					ret = true;
				}
				++y;
				return ret;
			});
		
		column.erase(it_erase, column.end());

		while (column.size() < columnSize)
		{
			int curY = column.size();
			auto newChip = balance::GenerateNewChip();
			newChips.push_back({ChipPos(x, curY), newChip });
			column.push_back(std::move(newChip));
		}
		++x;
	}
}





