#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"

namespace AMT
{
    class audCurve_Point : public SimpleBaseMetadataType<
        FieldType<float, "x">,
        FieldType<float, "y">
    >
    {
    public:
        audCurve_Point() = default;
    };

    class audCurve_Value : public SimpleBaseMetadataType<
        FieldType<float, "y">
    >
    {
    public:
        audCurve_Value() = default;
    };

    using audCurve_Constant = BaseMetadataType<1, "audCurve_Constant",
        FieldType<float, "Value">
    >;

    using audCurve_Linear = BaseMetadataType<2, "audCurve_Linear",
        FieldType<float, "LeftHandPairX">,
        FieldType<float, "LeftHandPairY">,
        FieldType<float, "RightHandPairX">,
        FieldType<float, "RightHandPairY">
    >;

    using audCurve_LinearDb = BaseMetadataType<3, "audCurve_LinearDb",
        FieldType<float, "LeftHandPairX">,
        FieldType<float, "LeftHandPairY">,
        FieldType<float, "RightHandPairX">,
        FieldType<float, "RightHandPairY">
    >;

    using audCurve_PiecewiseLinear = BaseMetadataType<4, "audCurve_PiecewiseLinear",
        FieldType<ArrayWrapper<audCurve_Point, uint32_t>, "Points">
    >;

    using audCurve_EqualPower = BaseMetadataType<5, "audCurve_EqualPower",
        FieldType<uint8_t, "Flip">
    >;

    using audCurve_ValueTable = BaseMetadataType<6, "audCurve_ValueTable",
        FieldType<ArrayWrapper<audCurve_Value, uint16_t>, "Values">
    >;

    using audCurve_Exponential = BaseMetadataType<7, "audCurve_Exponential",
        FieldType<uint8_t, "Flip">,
        FieldType<float, "Exponent">
    >;

    using audCurve_DecayingExponential = BaseMetadataType<8, "audCurve_DecayingExponential",
        FieldType<float, "HorizontalScaling">
    >;

    using audCurve_DecayingSquaredExponential = BaseMetadataType<9, "audCurve_DecayingSquaredExponential",
        FieldType<float, "HorizontalScaling">
    >;

    using audCurve_SineCurve = BaseMetadataType<10, "audCurve_SineCurve",
        FieldType<float, "StartPhase">,
        FieldType<float, "EndPhase">,
        FieldType<float, "Frequency">,
        FieldType<float, "VerticalScaling">,
        FieldType<float, "VerticalOffset">
    >;

    using audCurve_OneOverX = BaseMetadataType<11, "audCurve_OneOverX",
        FieldType<float, "HorizontalScaling">
    >;

    using audCurve_OneOverXSquared = BaseMetadataType<12, "audCurve_OneOverXSquared",
        FieldType<float, "HorizontalScaling">
    >;

    using audCurve_DefaultDistanceAttenuation = BaseMetadataType<13, "audCurve_DefaultDistanceAttenuation">;

    using audCurve_DefaultDistanceAttenuationClamped = BaseMetadataType<14, "audCurve_DefaultDistanceAttenuationClamped",
        FieldType<int16_t, "MaxGain">
    >;

    using audCurve_DistanceAttenuationValueTable = BaseMetadataType<15, "audCurve_DistanceAttenuationValueTable",
        FieldType<ArrayWrapper<audCurve_Value, uint16_t>, "Values">
    >;


    using CurvesMetadataHeader = BaseMetadataType
    <
        0,
        "CurvesMetadataHeader",
        FieldType<uint32_t, "Flags">,
        FieldType<uint16_t, "__unk09">,
        FieldType<uint16_t, "__unk0b">,
        FieldType<float, "MinInput">,
        FieldType<float, "MaxInput">
    >;

    using CurvesMetadataContainer = BaseMetadataContainer
    <
        CurvesMetadataHeader,
        true,
        audCurve_Constant,
        audCurve_Linear,
        audCurve_LinearDb,
        audCurve_PiecewiseLinear,
        audCurve_EqualPower,
        audCurve_ValueTable,
        audCurve_Exponential,
        audCurve_DecayingExponential,
        audCurve_DecayingSquaredExponential,
        audCurve_SineCurve,
        audCurve_OneOverX,
        audCurve_OneOverXSquared,
        audCurve_DefaultDistanceAttenuation,
        audCurve_DefaultDistanceAttenuationClamped,
        audCurve_DistanceAttenuationValueTable
    >;

    using CurvesMetadataMgr = MetadataMgr<CurvesMetadataContainer, 12>;

}; // namespace AMT
