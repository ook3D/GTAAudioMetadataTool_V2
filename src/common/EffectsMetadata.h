#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"
#include "BaseMetadata.h"

namespace AMT 
{

    // Null Effect
    using audNullEffect = BaseMetadataType<1, "audNullEffect">;

    /* Reverb effect */
    using audReverbEffect = BaseMetadataType
    <
        2, 
        "audReverbEffect", 
        FieldType<float, "__field00">,
        FieldType<float, "__field04">,
        FieldType<float, "__field08">,
        FieldType<float, "__field0C">
    >;

    // Biquad Filter
    using audBiquadFilterEffect = BaseMetadataType<3, "audBiquadFilterEffect">;

    // Convolution Effect
    using audConvolutionEffect = BaseMetadataType<4, "audConvolutionEffect">;

    // Compressor effect
    using audCompressorEffect = BaseMetadataType<5, "audCompressorEffect">;

    // Waveshaper effect
    using audWaveshaperEffect = BaseMetadataType<6, "audWaveshaperEffect">;

    // Delay Effect
    using audDelayEffect = PlaceholderMetadataType<7, "audDelayEffect">;

    using EffectsMetadataHeader = BaseMetadataType
    <
        0, 
        "EffectsMetadataHeader", 
        FieldType<uint32_t, "Flags">,
        FieldType<uint8_t, "__unk08">,
        FieldType<JoaatHash, "ChildEffect", true>,
        FieldType<uint8_t, "__unk0E">
    >;

    using EffectsMetadataContainer = BaseMetadataContainer
    <
        EffectsMetadataHeader,
        true,
        audNullEffect,
        audReverbEffect,
        audBiquadFilterEffect,
        audConvolutionEffect,
        audCompressorEffect,
        audWaveshaperEffect,
        audDelayEffect
    >;

    using EffectsMetadataMgr = MetadataMgr<EffectsMetadataContainer, 11>;
}; // namespace AMT
