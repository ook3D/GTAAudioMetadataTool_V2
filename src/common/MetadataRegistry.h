#pragma once

#include "MetadataTypeDef.h"
#include <map>
#include <string>

namespace AMT
{
    void RegisterSoundsSchema(std::map<std::string, MetadataFileDef>& registry);
    void RegisterGameSchema(std::map<std::string, MetadataFileDef>& registry);
    void RegisterCategoriesSchema(std::map<std::string, MetadataFileDef>& registry);
    void RegisterEffectsSchema(std::map<std::string, MetadataFileDef>& registry);
    void RegisterCurvesSchema(std::map<std::string, MetadataFileDef>& registry);

    class MetadataRegistry
    {
        std::map<std::string, MetadataFileDef> m_Defs;

    public:
        void RegisterAll()
        {
            RegisterSoundsSchema(m_Defs);
            RegisterGameSchema(m_Defs);
            RegisterCategoriesSchema(m_Defs);
            RegisterEffectsSchema(m_Defs);
            RegisterCurvesSchema(m_Defs);
        }

        const MetadataFileDef* GetFileDef(const std::string& key) const
        {
            auto it = m_Defs.find(key);
            if (it != m_Defs.end())
                return &it->second;
            return nullptr;
        }
    };
} // namespace AMT
