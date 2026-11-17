#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    void RegisterCategoriesSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 15;
        def.hasNameOffset = true;

        // CategoriesMetadataHeader
        def.headerFields = {
            UInt32("Flags") // 2-bit tristates (0 false, 1 true, 2 unset = inherit from parent):
                            // bits 0-1 MuteOnUserMusic, 2-3 Unpausable, 4-5 DisableRearAttenuation (name from GTA V),
                            // 6-7 Mute, 8-9 Solo; bits 10-31 unused
        };

        // audCategory (rage::Category). The game holds at most 13 child categories.
        def.types[0] = {0, "audCategory", {
            UInt16("ParentOverrides"),                     // bit set = don't combine with parent: 0 Volume, 1 Pitch, 2 LPFCutoff,
                                                           // 3 HPFCutoff, 4 DistanceRollOffScale, 5 OcclusionDamping,
                                                           // 6 EnvironmentalFilterDamping, 7 SourceReverbDamping,
                                                           // 8 DistanceReverbDamping, 9 EnvironmentalLoudness
            Int16("VolumeMaxDynamicRange"),                // dB*100, used at dynamic range scale 1.0; added to parent
            Int16("VolumeMinDynamicRange"),                // dB*100, used at dynamic range scale 0.0
            Int16("Pitch"),                                // cents, added to parent
            UInt16("LPFCutoff"),                           // Hz, min with parent, default 23900
            UInt16("HPFCutoff"),                           // Hz, max with parent, default 0
            UInt16("DistanceRollOffScaleMaxDynamicRange"), // x0.01, used at dynamic range scale 1.0; multiplied with parent
            UInt16("DistanceRollOffScaleMinDynamicRange"), // x0.01, used at dynamic range scale 0.0
            UInt16("OcclusionDamping"),                    // x0.01, multiplied with parent
            UInt16("EnvironmentalFilterDamping"),          // x0.01, multiplied with parent
            UInt16("SourceReverbDamping"),                 // x0.01, multiplied with parent
            UInt16("DistanceReverbDamping"),               // x0.01, multiplied with parent
            UInt16("EnvironmentalLoudness"),               // x0.01, multiplied with parent
            Array("ChildCategories", {{"", FieldKind::Hash, true}}, FieldKind::UInt8, true)
        }};

        registry["categories"] = std::move(def);
    }
} // namespace AMT
