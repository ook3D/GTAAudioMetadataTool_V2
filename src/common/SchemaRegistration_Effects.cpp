#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    void RegisterEffectsSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 11;
        def.hasNameOffset = true;

        // EffectsMetadataHeader
        def.headerFields = {
            UInt32("Flags"),
            UInt8("__unk08"),
            Hash("ChildEffect", true),
            UInt8("__unk0E")
        };

        // empty
        def.types[1] = {1, "audNullEffect", {}};

        def.types[2] = {2, "audReverbEffect", {
            Float("__field00"), 
            Float("__field04"),
            Float("__field08"), 
            Float("__field0C")
        }};

        // empty
        def.types[3] = {3, "audBiquadFilterEffect", {}};

        // empty
        def.types[4] = {4, "audConvolutionEffect", {}};

        // empty
        def.types[5] = {5, "audCompressorEffect", {}};

        // empty
        def.types[6] = {6, "audWaveshaperEffect", {}};

        // placeholder
        def.types[7] = {7, "audDelayEffect", { Placeholder("Data") }};

        registry["effects"] = std::move(def);
    }
} // namespace AMT
