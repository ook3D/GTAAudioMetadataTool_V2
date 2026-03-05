#include "pch.h"
#include "FieldIO.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>

namespace AMT
{
    namespace FieldIO
    {
        static double RoundForJson(float f)
        {
            return std::round(static_cast<double>(f) * 1000.0) / 1000.0;
        }

        static std::string ToHex(const uint8_t* data, uint32_t size)
        {
            std::ostringstream ss;
            for (uint32_t i = 0; i < size; i++)
            {
                if (i > 0) ss << ' ';
                ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]);
            }
            return ss.str();
        }

        static std::string ToHex32(const uint8_t* data, uint32_t size)
        {
            std::ostringstream ss;
            uint32_t i = 0;
            while (i + 4 <= size)
            {
                if (i > 0) ss << ' ';
                ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]);
                ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i+1]);
                ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i+2]);
                ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i+3]);
                i += 4;
            }
            while (i < size)
            {
                if (i > 0) ss << ' ';
                ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]);
                i++;
            }
            return ss.str();
        }

        uint32_t CountPrefixSize(FieldKind kind)
        {
            switch (kind)
            {
                case FieldKind::UInt8:  return 1;
                case FieldKind::UInt16: return 2;
                case FieldKind::UInt32: return 4;
                case FieldKind::Int8:   return 1;
                case FieldKind::Int16:  return 2;
                case FieldKind::Int32:  return 4;
                default: return 1;
            }
        }

        uint32_t ReadCountPrefix(uint8_t*& data, FieldKind kind)
        {
            switch (kind)
            {
                case FieldKind::UInt8:  { uint8_t v; memcpy(&v, data, 1); data += 1; return v; }
                case FieldKind::UInt16: { uint16_t v; memcpy(&v, data, 2); data += 2; return v; }
                case FieldKind::UInt32: { uint32_t v; memcpy(&v, data, 4); data += 4; return v; }
                case FieldKind::Int16:  { uint16_t v; memcpy(&v, data, 2); data += 2; return v; }
                default: { uint8_t v; memcpy(&v, data, 1); data += 1; return v; }
            }
        }

        void WriteCountPrefix(std::ostream& stream, uint32_t count, FieldKind kind)
        {
            switch (kind)
            {
                case FieldKind::UInt8:  { uint8_t v = static_cast<uint8_t>(count); IoUtils::WriteData(stream, v); break; }
                case FieldKind::UInt16: { uint16_t v = static_cast<uint16_t>(count); IoUtils::WriteData(stream, v); break; }
                case FieldKind::UInt32: { IoUtils::WriteData(stream, count); break; }
                case FieldKind::Int16:  { uint16_t v = static_cast<uint16_t>(count); IoUtils::WriteData(stream, v); break; }
                default: { uint8_t v = static_cast<uint8_t>(count); IoUtils::WriteData(stream, v); break; }
            }
        }

        // Get the binary size of a single element described by children fields, given JSON
        static uint32_t GetElementSize(const std::vector<FieldDef>& children, const ordered_json& elemJson)
        {
            // If single child with empty name, it's a primitive element type
            if (children.size() == 1 && children[0].name.empty())
            {
                return GetFieldSize(children[0], elemJson);
            }
            // Otherwise it's a struct-like element
            uint32_t size = 0;
            for (const auto& child : children)
            {
                if (elemJson.contains(child.name))
                    size += GetFieldSize(child, elemJson.at(child.name));
                else
                    size += GetFieldSize(child, ordered_json());
            }
            return size;
        }

        uint8_t* ReadField(uint8_t* data, const FieldDef& field, ordered_json& out, ordered_json* debugJson, uint32_t remainingSize)
        {
            uint8_t* start = data;
            switch (field.kind)
            {
                case FieldKind::UInt8:
                {
                    uint8_t v; memcpy(&v, data, 1); data += 1;
                    out = v;
                    break;
                }
                case FieldKind::UInt16:
                {
                    uint16_t v; memcpy(&v, data, 2); data += 2;
                    out = v;
                    break;
                }
                case FieldKind::UInt32:
                {
                    uint32_t v; memcpy(&v, data, 4); data += 4;
                    out = v;
                    break;
                }
                case FieldKind::Int8:
                {
                    int8_t v; memcpy(&v, data, 1); data += 1;
                    out = v;
                    break;
                }
                case FieldKind::Int16:
                {
                    int16_t v; memcpy(&v, data, 2); data += 2;
                    out = v;
                    break;
                }
                case FieldKind::Int32:
                {
                    int32_t v; memcpy(&v, data, 4); data += 4;
                    out = v;
                    break;
                }
                case FieldKind::Float:
                {
                    float v; memcpy(&v, data, 4); data += 4;
                    out = RoundForJson(v);
                    break;
                }
                case FieldKind::Hash:
                {
                    uint32_t v; memcpy(&v, data, 4); data += 4;
                    JoaatHash h(v);
                    out = h.ToString();
                    break;
                }
                case FieldKind::String:
                {
                    uint32_t len = ReadCountPrefix(data, field.countKind);
                    std::string s(reinterpret_cast<char*>(data), len);
                    data += len;
                    out = s;
                    break;
                }
                case FieldKind::Array:
                {
                    uint32_t count = ReadCountPrefix(data, field.countKind);
                    out = ordered_json::array();

                    for (uint32_t i = 0; i < count; i++)
                    {
                        if (field.children.size() == 1 && field.children[0].name.empty())
                        {
                            // Primitive array element
                            ordered_json elemVal;
                            data = ReadField(data, field.children[0], elemVal, nullptr, 0);
                            out.push_back(elemVal);
                        }
                        else
                        {
                            // Struct array element
                            ordered_json elemObj;
                            for (const auto& child : field.children)
                            {
                                ordered_json childVal;
                                data = ReadField(data, child, childVal, nullptr, 0);
                                elemObj[child.name] = childVal;
                            }
                            out.push_back(elemObj);
                        }
                    }
                    break;
                }
                case FieldKind::FixedArray:
                {
                    out = ordered_json::array();
                    for (int i = 0; i < field.fixedCount; i++)
                    {
                        if (field.children.size() == 1 && field.children[0].name.empty())
                        {
                            ordered_json elemVal;
                            data = ReadField(data, field.children[0], elemVal, nullptr, 0);
                            out.push_back(elemVal);
                        }
                        else
                        {
                            ordered_json elemObj;
                            for (const auto& child : field.children)
                            {
                                ordered_json childVal;
                                data = ReadField(data, child, childVal, nullptr, 0);
                                elemObj[child.name] = childVal;
                            }
                            out.push_back(elemObj);
                        }
                    }
                    break;
                }
                case FieldKind::Struct:
                {
                    ordered_json obj;
                    for (const auto& child : field.children)
                    {
                        ordered_json childVal;
                        data = ReadField(data, child, childVal, nullptr, 0);
                        obj[child.name] = childVal;
                    }
                    out = obj;
                    break;
                }
                case FieldKind::OptionalBitfield:
                {
                    uint32_t bitfield; memcpy(&bitfield, data, 4); data += 4;
                    ordered_json obj;

                    for (size_t i = 0; i < field.children.size(); i++)
                    {
                        if (bitfield & (1u << i))
                        {
                            ordered_json childVal;
                            data = ReadField(data, field.children[i], childVal, nullptr, 0);
                            obj[field.children[i].name] = childVal;
                        }
                    }
                    out = obj;
                    break;
                }
                case FieldKind::Enum:
                {
                    int64_t rawVal = 0;
                    switch (field.enumBaseKind)
                    {
                        case FieldKind::UInt8:  { uint8_t v; memcpy(&v, data, 1); data += 1; rawVal = v; break; }
                        case FieldKind::UInt16: { uint16_t v; memcpy(&v, data, 2); data += 2; rawVal = v; break; }
                        case FieldKind::UInt32: { uint32_t v; memcpy(&v, data, 4); data += 4; rawVal = v; break; }
                        case FieldKind::Int8:   { int8_t v; memcpy(&v, data, 1); data += 1; rawVal = v; break; }
                        case FieldKind::Int16:  { int16_t v; memcpy(&v, data, 2); data += 2; rawVal = v; break; }
                        case FieldKind::Int32:  { int32_t v; memcpy(&v, data, 4); data += 4; rawVal = v; break; }
                        default: { uint8_t v; memcpy(&v, data, 1); data += 1; rawVal = v; break; }
                    }
                    // Find enum string
                    std::string enumStr;
                    for (const auto& [val, name] : field.enumValues)
                    {
                        if (val == rawVal) { enumStr = name; break; }
                    }
                    if (enumStr.empty())
                        out = rawVal;
                    else
                        out = enumStr;
                    break;
                }
                case FieldKind::Placeholder:
                {
                    uint32_t sz = field.placeholderSize > 0 ? static_cast<uint32_t>(field.placeholderSize) : remainingSize;
                    std::ostringstream ss;
                    for (uint32_t i = 0; i < sz; i++)
                    {
                        if (i > 0) ss << ' ';
                        ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(data[i]);
                    }
                    out = ss.str();
                    data += sz;
                    break;
                }
            }

            if (debugJson && !field.name.empty())
            {
                (*debugJson)[field.name] = ToHex(start, static_cast<uint32_t>(data - start));
            }

            return data;
        }

        void WriteField(std::ostream& stream, const FieldDef& field, const ordered_json& val)
        {
            switch (field.kind)
            {
                case FieldKind::UInt8:
                {
                    uint8_t v = val.get<uint8_t>();
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::UInt16:
                {
                    uint16_t v = val.get<uint16_t>();
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::UInt32:
                {
                    uint32_t v = val.get<uint32_t>();
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::Int8:
                {
                    int8_t v = val.get<int8_t>();
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::Int16:
                {
                    int16_t v = val.get<int16_t>();
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::Int32:
                {
                    int32_t v = val.get<int32_t>();
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::Float:
                {
                    float v = static_cast<float>(val.get<double>());
                    IoUtils::WriteData(stream, v);
                    break;
                }
                case FieldKind::Hash:
                {
                    std::string hashStr = val.get<std::string>();
                    JoaatHash h;
                    h.FromJson(ordered_json(hashStr));
                    IoUtils::WriteData(stream, h.Hash);
                    break;
                }
                case FieldKind::String:
                {
                    std::string s = val.get<std::string>();
                    WriteCountPrefix(stream, static_cast<uint32_t>(s.size()), field.countKind);
                    stream.write(s.c_str(), s.size());
                    break;
                }
                case FieldKind::Array:
                {
                    uint32_t count = static_cast<uint32_t>(val.size());
                    WriteCountPrefix(stream, count, field.countKind);

                    for (uint32_t i = 0; i < count; i++)
                    {
                        const auto& elem = val[i];
                        if (field.children.size() == 1 && field.children[0].name.empty())
                        {
                            WriteField(stream, field.children[0], elem);
                        }
                        else
                        {
                            for (const auto& child : field.children)
                            {
                                WriteField(stream, child, elem.at(child.name));
                            }
                        }
                    }
                    break;
                }
                case FieldKind::FixedArray:
                {
                    for (int i = 0; i < field.fixedCount; i++)
                    {
                        const auto& elem = val[i];
                        if (field.children.size() == 1 && field.children[0].name.empty())
                        {
                            WriteField(stream, field.children[0], elem);
                        }
                        else
                        {
                            for (const auto& child : field.children)
                            {
                                WriteField(stream, child, elem.at(child.name));
                            }
                        }
                    }
                    break;
                }
                case FieldKind::Struct:
                {
                    for (const auto& child : field.children)
                    {
                        WriteField(stream, child, val.at(child.name));
                    }
                    break;
                }
                case FieldKind::OptionalBitfield:
                {
                    auto bitfieldPos = stream.tellp();
                    stream.seekp(4, std::ios_base::cur);

                    uint32_t bitfield = 0;
                    for (size_t i = 0; i < field.children.size(); i++)
                    {
                        if (val.contains(field.children[i].name))
                        {
                            WriteField(stream, field.children[i], val.at(field.children[i].name));
                            bitfield |= (1u << i);
                        }
                    }

                    auto endPos = stream.tellp();
                    stream.seekp(bitfieldPos);
                    IoUtils::WriteData(stream, bitfield);
                    stream.seekp(endPos);
                    break;
                }
                case FieldKind::Enum:
                {
                    int64_t rawVal = 0;
                    if (val.is_string())
                    {
                        std::string s = val.get<std::string>();
                        for (const auto& [ev, en] : field.enumValues)
                        {
                            if (en == s) { rawVal = ev; break; }
                        }
                    }
                    else
                    {
                        rawVal = val.get<int64_t>();
                    }
                    switch (field.enumBaseKind)
                    {
                        case FieldKind::UInt8:  { uint8_t v = static_cast<uint8_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                        case FieldKind::UInt16: { uint16_t v = static_cast<uint16_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                        case FieldKind::UInt32: { uint32_t v = static_cast<uint32_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                        case FieldKind::Int8:   { int8_t v = static_cast<int8_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                        case FieldKind::Int16:  { int16_t v = static_cast<int16_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                        case FieldKind::Int32:  { int32_t v = static_cast<int32_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                        default: { uint8_t v = static_cast<uint8_t>(rawVal); IoUtils::WriteData(stream, v); break; }
                    }
                    break;
                }
                case FieldKind::Placeholder:
                {
                    std::string hexStr = val.get<std::string>();
                    std::istringstream ss(hexStr);
                    ss >> std::hex;
                    int byte;
                    while (ss >> byte)
                    {
                        uint8_t b = static_cast<uint8_t>(byte);
                        IoUtils::WriteData(stream, b);
                    }
                    break;
                }
            }
        }

        uint32_t GetFieldSize(const FieldDef& field, const ordered_json& val)
        {
            switch (field.kind)
            {
                case FieldKind::UInt8:  return 1;
                case FieldKind::UInt16: return 2;
                case FieldKind::UInt32: return 4;
                case FieldKind::Int8:   return 1;
                case FieldKind::Int16:  return 2;
                case FieldKind::Int32:  return 4;
                case FieldKind::Float:  return 4;
                case FieldKind::Hash:   return 4;
                case FieldKind::String:
                {
                    uint32_t len = static_cast<uint32_t>(val.get<std::string>().size());
                    return CountPrefixSize(field.countKind) + len;
                }
                case FieldKind::Array:
                {
                    uint32_t size = CountPrefixSize(field.countKind);
                    for (const auto& elem : val)
                    {
                        size += GetElementSize(field.children, elem);
                    }
                    return size;
                }
                case FieldKind::FixedArray:
                {
                    uint32_t size = 0;
                    for (const auto& elem : val)
                    {
                        size += GetElementSize(field.children, elem);
                    }
                    return size;
                }
                case FieldKind::Struct:
                {
                    uint32_t size = 0;
                    for (const auto& child : field.children)
                    {
                        if (val.contains(child.name))
                            size += GetFieldSize(child, val.at(child.name));
                    }
                    return size;
                }
                case FieldKind::OptionalBitfield:
                {
                    uint32_t size = 4; // bitfield uint32
                    for (const auto& child : field.children)
                    {
                        if (val.contains(child.name))
                            size += GetFieldSize(child, val.at(child.name));
                    }
                    return size;
                }
                case FieldKind::Enum:
                {
                    return CountPrefixSize(field.enumBaseKind);
                }
                case FieldKind::Placeholder:
                {
                    std::string hexStr = val.get<std::string>();
                    return static_cast<uint32_t>((hexStr.size() + 1) / 3);
                }
            }
            return 0;
        }

        static void CollectElementHashOffsets(const std::vector<FieldDef>& children, const ordered_json& elemJson, std::vector<uint32_t>& offsets, uint32_t& currentOffset)
        {
            if (children.size() == 1 && children[0].name.empty())
            {
                CollectHashOffsets(children[0], elemJson, offsets, currentOffset);
                currentOffset += GetFieldSize(children[0], elemJson);
            }
            else
            {
                for (const auto& child : children)
                {
                    const auto& childVal = elemJson.contains(child.name) ? elemJson.at(child.name) : ordered_json();
                    CollectHashOffsets(child, childVal, offsets, currentOffset);
                    currentOffset += GetFieldSize(child, childVal);
                }
            }
        }

        void CollectHashOffsets(const FieldDef& field, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset)
        {
            switch (field.kind)
            {
                case FieldKind::Hash:
                {
                    if (field.trackedAsHash)
                    {
                        std::string hashStr = val.get<std::string>();
                        JoaatHash h;
                        h.FromJson(ordered_json(hashStr));
                        if (h.Hash != 0xFFFFFFFF)
                            offsets.push_back(currentOffset);
                    }
                    break;
                }
                case FieldKind::Array:
                {
                    if (field.arrayElementsAreTrackedHashes)
                    {
                        uint32_t off = currentOffset + CountPrefixSize(field.countKind);
                        for (const auto& elem : val)
                        {
                            std::string hashStr = elem.get<std::string>();
                            JoaatHash h;
                            h.FromJson(ordered_json(hashStr));
                            if (h.Hash != 0xFFFFFFFF)
                                offsets.push_back(off);
                            off += 4;
                        }
                    }
                    else
                    {
                        uint32_t off = currentOffset + CountPrefixSize(field.countKind);
                        for (const auto& elem : val)
                        {
                            CollectElementHashOffsets(field.children, elem, offsets, off);
                        }
                    }
                    break;
                }
                case FieldKind::FixedArray:
                {
                    uint32_t off = currentOffset;
                    for (const auto& elem : val)
                    {
                        CollectElementHashOffsets(field.children, elem, offsets, off);
                    }
                    break;
                }
                case FieldKind::Struct:
                {
                    uint32_t off = currentOffset;
                    for (const auto& child : field.children)
                    {
                        const auto& childVal = val.contains(child.name) ? val.at(child.name) : ordered_json();
                        CollectHashOffsets(child, childVal, offsets, off);
                        off += GetFieldSize(child, childVal);
                    }
                    break;
                }
                case FieldKind::OptionalBitfield:
                {
                    uint32_t off = currentOffset + 4; // skip bitfield
                    for (const auto& child : field.children)
                    {
                        if (val.contains(child.name))
                        {
                            CollectHashOffsets(child, val.at(child.name), offsets, off);
                            off += GetFieldSize(child, val.at(child.name));
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        static void CollectElementArchiveOffsets(const std::vector<FieldDef>& children, const ordered_json& elemJson, std::vector<uint32_t>& offsets, uint32_t& currentOffset)
        {
            if (children.size() == 1 && children[0].name.empty())
            {
                CollectArchiveOffsets(children[0], elemJson, offsets, currentOffset);
                currentOffset += GetFieldSize(children[0], elemJson);
            }
            else
            {
                for (const auto& child : children)
                {
                    const auto& childVal = elemJson.contains(child.name) ? elemJson.at(child.name) : ordered_json();
                    CollectArchiveOffsets(child, childVal, offsets, currentOffset);
                    currentOffset += GetFieldSize(child, childVal);
                }
            }
        }

        void CollectArchiveOffsets(const FieldDef& field, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset)
        {
            switch (field.kind)
            {
                case FieldKind::Hash:
                {
                    if (field.trackedAsArchive)
                    {
                        std::string hashStr = val.get<std::string>();
                        JoaatHash h;
                        h.FromJson(ordered_json(hashStr));
                        if (h.Hash != 0xFFFFFFFF)
                            offsets.push_back(currentOffset);
                    }
                    break;
                }
                case FieldKind::Array:
                {
                    uint32_t off = currentOffset + CountPrefixSize(field.countKind);
                    for (const auto& elem : val)
                    {
                        CollectElementArchiveOffsets(field.children, elem, offsets, off);
                    }
                    break;
                }
                case FieldKind::FixedArray:
                {
                    uint32_t off = currentOffset;
                    for (const auto& elem : val)
                    {
                        CollectElementArchiveOffsets(field.children, elem, offsets, off);
                    }
                    break;
                }
                case FieldKind::Struct:
                {
                    uint32_t off = currentOffset;
                    for (const auto& child : field.children)
                    {
                        const auto& childVal = val.contains(child.name) ? val.at(child.name) : ordered_json();
                        CollectArchiveOffsets(child, childVal, offsets, off);
                        off += GetFieldSize(child, childVal);
                    }
                    break;
                }
                case FieldKind::OptionalBitfield:
                {
                    uint32_t off = currentOffset + 4;
                    for (const auto& child : field.children)
                    {
                        if (val.contains(child.name))
                        {
                            CollectArchiveOffsets(child, val.at(child.name), offsets, off);
                            off += GetFieldSize(child, val.at(child.name));
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        void CollectArchiveNames(const FieldDef& field, const ordered_json& val, nlohmann::fifo_map<std::string, int>& names)
        {
            switch (field.kind)
            {
                case FieldKind::Hash:
                {
                    if (field.trackedAsArchive)
                    {
                        std::string hashStr = val.get<std::string>();
                        JoaatHash h;
                        h.FromJson(ordered_json(hashStr));
                        if (h.Hash != 0xFFFFFFFF)
                            names[hashStr] = 1;
                    }
                    break;
                }
                case FieldKind::Array:
                {
                    for (const auto& elem : val)
                    {
                        if (field.children.size() == 1 && field.children[0].name.empty())
                            CollectArchiveNames(field.children[0], elem, names);
                        else
                            for (const auto& child : field.children)
                                if (elem.contains(child.name))
                                    CollectArchiveNames(child, elem.at(child.name), names);
                    }
                    break;
                }
                case FieldKind::FixedArray:
                {
                    for (const auto& elem : val)
                    {
                        if (field.children.size() == 1 && field.children[0].name.empty())
                            CollectArchiveNames(field.children[0], elem, names);
                        else
                            for (const auto& child : field.children)
                                if (elem.contains(child.name))
                                    CollectArchiveNames(child, elem.at(child.name), names);
                    }
                    break;
                }
                case FieldKind::Struct:
                {
                    for (const auto& child : field.children)
                        if (val.contains(child.name))
                            CollectArchiveNames(child, val.at(child.name), names);
                    break;
                }
                case FieldKind::OptionalBitfield:
                {
                    for (const auto& child : field.children)
                        if (val.contains(child.name))
                            CollectArchiveNames(child, val.at(child.name), names);
                    break;
                }
                default:
                    break;
            }
        }

        // Batch operations on field vectors

        uint8_t* ReadFields(uint8_t* data, const std::vector<FieldDef>& fields, ordered_json& out, ordered_json* debugJson, uint32_t remainingSize)
        {
            for (const auto& field : fields)
            {
                ordered_json fieldVal;
                uint32_t remaining = remainingSize > 0 ? remainingSize - static_cast<uint32_t>(data - data) : 0;
                data = ReadField(data, field, fieldVal, debugJson, remaining);
                out[field.name] = fieldVal;
            }
            return data;
        }

        void WriteFields(std::ostream& stream, const std::vector<FieldDef>& fields, const ordered_json& val)
        {
            for (const auto& field : fields)
            {
                WriteField(stream, field, val.at(field.name));
            }
        }

        uint32_t GetFieldsSize(const std::vector<FieldDef>& fields, const ordered_json& val)
        {
            uint32_t size = 0;
            for (const auto& field : fields)
            {
                if (val.contains(field.name))
                    size += GetFieldSize(field, val.at(field.name));
            }
            return size;
        }

        void CollectAllHashOffsets(const std::vector<FieldDef>& fields, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset)
        {
            for (const auto& field : fields)
            {
                if (val.contains(field.name))
                {
                    CollectHashOffsets(field, val.at(field.name), offsets, currentOffset);
                    currentOffset += GetFieldSize(field, val.at(field.name));
                }
            }
        }

        void CollectAllArchiveOffsets(const std::vector<FieldDef>& fields, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset)
        {
            for (const auto& field : fields)
            {
                if (val.contains(field.name))
                {
                    CollectArchiveOffsets(field, val.at(field.name), offsets, currentOffset);
                    currentOffset += GetFieldSize(field, val.at(field.name));
                }
            }
        }

        void CollectAllArchiveNames(const std::vector<FieldDef>& fields, const ordered_json& val, nlohmann::fifo_map<std::string, int>& names)
        {
            for (const auto& field : fields)
            {
                if (val.contains(field.name))
                    CollectArchiveNames(field, val.at(field.name), names);
            }
        }
    } // namespace FieldIO
} // namespace AMT
