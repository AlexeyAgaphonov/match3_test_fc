#include "Render.hpp"

#include <stack>

static std::stack<sf::Vector2f> GlobalVectorRenderStack = std::stack<sf::Vector2f>();

void core::render::PushVector(const sf::Vector2f &vec)
{
	GlobalVectorRenderStack.push(vec);
}

void core::render::PopVector()
{
	GlobalVectorRenderStack.pop();
}
