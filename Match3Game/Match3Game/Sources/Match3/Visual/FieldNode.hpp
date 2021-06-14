#pragma once

#include "Core/Node.hpp"

#include "Match3/Logic/Field.hpp"

namespace match3
{
	constexpr float ChipRadius = 32.f;
	constexpr float ChipDistance = ChipRadius * 2.0f + ChipRadius * 0.1f;
	
	class FieldNode : public core::Node
	{
	public:
		FieldNode(std::shared_ptr<Field> field);

		void Update(float dt) override;
	protected:
		
		void InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform) override;
	private:
		float _width;
		float _height;
		
		sf::Font _font;
		
		std::shared_ptr<Field> _field;
		std::vector<sf::CircleShape> _chipDrawers;
	};
}
