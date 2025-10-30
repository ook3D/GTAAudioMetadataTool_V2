#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"
#include "BaseMetadata.h"

namespace AMT 
{
    enum class SoundMetadataTypeId : uint8_t
    {
        SoundsMetadataHeader = 0,
        LoopingSound = 1,
        EnvelopeSound = 2,
        TwinLoopSound = 3,
        SpeechSound = 4,
        OnStopSound = 5,
        WrapperSound = 6,
        SequentialSound = 7,
        StreamingSound = 8,
        RetriggeredOverlappedSound = 9,
        CrossfadeSound = 10,
        CollapsingStereoSound = 11,
        SimpleSound = 12,
        MultitrackSound = 13,
        RandomizedSound = 14,
        SwitchSound = 16,
        VariableCurveSound = 17,
        VariablePrintValueSound = 18,
        AssertSound = 19,
        VariableSetTimeSound = 20,
        VariableBlockSound = 21,
        IfSound = 22,
        ForLoopSound = 23,
        MathOperationSound = 24,
        
        // placeholders
        MultitrackSoundTrack = 0,
        VariableBlockSoundVariable = 0,
        MathOperationSoundOperation = 0,
        RandomizedSoundSound = 0,
        TwinLoopSoundSound = 0
    };

    // Simple sound
    class audSimpleSound : public SimpleBaseMetadataType
    <
        FieldType<uint32_t, "WaveSlotIndex">,
        FieldType<JoaatHash, "ArchiveHash", false, true>,
        FieldType<JoaatHash, "SoundHash">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::SimpleSound);
        static constexpr const char* Name = "audSimpleSound";
        
        audSimpleSound() = default;
        audSimpleSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Multitrack sound track
    class audMultitrackSound_Track : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "TrackHash", true>,
        FieldType<uint32_t, "unused">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::MultitrackSoundTrack);
        static constexpr const char* Name = "audMultitrackSound_Track";
        
        audMultitrackSound_Track() = default;
        audMultitrackSound_Track(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Multitrack sound
    class audMultitrackSound : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<audMultitrackSound_Track>, "Tracks">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::MultitrackSound);
        static constexpr const char* Name = "audMultitrackSound";
        
        audMultitrackSound() = default;
        audMultitrackSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Looping Sound
    class audLoopingSound : public SimpleBaseMetadataType
    <
        FieldType<uint16_t, "LoopCount">,
        FieldType<uint16_t, "LoopCountVariance">,
        FieldType<JoaatHash, "SoundHash", true>
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::LoopingSound);
        static constexpr const char* Name = "audLoopingSound";
        
        audLoopingSound() = default;
        audLoopingSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Envelope Sound
    class audEnvelopeSound : public SimpleBaseMetadataType
    <
        FieldType<uint16_t, "Attack">,
        FieldType<uint16_t, "Decay">, 
        FieldType<uint8_t, "Sustain">,
        FieldType<int32_t, "Hold">, 
        FieldType<int32_t, "Release">,
        FieldType<JoaatHash, "AttackCurve">,
        FieldType<JoaatHash, "DecayCurve">,
        FieldType<JoaatHash, "ReleaseCurve">,
        FieldType<JoaatHash, "AttackVariable">,
        FieldType<JoaatHash, "DecayVariable">,
        FieldType<JoaatHash, "SustainVariable">,
        FieldType<JoaatHash, "HoldVariable">,
        FieldType<JoaatHash, "ReleaseVariable">,
        FieldType<JoaatHash, "SoundHash", true>
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::EnvelopeSound);
        static constexpr const char* Name = "audEnvelopeSound";
        
        audEnvelopeSound() = default;
        audEnvelopeSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Variable Block Sound Variable
    class audVariableBlockSound_Variable : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "Hash">, 
        FieldType<float, "Data">,
        FieldType<uint8_t, "VariableType">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::VariableBlockSoundVariable);
        static constexpr const char* Name = "audVariableBlockSound_Variable";
        
        audVariableBlockSound_Variable() = default;
        audVariableBlockSound_Variable(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Variable Block Sound
    class audVariableBlockSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>,
        FieldType<ArrayWrapper<audVariableBlockSound_Variable>, "Variables">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::VariableBlockSound);
        static constexpr const char* Name = "audVariableBlockSound";
        
        audVariableBlockSound() = default;
        audVariableBlockSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    /* Math Operation */
    #define X EnumWrapperValue
    using eMathOperation = EnumWrapper<uint8_t, 
        X{"MATH_OPERATION_ADD", 0}, X{"MATH_OPERATION_SUBTRACT", 1},
        X{"MATH_OPERATION_MULTIPLY", 2}, X{"MATH_OPERATION_DIVIDE", 3},
        X{"MATH_OPERATION_SET", 4}, X{"MATH_OPERATION_MOD", 5},
        X{"MATH_OPERATION_MIN", 6}, X{"MATH_OPERATION_MAX", 7},
        X{"MATH_OPERATION_ABS", 8}, X{"MATH_OPERATION_SIGN", 9},
        X{"MATH_OPERATION_FLOOR", 10}, X{"MATH_OPERATION_CEIL", 11},
        X{"MATH_OPERATION_RAND", 12}, X{"MATH_OPERATION_SIN", 13},
        X{"MATH_OPERATION_COS", 14}, X{"MATH_OPERATION_SQRT", 15},
        X{"MATH_OPERATION_DBTOLINEAR", 16}, X{"MATH_OPERATION_LINEARTODB", 17},
        X{"MATH_OPERATION_PITCHTORATIO", 18}, X{"MATH_OPERATION_RATIOTOPITCH", 19},
        X{"MATH_OPERATION_GETTIME", 20}, X{"MATH_OPERATION_FSEL", 21},
        X{"MATH_OPERATION_VALUEINRANGE", 22}, X{"MATH_OPERATION_CLAMP", 23},
        X{"MATH_OPERATION_POW", 24}, X{"MATH_OPERATION_ROUND", 25},
        X{"MATH_OPERATION_SCALEDSIN", 26}, X{"MATH_OPERATION_SCALEDTRI", 27},
        X{"MATH_OPERATION_SCALEDSAW", 28}, X{"MATH_OPERATION_SCALEDSQUARE", 29},
        X{"MATH_OPERATION_SMOOTH", 30}, X{"MATH_OPERATION_GETSCALEDTIME", 31}
    >;
    #undef X

    // Math Operation Sound Operation
    class audMathOperationSound_Operation : public SimpleBaseMetadataType
    <
        FieldType<eMathOperation, "Operation">,
        FieldType<float, "OperandAStatic">,
        FieldType<JoaatHash, "OperandAVariable">,
        FieldType<float, "OperandBStatic">,
        FieldType<JoaatHash, "OperandBVariable">,
        FieldType<float, "OperandCStatic">,
        FieldType<JoaatHash, "OperandCVariable">,
        FieldType<JoaatHash, "OutputVariable">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::MathOperationSoundOperation);
        static constexpr const char* Name = "audMathOperationSound_Operation";
        
        audMathOperationSound_Operation() = default;
        audMathOperationSound_Operation(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Math Operation Sound
    class audMathOperationSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>,
        FieldType<ArrayWrapper<audMathOperationSound_Operation>, "Operations">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::MathOperationSound);
        static constexpr const char* Name = "audMathOperationSound";
        
        audMathOperationSound() = default;
        audMathOperationSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Variable Curve Sound
    class audVariableCurveSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>,
        FieldType<JoaatHash, "InputVariable">, 
        FieldType<JoaatHash, "OutputVariable">,
        FieldType<JoaatHash, "Curve">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::VariableCurveSound);
        static constexpr const char* Name = "audVariableCurveSound";
        
        audVariableCurveSound() = default;
        audVariableCurveSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Randomized Sound Sound
    class audRandomizedSound_Sound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>,
        FieldType<float, "Weight">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::RandomizedSoundSound);
        static constexpr const char* Name = "audRandomizedSound_Sound";
        
        audRandomizedSound_Sound() = default;
        audRandomizedSound_Sound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Randomized Sound
    class audRandomizedSound : public SimpleBaseMetadataType
    <
        FieldType<uint32_t, "unused">,
        FieldType<uint8_t, "HistoryIndex">,
        FieldType<ArrayWrapper<uint8_t>, "HistorySpace">,
        FieldType<ArrayWrapper<audRandomizedSound_Sound>, "Sounds">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::RandomizedSound);
        static constexpr const char* Name = "audRandomizedSound";
        
        audRandomizedSound() = default;
        audRandomizedSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Switch Sound
    class audSwitchSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "ControlVariable">,
        FieldType<ArrayWrapper<JoaatHash, uint8_t, true>, "Sounds">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::SwitchSound);
        static constexpr const char* Name = "audSwitchSound";
        
        audSwitchSound() = default;
        audSwitchSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Twin Loop Sound Sound
    class audTwinLoopSound_Sound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>,
        FieldType<uint32_t, "unused">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::TwinLoopSoundSound);
        static constexpr const char* Name = "audTwinLoopSound_Sound";
        
        audTwinLoopSound_Sound() = default;
        audTwinLoopSound_Sound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Twin Loop Sound
    class audTwinLoopSound : public SimpleBaseMetadataType
    <
        FieldType<int16_t, "MinSwapTime">,
        FieldType<int16_t, "MaxSwapTime">, 
        FieldType<int16_t, "MinCrossfadeTime">,
        FieldType<int16_t, "MaxCrossfadeTime">, 
        FieldType<JoaatHash, "CrossfadeCurve">,
        FieldType<JoaatHash, "MinSwapTimeVariable">,
        FieldType<JoaatHash, "MaxSwapTimeVariable">,
        FieldType<JoaatHash, "MinCrossfadeTimeVariable">,
        FieldType<JoaatHash, "MaxCrossfadeTimeVariable">,
        FieldType<ArrayWrapper<audTwinLoopSound_Sound>, "Sounds">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::TwinLoopSound);
        static constexpr const char* Name = "audTwinLoopSound";
        
        audTwinLoopSound() = default;
        audTwinLoopSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Sequential Sound
    class audSequentialSound : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<audTwinLoopSound_Sound>, "Sounds">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::SequentialSound);
        static constexpr const char* Name = "audSequentialSound";
        
        audSequentialSound() = default;
        audSequentialSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // OnStop Sound
    class audOnStopSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "ChildSound", true>,
        FieldType<JoaatHash, "OnPauseSound", true>,
        FieldType<JoaatHash, "OnEndSound", true>
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::OnStopSound);
        static constexpr const char* Name = "audOnStopSound";
        
        audOnStopSound() = default;
        audOnStopSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Retriggered Overlapped Sound
    class audRetriggeredOverlappedSound : public SimpleBaseMetadataType
    <
        FieldType<int16_t, "LoopCount">,
        FieldType<uint16_t, "DelayTime">, 
        FieldType<JoaatHash, "LoopCountVariable">,
        FieldType<JoaatHash, "DelayTimeVariable">,
        FieldType<JoaatHash, "SoundHash", true>
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::RetriggeredOverlappedSound);
        static constexpr const char* Name = "audRetriggeredOverlappedSound";
        
        audRetriggeredOverlappedSound() = default;
        audRetriggeredOverlappedSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    /* If sound */
    #define X EnumWrapperValue
    using eIfCondition = EnumWrapper<uint8_t,
        X{"IF_CONDITION_LESS_THAN", 0},
        X{"IF_CONDITION_LESS_THAN_OR_EQUAL_TO", 1},
        X{"IF_CONDITION_GREATER_THAN", 2},
        X{"IF_CONDITION_GREATER_THAN_OR_EQUAL_TO", 3},
        X{"IF_CONDITION_EQUAL_TO", 4},
        X{"IF_CONDITION_NOT_EQUAL_TO", 5}
    >;
    #undef X

    // If Sound
    class audIfSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "TrueSound", true>,
        FieldType<JoaatHash, "FalseSound", true>, 
        FieldType<JoaatHash, "VariableA">,
        FieldType<eIfCondition, "Operator">, 
        FieldType<float, "OperandBStatic">,
        FieldType<JoaatHash, "OperandBVariable">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::IfSound);
        static constexpr const char* Name = "audIfSound";
        
        audIfSound() = default;
        audIfSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // For Loop Sound
    class audForLoopSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>,
        FieldType<float, "LoopCounterInitialValue">,
        FieldType<JoaatHash, "LoopCounterInitialVariable">,
        FieldType<float, "LoopCounterConditionValue">,
        FieldType<JoaatHash, "LoopCounterConditionVariable">,
        FieldType<float, "LoopCounterIncrementValue">,
        FieldType<JoaatHash, "LoopCounterIncrementVariable">,
        FieldType<JoaatHash, "LoopCounterVariable">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::ForLoopSound);
        static constexpr const char* Name = "audForLoopSound";
        
        audForLoopSound() = default;
        audForLoopSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Variable Print Value Sound
    class audVariablePrintValueSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "VariableHash">,
        FieldType<FixedArrayWrapper<uint8_t, 15>, "Value">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::VariablePrintValueSound);
        static constexpr const char* Name = "audVariablePrintValueSound";
        
        audVariablePrintValueSound() = default;
        audVariablePrintValueSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Crossfade Sound
    class audCrossfadeSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "NearSound", true>,
        FieldType<JoaatHash, "FarSound", true>, 
        FieldType<uint8_t, "Mode">,
        FieldType<float, "MinDistance">, 
        FieldType<float, "MaxDistance">,
        FieldType<int32_t, "Hysteresis">, 
        FieldType<JoaatHash, "CrossfadeCurve">,
        FieldType<JoaatHash, "DistanceVariable">, 
        FieldType<JoaatHash, "MinDistanceVariable">,
        FieldType<JoaatHash, "MaxDistanceVariable">,
        FieldType<JoaatHash, "CrossfadeVariable">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::CrossfadeSound);
        static constexpr const char* Name = "audCrossfadeSound";
        
        audCrossfadeSound() = default;
        audCrossfadeSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Streaming Sound
    class audStreamingSound : public SimpleBaseMetadataType
    <
        FieldType<uint32_t, "Duration">,
        FieldType<ArrayWrapper<audTwinLoopSound_Sound>, "Sounds">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::StreamingSound);
        static constexpr const char* Name = "audStreamingSound";
        
        audStreamingSound() = default;
        audStreamingSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Collapsing Stereo Sound
    class audCollapsingStereoSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "LeftSound", true>,
        FieldType<JoaatHash, "RightSound", true>, 
        FieldType<float, "MinDistance">,
        FieldType<float, "MaxDistance">, 
        FieldType<JoaatHash, "MinDistanceVariable">,
        FieldType<JoaatHash, "MaxDistanceVariable">,
        FieldType<JoaatHash, "CrossfadeOverrideVariable">,
        FieldType<JoaatHash, "FrontendLeftPanVariable">,
        FieldType<JoaatHash, "FrontendRightPanVariable">, 
        FieldType<uint8_t, "Mode">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::CollapsingStereoSound);
        static constexpr const char* Name = "audCollapsingStereoSound";
        
        audCollapsingStereoSound() = default;
        audCollapsingStereoSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Wrapper Sound
    class audWrapperSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "SoundHash", true>
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::WrapperSound);
        static constexpr const char* Name = "audWrapperSound";
        
        audWrapperSound() = default;
        audWrapperSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Variable Set Time Sound
    class audVariableSetTimeSound : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "VariableHash">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::VariableSetTimeSound);
        static constexpr const char* Name = "audVariableSetTimeSound";
        
        audVariableSetTimeSound() = default;
        audVariableSetTimeSound(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Speech Sound
    class audSpeechSound : public BaseMetadataType<4, "audSpeechSound">
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::SpeechSound);
        static constexpr const char* Name = "audSpeechSound";
        
        audSpeechSound() = default;
    };

    // Assert Sound
    class audAssertSound : public BaseMetadataType<19, "audAssertSound">
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::AssertSound);
        static constexpr const char* Name = "audAssertSound";
        
        audAssertSound() = default;
    };

    // Sounds Metadata Header
    class SoundsMetadataHeader : public BaseMetadataType
    <
        0,
        "SoundsMetadataHeader", 
        FieldType<uint32_t, "Flags">,
        FieldType<uint16_t, "__unk09">,

        // Flags for the header
        FieldType<OptionalBitfieldContainer
        <
            FieldType<OptionalWrapper<int16_t>, "Volume">,
            FieldType<OptionalWrapper<uint16_t>, "VolumeVariance">,
            FieldType<OptionalWrapper<int16_t>, "Pitch">,
            FieldType<OptionalWrapper<uint16_t>, "PitchVariance">,
            FieldType<OptionalWrapper<uint16_t>, "Pan">,
            FieldType<OptionalWrapper<uint16_t>, "PanVariance">,
            FieldType<OptionalWrapper<int16_t>, "PreDelay">,
            FieldType<OptionalWrapper<uint16_t>, "PreDelayVariance">,
            FieldType<OptionalWrapper<int32_t>, "StartOffset">,
            FieldType<OptionalWrapper<int32_t>, "StartOffsetVariance">,
            FieldType<OptionalWrapper<uint16_t>, "AttackTime">,
            FieldType<OptionalWrapper<uint16_t>, "ReleaseTime">,
            FieldType<OptionalWrapper<uint16_t>, "DopplerFactor">,
            FieldType<OptionalWrapper<JoaatHash>, "CategoryHash">,
            FieldType<OptionalWrapper<JoaatHash>, "VolumeCurve">,
            FieldType<OptionalWrapper<uint16_t>, "VolumeCurveScale">,
            FieldType<OptionalWrapper<int8_t>, "SpeakerMask">,
            FieldType<OptionalWrapper<int8_t>, "EffectRoute">,
            FieldType<OptionalWrapper<JoaatHash>, "VolumeVariable">,
            FieldType<OptionalWrapper<JoaatHash>, "PitchVariable">,
            FieldType<OptionalWrapper<JoaatHash>, "PanVariable">,
            FieldType<OptionalWrapper<JoaatHash>, "UnkVariable2">,
            FieldType<OptionalWrapper<JoaatHash>, "UnkVariable3">,
            FieldType<OptionalWrapper<JoaatHash>, "CutoffVariable">
        >, "Header">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(SoundMetadataTypeId::SoundsMetadataHeader);
        static constexpr const char* Name = "SoundsMetadataHeader";
        
        SoundsMetadataHeader() = default;
        SoundsMetadataHeader(uint8_t *&data, uint32_t size) : BaseMetadataType(data, size) {}
    };

    using SoundsMetadataContainer = BaseMetadataContainer
    <
        SoundsMetadataHeader, 
        true, 
        audLoopingSound, 
        audMultitrackSound,
        audSimpleSound, 
        audEnvelopeSound, 
        audMathOperationSound,
        audVariableBlockSound, 
        audRandomizedSound, 
        audVariableCurveSound,
        audSwitchSound, 
        audTwinLoopSound, 
        audSequentialSound, 
        audOnStopSound,
        audRetriggeredOverlappedSound, 
        audIfSound, 
        audForLoopSound,
        audVariablePrintValueSound, 
        audCrossfadeSound, 
        audStreamingSound,
        audCollapsingStereoSound, 
        audWrapperSound, 
        audVariableSetTimeSound,
        audSpeechSound, 
        audAssertSound
    >;

    using SoundsMetadataMgr = MetadataMgr<SoundsMetadataContainer, 15>;
}; // namespace AMT
