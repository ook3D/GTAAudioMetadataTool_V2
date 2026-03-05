#include "pch.h"
#include "MetadataObject.h"
#include <cstring>

namespace AMT
{
    uint32_t MetadataObject::GetHeaderHeaderSize() const
    {
        uint32_t size = 1; // type ID byte
        if (m_FileDef->hasNameOffset)
            size += 4; // name offset uint32
        return size;
    }

    uint32_t MetadataObject::GetHeaderSize() const
    {
        return GetHeaderHeaderSize() + FieldIO::GetFieldsSize(m_FileDef->headerFields, m_HeaderValues);
    }

    void MetadataObject::Read(uint8_t* data, uint32_t size, bool debugMode)
    {
        uint8_t* start = data;

        // Read type ID
        uint8_t typeId;
        memcpy(&typeId, data, 1);
        data += 1;
        m_TypeId = typeId;

        // Skip name offset if present
        if (m_FileDef->hasNameOffset)
        {
            data += 4;
        }

        // Read header fields
        ordered_json* dbg = debugMode ? &m_DebugInfo : nullptr;
        data = FieldIO::ReadFields(data, m_FileDef->headerFields, m_HeaderValues, dbg, 0);

        // Find the type definition
        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            m_TypeName = it->second.name;
            uint32_t remaining = size - static_cast<uint32_t>(data - start);
            data = FieldIO::ReadFields(data, it->second.fields, m_TypeValues, dbg, remaining);
        }
    }

    void MetadataObject::Write(std::ostream& out, uint32_t nameOffset)
    {
        // Write type ID
        uint8_t id = static_cast<uint8_t>(m_TypeId);
        IoUtils::WriteData(out, id);

        // Write name offset if present
        if (m_FileDef->hasNameOffset)
        {
            IoUtils::WriteData(out, nameOffset);
        }

        // Write header fields
        FieldIO::WriteFields(out, m_FileDef->headerFields, m_HeaderValues);

        // Write type-specific fields
        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            FieldIO::WriteFields(out, it->second.fields, m_TypeValues);
        }
    }

    uint32_t MetadataObject::GetSize() const
    {
        uint32_t size = GetHeaderSize();
        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            size += FieldIO::GetFieldsSize(it->second.fields, m_TypeValues);
        }
        return size;
    }

    void MetadataObject::ToJson(ordered_json& j) const
    {
        j["Type"] = m_TypeName;

        // Header fields
        for (const auto& [key, val] : m_HeaderValues.items())
        {
            j[key] = val;
        }

        // Type-specific fields
        ordered_json meta;
        for (const auto& [key, val] : m_TypeValues.items())
        {
            meta[key] = val;
        }
        j["Metadata"] = meta;

        // Debug info
        if (!m_DebugInfo.is_null() && !m_DebugInfo.empty())
        {
            j["_debug"] = m_DebugInfo;
        }
    }

    void MetadataObject::FromJson(const ordered_json& j)
    {
        std::string typeName = j.at("Type").get<std::string>();
        m_TypeName = typeName;

        // Find type ID from name
        for (const auto& [id, typeDef] : m_FileDef->types)
        {
            if (typeDef.name == typeName)
            {
                m_TypeId = id;
                break;
            }
        }

        // Load header fields
        m_HeaderValues = ordered_json::object();
        for (const auto& field : m_FileDef->headerFields)
        {
            if (j.contains(field.name))
            {
                m_HeaderValues[field.name] = j.at(field.name);
            }
        }

        // Load type-specific fields
        m_TypeValues = ordered_json::object();
        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            const auto& metaJson = j.at("Metadata");
            for (const auto& field : it->second.fields)
            {
                if (metaJson.contains(field.name))
                {
                    m_TypeValues[field.name] = metaJson.at(field.name);
                }
            }
        }
    }

    void MetadataObject::GetHashOffsets(std::vector<uint32_t>& out, uint32_t currentOffset) const
    {
        uint32_t headerHeaderSize = GetHeaderHeaderSize();
        FieldIO::CollectAllHashOffsets(m_FileDef->headerFields, m_HeaderValues, out, currentOffset + headerHeaderSize);

        uint32_t headerSize = GetHeaderSize();
        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            FieldIO::CollectAllHashOffsets(it->second.fields, m_TypeValues, out, currentOffset + headerSize);
        }
    }

    void MetadataObject::GetArchiveOffsets(std::vector<uint32_t>& out, uint32_t currentOffset) const
    {
        uint32_t headerHeaderSize = GetHeaderHeaderSize();
        FieldIO::CollectAllArchiveOffsets(m_FileDef->headerFields, m_HeaderValues, out, currentOffset + headerHeaderSize);

        uint32_t headerSize = GetHeaderSize();
        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            FieldIO::CollectAllArchiveOffsets(it->second.fields, m_TypeValues, out, currentOffset + headerSize);
        }
    }

    void MetadataObject::GetArchiveNames(nlohmann::fifo_map<std::string, int>& out) const
    {
        FieldIO::CollectAllArchiveNames(m_FileDef->headerFields, m_HeaderValues, out);

        auto it = m_FileDef->types.find(m_TypeId);
        if (it != m_FileDef->types.end())
        {
            FieldIO::CollectAllArchiveNames(it->second.fields, m_TypeValues, out);
        }
    }
} // namespace AMT
