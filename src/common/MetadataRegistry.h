#pragma once

#include "MetadataTypeDef.h"
#include <map>
#include <string>
#include <iostream>
#include <set>

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

        static int CheckList(const std::string& file, const std::string& typeName, const std::vector<FieldDef>& fields, bool isError)
        {
            int found = 0;
            std::set<std::string> seen;
            for (const auto& field : fields)
            {
                if (!field.name.empty() && !seen.insert(field.name).second)
                {
                    std::cerr << (isError ? "ERROR" : "WARNING") << ": duplicate field name in file '" << file
                              << "', type '" << typeName << "': '" << field.name << "'\n";
                    found++;
                }
                if (!field.children.empty())
                {
                    found += CheckList(file, typeName + "." + field.name, field.children, isError);
                }
            }
            return found;
        }

    public:
        void RegisterAll()
        {
            RegisterSoundsSchema(m_Defs);
            RegisterGameSchema(m_Defs);
            RegisterCategoriesSchema(m_Defs);
            RegisterEffectsSchema(m_Defs);
            RegisterCurvesSchema(m_Defs);
        }

        // Fields are keyed by name in JSON, so duplicate names inside one list lose data.
        // Returns the number of duplicates found; each one is printed to stderr.
        // Only the "game" schema is reported as an error, other schemas as warnings.
        int CheckDuplicateFieldNames(int& errorCount) const
        {
            int total = 0;
            errorCount = 0;
            for (const auto& [fileKey, fileDef] : m_Defs)
            {
                bool isError = (fileKey == "game");
                int found = CheckList(fileKey, "<header>", fileDef.headerFields, isError);
                for (const auto& [id, typeDef] : fileDef.types)
                {
                    found += CheckList(fileKey, typeDef.name, typeDef.fields, isError);
                }
                total += found;
                if (isError)
                {
                    errorCount += found;
                }
            }
            return total;
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
