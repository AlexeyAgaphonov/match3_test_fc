#pragma once

#include "Core/Node.hpp"
#include "Match3/Logic/Chip.hpp"

namespace match3
{
	class ChipNode : public core::Node
	{
	public:
		ChipNode() = delete;
		ChipNode(ChipType type, const std::string& name);
		virtual ~ChipNode();

	protected:
		void InnerUpdate(float dt) override;
		void InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform) override;
	private:
		bool _selected;
		ChipType _type;
	};
}
