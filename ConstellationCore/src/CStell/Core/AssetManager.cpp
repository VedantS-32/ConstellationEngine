#include "CStellpch.h"
#include "AssetManager.h"

namespace CStell
{
	AssetManager* AssetManager::s_AssetManager = nullptr;

	void AssetManager::UnloadAsset(const std::string& assetPath)
	{
		m_Assets.erase(assetPath);
	}

	// WARNING this will empty entire asset library
	void AssetManager::ClearAssets()
	{
		m_Assets.clear();
	}

	void AssetManager::Init()
	{
		s_AssetManager = new AssetManager;
	}
}
