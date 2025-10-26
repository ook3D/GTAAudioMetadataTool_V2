#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"
#include "BaseMetadata.h"

namespace AMT 
{

    using audCurve_Constant = PlaceholderMetadataType<1, "audCurve_Constant">;
    using audCurve_Linear = PlaceholderMetadataType<2, "audCurve_Linear">;
    using audCurve_LinearDb = PlaceholderMetadataType<3, "audCurve_LinearDb">;
    using audCurve_PiecewiseLinear = PlaceholderMetadataType<4, "audCurve_PiecewiseLinear">;
    using audCurve_EqualPower = PlaceholderMetadataType<5, "audCurve_EqualPower">;
    using audCurve_ValueTable = PlaceholderMetadataType<6, "audCurve_ValueTable">;
    using audCurve_Exponential = PlaceholderMetadataType<7, "audCurve_Exponential">;
    using audCurve_DecayingExponential = PlaceholderMetadataType<8, "audCurve_DecayingExponential">;
    using audCurve_DecayingSquaredExponential = PlaceholderMetadataType<9, "audCurve_DecayingSquaredExponential">;
    using audCurve_SineCurve = PlaceholderMetadataType<10, "audCurve_SineCurve">;
    using audCurve_OneOverX = PlaceholderMetadataType<11, "audCurve_OneOverX">;
    using audCurve_OneOverXSquared = PlaceholderMetadataType<12, "audCurve_OneOverXSquared">;
    using audCurve_DefaultDistanceAttenuatio = PlaceholderMetadataType<13, "audCurve_DefaultDistanceAttenuatio">;
    using audCurve_Debug = PlaceholderMetadataType<14, "audCurve_Debug">;

    
    using CurvesMetadataHeader = BaseMetadataType
    <
        0, 
        "CurvesMetadataHeader", 
        FieldType<uint32_t, "Flags">,
        FieldType<uint16_t, "__unk09">,
        FieldType<uint32_t, "__unk0d">,
        FieldType<uint32_t, "__unk11">
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
        audCurve_DefaultDistanceAttenuatio,
        audCurve_Debug
    >;

    using CurvesMetadataMgr = MetadataMgr<CurvesMetadataContainer, 12>;

}; // namespace AMT
