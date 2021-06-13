#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include "Node.hpp"

namespace core
{
	constexpr int32_t FramesPerSecond = 60;
	constexpr int32_t TicksDelay = 1000 / FramesPerSecond;

	class Application
	{
	public:
		Application();

		void HandleUpdate();

	private:
		std::unique_ptr<Node> _rootNode;
		sf::Texture texture1;
		sf::Sprite sprite;
		std::unique_ptr<sf::RenderWindow> _window;
	};
}
