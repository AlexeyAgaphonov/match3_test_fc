#include "Field.hpp"

#include "Balance.hpp"

using namespace match3;




Field::Field()
{
	
}

void Field::Init(int width, int height)
{
	_chipsField = balance::GenerateNewField(width, height);
}


