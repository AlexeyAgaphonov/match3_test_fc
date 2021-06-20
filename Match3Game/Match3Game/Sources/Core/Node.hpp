#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>
#include <string>

namespace core
{
	namespace NodeState
	{
		const static uint8_t Updated =		0b00000001;
		const static uint8_t Drawable =		0b00000010;
		const static uint8_t Clickable =	0b00000100;
	}
	
	class Node : public sf::Transformable
	{
	public:
		
		using Ptr = std::unique_ptr<Node>;
		
		Node(const std::string &);
		Node(const std::string &, const sf::Vector2f &);
		virtual ~Node();

		void Draw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform);
		void Update(float dt);

		const std::string& GetName() const;
		
		void AddChild(Node::Ptr &&);
		void RemoveChildByName(const std::string &);

		bool MouseDown(const sf::Vector2f& pos);
		bool MouseUp(const sf::Vector2f& pos);
		void MouseMove(const sf::Vector2f& pos);
		void MouseCancel();

		inline bool IsUpdated() const { return _state & NodeState::Updated; }
		inline bool IsDrawable() const { return _state & NodeState::Drawable;  }
		inline bool IsClickable() const { return _state & NodeState::Clickable;  }
		
		inline bool IsActive() const { return _state & (NodeState::Updated | NodeState::Drawable); }

		virtual void AcceptMessage(const std::string& message, const std::string& data) {}
	protected:
		virtual bool InnerMouseDown(const sf::Vector2f& pos) { return false; }
		virtual bool InnerMouseUp(const sf::Vector2f& pos) { return false; }
		virtual void InnerMouseMove(const sf::Vector2f& pos) {}
		virtual void InnerMouseCancel() {}
		
		virtual void InnerDraw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform) { }
		virtual void InnerUpdate(float dt) {};
		
		const sf::Vector2f& TransformPoint(const sf::Vector2f& pos);

		void SendMessageToChild(const std::string& childName, const std::string& msg, const std::string& data = "");
	private:
		std::string _name;
		std::vector<Node::Ptr> _children;

		uint8_t _state = 0;
	};
}
