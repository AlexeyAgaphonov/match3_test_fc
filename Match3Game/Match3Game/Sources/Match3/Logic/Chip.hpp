#pragma once
#include <cstdint>

namespace match3
{
	using ChipProperties = uint8_t;
	constexpr ChipProperties CF_Movable	= 0b00000001;
	constexpr ChipProperties CF_Matchable = 0b00000010;
	

	// Color or another type
	enum class ChipType : uint8_t
	{
		Red,
		Yellow,
		Green,
		Blue,
		Purple,
		ColorSize
	};
	
	class Chip
	{
	public:
		Chip() {}
		Chip(ChipType type, ChipProperties properties);

		const ChipProperties& GetProperties() const { return _properties;  }
		const ChipType& GetType() const { return _type; }
		bool IsMovable() const { return _properties & CF_Movable; }
		bool IsMatchable() const { return _properties & CF_Movable; }

	private:
		ChipType _type;
		ChipProperties _properties;
	};

	
}
