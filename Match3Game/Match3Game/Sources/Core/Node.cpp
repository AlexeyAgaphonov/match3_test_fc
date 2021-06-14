#include "Node.hpp"

#include <algorithm>

#include "Render/Render.hpp"

using namespace core;

Node::Node(const std::string& name)
	: _name(name)
{
	_state = NodeState::Updated | NodeState::Drawable;
}

Node::Node(const std::string& name, const sf::Vector2f& pos)
	: _name(name)
{
	_state = NodeState::Updated & NodeState::Drawable;
	setPosition(pos.x, pos.y);
}

const std::string& Node::GetName() const
{
	return _name;
}

Node::~Node()
{
	
}

void Node::Draw(sf::RenderTarget& target, sf::RenderStates states, sf::Transform parentTransform)
{
	if (IsDrawable())
	{
		const auto currTransform = parentTransform * getTransform();
		InnerDraw(target, states, currTransform);
		for (auto& childNode: _children)
		{
			childNode->Draw(target, states, currTransform);
		}
	}
}


void Node::Update(float dt)
{
	for (auto& childNode: _children)
	{
		if (childNode->IsUpdated())
		{
			childNode->Update(dt);
		}
	}
}

void Node::AddChild(Node::Ptr&& ptr)
{
	_children.push_back(std::move(ptr));
}

void Node::RemoveChildByName(const std::string& name)
{
	auto it = std::remove_if(_children.begin(), _children.end(), [name](const Node::Ptr &node)->bool
	{
		return node->GetName() == name;
	});
	_children.erase(it, _children.end());
}
