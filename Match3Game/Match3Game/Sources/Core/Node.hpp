#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>
#include <string>

namespace core
{
	namespace NodeState
	{
		const static uint8_t Updated = 0b00000001;
		const static uint8_t Drawable = 0b00000010;
	}
	
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
		void RemoveChildByName(const std::string &);

		inline bool IsUpdated() const { return _state & NodeState::Updated; }
		inline bool IsDrawable() const { return _state & NodeState::Drawable;  }
		inline bool IsTurnedOff() const { return _state & (NodeState::Updated | NodeState::Drawable); }
	private:
		void PreRender();
		void AfterRender();
	private:
		std::string _name;
		std::vector<Node::Ptr> _children;
		sf::Vector2f _position;

		uint8_t _state = 0;
	};
}
