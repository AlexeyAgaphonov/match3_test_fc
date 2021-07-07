#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include "Node.hpp"
#include "Match3/Logic/Field.hpp"
#include "Match3/Visual/FieldNode.hpp"
#include <functional>
#include <boost/intrusive_ptr.hpp>

namespace core
{
	constexpr int32_t FramesPerSecond = 60;
	constexpr int32_t TicksDelay = 1000 / FramesPerSecond;

	class Application
	{
	public:
		Application();

		void HandleUpdate();

		core::Node* GetRootNode() { return _rootNode.get();  }
	private:
		void NewField();
		void EventFromField(match3::FieldEvent event);
	private:
		bool _recreateField = false;
		int _lastFieldSize = 10;
		sf::RenderStates _renderStates;
		
		Node::Ptr _rootNode;

		boost::intrusive_ptr<match3::Field> _field;
		
		sf::RenderWindow _window;
	};
}
