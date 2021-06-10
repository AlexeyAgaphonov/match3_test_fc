#include "Node.hpp"

#include <algorithm>

Core::Node::Node(const std::string& name)
	:
	_name(name)
{
	
}

Core::Node::Node(const std::string& name, const sf::Vector2f& pos)
	:
	_name(name), 
	_position(pos)
{
	
}

const std::string& Core::Node::GetName() const
{
	return _name;
}

Core::Node::~Node()
{
	
}

void Core::Node::PreRender()
{
	
}


void Core::Node::Render()
{
	
}

void Core::Node::AfterRender()
{

}

void Core::Node::Update(float dt)
{
	for (auto& childNode: _children)
	{
		childNode->Update(dt);
	}
}

void Core::Node::AddChild(Node::Ptr&& ptr)
{
	_children.push_back(std::move(ptr));
}

void Core::Node::RemoveChildByName(const std::string& name)
{
	auto it = std::remove_if(_children.begin(), _children.end(), [name](const Node::Ptr &node)->bool
	{
		return node->GetName() == name;
	});
	_children.erase(it, _children.end());
}
