#include "pch.h"
#include "MetadataFile.h"
#include "HashManager.h"

namespace AMT
{
    void MetadataFile::ReadObjectsData(std::istream& in)
    {
        uint32_t dataSize = IoUtils::ReadData<uint32_t>(in);
        m_RawData = std::make_unique<uint8_t[]>(dataSize);
        IoUtils::ReadData(in, m_RawData.get(), dataSize);
    }

    void MetadataFile::ReadArchiveList(std::istream& in)
    {
        const uint32_t SANE_SIZE = static_cast<uint32_t>(1e9);

        uint32_t blockSize = IoUtils::ReadData<uint32_t>(in);
        uint32_t numArchives = IoUtils::ReadData<uint32_t>(in);

        if (numArchives == 0)
            return;

        if (blockSize > SANE_SIZE)
            throw std::runtime_error("Sanity check failed on reading archive lists");

        uint32_t namesSize = blockSize - 4 - (numArchives * 4);
        auto offsets = std::make_unique<uint32_t[]>(numArchives);
        auto strs = std::make_unique<char[]>(namesSize);

        IoUtils::ReadData(in, offsets.get(), numArchives * 4);
        IoUtils::ReadData(in, strs.get(), namesSize);

        for (uint32_t i = 0; i < numArchives; i++)
        {
            std::string archive(strs.get() + offsets[i]);
            std::replace(archive.begin(), archive.end(), '\\', '/');
            HashManager::Instance()->AddHash(archive);
        }
    }

    void MetadataFile::ReadObjectsMetadata(std::istream& in)
    {
        uint32_t numObjects = IoUtils::ReadData<uint32_t>(in);
        uint32_t sizeOfNames = IoUtils::ReadData<uint32_t>(in);

        for (uint32_t i = 0; i < numObjects; i++)
        {
            uint8_t nameLen = IoUtils::ReadData<uint8_t>(in);
            auto nameRaw = std::make_unique<char[]>(nameLen);
            IoUtils::ReadData(in, nameRaw.get(), nameLen);

            uint32_t offset = IoUtils::ReadData<uint32_t>(in);
            uint32_t size = IoUtils::ReadData<uint32_t>(in);

            std::string name(nameRaw.get(), nameLen);
            HashManager::Instance()->AddHash(name);

            MetadataObject obj(m_FileDef);
            obj.SetName(name);
            obj.Read(&(m_RawData.get()[offset]), size, m_DebugMode);

            m_Objects.push_back(std::move(obj));
        }
    }

    void MetadataFile::Read(std::istream& in)
    {
        uint32_t suffix = IoUtils::ReadData<uint32_t>(in);
        if (suffix != m_FileDef->suffix)
        {
            std::cout << "Suffix mismatch" << std::endl;
        }

        ReadObjectsData(in);
        ReadArchiveList(in);
        ReadObjectsMetadata(in);
    }

    void MetadataFile::WriteObjectsData(std::ostream& out)
    {
        auto pos = out.tellp();
        out.seekp(4, std::ios_base::cur); // skip data size

        uint32_t nameOffset = 0;
        for (auto& obj : m_Objects)
        {
            uint8_t zero = 0;
            IoUtils::WriteData(out, zero); // null padding byte

            m_InternalObjectOffsets.push_back(static_cast<uint32_t>(out.tellp()));
            obj.Write(out, nameOffset);

            nameOffset += static_cast<uint32_t>(obj.GetName().size() + 1);
        }

        auto end = out.tellp();
        out.seekp(pos);
        IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(end - pos - 4));
        out.seekp(end);
    }

    nlohmann::fifo_map<std::string, int> MetadataFile::GenerateArchiveNames()
    {
        nlohmann::fifo_map<std::string, int> names;
        for (auto& obj : m_Objects)
        {
            obj.GetArchiveNames(names);
        }
        return names;
    }

    void MetadataFile::WriteArchiveList(std::ostream& out)
    {
        auto archiveNames = GenerateArchiveNames();

        auto blockStart = out.tellp();
        out.seekp(8 + (archiveNames.size() * 4), std::ios_base::cur);

        std::vector<uint32_t> archiveOffsets;
        auto strsStart = out.tellp();

        for (const auto& [name, _] : archiveNames)
        {
            std::string winName = name;
            std::replace(winName.begin(), winName.end(), '/', '\\');

            archiveOffsets.push_back(static_cast<uint32_t>(out.tellp() - strsStart));
            out.write(winName.c_str(), winName.size() + 1);
        }

        auto blockEnd = out.tellp();
        out.seekp(blockStart);

        IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(blockEnd - blockStart - 4));
        IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(archiveNames.size()));
        if (!archiveOffsets.empty())
        {
            IoUtils::WriteData<uint32_t>(out, archiveOffsets[0], static_cast<uint32_t>(4 * archiveOffsets.size()));
        }

        out.seekp(blockEnd);
    }

    void MetadataFile::WriteObjectsMetadata(std::ostream& out)
    {
        auto blockStart = out.tellp();
        out.seekp(8, std::ios_base::cur);

        uint32_t objectNamesCapacity = 0;
        size_t index = 0;
        for (auto& obj : m_Objects)
        {
            IoUtils::WriteData<uint8_t>(out, static_cast<uint8_t>(obj.GetName().length()));
            out.write(obj.GetName().c_str(), obj.GetName().length());

            IoUtils::WriteData<uint32_t>(out, m_InternalObjectOffsets[index] - 8);
            IoUtils::WriteData(out, obj.GetSize());

            objectNamesCapacity += static_cast<uint32_t>(obj.GetName().length() + 1);
            index++;
        }

        auto blockEnd = out.tellp();
        out.seekp(blockStart);

        IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(m_Objects.size()));
        IoUtils::WriteData<uint32_t>(out, objectNamesCapacity);

        out.seekp(blockEnd);
    }

    void MetadataFile::WriteObjectArchiveNamesOffsets(std::ostream& out)
    {
        std::vector<uint32_t> hashOffsets;
        std::vector<uint32_t> archiveOffsets;

        size_t index = 0;
        for (auto& obj : m_Objects)
        {
            obj.GetArchiveOffsets(archiveOffsets, m_InternalObjectOffsets[index]);
            obj.GetHashOffsets(hashOffsets, m_InternalObjectOffsets[index]);
            index++;
        }

        IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(hashOffsets.size()));
        if (!hashOffsets.empty())
        {
            IoUtils::WriteData<uint32_t>(out, hashOffsets[0], static_cast<uint32_t>(hashOffsets.size() * 4));
        }
        IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(archiveOffsets.size()));
        if (!archiveOffsets.empty())
        {
            IoUtils::WriteData<uint32_t>(out, archiveOffsets[0], static_cast<uint32_t>(archiveOffsets.size() * 4));
        }
    }

    void MetadataFile::Write(std::ostream& out)
    {
        IoUtils::WriteData<uint32_t>(out, m_FileDef->suffix);
        WriteObjectsData(out);
        WriteArchiveList(out);
        WriteObjectsMetadata(out);
        WriteObjectArchiveNamesOffsets(out);
    }

    void MetadataFile::ToJson(ordered_json& j) const
    {
        for (const auto& obj : m_Objects)
        {
            ordered_json objJson;
            obj.ToJson(objJson);
            j[obj.GetName()] = objJson;
        }
    }

    void MetadataFile::FromJson(const ordered_json& j)
    {
        for (const auto& [key, value] : j.items())
        {
            MetadataObject obj(m_FileDef);
            obj.FromJson(value);
            obj.SetName(key);
            m_Objects.push_back(std::move(obj));
        }
    }
} // namespace AMT
