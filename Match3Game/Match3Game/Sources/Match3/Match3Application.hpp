#pragma once

#include <Core/Application.hpp>
#include <Core/Node.hpp>

#include "Logic/Field.hpp"

namespace match3 {
	enum class FieldEvent;
}

class Match3Application : public core::Application
{
public:
	Match3Application();
	virtual ~Match3Application();
	core::Node* GetRootNode() { return _rootNode.get(); }

protected:
	void HandleUpdate(float dt) override;
	void HandleEvent(const sf::Event& event) override;
	void HandleDraw(sf::RenderTarget& target, sf::RenderStates states) override;
private:
	void NewField();
	void EventFromField(match3::FieldEvent event);

private:
	bool _recreateField = false;
	int _lastFieldSize = 10;

	core::Node::Ptr _rootNode;

	boost::intrusive_ptr<match3::Field> _field;
};
