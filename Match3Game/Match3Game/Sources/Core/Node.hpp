#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>
#include <string>

namespace Core
{
	class Node
	{
	public:
		using Ptr = std::unique_ptr<Node>;
		
		Node(const std::string &);
		Node(const std::string &, const sf::Vector2f &);
		virtual ~Node();

		virtual void Render();
		virtual void Update(float dt);

		const std::string& GetName() const;
		
		void AddChild(Node::Ptr &&);
		void RemoveChildByName(const std::string &);\
	private:
		void PreRender();
		void AfterRender();
	private:
		std::string _name;
		std::vector<Node::Ptr> _children;
		sf::Vector2f _position;
	};
}
