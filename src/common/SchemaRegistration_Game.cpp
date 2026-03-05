#include "pch.h"
#include "MetadataRegistry.h"

namespace AMT
{
    static std::vector<std::pair<int64_t, std::string>> RadioTrackCategoryTypeEnum()
    {
        return {
            {0, "AD"}, 
            {1, "IDENT"}, 
            {2, "MUSIC"}, 
            {3, "NEWS"},
            {4, "WEATHER"}, 
            {5, "DJ_SOLO"}, 
            {6, "USER_INTRO"}, 
            {7, "USER_OUTRO"},
            {8, "USER_TO_AD"}, 
            {9, "USER_TO_NEWS"}
        };
    }

    void RegisterGameSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 16;
        def.hasNameOffset = true;

        // GameMetadataHeader
        def.headerFields = {
            UInt32("nametableOffset"),
            UInt8("padding")
        };

        def.types[1] = {1, "gameCollision", {
            Hash("HardImpact"),
            Hash("ScrapeSound"), 
            Hash("BreakSound"), 
            Hash("BulletImpactSound"),
            UInt16("Hardness"), 
            UInt8("MaxImpulseMag"), 
            UInt8("MaxScrapeSpeed"),
            UInt16("Unk03"), 
            UInt8("Unk04"),
            Hash("FootstepSettings"), 
            UInt8("FootstepScaling"), 
            UInt8("ScuffstepScaling"),
            Hash("ImpactStartOffsetCurve"), 
            Hash("ImpactVolCurve"),
            Hash("ScrapePitchCurve"), 
            Hash("ScrapeVolCurve"),
            Hash("FastTyreRoll"), 
            Hash("DetailTyreRoll"),
            Hash("MainSkid"), 
            Hash("SideSkid"),
            Hash("MetalShellCasing"), 
            Hash("PlasticShellCasing"),
            Hash("RollSound"), 
            Hash("RainLoop"), 
            Hash("TyreBump"),
            Hash("ShockwaveSound"), 
            Hash("RandomAmbient"),
            UInt8("Unk07"), 
            Hash("DoorMaterial")
        }};

        def.types[2] = {2, "gameAmbientEmitter", {
            Hash("ChildSound"), 
            Hash("RadioStation"),
            Float("PosX"), 
            Float("PosY"), 
            Float("PosZ"),
            UInt32("padding"),
            UInt32("InteriorRoom"), 
            Int32("Volume"),
            UInt16("LPFCutoff"),
            UInt16("HPFCutoff"), 
            UInt16("RolloffFactor"),
            Hash("Interior"), 
            UInt32("padding")
        }};

        def.types[3] = {3, "gameEmitterEntity", {
            Hash("ChildSound"), 
            Float("MaxDistance"),
            Float("BusinessHoursProbability"), 
            Float("EveningProbability"),
            Float("NightProbability"),
            Float("ConeInnerAngle"), 
            Float("ConeOuterAngle"), 
            Float("ConeMaxAtten"),
            Int32("unk2")
        }};

        // placeholder
        def.types[4] = {4, "gameHeli", { Placeholder("Data") }};

        // placeholder
        def.types[5] = {5, "gameMeleeCombat", { Placeholder("Data") }};

        def.types[6] = {6, "gameSpeechContexts", {
            Array("Contexts", {
                Hash("ContextHash"),
                UInt32("_field04"), 
                Int32("_field08"),
                UInt32("_field0C"), 
                UInt8("_field10"),
                Hash("UnkHash"), 
                UInt32("_field15")
            }, FieldKind::UInt16)
        }};

        // placeholder
        def.types[7] = {7, "gameBoat", { Placeholder("Data") }};

        def.types[8] = {8, "gameWeapon", {
            Hash("FireHash"), 
            Hash("EchoHash"), 
            Hash("CasingBounceHash"),
            Hash("SwipeSoundHash"), 
            Hash("CollisionHash"), 
            Hash("MeleeCollisionHash"),
            Hash("HeftHash"), 
            Hash("PutDownHash"), 
            Hash("RattleCollisionHash"),
            Hash("PickupSoundHash"), 
            UInt8("__field00"),
            Hash("SafetyOnSound"), 
            Hash("SafetyOffSound"),
            Hash("SlomoSwooshSound"), 
            UInt32("__field04"),
            Hash("SlomoXfadeHash"), 
            Hash("SlomoCollisionHash")
        }};

        // placeholder
        def.types[9] = {9, "gameFootsteps", { Placeholder("Data") }};

        def.types[10] = {10, "gameRadioStationList", {
            Array("Stations", {{"", FieldKind::Hash}})
        }};

        def.types[11] = {11, "gameRadioStation", {
            Int32("unused"), 
            Int32("WheelPosition"),
            UInt8("Genre"), 
            UInt8("padding01"),
            Int32("padding02"),
            Int32("padding03"),
            UInt8("AmbientRadioVol"),
            String("Name"),
            Array("TrackCategories", {{"", FieldKind::Hash}})
        }};

        def.types[12] = {12, "gameRadioStationTrackCategory", {
            Enum("Type", FieldKind::UInt8, RadioTrackCategoryTypeEnum()),
            UInt32("padding00"), 
            UInt8("padding01"),
            Array("NumHistorySpaceElems", {{"", FieldKind::Hash}}),
            UInt16("padding02"),
            UInt32("padding03"),
            Array("Tracks", {
                Hash("Context"),
                Hash("SoundRef")
            })
        }};

        def.types[13] = {13, "gameRadioStationCategoryWeights", {
            Array("Weights", {
                Enum("CategoryType", FieldKind::UInt8, RadioTrackCategoryTypeEnum()),
                Int32("Value")
            })
        }};

        def.types[14] = {14, "gameCrime", {
            Array("CrimeInstructions", {
                Hash("Hash"), 
                Float("Weight")
            }),
            Array("CrimeDescriptions", {
                Hash("Hash"),
                Float("Weight")
            })
        }};

        // placeholder
        def.types[15] = {15, "gameClothing", { Placeholder("Data") }};

        def.types[16] = {16, "gamePed", {
            Array("VoiceGroups", {
                Hash("VoiceHash"), 
                UInt32("ReferenceCount")
            }),
            Array("MiniVoiceGroups", {
                Hash("VoiceHash"), 
                UInt32("ReferenceCount")
            }),
            Array("GangVoiceGroups", {
                Hash("VoiceHash"), 
                UInt32("ReferenceCount")
            })
        }};

        def.types[17] = {17, "gameAmbientEmitterList", {
            Array("Ambient Emitters", {{"", FieldKind::Hash}}, FieldKind::UInt16)
        }};

        // placeholder
        def.types[18] = {18, "gameScriptedReport", { Placeholder("Data") }};

        def.types[19] = {19, "gameAmbientZone", {
            Float("MinX"), 
            Float("MinY"), 
            Float("MinZ"),
            Float("MaxX"), 
            Float("MaxY"), 
            Float("MaxZ"),
            UInt8("RulesCount"),
            Array("Rules", {{"", FieldKind::Hash}})
        }};

        def.types[20] = {20, "gameSoundRules", {
            Float("Weight"), 
            Float("OffsetX"), 
            Float("OffsetY"),
            UInt8("HoursStart"), 
            UInt8("HoursEnd"),
            Int16("__field0f"),
            Hash("SoundHash"), 
            Hash("CategoryHash"),
            UInt32("Unknown")
        }};

        def.types[21] = {21, "gameAmbientZoneList", {
            Array("Zones", {{"", FieldKind::Hash}})
        }};

        // placeholder
        def.types[22] = {22, "gameTrainStation", { Placeholder("Data") }};

        def.types[23] = {23, "gameCutscene", {
            Array("Categories", {
                Hash("Category"), 
                UInt8("Intensity")
            })
        }};

        def.types[24] = {24, "gameInterior", {
            Array("InteriorRooms", {
                Hash("RoomName"), 
                Int8("padding"),
                Float("ReverbLarge"), 
                Float("ReverbMedium"), 
                Float("ReverbSmall"),
                Hash("RoomSound"), 
                Int8("RainType"),
                Float("ExteriorAudibility"), 
                Float("RoomOcclusionDamping"),
                Float("NonMarkedPortalOcclusion"), 
                Float("DistanceFromPortalForOcclusion"),
                Int8("padding"), 
                Float("DistanceFromPortalFadeDistance"),
                Hash("WeaponMetrics"), 
                Hash("InteriorWallaSoundSet")
            })
        }};

        // gameDoor (id=25)
        def.types[25] = {25, "gameDoor", {
            Hash("Brush"), 
            Hash("Limit"), 
            Hash("Open"), 
            Hash("Close"), 
            Hash("unk00")
        }};

        def.types[0] = {0, "gameAutomobile", {
            Int32("masterVolume"), 
            Int32("MaxConeAttenuation"),
            Hash("lowEngineLoop"), 
            Hash("highEngineLoop"),
            Hash("lowExhaustLoop"), 
            Hash("highExhaustLoop"),
            Hash("revsOffLoop"),
            Float("unk1"), 
            Float("unk2"), 
            Int32("unk3"), 
            Int32("unk4"),
            Float("unk5"), 
            Float("unk6"), 
            Int32("unk7"), 
            Int32("unk8"),
            Float("unk9"), 
            Float("unk10"), 
            Int32("unk11"), 
            Int32("unk12"),
            Float("unk13"), 
            Float("unk14"),
            Int32("unk15"), 
            Int32("unk16"), 
            Int32("unk17"),
            Float("unk18"), 
            Float("unk19"), 
            Int32("unk20"), 
            Int32("unk21"),
            Hash("engineWaveShape"), 
            UInt32("unk22"),
            Hash("exhaustWaveShape"), 
            UInt32("unk23"),
            Int32("MinPitch"), 
            Int32("MaxPitch"),
            Hash("engineIdleLoopSound"), 
            Hash("exhaustIdleLoopSound"),
            Int32("IdleMinPitch"), 
            Int32("IdleMaxPitch"),
            Hash("transmissionSound"), 
            Int32("TransWhineMinPitch"), 
            Int32("TransWhineMaxPitch"),
            Hash("InductionLoop"), 
            Int32("InductionMinPitch"), 
            Int32("InductionMaxPitch"),
            Hash("exhaustPopSound"),
            Hash("TurboWhine"), 
            Int32("TurboMinPitch"), 
            Int32("TurboMaxPitch"),
            Hash("dumpValveSound"), 
            Hash("startupRevs"),
            Hash("hornSounds"), 
            Hash("doorOpenSound"), 
            Hash("doorCloseSound"),
            Hash("bootOpenSound"), 
            Hash("bootCloseSound"),
            Float("BrakeSqueekFactor"),
            Hash("suspensionUpSound"), 
            Hash("suspensionDownSound"),
            Float("minSuspCompThresh"), 
            Float("maxSuspCompThresh"),
            Hash("policeScannerManufacturerSound"), 
            Hash("policeScannerModelSound"),
            Hash("policeScannerVehicleCategorySound"),
            Hash("gearTransmissionSound"),
            Int32("GearTransMinPitch"), 
            Int32("GearTransMaxPitch"),
            Int32("GTThrottleVol"), 
            Int32("DumpValveProb"),
            Int32("TurboSpinUpSpeed"), 
            Int32("VolumeBoost"),
            Int32("ExhaustBoost"), 
            Int32("TransmissionBoost"),
            Hash("jumpLandSound"), 
            Int32("JumpLandMinThresh"), 
            Int32("JumpLandMaxThresh"),
            Hash("ignitionSound"), 
            Hash("engineShutDownSound"),
            Int8("VolumeCategory"), 
            Int8("GPSType"), 
            Int8("RadioType"), 
            Int8("RadioGenre"),
            Hash("indicatorOnSound"), 
            Hash("indicatorOffSound"),
            Hash("coolingFanSound"), 
            Hash("handbrakeSound2"),
            Hash("nullSound2"), 
            Hash("nullSound3"), 
            Hash("handbrakeSound"),
            Int16("GpsVoice"), 
            Int8("RadioLeakage")
        }};

        registry["game"] = std::move(def);
    }
} // namespace AMT
