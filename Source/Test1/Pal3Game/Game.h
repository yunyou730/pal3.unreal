#pragma once
#include "ServiceLocator.h"

namespace pal3
{
	class ServiceLocator;
	class Game
	{
	public:
		static Game* Instance();
		static void Release();
	public:
		~Game();
		
		template<typename T>
		T* GetService()
		{
			return _serviceLocator->Get<T>();
		}

	private:
		Game();
		void Initialize();

	private:
		static Game* sInstance;

		ServiceLocator* _serviceLocator = nullptr;
	};
}