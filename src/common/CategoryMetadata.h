#pragma once

#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"

namespace AMT
{
    using CategoriesMetadataHeader = BaseMetadataType
    <
        0, 
        "CategoriesMetadataHeader"
    >;

    using audCategory = BaseMetadataType
    <
        0, 
        "audCategory", 
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
    >;

    using CategoriesMetadataContainer = BaseMetadataContainer<CategoriesMetadataHeader, true, audCategory>;
    using CategoriesMetadataMgr = MetadataMgr<CategoriesMetadataContainer, 15>;
}; // namespace AMT
