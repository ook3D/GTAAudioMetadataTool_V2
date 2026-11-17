#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    void RegisterEffectsSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 11;
        def.hasNameOffset = true;

        // EffectsMetadataHeader (+0 ClassId and +1 NameTableOffset are handled by the container)
        def.headerFields = {
            UInt32("Flags"),             // +5, always 0 in IV data; not read by effect code
            UInt8("__unk09"),            // +9, always 0; not read
            Hash("NextEffect", true),    // +10, next effect in the serial chain, -1 = end
            UInt8("ChannelMask")         // +14, channels processed; IV data reads MSB-first (0x80 = ch0, 0xCC = FL/FR/SL/SR, 0xEF = all but LFE)
        };

        def.types[1] = {1, "audNullEffect", {}};

        def.types[2] = {2, "audReverbEffect", {
            Float("RoomSize"),           // 0..1
            Float("Wet"),                // 0..1
            Float("Dry"),                // 0..1
            Float("Damping")             // 0..1
        }};

        // parameters are set at runtime by game code
        def.types[3] = {3, "audBiquadFilterEffect", {}};

        def.types[4] = {4, "audConvolutionEffect", {
            String("ImpulseResponseFile")   // asset name of the impulse response
        }};

        // parameters come from global compressor defaults
        def.types[5] = {5, "audCompressorEffect", {}};

        // parameters are set at runtime
        def.types[6] = {6, "audWaveshaperEffect", {}};

        def.types[7] = {7, "audDelayEffect", {
            UInt16("Time"),              // ms
            Int16("Gain"),               // hundredths of dB
            Int16("FeedbackGain")        // hundredths of dB
        }};

        registry["effects"] = std::move(def);
    }
} // namespace AMT
