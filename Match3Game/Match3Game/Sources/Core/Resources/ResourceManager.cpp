#include "ResourceManager.hpp"


using namespace core::resources;

ResourceManager::ResourceManager()
{
	for (size_t i = 0; i < static_cast<size_t>(Type::Size); ++i)
	{
		_resources[i].reset(new ResourceTypedGroup());
	}
}

void ResourceManager::LoadFromFile(std::string_view pathToFile)
{
	
}
