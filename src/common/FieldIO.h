#pragma once

#include "FieldDef.h"
#include "HashManager.h"
#include "IoUtils.h"
#include <cstdint>
#include <vector>
#include <fifo_map.hpp>
#include <nlohmann/json.hpp>

namespace AMT
{
    template <class K, class V, class dummy_compare, class A>
    using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
    using ordered_json = nlohmann::basic_json<my_workaround_fifo_map>;

    namespace FieldIO
    {
        // Read a field from binary buffer into JSON. Returns new pointer position.
        // If debugJson is non-null, writes hex debug info there.
        uint8_t* ReadField(uint8_t* data, const FieldDef& field, ordered_json& out, ordered_json* debugJson, uint32_t remainingSize);

        // Write a field from JSON to binary stream.
        void WriteField(std::ostream& stream, const FieldDef& field, const ordered_json& val);

        // Compute the binary size of a field given its JSON value.
        uint32_t GetFieldSize(const FieldDef& field, const ordered_json& val);

        // Collect hash offsets for patching.
        void CollectHashOffsets(const FieldDef& field, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset);

        // Collect archive offsets for patching.
        void CollectArchiveOffsets(const FieldDef& field, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset);

        // Collect archive names.
        void CollectArchiveNames(const FieldDef& field, const ordered_json& val, nlohmann::fifo_map<std::string, int>& names);

        // Read all fields in a schema from binary. Returns new pointer.
        uint8_t* ReadFields(uint8_t* data, const std::vector<FieldDef>& fields, ordered_json& out, ordered_json* debugJson, uint32_t remainingSize);

        // Write all fields in a schema to stream.
        void WriteFields(std::ostream& stream, const std::vector<FieldDef>& fields, const ordered_json& val);

        // Get total size of all fields.
        uint32_t GetFieldsSize(const std::vector<FieldDef>& fields, const ordered_json& val);

        // Collect hash offsets for all fields.
        void CollectAllHashOffsets(const std::vector<FieldDef>& fields, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset);

        // Collect archive offsets for all fields.
        void CollectAllArchiveOffsets(const std::vector<FieldDef>& fields, const ordered_json& val, std::vector<uint32_t>& offsets, uint32_t currentOffset);

        // Collect archive names for all fields.
        void CollectAllArchiveNames(const std::vector<FieldDef>& fields, const ordered_json& val, nlohmann::fifo_map<std::string, int>& names);

        // size of a count prefix
        uint32_t CountPrefixSize(FieldKind kind);

        // read count prefix from buffer
        uint32_t ReadCountPrefix(uint8_t*& data, FieldKind kind);

        // write count prefix to stream
        void WriteCountPrefix(std::ostream& stream, uint32_t count, FieldKind kind);
    } // namespace FieldIO
} // namespace AMT
