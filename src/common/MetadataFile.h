#pragma once

#include "MetadataObject.h"
#include "MetadataTypeDef.h"
#include <vector>
#include <string>
#include <iostream>
#include <memory>

namespace AMT
{
    class MetadataFile
    {
    public:
        MetadataFile() = default;
        MetadataFile(const MetadataFileDef* fileDef, bool debugMode = false) : m_FileDef(fileDef), m_DebugMode(debugMode) {}

        void Read(std::istream& in);
        void Write(std::ostream& out);

        void ToJson(ordered_json& j) const;
        void FromJson(const ordered_json& j);

    private:
        void ReadObjectsData(std::istream& in);
        void ReadArchiveList(std::istream& in);
        void ReadObjectsMetadata(std::istream& in);

        void WriteObjectsData(std::ostream& out);
        void WriteArchiveList(std::ostream& out);
        void WriteObjectsMetadata(std::ostream& out);
        void WriteObjectArchiveNamesOffsets(std::ostream& out);

        nlohmann::fifo_map<std::string, int> GenerateArchiveNames();

        const MetadataFileDef* m_FileDef = nullptr;
        bool m_DebugMode = false;
        std::vector<MetadataObject> m_Objects;
        std::vector<uint32_t> m_InternalObjectOffsets;
        std::unique_ptr<uint8_t[]> m_RawData;
    };
} // namespace AMT
