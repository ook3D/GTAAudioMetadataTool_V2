#pragma once

#include "BaseMetadata.h"
#include <vector>
#include <variant>
#include <iostream>
#include "IoUtils.h"
#include <memory>
#include "HashManager.h"

namespace AMT 
{

    template <typename Type, int Suffix> class MetadataMgr
    {
        std::vector<Type> Objects;
        std::vector<uint32_t> m_InternalObjectOffsets;
        
        std::unique_ptr<uint8_t[]> RawData;

    public:
        void ReadObjectsData (std::istream &in)
        {
            uint32_t dataSize = IoUtils::ReadData<uint32_t>(in);
            RawData = std::make_unique<uint8_t[]>(dataSize);

            IoUtils::ReadData(in, RawData.get(), dataSize);
        }

        void ReadArchiveList(std::istream &in)
        {
            const uint32_t SANE_SIZE = static_cast<uint32_t>(1e9);

            uint32_t blockSize   = IoUtils::ReadData<uint32_t> (in);
            uint32_t numArchives = IoUtils::ReadData<uint32_t> (in);

            if (numArchives == 0)
            {
                return;
            }
            if (blockSize > SANE_SIZE)
            {
                throw std::runtime_error ("Sanity check failed on reading archive lists");
            }

            uint32_t names_size = blockSize - 4 - (numArchives * 4);

            auto offsets = std::make_unique<uint32_t[]>(numArchives);
            auto strs = std::make_unique<char[]>(names_size);

            IoUtils::ReadData(in, offsets.get(), numArchives * 4);
            IoUtils::ReadData(in, strs.get(), names_size);

            for (uint32_t i = 0; i < numArchives; i++)
            {
                std::string archive(strs.get() + offsets[i]);

                std::replace(std::begin (archive), std::end (archive), '\\', '/');
                HashManager::Instance()->AddHash(archive);
            }
        }

        void ReadObjectsMetadata(std::istream &in)
        {
            uint32_t numObjects = IoUtils::ReadData<uint32_t>(in);
            uint32_t sizeOfNames = IoUtils::ReadData<uint32_t>(in);

            for (uint32_t i = 0; i < numObjects; i++)
            {
                uint8_t nameLen = IoUtils::ReadData<uint8_t> (in);
                std::unique_ptr<char[]> nameRaw = std::make_unique<char[]>(nameLen);
                IoUtils::ReadData(in, nameRaw.get(), nameLen);

                uint32_t offset = IoUtils::ReadData<uint32_t>(in);
                uint32_t size = IoUtils::ReadData<uint32_t>(in);

                const std::string name(nameRaw.get(), nameLen);

                HashManager::Instance()->AddHash(name);

                Type data(name, size);
                data.Read(&(RawData.get()[offset]), size);

                Objects.push_back(data);
            }
        }

        void Read(std::istream &in)
        {
            if (IoUtils::ReadData<uint32_t>(in) != Suffix)
            {
                std::cout << "Suffix mismatch";
            }
            ReadObjectsData(in);
            ReadArchiveList(in);
            ReadObjectsMetadata(in);
        }

        void WriteObjectsData(std::ostream &out)
        {
            auto pos = out.tellp();
            out.seekp(4, std::ios_base::cur); // Skip writing length for now.

            uint32_t nameOffset = 0;

            for (auto &i : Objects)
            {
                IoUtils::WriteData<uint8_t> (out, 0);

                m_InternalObjectOffsets.push_back(static_cast<uint32_t>(out.tellp()));
                i.Write(out, nameOffset);

                nameOffset += static_cast<uint32_t>(i.GetName().size() + 1);
            }

            auto end = out.tellp();
            out.seekp(pos);

            IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(end - pos - 4));
            out.seekp(end);
        }

        auto GenerateArchiveNames ()
        {
            nlohmann::fifo_map<std::string, int> Names;
            for (auto &i : Objects)
            {
                i.GetArchiveNames (Names);
            }

            return Names;
        }

        void WriteArchiveList(std::ostream &out)
        {
            auto ArchiveNames = GenerateArchiveNames();

            // We're also skipping writing the block size, number of
            // archives (both 8 bytes), and the offsets to each of the
            // strings (which is 4 bytes each for all the archives).
            auto blockStart = out.tellp();
            out.seekp(8 + (ArchiveNames.size() * 4), std::ios_base::cur);

            std::vector<uint32_t> archiveOffsets;
            auto strsStart = out.tellp();

            for (const auto &[i, _] : ArchiveNames)
            {
                std::string name = i;
                std::replace(std::begin(name), std::end(name), '/', '\\');

                archiveOffsets.push_back(static_cast<uint32_t>(out.tellp() - strsStart));
                out.write(name.c_str(), name.size() + 1);
            }

            auto blockEnd = out.tellp();
            out.seekp(blockStart);

            // Finally write the size and offsets
            IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(blockEnd - blockStart - 4));
            IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(ArchiveNames.size ()));
            IoUtils::WriteData<uint32_t>(out, archiveOffsets[0], static_cast<uint32_t>(4 * archiveOffsets.size ()));

            out.seekp(blockEnd);
        }

        void WriteObjectsMetadata(std::ostream &out)
        {
            // Skip writing number of objects and size of strings for now.
            auto blockStart = out.tellp();
            out.seekp(8, std::ios_base::cur);

            uint32_t objectNamesCapacity = 0;
            size_t index = 0;
            for (auto &i : Objects)
            {
                // Don't include the null terminator as is consistent with the
                // game.
                IoUtils::WriteData<uint8_t>(out, static_cast<uint8_t>(i.GetName().length()));
                out.write(i.GetName().c_str(), i.GetName().length());

                // -8 for the prefix and the data length
                IoUtils::WriteData<uint32_t>(out, m_InternalObjectOffsets[index] - 8);
                IoUtils::WriteData(out, i.GetSize());

                objectNamesCapacity += static_cast<uint32_t>(i.GetName().length() + 1);

                index++;
            }

            auto block_end = out.tellp();
            out.seekp(blockStart);

            IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(Objects.size()));
            IoUtils::WriteData<uint32_t>(out, objectNamesCapacity);

            out.seekp(block_end);
        }

        void WriteObjectArchiveNamesOffsets(std::ostream &out)
        {
            std::vector<uint32_t> objectNamesOffsets;
            std::vector<uint32_t> archiveNamesOffsets;

            // TODO: Add checks for if the hashes exist...
            size_t index = 0;
            for (auto &i : Objects)
            {
                i.GetArchiveOffsets(archiveNamesOffsets, m_InternalObjectOffsets[index]);
                i.GetHashOffsets(objectNamesOffsets, m_InternalObjectOffsets[index]);

                index++;
            }

            IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(objectNamesOffsets.size()));
            IoUtils::WriteData<uint32_t>(out, objectNamesOffsets[0], static_cast<uint32_t>(objectNamesOffsets.size() * 4));
            IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(archiveNamesOffsets.size()));
            IoUtils::WriteData<uint32_t>(out, archiveNamesOffsets[0], static_cast<uint32_t>(archiveNamesOffsets.size() * 4));
        }

        void Write(std::ostream &out)
        {
            IoUtils::WriteData<uint32_t>(out, Suffix);

            WriteObjectsData(out);
            WriteArchiveList(out);
            WriteObjectsMetadata(out);
            WriteObjectArchiveNamesOffsets(out);
        }

        void ToJson(ordered_json &j) const
        {
            for (auto &i : Objects)
            {
                j[i.GetName ()] = i;
            }
        }

        void FromJson(const ordered_json &j)
        {
            for (const auto &[key, value] : j.items())
            {
                Objects.push_back(value);
                Objects.back().SetName(key);
            }
        }
    };
}; // namespace AMT
