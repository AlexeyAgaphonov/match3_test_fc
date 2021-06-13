#pragma once
#include <vector>


#include "Chip.hpp"

namespace match3
{
	using ChipsField = std::vector<std::vector<Chip>>;
}


namespace match3
{
	class Field
	{
	public:
		Field();
		void Init(int width, int height);
		 
	private:
		ChipsField _chipsField;
	};
	
}
