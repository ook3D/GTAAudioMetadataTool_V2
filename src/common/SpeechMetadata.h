#pragma once

#include "BaseTypes.h"
#include "IoUtils.h"
#include "HashManager.h"
#include <vector>
#include <string>
#include <cstring>

namespace AMT
{
    class SpeechMetadataMgr
    {
    public:
        void Read  (std::istream &in);
        void Write (std::ostream &out);
        void ToJson  (ordered_json &j) const;
        void FromJson(const ordered_json &j);

    private:

        struct ContextEntry
        {
            uint32_t bankNameIndex;
            int32_t  variationDataOffsetBytes;
            uint32_t nameHash;
            uint8_t  contextData;
            uint8_t  numVariations;
        };

        struct VoiceEntry
        {
            uint32_t nameHash;
            uint16_t numContexts;
            uint32_t firstContextIndex; // index into m_Contexts
        };

        // raw variation data bytes
        std::vector<uint8_t> m_VariationData;

        // all context entries (laid out voice-by-voice)
        std::vector<ContextEntry> m_Contexts;

        // voice entries
        std::vector<VoiceEntry> m_Voices;

        // string table (bank names)
        std::vector<std::string> m_Strings;
    };

}; // namespace AMT
