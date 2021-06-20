#pragma once

#include "Core/Node.hpp"

#include "Match3/Logic/Field.hpp"

namespace match3
{
	constexpr float ChipRadius = 28.f;
	constexpr float ChipDistance = 64.f;
	
	class FieldNode : public core::Node
	{
	public:
		FieldNode(std::shared_ptr<Field> field);

	protected:
		void InnerUpdate(float dt) override;
		
		bool InnerMouseDown(const sf::Vector2f& pos) override;
		void InnerMouseCancel() override;
		
		void InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform) override;

		ChipPos GetSelectedChipPosByRenderPos(const sf::Vector2f& pos);
		void StartSwiping(const ChipPos& from, SwipeDirection dir);
	private:
		
		bool _mouseBlocked = false;
		bool _mousePressed = false;
		float _width;
		float _height;
		
		sf::Font _font;
		sf::CircleShape _cursor;
		ChipPos _selectedChip;
		const ChipPos EMPTY_CHIP_POS = ChipPos(-1, 1);
		
		std::shared_ptr<Field> _field;
	};
}
