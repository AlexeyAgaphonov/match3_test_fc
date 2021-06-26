#include "Sources/Core/Application.hpp"
#include "Sources/Core/Node.hpp"
#include "Sources/Match3/Visual/FieldNode.hpp"
#include "Core/Node.hpp"

int main()
{
	std::unique_ptr<core::Application> app;
	app.reset(new core::Application);
	auto field = app->GetFieldPtr();
	app->GetRootNode()->AddChild(core::Node::Ptr(new match3::FieldNode(field)));
	
	app->HandleUpdate();
	
	return 0;
}
