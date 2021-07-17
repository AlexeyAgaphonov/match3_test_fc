#pragma once

#include <SFML/Graphics.hpp>
#include "Node.hpp"

namespace core
{
	constexpr int32_t FramesPerSecond = 60;
	constexpr int32_t TicksDelay = 1000 / FramesPerSecond;

	class Application
	{
	public:
		Application(const sf::VideoMode& videoMode, const std::string& appName);
		virtual ~Application(){}

		int RunCycle();

	protected:
		virtual void HandleUpdate(float dt) = 0;
		virtual void HandleEvent(const sf::Event& event) = 0;
		virtual void HandleDraw(sf::RenderTarget& target, sf::RenderStates states) = 0;
		void CloseWindow() { _window.close(); }
		
	private:
		sf::RenderStates _renderStates;

		sf::RenderWindow _window;
	};
}
