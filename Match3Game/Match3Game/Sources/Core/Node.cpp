#include "Node.hpp"

#include <algorithm>

#include "Render/Render.hpp"

using namespace core;

Node::Node(const std::string& name)
	:
	_name(name)
{
	
}

Node::Node(const std::string& name, const sf::Vector2f& pos)
	:
	_name(name), 
	_position(pos)
{
	
}

const std::string& Node::GetName() const
{
	return _name;
}

Node::~Node()
{
	
}

void Node::PreRender()
{
	if (IsDrawable())
	{
		render::PushVector(_position);
	}
}


void Node::Render()
{
	for (const auto& childNode: _children)
	{
		childNode->Render();
	}
}

void Node::AfterRender()
{
	if (IsDrawable())
	{
		render::PopVector();
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
