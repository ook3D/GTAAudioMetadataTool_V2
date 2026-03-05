#pragma once

#include "MetadataTypeDef.h"
#include "FieldIO.h"
#include <string>
#include <vector>

namespace AMT
{
    class MetadataObject
    {
    public:
        MetadataObject() = default;
        MetadataObject(const MetadataFileDef* fileDef) : m_FileDef(fileDef) {}

        void Read(uint8_t* data, uint32_t size, bool debugMode = false);
        void Write(std::ostream& out, uint32_t nameOffset);
        uint32_t GetSize() const;

        void ToJson(ordered_json& j) const;
        void FromJson(const ordered_json& j);

        void GetHashOffsets(std::vector<uint32_t>& out, uint32_t currentOffset) const;
        void GetArchiveOffsets(std::vector<uint32_t>& out, uint32_t currentOffset) const;
        void GetArchiveNames(nlohmann::fifo_map<std::string, int>& out) const;

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

    private:
        uint32_t GetHeaderHeaderSize() const;
        uint32_t GetHeaderSize() const;

        const MetadataFileDef* m_FileDef = nullptr;
        std::string m_Name;
        int m_TypeId = 0;
        std::string m_TypeName;
        ordered_json m_HeaderValues;
        ordered_json m_TypeValues;
        ordered_json m_DebugInfo;
    };
} // namespace AMT
