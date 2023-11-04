#include "Game.h"
#include "ServiceLocator.h"

#include "Pal3Core/headers/CpkFileSystem.h"
#include "Pal3Game/Settings.h"
#include "Pal3Game/RawAssetCache.h"
#include "Pal3Game/RenderUtils.h"

namespace pal3
{
	Game* Game::sInstance = nullptr;

	Game::Game()
	{
		_serviceLocator = new ServiceLocator();
	}

	Game::~Game()
	{
		delete _serviceLocator;
		_serviceLocator = nullptr;
	}

	Game* Game::Instance()
	{
		if (sInstance == nullptr)
		{
			sInstance = new Game();
			sInstance->Initialize();
		}
		return sInstance;
	}

	void Game::Initialize()
	{
		auto settings = _serviceLocator->Register<pal3::Settings>(new pal3::Settings());
		settings->Initialize();

		auto cpkFileSystem = _serviceLocator->Register<pal3::CpkFileSystem>(new pal3::CpkFileSystem());
		cpkFileSystem->Mount(settings->GetResourceRoot() + "basedata.cpk");
		
		auto rawAssetCache = _serviceLocator->Register<pal3::RawAssetCache>(new pal3::RawAssetCache());
	}

	void Game::Release()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}
}
