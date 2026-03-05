#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    void RegisterCurvesSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 12;
        def.hasNameOffset = true;

        // CurvesMetadataHeader
        def.headerFields = {
            UInt32("Flags"),
            UInt16("__unk09"),
            UInt16("__unk0b"),
            Float("MinInput"),
            Float("MaxInput")
        };

        def.types[1] = {1, "audCurve_Constant", {
            Float("Value")
        }};

        def.types[2] = {2, "audCurve_Linear", {
            Float("LeftHandPairX"), 
            Float("LeftHandPairY"),
            Float("RightHandPairX"), 
            Float("RightHandPairY")
        }};

        def.types[3] = {3, "audCurve_LinearDb", {
            Float("LeftHandPairX"), 
            Float("LeftHandPairY"),
            Float("RightHandPairX"), 
            Float("RightHandPairY")
        }};

        def.types[4] = {4, "audCurve_PiecewiseLinear", {
            Array("Points", { Float("x"), Float("y") }, FieldKind::UInt32)
        }};

        def.types[5] = {5, "audCurve_EqualPower", {
            UInt8("Flip")
        }};

        def.types[6] = {6, "audCurve_ValueTable", {
            Array("Values", { Float("y") }, FieldKind::UInt16)
        }};

        def.types[7] = {7, "audCurve_Exponential", {
            UInt8("Flip"), Float("Exponent")
        }};

        def.types[8] = {8, "audCurve_DecayingExponential", {
            Float("HorizontalScaling")
        }};

        def.types[9] = {9, "audCurve_DecayingSquaredExponential", {
            Float("HorizontalScaling")
        }};

        def.types[10] = {10, "audCurve_SineCurve", {
            Float("StartPhase"), 
            Float("EndPhase"),
            Float("Frequency"), 
            Float("VerticalScaling"), 
            Float("VerticalOffset")
        }};

        def.types[11] = {11, "audCurve_OneOverX", {
            Float("HorizontalScaling")
        }};

        def.types[12] = {12, "audCurve_OneOverXSquared", {
            Float("HorizontalScaling")
        }};

        // empty
        def.types[13] = {13, "audCurve_DefaultDistanceAttenuation", {}};

        def.types[14] = {14, "audCurve_DefaultDistanceAttenuationClamped", {
            Int16("MaxGain")
        }};

        def.types[15] = {15, "audCurve_DistanceAttenuationValueTable", {
            Array("Values", { Float("y") }, FieldKind::UInt16)
        }};

        registry["curves"] = std::move(def);
    }
} // namespace AMT
