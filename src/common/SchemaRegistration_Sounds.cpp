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
            {15, "MATH_OPERATION_SQRT"}, // IV computes fabs(A) here (same case as ABS)
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

    static std::vector<std::pair<int64_t, std::string>> CrossfadeModeEnum()
    {
        return {
            {0, "CROSSFADE_MODE_FIXED"},     // evaluated once on play, inaudible child killed
            {1, "CROSSFADE_MODE_BOTH"},      // re-evaluated every frame, both children kept
            {2, "CROSSFADE_MODE_RETRIGGER"}  // per frame; kill/restart only outside Hysteresis
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
            UInt32("Flags"), // 2-bit tristates: 0 false, 1 true, 2 unset
            UInt16("ParentOverrides"), // bit0 Volume, 1 Pitch, 2 Pan, 3 PreDelay, 4 StartOffset, 5 Attack?, 6 Release?, 7 Doppler, 8 VolumeCurve?, 9 VolumeCurveScale, 10 SpeakerMask
            OptionalBitfield("Header", {
                Int16("Volume"),
                UInt16("VolumeVariance"),
                Int16("Pitch"),
                UInt16("PitchVariance"),
                Int16("Pan"), // -1 = no pan, else degrees
                UInt16("PanVariance"),
                UInt16("PreDelay"),
                UInt16("PreDelayVariance"),
                Int32("StartOffset"),
                Int32("StartOffsetVariance"),
                UInt16("AttackTime"),
                Int16("ReleaseTime"), // -1 = none
                UInt16("DopplerFactor"),
                Hash("CategoryHash"),
                Hash("VolumeCurve"),
                UInt16("VolumeCurveScale"),
                UInt8("SpeakerMask"),
                UInt8("EffectRoute"),
                Hash("VolumeVariable"),
                Hash("PitchVariable"),
                Hash("PanVariable"),
                Hash("PreDelayVariable"),
                Hash("StartOffsetVariable"),
                Hash("CutoffVariable")
            })
        };

        def.types[12] = {12, "audSimpleSound", {
            UInt32("WaveSlotPtr"), // runtime audWaveSlot* cache, 0 in file
            Hash("BankName", false, true),
            Hash("WaveName") // wave name within the bank, not a sound ref
        }};

        def.types[13] = {13, "audMultitrackSound", {
            Array("Tracks", {
                Hash("TrackHash", true),
                Hash("ChildName") // per-child reference name used by GetChildSoundReference
            })
        }};

        def.types[1] = {1, "audLoopingSound", {
            Int16("LoopCount"), // -1 = infinite
            UInt16("LoopCountVariance"),
            Hash("SoundHash", true)
        }};

        def.types[2] = {2, "audEnvelopeSound", {
            UInt16("Attack"), // ms
            UInt16("Decay"), // ms
            UInt8("Sustain"), // percent
            Int32("Hold"), // ms, -1 = hold until stopped
            Int32("Release"), // ms, -1 = no release
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
            UInt16("MinSwapTime"), // ms
            UInt16("MaxSwapTime"), // ms
            UInt16("MinCrossfadeTime"), // ms
            UInt16("MaxCrossfadeTime"), // ms
            Hash("CrossfadeCurve"), // must be EQUAL_POWER_RISE or LINEAR_FALL
            Hash("MinSwapTimeVariable"),
            Hash("MaxSwapTimeVariable"),
            Hash("MinCrossfadeTimeVariable"),
            Hash("MaxCrossfadeTimeVariable"),
            Array("Sounds", {
                Hash("SoundHash", true),
                Hash("ChildName")
            })
        }};

        // empty
        def.types[4] = {4, "audSpeechSound", {}};

        def.types[5] = {5, "audOnStopSound", {
            Hash("ChildSound", true),
            Hash("OnStopSound", true), // played when stopped
            Hash("OnFinishedSound", true) // played when child ends naturally
        }};

        def.types[6] = {6, "audWrapperSound", {
            Hash("SoundHash", true)
        }};

        def.types[7] = {7, "audSequentialSound", {
            Array("Sounds", {
                Hash("SoundHash", true),
                Hash("ChildName")
            })
        }};

        def.types[8] = {8, "audStreamingSound", {
            UInt32("Duration"), // ms, 0 = use wave length
            Array("Sounds", {
                Hash("SoundHash", true),
                Hash("ChildName")
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
            Enum("Mode", FieldKind::UInt8, CrossfadeModeEnum()),
            Float("MinDistance"),
            Float("MaxDistance"),
            Float("Hysteresis"),
            Hash("MinDistanceVariable"),
            Hash("MaxDistanceVariable"),
            Hash("HysteresisVariable"), // not read by the game
            Hash("CrossfadeVariable"),
            Hash("CrossfadeCurve")
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
            Enum("Mode", FieldKind::UInt8, CrossfadeModeEnum())
        }};

        def.types[14] = {14, "audRandomizedSound", {
            Hash("PlayDirectionVariable"), // sequential mode: value < 0 steps backwards
            UInt8("HistoryIndex"), // runtime
            Array("HistorySpace", {{"", FieldKind::UInt8}}), // runtime no-repeat ring buffer, 0xFF = empty
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
            String("Value") // editor text, not read by the game
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
                Float("Data"), // initial value
                UInt8("VariableType") // 1 = code-writable (requested settings) slot, else sound-local
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
