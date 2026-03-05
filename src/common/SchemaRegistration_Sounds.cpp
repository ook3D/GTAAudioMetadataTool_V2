#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    static std::vector<std::pair<int64_t, std::string>> MathOperationEnum()
    {
        return {
            {0, "MATH_OPERATION_ADD"}, 
            {1, "MATH_OPERATION_SUBTRACT"},
            {2, "MATH_OPERATION_MULTIPLY"}, 
            {3, "MATH_OPERATION_DIVIDE"},
            {4, "MATH_OPERATION_SET"}, 
            {5, "MATH_OPERATION_MOD"},
            {6, "MATH_OPERATION_MIN"}, 
            {7, "MATH_OPERATION_MAX"},
            {8, "MATH_OPERATION_ABS"}, 
            {9, "MATH_OPERATION_SIGN"},
            {10, "MATH_OPERATION_FLOOR"}, 
            {11, "MATH_OPERATION_CEIL"},
            {12, "MATH_OPERATION_RAND"}, 
            {13, "MATH_OPERATION_SIN"},
            {14, "MATH_OPERATION_COS"}, 
            {15, "MATH_OPERATION_SQRT"},
            {16, "MATH_OPERATION_DBTOLINEAR"}, 
            {17, "MATH_OPERATION_LINEARTODB"},
            {18, "MATH_OPERATION_PITCHTORATIO"}, 
            {19, "MATH_OPERATION_RATIOTOPITCH"},
            {20, "MATH_OPERATION_GETTIME"}, 
            {21, "MATH_OPERATION_FSEL"},
            {22, "MATH_OPERATION_VALUEINRANGE"}, 
            {23, "MATH_OPERATION_CLAMP"},
            {24, "MATH_OPERATION_POW"}, 
            {25, "MATH_OPERATION_ROUND"},
            {26, "MATH_OPERATION_SCALEDSIN"}, 
            {27, "MATH_OPERATION_SCALEDTRI"},
            {28, "MATH_OPERATION_SCALEDSAW"}, 
            {29, "MATH_OPERATION_SCALEDSQUARE"},
            {30, "MATH_OPERATION_SMOOTH"},
            {31, "MATH_OPERATION_GETSCALEDTIME"}
        };
    }

    static std::vector<std::pair<int64_t, std::string>> IfConditionEnum()
    {
        return {
            {0, "IF_CONDITION_LESS_THAN"},
            {1, "IF_CONDITION_LESS_THAN_OR_EQUAL_TO"},
            {2, "IF_CONDITION_GREATER_THAN"},
            {3, "IF_CONDITION_GREATER_THAN_OR_EQUAL_TO"},
            {4, "IF_CONDITION_EQUAL_TO"},
            {5, "IF_CONDITION_NOT_EQUAL_TO"}
        };
    }

    void RegisterSoundsSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 15;
        def.hasNameOffset = true;

        def.headerFields = {
            UInt32("Flags"),
            UInt16("__unk09"),
            OptionalBitfield("Header", {
                Int16("Volume"),
                UInt16("VolumeVariance"),
                Int16("Pitch"),
                UInt16("PitchVariance"),
                UInt16("Pan"),
                UInt16("PanVariance"),
                Int16("PreDelay"),
                UInt16("PreDelayVariance"),
                Int32("StartOffset"),
                Int32("StartOffsetVariance"),
                UInt16("AttackTime"),
                UInt16("ReleaseTime"),
                UInt16("DopplerFactor"),
                Hash("CategoryHash"),
                Hash("VolumeCurve"),
                UInt16("VolumeCurveScale"),
                Int8("SpeakerMask"),
                Int8("EffectRoute"),
                Hash("VolumeVariable"),
                Hash("PitchVariable"),
                Hash("PanVariable"),
                Hash("UnkVariable2"),
                Hash("UnkVariable3"),
                Hash("CutoffVariable")
            })
        };

        def.types[12] = {12, "audSimpleSound", {
            UInt32("WaveSlotIndex"),
            Hash("ArchiveHash", false, true),
            Hash("SoundHash")
        }};

        def.types[13] = {13, "audMultitrackSound", {
            Array("Tracks", {
                Hash("TrackHash", true),
                UInt32("unused")
            })
        }};

        def.types[1] = {1, "audLoopingSound", {
            UInt16("LoopCount"),
            UInt16("LoopCountVariance"),
            Hash("SoundHash", true)
        }};

        def.types[2] = {2, "audEnvelopeSound", {
            UInt16("Attack"),
            UInt16("Decay"),
            UInt8("Sustain"),
            Int32("Hold"),
            Int32("Release"),
            Hash("AttackCurve"),
            Hash("DecayCurve"),
            Hash("ReleaseCurve"),
            Hash("AttackVariable"),
            Hash("DecayVariable"),
            Hash("SustainVariable"),
            Hash("HoldVariable"),
            Hash("ReleaseVariable"),
            Hash("SoundHash", true)
        }};

        def.types[3] = {3, "audTwinLoopSound", {
            Int16("MinSwapTime"),
            Int16("MaxSwapTime"),
            Int16("MinCrossfadeTime"),
            Int16("MaxCrossfadeTime"),
            Hash("CrossfadeCurve"),
            Hash("MinSwapTimeVariable"),
            Hash("MaxSwapTimeVariable"),
            Hash("MinCrossfadeTimeVariable"),
            Hash("MaxCrossfadeTimeVariable"),
            Array("Sounds", {
                Hash("SoundHash", true),
                UInt32("unused")
            })
        }};

        // empty
        def.types[4] = {4, "audSpeechSound", {}};

        def.types[5] = {5, "audOnStopSound", {
            Hash("ChildSound", true),
            Hash("OnPauseSound", true),
            Hash("OnEndSound", true)
        }};

        def.types[6] = {6, "audWrapperSound", {
            Hash("SoundHash", true)
        }};

        def.types[7] = {7, "audSequentialSound", {
            Array("Sounds", {
                Hash("SoundHash", true),
                UInt32("unused")
            })
        }};

        def.types[8] = {8, "audStreamingSound", {
            UInt32("Duration"),
            Array("Sounds", {
                Hash("SoundHash", true),
                UInt32("unused")
            })
        }};

        def.types[9] = {9, "audRetriggeredOverlappedSound", {
            Int16("LoopCount"),
            UInt16("DelayTime"),
            Hash("LoopCountVariable"),
            Hash("DelayTimeVariable"),
            Hash("SoundHash", true)
        }};

        def.types[10] = {10, "audCrossfadeSound", {
            Hash("NearSound", true),
            Hash("FarSound", true),
            UInt8("Mode"),
            Float("MinDistance"),
            Float("MaxDistance"),
            Int32("Hysteresis"),
            Hash("CrossfadeCurve"),
            Hash("DistanceVariable"),
            Hash("MinDistanceVariable"),
            Hash("MaxDistanceVariable"),
            Hash("CrossfadeVariable")
        }};

        def.types[11] = {11, "audCollapsingStereoSound", {
            Hash("LeftSound", true),
            Hash("RightSound", true),
            Float("MinDistance"),
            Float("MaxDistance"),
            Hash("MinDistanceVariable"),
            Hash("MaxDistanceVariable"),
            Hash("CrossfadeOverrideVariable"),
            Hash("FrontendLeftPanVariable"),
            Hash("FrontendRightPanVariable"),
            UInt8("Mode")
        }};

        def.types[14] = {14, "audRandomizedSound", {
            UInt32("unused"),
            UInt8("HistoryIndex"),
            Array("HistorySpace", {{"", FieldKind::UInt8}}),
            Array("Sounds", {
                Hash("SoundHash", true),
                Float("Weight")
            })
        }};

        def.types[16] = {16, "audSwitchSound", {
            Hash("ControlVariable"),
            Array("Sounds", {{"", FieldKind::Hash, true}}, FieldKind::UInt8, true)
        }};

        def.types[17] = {17, "audVariableCurveSound", {
            Hash("SoundHash", true),
            Hash("InputVariable"),
            Hash("OutputVariable"),
            Hash("Curve")
        }};

        def.types[18] = {18, "audVariablePrintValueSound", {
            Hash("VariableHash"),
            FixedArray("Value", {{"", FieldKind::UInt8}}, 15)
        }};

        // empty
        def.types[19] = {19, "audAssertSound", {}};

        def.types[20] = {20, "audVariableSetTimeSound", {
            Hash("VariableHash")
        }};

        def.types[21] = {21, "audVariableBlockSound", {
            Hash("SoundHash", true),
            Array("Variables", {
                Hash("Hash"),
                Float("Data"),
                UInt8("VariableType")
            })
        }};

        def.types[22] = {22, "audIfSound", {
            Hash("TrueSound", true),
            Hash("FalseSound", true),
            Hash("VariableA"),
            Enum("Operator", FieldKind::UInt8, IfConditionEnum()),
            Float("OperandBStatic"),
            Hash("OperandBVariable")
        }};

        def.types[23] = {23, "audForLoopSound", {
            Hash("SoundHash", true),
            Float("LoopCounterInitialValue"),
            Hash("LoopCounterInitialVariable"),
            Float("LoopCounterConditionValue"),
            Hash("LoopCounterConditionVariable"),
            Float("LoopCounterIncrementValue"),
            Hash("LoopCounterIncrementVariable"),
            Hash("LoopCounterVariable")
        }};

        def.types[24] = {24, "audMathOperationSound", {
            Hash("SoundHash", true),
            Array("Operations", {
                Enum("Operation", FieldKind::UInt8, MathOperationEnum()),
                Float("OperandAStatic"),
                Hash("OperandAVariable"),
                Float("OperandBStatic"),
                Hash("OperandBVariable"),
                Float("OperandCStatic"),
                Hash("OperandCVariable"),
                Hash("OutputVariable")
            })
        }};

        registry["sounds"] = std::move(def);
    }
} // namespace AMT
