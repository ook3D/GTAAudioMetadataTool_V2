#pragma once

#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"

namespace AMT
{

    enum class CategoryMetadataTypeId : uint8_t
    {
        GameMetadataHeader = 0,
        AudCategory = 0,
    };

    class CategoriesMetadataHeader : public PlaceholderMetadataType<0, "CategoriesMetadataHeader"> 
    {
    public:
        static constexpr int Type = static_cast<int>(CategoryMetadataTypeId::GameMetadataHeader);
        static constexpr const char* Name = "CategoriesMetadataHeader";
        
        gameClothing() = default;
    };

    class audCategory : public SimpleBaseMetadataType
    <
        FieldType<uint32_t, "Flags">,
        FieldType<int16_t, "__field09">, 
        FieldType<int16_t, "__field0b">,
        FieldType<int16_t, "__field0d">, 
        FieldType<int16_t, "__field0f">,
        FieldType<int16_t, "__field11">, 
        FieldType<int16_t, "__field13">,
        FieldType<int16_t, "__field15">, 
        FieldType<int16_t, "__field17">,
        FieldType<uint16_t, "__field19">, 
        FieldType<uint16_t, "__field1b">,
        FieldType<uint16_t, "__field1d">, 
        FieldType<uint16_t, "__field1f">,
        FieldType<uint16_t, "__field21">,
        FieldType<ArrayWrapper<JoaatHash, uint8_t, true>, "ChildCategories">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(CategoryMetadataTypeId::AudCategory);
        static constexpr const char* Name = "audCategory";
        
        audCategory() = default;
        audCategory(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    using CategoriesMetadataContainer = BaseMetadataContainer
    <
        CategoriesMetadataHeader, 
        true, 
        audCategory
    >;

    using CategoriesMetadataMgr = MetadataMgr<CategoriesMetadataContainer, 15>;
}; // namespace AMT
