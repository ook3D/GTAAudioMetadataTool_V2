#pragma once

#include "FieldDef.h"
#include <map>
#include <string>
#include <vector>

namespace AMT
{
    struct MetadataTypeDef
    {
        int id;
        std::string name;
        std::vector<FieldDef> fields;
    };

    struct MetadataFileDef
    {
        uint32_t suffix;
        bool hasNameOffset;
        std::vector<FieldDef> headerFields;
        std::map<int, MetadataTypeDef> types;
    };
} // namespace AMT
