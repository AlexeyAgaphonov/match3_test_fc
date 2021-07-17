#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Core/RefCounter.hpp"
#include "Chip.hpp"

namespace match3
{
	using ChipsField = std::vector<std::vector<Chip>>;
	using ChipPos = sf::Vector2i;
}


namespace match3
{
	enum class SwipeDirection : uint8_t
	{
		None	= 0,
		Up		= 1,
		Left	= 2,
		Right	= 3,
		Down	= 4,
		Size	= 5
	};

	std::string ConvertSwipeDirectionToStr(SwipeDirection dir);
	SwipeDirection ConvertStrToSwipeDirection(const std::string& dirStr);

	class Field : public core::RefCounter
	{
	public:
		Field();
		void Init(int width, int height);

		const ChipsField& GetChipsField() const { return _chipsField; }
		bool TryToSwipeChip(const ChipPos& chipPos, SwipeDirection dir);
		void MatchAndMarkChips();
		void RemoveMarkedChipsdAndGen(std::vector<ChipPos> &wereDestroyed, std::vector<std::pair<ChipPos, Chip>>& newChips);
	private:
		void MatchLine(std::vector<Chip*>& line);
	private:
		ChipsField _chipsField;
	};
	
}
