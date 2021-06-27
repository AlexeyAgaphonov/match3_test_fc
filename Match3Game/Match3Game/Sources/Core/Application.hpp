#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include "Node.hpp"
#include "Match3/Logic/Field.hpp"
#include "Match3/Visual/FieldNode.hpp"
#include <functional>

namespace core
{
	constexpr int32_t FramesPerSecond = 60;
	constexpr int32_t TicksDelay = 1000 / FramesPerSecond;

	class Application
	{
	public:
		Application();

		void HandleUpdate();

		std::shared_ptr<match3::Field> GetFieldPtr();

		core::Node* GetRootNode() { return _rootNode.get();  }
	private:
		void NewField();
		void EventFromField(match3::FieldEvent event);
	private:
		bool _recreateField = false;
		int _lastFieldSize = 4;
		sf::RenderStates _renderStates;
		
		std::unique_ptr<Node> _rootNode;

		std::shared_ptr<match3::Field> _field;
		
		sf::RenderWindow _window;
	};
}
