#pragma once

#include "Core/Node.hpp"

#include "Match3/Logic/Field.hpp"
#include "Match3/Logic/Chip.hpp"

namespace match3
{
	constexpr float ChipRadius = 32.f;
	
	class FieldNode : public core::Node
	{
	public:
		FieldNode(std::weak_ptr<Field>&& field);

		void Render() override;
	private:
		std::weak_ptr<Field> _field;
		std::map<ChipType, sf::CircleShape> _chipDrawers;
	};
}
