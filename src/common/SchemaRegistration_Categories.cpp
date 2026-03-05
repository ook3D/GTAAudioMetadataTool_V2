#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    void RegisterCategoriesSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 15;
        def.hasNameOffset = true;

        // CategoriesMetadataHeader, empty
        def.headerFields = {};

        // audCategory
        def.types[0] = {0, "audCategory", {
            UInt32("Flags"),
            Int16("__field09"), 
            Int16("__field0b"),
            Int16("__field0d"), 
            Int16("__field0f"),
            Int16("__field11"), 
            Int16("__field13"),
            Int16("__field15"), 
            Int16("__field17"),
            UInt16("__field19"), 
            UInt16("__field1b"),
            UInt16("__field1d"), 
            UInt16("__field1f"),
            UInt16("__field21"),
            Array("ChildCategories", {{"", FieldKind::Hash, true}}, FieldKind::UInt8, true)
        }};

        registry["categories"] = std::move(def);
    }
} // namespace AMT
