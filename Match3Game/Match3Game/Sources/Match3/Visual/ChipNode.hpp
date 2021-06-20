#pragma once

#include "Core/Node.hpp"
#include "Match3/Logic/Chip.hpp"
#include "Match3/Logic/Field.hpp"

namespace match3
{
	class ChipNode : public core::Node
	{
	public:
		ChipNode() = delete;
		ChipNode(ChipType type, const std::string& name);
		virtual ~ChipNode();

		static std::string ConvertChipPosToName(const ChipPos& pos);
		virtual void AcceptMessage(const std::string& message, const std::string& data) override;
	protected:
		void InnerUpdate(float dt) override;
		void InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform) override;
	private:
		bool _selected;
		ChipType _type;

		struct SwipingAnimData
		{
			bool match = false;
			bool activated = false;
			const float duration = 0.5f;
			float timer = 0.f;
			SwipeDirection dir = SwipeDirection::None;
			sf::Vector2f offset;
		} _swipingAnimData;
	};
}
