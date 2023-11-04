#pragma once
#include <map>
#include <string>

namespace pal3
{
	class ServiceLocator
	{
	public:
		~ServiceLocator();

		template<typename T>
		T* Register(T* service)
		{
			std::string className = typeid(T).name();
			auto it = _services.find(className);
			if (it == _services.end())
			{
				_services.insert(std::make_pair(className, static_cast<void*>(service)));
				return service;
			}
			return nullptr;
		}

		template<typename T>
		T* Get()
		{
			std::string className = typeid(T).name();
			auto it = _services.find(className);
			if (it != _services.end())
			{
				return static_cast<T*>(it->second);
			}
			return nullptr;
		}

	private:
		std::map<std::string, void*> _services;
	};
}
