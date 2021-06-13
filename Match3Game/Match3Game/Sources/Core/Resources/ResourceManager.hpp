#pragma once
#include <string_view>
#include <any>
#include <array>
#include <map>
#include <memory>

namespace core::resources
{
	enum class Type : uint8_t
	{
		Texture,
		Size // Should be last one
	};
	
	class ResourceTypedGroup
	{
	public:
		std::map<std::string, std::string> storage;
	};
	
	class ResourceManager
	{
	public:
		ResourceManager();

		void LoadFromFile(std::string_view pathToFile);
	private:
		std::array<std::unique_ptr<ResourceTypedGroup>, static_cast<size_t>(Type::Size)> _resources;
	};
}

