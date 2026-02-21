#include "pch.h"
#include "SpeechMetadata.h"
#include <algorithm>

namespace AMT
{

void SpeechMetadataMgr::Read(std::istream &in)
{
    uint32_t numVarData = IoUtils::ReadData<uint32_t>(in);
    m_VariationData.resize(numVarData);
    if (numVarData > 0)
        IoUtils::ReadData(in, m_VariationData.data(), numVarData);

    uint32_t numContexts = IoUtils::ReadData<uint32_t>(in);
    m_Contexts.clear();
    m_Contexts.reserve(numContexts);

    for (uint32_t i = 0; i < numContexts; i++)
    {
        ContextEntry c;
        c.bankNameIndex            = IoUtils::ReadData<uint32_t>(in);
        c.variationDataOffsetBytes = IoUtils::ReadData<int32_t>(in);
        c.nameHash                 = IoUtils::ReadData<uint32_t>(in);
        c.contextData              = IoUtils::ReadData<uint8_t>(in);
        c.numVariations            = IoUtils::ReadData<uint8_t>(in);
        m_Contexts.push_back(c);
    }

    uint32_t numVoices = IoUtils::ReadData<uint32_t>(in);
    m_Voices.clear();
    m_Voices.reserve(numVoices);

    for (uint32_t i = 0; i < numVoices; i++)
    {
        VoiceEntry v;
        uint32_t contextsOffsetBytes = IoUtils::ReadData<uint32_t>(in);
        v.nameHash    = IoUtils::ReadData<uint32_t>(in);
        v.numContexts = IoUtils::ReadData<uint16_t>(in);

        // Convert byte offset into context index (each context entry is 14 bytes)
        v.firstContextIndex = contextsOffsetBytes / 14;
        m_Voices.push_back(v);
    }

    uint32_t numStrings = IoUtils::ReadData<uint32_t>(in);
    m_Strings.clear();

    if (numStrings > 0)
    {
        std::vector<uint32_t> offsets(numStrings);
        IoUtils::ReadData(in, offsets.data(), numStrings * 4);

        // Read the rest of the stream as the string heap
        auto heapStart = in.tellg();
        in.seekg(0, std::ios::end);
        auto heapSize = in.tellg() - heapStart;
        in.seekg(heapStart);

        std::vector<char> heap(static_cast<size_t>(heapSize));
        IoUtils::ReadData(in, heap.data(), static_cast<uint32_t>(heapSize));

        m_Strings.reserve(numStrings);
        for (uint32_t i = 0; i < numStrings; i++)
        {
            std::string s(heap.data() + offsets[i]);
            std::replace(s.begin(), s.end(), '\\', '/');
            HashManager::Instance()->AddHash(s);
            m_Strings.push_back(std::move(s));
        }
    }
}

void SpeechMetadataMgr::ToJson(ordered_json &j) const
{
    // String table
    j["StringTable"] = m_Strings;

    // Voices
    ordered_json voicesObj;
    for (const auto &voice : m_Voices)
    {
        std::string voiceKey = HashManager::Instance()->HashToString(voice.nameHash);
        ordered_json voiceJson;

        ordered_json contextsArr = ordered_json::array();
        for (uint16_t ci = 0; ci < voice.numContexts; ci++)
        {
            uint32_t idx = voice.firstContextIndex + ci;
            if (idx >= m_Contexts.size()) break;

            const auto &ctx = m_Contexts[idx];
            ordered_json ctxJson;

            ctxJson["nameHash"]    = HashManager::Instance()->HashToString(ctx.nameHash);
            ctxJson["bankName"]    = (ctx.bankNameIndex < m_Strings.size())
                                   ? m_Strings[ctx.bankNameIndex]
                                   : std::to_string(ctx.bankNameIndex);
            ctxJson["contextData"] = ctx.contextData;

            // Variation data
            ordered_json varArr = ordered_json::array();
            if (ctx.variationDataOffsetBytes >= 0)
            {
                uint32_t off = static_cast<uint32_t>(ctx.variationDataOffsetBytes);
                for (uint8_t k = 0; k < ctx.numVariations && off + k < m_VariationData.size(); k++)
                {
                    varArr.push_back(m_VariationData[off + k]);
                }
            }
            ctxJson["VariationData"] = varArr;

            contextsArr.push_back(ctxJson);
        }

        voiceJson["Contexts"] = contextsArr;
        voicesObj[voiceKey] = voiceJson;
    }
    j["Voices"] = voicesObj;
}

void
SpeechMetadataMgr::FromJson(const ordered_json &j)
{
    // String table
    m_Strings.clear();
    if (j.contains("StringTable"))
    {
        for (const auto &s : j.at("StringTable"))
            m_Strings.push_back(s.get<std::string>());
    }

    // Build a map from string -> index for bank name lookups
    std::unordered_map<std::string, uint32_t> stringIndex;
    for (uint32_t i = 0; i < m_Strings.size(); i++)
        stringIndex[m_Strings[i]] = i;

    m_Voices.clear();
    m_Contexts.clear();
    m_VariationData.clear();

    if (!j.contains("Voices")) return;

    for (const auto &[voiceKey, voiceVal] : j.at("Voices").items())
    {
        VoiceEntry v;
        JoaatHash vh;
        vh.FromJson(ordered_json(voiceKey));
        v.nameHash = vh.Hash;
        v.firstContextIndex = static_cast<uint32_t>(m_Contexts.size());

        const auto &ctxArr = voiceVal.at("Contexts");
        v.numContexts = static_cast<uint16_t>(ctxArr.size());

        for (const auto &ctxJson : ctxArr)
        {
            ContextEntry c;

            JoaatHash ch;
            ch.FromJson(ctxJson.at("nameHash"));
            c.nameHash = ch.Hash;

            // Resolve bank name to index
            std::string bankName = ctxJson.at("bankName").get<std::string>();
            auto it = stringIndex.find(bankName);
            if (it != stringIndex.end())
                c.bankNameIndex = it->second;
            else
                c.bankNameIndex = static_cast<uint32_t>(std::stoul(bankName));

            c.contextData = ctxJson.at("contextData").get<uint8_t>();

            const auto &varArr = ctxJson.at("VariationData");
            if (varArr.empty())
            {
                c.variationDataOffsetBytes = -1;
                c.numVariations = static_cast<uint8_t>(varArr.size());
            }
            else
            {
                c.variationDataOffsetBytes = static_cast<int32_t>(m_VariationData.size());
                c.numVariations = static_cast<uint8_t>(varArr.size());
                for (const auto &vd : varArr)
                    m_VariationData.push_back(vd.get<uint8_t>());
            }

            m_Contexts.push_back(c);
        }

        m_Voices.push_back(v);
    }
}

void SpeechMetadataMgr::Write(std::ostream &out)
{
    IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(m_VariationData.size()));
    if (!m_VariationData.empty())
        out.write(reinterpret_cast<const char *>(m_VariationData.data()),
                  m_VariationData.size());

    IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(m_Contexts.size()));
    for (const auto &c : m_Contexts)
    {
        IoUtils::WriteData<uint32_t>(out, c.bankNameIndex);
        IoUtils::WriteData<int32_t>(out, c.variationDataOffsetBytes);
        IoUtils::WriteData<uint32_t>(out, c.nameHash);
        IoUtils::WriteData<uint8_t>(out, c.contextData);
        IoUtils::WriteData<uint8_t>(out, c.numVariations);
    }

    IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(m_Voices.size()));
    for (const auto &v : m_Voices)
    {
        // Convert context index back to byte offset (14 bytes per entry)
        uint32_t offsetBytes = v.firstContextIndex * 14;
        IoUtils::WriteData<uint32_t>(out, offsetBytes);
        IoUtils::WriteData<uint32_t>(out, v.nameHash);
        IoUtils::WriteData<uint16_t>(out, v.numContexts);
    }

    IoUtils::WriteData<uint32_t>(out, static_cast<uint32_t>(m_Strings.size()));

    if (!m_Strings.empty())
    {
        // Build offset table + heap
        std::vector<uint32_t> offsets;
        std::string heap;

        for (const auto &s : m_Strings)
        {
            std::string winStr = s;
            std::replace(winStr.begin(), winStr.end(), '/', '\\');

            offsets.push_back(static_cast<uint32_t>(heap.size()));
            heap.append(winStr);
            heap.push_back('\0');
        }

        out.write(reinterpret_cast<const char *>(offsets.data()),
                  offsets.size() * 4);
        out.write(heap.data(), heap.size());
    }
}
}; // namespace AMT
