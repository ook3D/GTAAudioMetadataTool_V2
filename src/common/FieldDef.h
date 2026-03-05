#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace AMT
{
    enum class FieldKind : uint8_t
    {
        UInt8, 
        UInt16, 
        UInt32, 
        Int8, 
        Int16, 
        Int32, 
        Float,
        Hash,
        String,
        Array,
        FixedArray,
        Struct,
        OptionalBitfield,
        Enum,
        Placeholder
    };

    struct FieldDef
    {
        std::string name;
        FieldKind kind;
        bool trackedAsHash = false;
        bool trackedAsArchive = false;
        FieldKind countKind = FieldKind::UInt8;
        bool arrayElementsAreTrackedHashes = false;
        std::vector<FieldDef> children;
        int fixedCount = 0;
        std::vector<std::pair<int64_t, std::string>> enumValues;
        FieldKind enumBaseKind = FieldKind::UInt8;
        int placeholderSize = 0;
    };

    inline FieldDef UInt8(const std::string& name) { return {name, FieldKind::UInt8}; }
    inline FieldDef UInt16(const std::string& name) { return {name, FieldKind::UInt16}; }
    inline FieldDef UInt32(const std::string& name) { return {name, FieldKind::UInt32}; }
    inline FieldDef Int8(const std::string& name) { return {name, FieldKind::Int8}; }
    inline FieldDef Int16(const std::string& name) { return {name, FieldKind::Int16}; }
    inline FieldDef Int32(const std::string& name) { return {name, FieldKind::Int32}; }
    inline FieldDef Float(const std::string& name) { return {name, FieldKind::Float}; }

    inline FieldDef Hash(const std::string& name, bool isHash = false, bool isArchive = false)
    {
        FieldDef f{name, FieldKind::Hash};
        f.trackedAsHash = isHash;
        f.trackedAsArchive = isArchive;
        return f;
    }

    inline FieldDef String(const std::string& name, FieldKind countKind = FieldKind::UInt8)
    {
        FieldDef f{name, FieldKind::String};
        f.countKind = countKind;
        return f;
    }

    inline FieldDef Array(const std::string& name, std::vector<FieldDef> elementFields, FieldKind countKind = FieldKind::UInt8, bool elementsAreTrackedHashes = false)
    {
        FieldDef f{name, FieldKind::Array};
        f.countKind = countKind;
        f.children = std::move(elementFields);
        f.arrayElementsAreTrackedHashes = elementsAreTrackedHashes;
        return f;
    }

    inline FieldDef FixedArray(const std::string& name, std::vector<FieldDef> elementFields, int count)
    {
        FieldDef f{name, FieldKind::FixedArray};
        f.children = std::move(elementFields);
        f.fixedCount = count;
        return f;
    }

    inline FieldDef Struct(const std::string& name, std::vector<FieldDef> fields)
    {
        FieldDef f{name, FieldKind::Struct};
        f.children = std::move(fields);
        return f;
    }

    inline FieldDef OptionalBitfield(const std::string& name, std::vector<FieldDef> fields)
    {
        FieldDef f{name, FieldKind::OptionalBitfield};
        f.children = std::move(fields);
        return f;
    }

    inline FieldDef Enum(const std::string& name, FieldKind baseKind, std::vector<std::pair<int64_t, std::string>> values)
    {
        FieldDef f{name, FieldKind::Enum};
        f.enumBaseKind = baseKind;
        f.enumValues = std::move(values);
        return f;
    }

    inline FieldDef Placeholder(const std::string& name, int size = 0)
    {
        FieldDef f{name, FieldKind::Placeholder};
        f.placeholderSize = size;
        return f;
    }

} // namespace AMT
