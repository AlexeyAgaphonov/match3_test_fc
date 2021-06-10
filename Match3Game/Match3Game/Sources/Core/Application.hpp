#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

namespace Core
{
	const constexpr int32_t FramesPerSecond = 60;
	const constexpr int32_t TicksDelay = 1000 / FramesPerSecond;

	class Application
	{
	public:
		Application();

		void HandleUpdate();

	private:
		sf::Texture texture1;
		sf::Sprite sprite;
		std::unique_ptr<sf::RenderWindow> _window;
	};
	
}
