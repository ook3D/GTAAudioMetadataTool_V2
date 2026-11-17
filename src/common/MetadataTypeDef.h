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
        uint32_t alignment = 1; // required start alignment of the object in the data blob (after the separator byte)
    };

    struct MetadataFileDef
    {
        uint32_t suffix;
        bool hasNameOffset;
        std::vector<FieldDef> headerFields;
        std::map<int, MetadataTypeDef> types;
    };
} // namespace AMT
