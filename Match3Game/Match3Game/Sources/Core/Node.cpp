#include "Node.hpp"

#include <algorithm>

#include "Render/Render.hpp"

using namespace core;

Node::Node(const std::string& name)
	: _name(name)
{
	_state = NodeState::Updated | NodeState::Drawable | NodeState::Clickable;
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
	if (IsUpdated())
	{
		InnerUpdate(dt);

		for (auto& childNode : _children)
		{
			if (childNode->IsUpdated())
			{
				childNode->Update(dt);
			}
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

bool Node::MouseDown(const sf::Vector2f& pos)
{
	if (!IsActive())
	{
		return false;
	}

	auto newPos = TransformPoint(pos);
	
	InnerMouseDown(newPos);

	for (auto& childNode : _children)
	{
		if (childNode->IsClickable())
		{
			childNode->MouseDown(newPos);
		}
	}
}

void Node::MouseMove(const sf::Vector2f& pos)
{
	if (!IsActive())
	{
		return;
	}

	auto newPos = TransformPoint(pos);
	
	InnerMouseMove(newPos);

	for (auto& childNode : _children)
	{
		if (childNode->IsClickable())
		{
			childNode->MouseMove(newPos);
		}
	}
}

bool Node::MouseUp(const sf::Vector2f& pos)
{
	if (!IsActive())
	{
		return false;
	}

	auto newPos = TransformPoint(pos);
	
	InnerMouseUp(newPos);

	for (auto& childNode : _children)
	{
		if (childNode->IsClickable())
		{
			childNode->MouseUp(newPos);
		}
	}
}

void Node::MouseCancel()
{
	if (!IsActive())
	{
		return;
	}

	InnerMouseCancel();
	for (auto& childNode : _children)
	{
		if (childNode->IsClickable())
		{
			childNode->MouseCancel();
		}
	}
}

const sf::Vector2f& Node::TransformPoint(const sf::Vector2f& pos)
{
	// TODO
	constexpr float MagicMultiplier = 1.0f;
	return pos - getPosition() * MagicMultiplier;
}

void Node::SendMessageToChild(const std::string& childName, const std::string& msg, const std::string& data)
{
	for (auto& child: _children)
	{
		if (child && child->GetName() == childName)
		{
			child->AcceptMessage(msg, data);
		}
	}
}

void Node::AcceptMessage(const std::string& message, const std::string& data)
{
	if (message == "ChangeName")
	{
		_name = data;
	}
}

