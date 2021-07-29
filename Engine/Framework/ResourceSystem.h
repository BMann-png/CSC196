#pragma once
#include "Base/System.h"
#include <string>
#include <map>
#include <memory>
#include <algorithm>

namespace pbls
{
	class Resource
	{
	public:
		virtual bool Load(const std::string& filename) { return true; };
	};

	class ResourceSystem : public System
	{
	public:
		void Startup() override {};
		void Shutdown() override {};
		void Update(float dt) override {};

		template <typename T>
		std::shared_ptr<T> Get(const std::string& name);

	private:
		std::map<std::string, std::shared_ptr<Resource>> resources;
	};

	template<typename T>
	inline std::shared_ptr<T> ResourceSystem::Get(const std::string& name)
	{
		if (resources.find(name) != resources.end())
		{
			return std::dynamic_pointer_cast<T>(resources[name]);
		}
		else
		{
			std::shared_ptr resource = std::make_shared<T>(); //new shape
			resource->Load(name);

			resources[name] = resource;

			return resource;
		}
	}
}