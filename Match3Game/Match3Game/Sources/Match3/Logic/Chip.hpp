#pragma once
#include <cstdint>

namespace match3
{
	using ChipProperties = uint8_t;
	ChipProperties CF_Movable	= 0b00000001;
	ChipProperties CF_Matchable = 0b00000010;
	

	// Color or enother object
	enum class ChipType : uint8_t
	{
		Red,
		Yellow,
		Green,
		Blue,
		Purple,
	};
	
	class Chip
	{
	public:
		Chip() = delete;
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
