#pragma once

#include "BaseTypes.h"
#include "IoUtils.h"
#include <tuple>
#include <vector>
#include <variant>
#include <string>
#include <cstring>
#include <bitset>

namespace AMT
{

// Base class for metadata types without compile-time type/name (defined by derived class)
template <typename... Args> 
class SimpleBaseMetadataType 
{
public:
    std::tuple<Args...> Data;

    SimpleBaseMetadataType() = default;
    SimpleBaseMetadataType(uint8_t *&data, uint32_t size) 
    { 
        data = Read(data); 
    }

    // Recursively collect archive offsets from all fields
    template <unsigned int i = 0>
    void GetArchiveOffsets(std::vector<uint32_t> &offsets, uint32_t currentOffset = 0) const 
    {
        if constexpr (i < sizeof...(Args)) 
        {
            BasicData::GetArchiveOffsets(offsets, currentOffset, std::get<i>(Data));
            currentOffset += BasicData::GetSize(0, std::get<i>(Data).Data);
            GetArchiveOffsets<i + 1>(offsets, currentOffset);
        }
    }

    // Convert all named fields to JSON
    template <unsigned int i = 0>
    void ToJson(ordered_json &json) const 
    {
        if constexpr (i < sizeof...(Args)) 
        {
            const auto& field = std::get<i>(Data);
            if (field.Name[0] != '\0')
            {
                to_json(json[field.Name], field.Data);
            }
            ToJson<i + 1>(json);
        }
    }

    // Load all named fields from JSON
    template <unsigned int i = 0>
    void FromJson(const ordered_json &json)
     {
        if constexpr (i < sizeof...(Args))
        {
            auto& field = std::get<i>(Data);
            if (field.Name[0] != '\0') 
            {
                field.Data = json.at(field.Name);
            }
            FromJson<i + 1>(json);
        }
    }

    template <unsigned int i = 0>
    void GetHashOffsets(std::vector<uint32_t> &out, uint32_t currentOffset = 0) const 
    {
        if constexpr (i < sizeof...(Args)) 
        {
            BasicData::GetHashOffsets(out, currentOffset, std::get<i>(Data));
            currentOffset += BasicData::GetSize(0, std::get<i>(Data).Data);
            GetHashOffsets<i + 1>(out, currentOffset);
        }
    }

    template <unsigned int i = 0>
    inline void GetArchiveNames(nlohmann::fifo_map<std::string, int> &out) const 
    {
        if constexpr (i < sizeof...(Args)) 
        {
            BasicData::GetArchiveNames(out, std::get<i>(Data));
            GetArchiveNames<i + 1>(out);
        }
    }

    // Calculate total size of all fields
    template <unsigned int i = 0>
    uint32_t GetSize(uint32_t initialSize = 0) const 
    {
        if constexpr (i == sizeof...(Args)) 
        {
            return initialSize;
        }
        else
        {
            initialSize = BasicData::GetSize(initialSize, std::get<i>(Data).Data);
            return GetSize<i + 1>(initialSize);
        }
    }

    // Read all fields from binary data
    template <unsigned int i = 0>
    uint8_t* Read(uint8_t *data) 
    {
        if constexpr (i == sizeof...(Args)) 
        {
            return data;
        } 
        else 
        {
            data = BasicData::Read(data, std::get<i>(Data).Data);
            return Read<i + 1>(data);
        }
    }

    // Write all fields to binary stream
    template <unsigned int i = 0>
    void Write(std::ostream &stream) 
    {
        if constexpr (i == sizeof...(Args)) 
        {
            return;
        } 
        else 
        {
            BasicData::Write(stream, std::get<i>(Data).Data);
            Write<i + 1>(stream);
        }
    }
};

// Base class for all metadata types with compile-time ID and name
template <int id, FixedString name, typename... Args> 
class BaseMetadataType 
{
public:
    std::tuple<Args...> Data;
    inline static const char *TypeName = name;

    static int GetId() 
    { 
        return id; 
    }

    BaseMetadataType() = default;
    BaseMetadataType(uint8_t *&data, uint32_t size)
    { 
        data = Read(data); 
    }

    // Recursively collect archive offsets from all fields
    template <unsigned int i = 0>
    void GetArchiveOffsets(std::vector<uint32_t> &offsets, uint32_t currentOffset = 0) const 
    {
        if constexpr (i < sizeof...(Args)) 
        {
            BasicData::GetArchiveOffsets(offsets, currentOffset, std::get<i>(Data));
            currentOffset += BasicData::GetSize(0, std::get<i>(Data).Data);
            GetArchiveOffsets<i + 1>(offsets, currentOffset);
        }
    }

    // Convert all named fields to JSON
    template <unsigned int i = 0>
    void ToJson(ordered_json &json) const
    {
        if constexpr (i < sizeof...(Args))
        {
            const auto& field = std::get<i>(Data);
            if (field.Name[0] != '\0')
            {
                to_json(json[field.Name], field.Data);
            }
            ToJson<i + 1>(json);
        }
    }

    // Load all named fields from JSON
    template <unsigned int i = 0>
    void FromJson(const ordered_json &json)
    {
        if constexpr (i < sizeof...(Args))
        {
            auto& field = std::get<i>(Data);
            if (field.Name[0] != '\0')
            {
                field.Data = json.at(field.Name);
            }
            FromJson<i + 1>(json);
        }
    }

    template <unsigned int i = 0>
    void GetHashOffsets(std::vector<uint32_t> &out, uint32_t currentOffset = 0) const
    {
        if constexpr (i < sizeof...(Args))
        {
            BasicData::GetHashOffsets(out, currentOffset, std::get<i>(Data));

            currentOffset += BasicData::GetSize(0, std::get<i> (Data).Data);
            GetHashOffsets<i + 1>(out, currentOffset);
        }
    }

    template <unsigned int i = 0>
    inline void GetArchiveNames(nlohmann::fifo_map<std::string, int> &out) const
    {
        if constexpr (i < sizeof...(Args))
        {
            BasicData::GetArchiveNames(out, std::get<i>(Data));
            GetArchiveNames<i + 1>(out);
        }
    }

    // Calculate total size of all fields
    template <unsigned int i = 0>
    uint32_t GetSize(uint32_t initialSize = 0) const 
    {
        if constexpr (i == sizeof...(Args)) 
        {
            return initialSize;
        }
        else
        {
            initialSize = BasicData::GetSize(initialSize, std::get<i>(Data).Data);
            return GetSize<i + 1>(initialSize);
        }
    }

    // Read all fields from binary data
    template <unsigned int i = 0>
    uint8_t* Read(uint8_t *data) 
    {
        if constexpr (i == sizeof...(Args)) 
        {
            return data;
        } 
        else 
        {
            data = BasicData::Read(data, std::get<i>(Data).Data);
            return Read<i + 1>(data);
        }
    }

    // Write all fields to binary stream
    template <unsigned int i = 0>
    void Write(std::ostream &stream)
     {
        if constexpr (i == sizeof...(Args)) 
        {
            return;
        } 
        else 
        {
            BasicData::Write(stream, std::get<i>(Data).Data);
            Write<i + 1>(stream);
        }
    }
};

template <typename... Args>
class OptionalBitfieldContainer : public BaseMetadataType<0, "", Args...>
{
private:
    std::bitset<sizeof...(Args)> m_InternalBitfield;
    uint32_t m_BitfieldWritePos = 0;
    using base = BaseMetadataType<0, "", Args...>;

public:
    template <unsigned int i = 0>
    uint8_t *Read(uint8_t *data)
    {
        if constexpr (i == 0)
        {
            uint32_t bitset = IoUtils::ReadData<uint32_t>(data);
            m_InternalBitfield = bitset;
        }

        if constexpr (i == sizeof...(Args))
        {
            return data;
        }
        else
        {
            if (m_InternalBitfield[i])
            {
                data = BasicData::Read(data, std::get<i>(this->Data).Data);
            }
            return Read<i + 1>(data);
        }
    }

    template <unsigned int i = 0>
    void Write(std::ostream &out)
    {
        if constexpr (i == 0)
        {
            m_BitfieldWritePos = static_cast<uint32_t>(out.tellp());
            out.seekp(4, std::ios_base::cur);
            m_InternalBitfield.reset();
        }

        if constexpr (i == sizeof...(Args))
        {
            size_t retPos = out.tellp();
            out.seekp(m_BitfieldWritePos);

            IoUtils::WriteData(out, uint32_t(m_InternalBitfield.to_ulong()));
            out.seekp(retPos);
            return;
        }
        else
        {
            if (std::get<i> (this->Data).Data.HasValue())
            {
                BasicData::Write(out, std::get<i>(this->Data).Data);
                m_InternalBitfield[i] = true;
            }
            Write<i + 1>(out);
        }
    }

    uint32_t GetSize(uint32_t initialSize = 0) const
    {
        return base::GetSize(initialSize) + 4;
    }

    template <unsigned int i = 0>
    void ToJson(ordered_json &j) const
    {
        if constexpr (i < sizeof...(Args))
        {
            if (std::get<i>(this->Data).Data.HasValue())
            {
                to_json(j[std::get<i>(this->Data).Name], std::get<i>(this->Data).Data);
            }
            ToJson<i + 1>(j);
        }
    }

    template <unsigned int i = 0>
    void FromJson(const ordered_json &j)
    {
        if constexpr (i < sizeof...(Args))
        {
            if (j.count(std::get<i>(this->Data).Name))
            {
                std::get<i>(this->Data).Data = j.at(std::get<i>(this->Data).Name);
            }
            FromJson<i + 1>(j);
        }
    }
};

template <typename HeaderType, bool hasNameOffset, typename... Types>
class BaseMetadataContainer
{
    std::string Name;
    HeaderType Header;
    std::variant<Types...> Data;

public:
    BaseMetadataContainer() = default;
    BaseMetadataContainer(std::string name, uint32_t size) : Name (name) {}

    std::string GetName() const
    {
        return Name;
    }

    void SetName(const std::string &name)
    {
        Name = name;
    }

    void Read(uint8_t *in, uint32_t size)
    {
        uint8_t Id = IoUtils::ReadData<uint8_t>(in);
        if constexpr (hasNameOffset)
        {
            IoUtils::ReadData<uint32_t>(in);
        }

        in = Header.Read(in);
        
        // Helper to get type ID from either GetId() method or static Type member
        auto getTypeId = []<typename T>() -> int 
        {
            if constexpr (requires { T::GetId(); }) 
            {
                return T::GetId();
            }
            else if constexpr (requires { T::Type; }) 
            {
                return T::Type;
            } 
            else
            {
                return -1;
            }
        };
        
        // Helper to construct the appropriate type
        auto constructType = [&]<typename T>() -> T 
        {
            uint32_t remainingSize = size - GetHeaderSize();
            if constexpr (requires { T(in, remainingSize); })
            {
                return T(in, remainingSize); // For types that have constructor with (uint8_t*&, uint32_t)
            } 
            else 
            {
                return T{}; // For types that only have default constructor
            }
        };
        
        ((Id == getTypeId.template operator()<Types>() ? (Data = constructType.template operator()<Types>(), true) : false), ...);
    }

    void Write(std::ostream &out, uint32_t nameOffset)
    {
        uint8_t Id = 0;
        std::visit([&] (auto &&arg) 
        { 
            using T = std::decay_t<decltype(arg)>;
            if constexpr (requires { arg.GetId(); }) 
            {
                Id = arg.GetId();
            } 
            else if constexpr (requires { T::Type; }) 
            {
                Id = T::Type;
            }
        }, Data);

        IoUtils::WriteData(out, Id);
        if constexpr (hasNameOffset)
        {
            IoUtils::WriteData(out, nameOffset);
        }

        Header.Write(out);
        std::visit([&] (auto &&arg) { arg.Write(out); }, Data);
    }

    inline uint32_t GetSize() const
    {
        return std::visit([&] (auto &&arg) { return arg.GetSize() + GetHeaderSize(); }, Data);
    }

    void ToJson(ordered_json &j) const
    {
        std::visit([&] (auto &&arg) 
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (requires { arg.TypeName; }) 
            {
                j["Type"] = arg.TypeName;
            } 
            else if constexpr (requires { T::Name; }) 
            {
                j["Type"] = T::Name;
            }
            to_json(j, Header);
            j["Metadata"] = arg;
        },
        Data);
    }

    void FromJson(const ordered_json &j)
    {
        from_json(j, Header);
        
        auto getTypeName = []<typename T>() -> const char* 
        {
            if constexpr (requires { T::TypeName; })
            {
                return T::TypeName;
            }
            else if constexpr (requires { T::Name; })
            {
                return T::Name;
            } 
            else
            {
                return "";
            }
        };
        
        (..., (getTypeName.template operator()<Types>() == j.at("Type") ? (Data = j.at("Metadata").get<Types>(), false) : true));
    }

    uint32_t GetHeaderHeaderSize() const
    {
        uint32_t size = 1; // for the type
        if constexpr (hasNameOffset)
        {
            size += 4;
        }
        return size;
    }

    
    uint32_t GetHeaderSize() const
    {
        return BasicData::GetSize(GetHeaderHeaderSize(), Header);
    }

    void GetHashOffsets(std::vector<uint32_t> &out, uint32_t currentOffset = 0) const
    {
        BasicData::GetHashOffsets(out, GetHeaderHeaderSize() + currentOffset, Header);

        std::visit ([&] (auto &&arg) 
        {
            BasicData::GetHashOffsets(out, GetHeaderSize() + currentOffset, arg);
        },
        Data);
    }

    void GetArchiveOffsets(std::vector<uint32_t>& out, uint32_t currentOffset = 0) const
    {
        BasicData::GetArchiveOffsets(out, GetHeaderHeaderSize() + currentOffset, Header);

        std::visit([&] (auto &&arg) 
        {
            BasicData::GetArchiveOffsets(out, GetHeaderSize() + currentOffset, arg);
        },
        Data);
    }

    void GetArchiveNames(nlohmann::fifo_map<std::string, int> &out) const
    {
        return std::visit([&] (auto &&arg) { return BasicData::GetArchiveNames(out, arg); }, Data);
    }
};

}; // namespace AMT
