#pragma once

#include "Core.h"
#include <unordered_map>

namespace CStell
{
	class CSTELL_API AssetManager
	{
    public:
        AssetManager() = default;

        template<typename T>
        Ref<T> LoadAsset(const std::string& assetPath);

        // Retrieve an already loaded asset
        template<typename T>
        Ref<T> GetAsset(const std::string& assetPath);

        // Unload an asset
        void UnloadAsset(const std::string& assetPath);

        // Clear all loaded assets
        void ClearAssets();

    public:
        static void Init();
        static AssetManager* GetInstance() { return s_AssetManager; }

    private:
        static AssetManager* s_AssetManager;

        // Storage for loaded assets
        std::unordered_map<std::string, std::shared_ptr<void>> m_Assets;

        // Internal helper to load different types of assets
        template<typename T>
        std::shared_ptr<T> LoadAssetInternal(const std::string& assetPath);
	};

    template<typename T>
    inline Ref<T> AssetManager::LoadAsset(const std::string& assetPath)
    {
        auto it = m_Assets.find(assetPath);
        if (it != m_Assets.end())
            return std::static_pointer_cast<T>(it->second);

        Ref<T> asset = LoadAssetInternal<T>(assetPath);
        if (asset)
            m_Assets[assetPath] = asset;

        return asset;
    }

    template<typename T>
    inline Ref<T> AssetManager::GetAsset(const std::string& assetPath)
    {
        auto it = m_Assets.find(assetPath);
        if (it != m_Assets.end())
            return std::static_pointer_cast<T>(it->second);

        return nullptr;
    }

    template<typename T>
    inline std::shared_ptr<T> AssetManager::LoadAssetInternal(const std::string& assetPath)
    {
        Ref<T> asset = T::Create(assetPath);
        if (asset)
            return asset;

        return nullptr;
    }
}
