#pragma once

#include "Core/Node.hpp"

#include "Match3/Logic/Field.hpp"

namespace match3
{
	constexpr float ChipRadius = 28.f;
	constexpr float ChipDistance = 64.f;

	enum class FieldEvent
	{
		DoNotHaveSwipes,
	};
	using FieldEventCallback = std::function<void(FieldEvent)>;
	
	class FieldNode : public core::Node
	{
	public:
		static sf::Vector2f CalcPositionBySize(int width, int height);
		static sf::Vector2f CalcPositionByField(std::shared_ptr<Field> field);
		
		FieldNode(boost::intrusive_ptr<Field> field);

		void SubscribeOnEvents(FieldEventCallback cb);
	protected:
		void InnerUpdate(float dt) override;
		void UpdateFallingChips(float dt);
		void UpdateBlocker(float dt);
		void UpdateFieldChecker(float dt);
		
		bool InnerMouseDown(const sf::Vector2f& pos) override;
		void InnerMouseCancel() override;
		
		void InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform) override;

		ChipPos GetSelectedChipPosByRenderPos(const sf::Vector2f& pos);
		void StartSwiping(const ChipPos& from, SwipeDirection dir);

		bool IsBlocked() const { return _areChipsFalling  || _blockerTimer > 0.f;  }

		sf::Vector2f ConvertChipPosToPosition(const ChipPos& chipPos);

		void RemoveChipsFromField(const std::vector<ChipPos>& chipPoses);

		void MoveChips();

		void AddChipNode(ChipType type, const ChipPos& chipPos);

		void CheckFieldAfterTime(float time);

		void StartFallingChips();

		bool TryToSwipe(const ChipPos &chipPos, SwipeDirection dir);

		void EmitEvent(FieldEvent event);

		bool AreChipsFalling() const { return _areChipsFalling; }

	private:
		bool _areChipsFalling = false;
		float _blockerTimer = 0.f;
		
		bool _mousePressed = false;
		float _width;
		float _height;
		
		ChipPos _selectedChip;
		const ChipPos EMPTY_CHIP_POS = ChipPos(-1, 1);
		
		boost::intrusive_ptr<Field> _field;

		struct
		{
			bool activated = false;
			float timer = 0.f;
			float duration = 0.f;
		} _checkerMatchField;

		std::vector<std::vector<core::Node::Ptr>> _chipNodes;
		std::vector< FieldEventCallback> _subscribers;
	};
}
