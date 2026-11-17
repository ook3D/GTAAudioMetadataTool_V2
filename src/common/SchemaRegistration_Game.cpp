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

    static std::vector<std::pair<int64_t, std::string>> RadioGenreEnum()
    {
        return {
            {0, "UNSPECIFIED"}, {1, "JAZZ"}, {2, "PUNK"}, {3, "DANCE"}, {4, "REGGAE"}, {5, "REGGAETON"}, {6, "TALK"},
            {7, "HIPHOP"}, {8, "DISCOFUNK"}, {9, "CLASSIC_ROCK"}, {10, "RUSSIAN"}, {11, "RNB"}, {12, "AMBIENT"}, {13, "OFF"}
        };
    }

    static std::vector<std::pair<int64_t, std::string>> SpeechVolumeTypeEnum()
    {
        return {
            {0, "NORMAL"}, {1, "SHOUTED"}, {2, "PAIN"}, {3, "MEGAPHONE"}, {4, "HELI_MEGAPHONE"}
        };
    }

    void RegisterGameSchema(std::map<std::string, MetadataFileDef>& registry)
    {
        MetadataFileDef def;
        def.suffix = 16;
        def.hasNameOffset = true;

        // GameMetadataHeader
        def.headerFields = {
            UInt32("Flags"),
            UInt8("padding")
        };

        def.types[1] = {1, "gameCollision", {
            Hash("HardImpact"),
            Hash("ScrapeSound"),
            Hash("BreakSound"),
            Hash("BulletImpactSound"),
            UInt8("Hardness"),
            UInt8("MinImpulseMag"),
            UInt8("MaxImpulseMag"),
            UInt8("MaxScrapeSpeed"),
            UInt16("ImpulseMagScalar"),
            UInt8("BulletCollisionScaling"),
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
            UInt8("MaterialType"),
            Hash("DoorSettings")
        }};

        def.types[2] = {2, "gameAmbientEmitter", {
            Hash("ChildSound"),
            Hash("RadioStation"),
            Float("PosX"),
            Float("PosY"),
            Float("PosZ"),
            Float("MinDistance"),
            Float("MaxDistance"),
            Int32("Volume"),
            UInt16("LPFCutoff"),
            UInt16("HPFCutoff"),
            UInt16("RolloffFactor"),
            Hash("Interior"),
            UInt32("InteriorRoomIndex")
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
            UInt32("StopAfterLoudSoundTime")     // ms
        }};

        // rage::HeliAudioSettings
        def.types[4] = {4, "gameHeli", {
            Hash("RotorLoop"),
            Hash("RearRotorLoop"),
            Hash("ExhaustLoop"),
            Hash("BankingLoop"),
            Hash("CabinToneLoop"),
            Float("ThrottleSmoothRate"),
            Hash("BankAngleVolumeCurve"),
            Hash("BankThrottleVolumeCurve"),
            Hash("BankThrottlePitchCurve"),
            Hash("RotorThrottleVolumeCurve"),
            Hash("RotorThrottlePitchCurve"),
            Hash("RotorThrottleGapCurve"),
            Hash("RearRotorThrottleVolumeCurve"),
            Hash("ExhaustThrottleVolumeCurve"),
            Hash("ExhaustThrottlePitchCurve"),
            UInt16("RotorConeFrontAngle"),
            UInt16("RotorConeRearAngle"),
            Int16("RotorConeAtten"),
            UInt16("RearRotorConeFrontAngle"),
            UInt16("RearRotorConeRearAngle"),
            Int16("RearRotorConeAtten"),
            UInt16("ExhaustConeFrontAngle"),
            UInt16("ExhaustConeRearAngle"),
            Int16("ExhaustConeAtten"),
            Float("Filter1Frequency"),
            Float("Filter1Bandwidth"),
            Hash("Filter1ThrottleResonanceCurve"),
            Float("Filter2Frequency"),
            Float("Filter2Bandwidth"),
            Hash("Filter2ThrottleResonanceCurve"),
            Hash("BankingResonanceCurve"),
            Hash("RotorVolumeStartupCurve"),
            Hash("BladeVolumeStartupCurve"),
            Hash("RotorPitchStartupCurve"),
            Hash("RearRotorVolumeStartupCurve"),
            Hash("ExhaustVolumeStartupCurve"),
            Hash("RotorGapStartupCurve"),
            Hash("StartUpOneShot"),
            UInt16("BladeConeUpAngle"),
            UInt16("BladeConeDownAngle"),
            Int16("BladeConeAtten"),
            UInt16("ThumpConeUpAngle"),
            UInt16("ThumpConeDownAngle"),
            Int16("ThumpConeAtten"),
            Hash("PoliceScannerManufacturer"),
            Hash("PoliceScannerModel"),
            Hash("PoliceScannerVehicleCategory"),
            UInt8("RadioType"),
            UInt8("RadioGenre"),
            Hash("DoorOpen"),
            Hash("DoorClose"),
            Hash("DoorLimit"),
            Hash("DamageLoop")
        }};

        def.types[5] = {5, "gameMeleeCombat", {
            Hash("SwipeSound"),       // "OVERRIDE" -> weapon SwipeSound
            Hash("HitSound"),         // non-ped target; "OVERRIDE" -> weapon CollisionSound
            Hash("HitPedSound"),      // ped target; "OVERRIDE" -> weapon MeleeCollisionSound
            Hash("ResponseSound"),    // played on victim ped
            Hash("SlomoHitPedSound"),
            Int32("Unk20"),           // 5, or 12 (JAB); not read by game
            Hash("Unk24")             // not read by game
        }};

        def.types[6] = {6, "gameSpeechContexts", {
            Array("Contexts", {
                Hash("ContextName"),
                Int32("RepeatTime"),
                Int32("RepeatTimeOnSameVoice"),
                Enum("VolumeType", FieldKind::UInt8, SpeechVolumeTypeEnum()),
                Hash("GenderNonSpecificVersion"),
                Hash("DrunkVersion"),
                UInt32("TimeCanNextPlay")
            }, FieldKind::UInt16)
        }};

        // rage::BoatAudioSettings
        def.types[7] = {7, "gameBoat", {
            Hash("Engine1Loop"),
            Hash("Engine1VolumeCurve"),
            Hash("Engine1PitchCurve"),
            Hash("Engine2Loop"),
            Hash("Engine2VolumeCurve"),
            Hash("Engine2PitchCurve"),
            Hash("LowResoLoop"),
            Hash("LowResoVolumeCurve"),
            Hash("LowResoPitchCurve"),
            Hash("ResoLoop"),
            Hash("ResoVolumeCurve"),
            Hash("ResoPitchCurve"),
            Hash("WaterTurbulenceLoop"),
            Hash("WaterTurbulenceVolumeCurve"),
            Hash("WaterTurbulencePitchCurve"),
            Float("EngineFilter1Frequency"),
            Float("EngineFilter1Bandwidth"),
            Int32("EngineFilter1GainMin"),
            Int32("EngineFilter1GainMax"),
            Float("EngineFilter2Frequency"),
            Float("EngineFilter2Bandwidth"),
            Int32("EngineFilter2GainMin"),
            Int32("EngineFilter2GainMax"),
            Float("ExhaustFilter1Frequency"),
            Float("ExhaustFilter1Bandwidth"),
            Int32("ExhaustFilter1GainMin"),
            Int32("ExhaustFilter1GainMax"),
            Float("ExhaustFilter2Frequency"),
            Float("ExhaustFilter2Bandwidth"),
            Int32("ExhaustFilter2GainMin"),
            Int32("ExhaustFilter2GainMax"),
            Int32("ExhaustFilter2ThrottleResonanceGain"),
            Float("ExhaustFilter3Frequency"),
            Float("ExhaustFilter3Bandwidth"),
            Int32("ExhaustFilter3GainMin"),
            Int32("ExhaustFilter3GainMax"),
            Hash("Unk144"),            // not read by game
            Int32("Unk148"),           // not read by game
            Hash("Unk152"),            // not read by game
            Int32("Unk156"),           // not read by game
            Hash("PoliceScannerManufacturer"),
            Hash("PoliceScannerModel"),
            Hash("PoliceScannerVehicleCategory"),
            UInt8("RadioType"),
            UInt8("RadioGenre"),
            Hash("HornLoop"),
            Hash("CollisionMaterial"),
            Hash("IgnitionOneShot"),
            Hash("ShutdownOneShot")
        }};

        def.types[8] = {8, "gameWeapon", {
            Hash("FireSound"),
            Hash("EchoSound"),
            Hash("CasingBounceSound"),
            Hash("SwipeSound"),
            Hash("CollisionSound"),
            Hash("MeleeCollisionSound"),
            Hash("HeftSound"),
            Hash("PutDownSound"),
            Hash("RattleCollisionSound"),
            Hash("PickupSound"),
            UInt8("ShellCasingType"),            // 0 metal, 1 plastic, other none
            Hash("SafetyOnSound"),
            Hash("SafetyOffSound"),
            Hash("SlomoSwooshSound"),
            UInt32("Unk53"),
            Hash("SlomoXfadeSound"),
            Hash("SlomoCollisionSound")
        }};

        // rage::FootstepSurfaceSounds. One array per shoe type (u8 count, always 5).
        // Element slot = footstep event: [0] Heel, [1] Toe, [2] Scuff/Skid, [3] Run, [4] JumpLand.
        def.types[9] = {9, "gameFootsteps", {
            Array("HighHeelsSounds",  {{"", FieldKind::Hash}}),   // shoe type 4
            Array("LeatherSounds",    {{"", FieldKind::Hash}}),   // shoe type 3
            Array("DefaultSounds",    {{"", FieldKind::Hash}}),   // shoe types 0/1/5/7+, fallback
            Array("RubberSoftSounds", {{"", FieldKind::Hash}}),   // shoe types 2, 15
            Array("HeavyBootsSounds", {{"", FieldKind::Hash}})    // shoe type 6 (used only if count == 5)
        }};

        def.types[10] = {10, "gameRadioStationList", {
            Array("Stations", {{"", FieldKind::Hash}})
        }};

        def.types[11] = {11, "gameRadioStation", {
            Hash("NameHash"),                    // runtime
            Int32("WheelPosition"),
            Enum("Genre", FieldKind::UInt8, RadioGenreEnum()),
            UInt8("NumTrackLists"),              // runtime
            UInt32("TrackListsPtr"),             // runtime
            UInt32("NextStationSettingsPtr"),    // runtime
            Int8("AmbientRadioVol"),             // dB
            String("Name"),
            Array("TrackLists", {{"", FieldKind::Hash}})
        }};

        def.types[12] = {12, "gameRadioStationTrackCategory", {
            Enum("Category", FieldKind::UInt8, RadioTrackCategoryTypeEnum()),
            UInt32("NextValidSelectionTime"),    // runtime
            UInt8("HistoryWriteIndex"),          // runtime
            Array("HistorySpace", {{"", FieldKind::Hash}}),
            UInt16("TotalNumTracks"),            // runtime
            UInt32("NextTrackListPtr"),          // runtime
            Array("Tracks", {
                Hash("Context"),
                Hash("SoundRef")
            })
        }};

        def.types[13] = {13, "gameRadioStationCategoryWeights", {
            Array("Weights", {
                Enum("Category", FieldKind::UInt8, RadioTrackCategoryTypeEnum()),
                UInt32("Value")
            })
        }};

        def.types[14] = {14, "gameCrime", {
            Array("CrimeInstructions", {
                Hash("SoundHash"),
                Float("Weight")
            }),
            Array("CrimeDescriptions", {
                Hash("SoundHash"),
                Float("Weight")
            })
        }};

        // rage::ClothingSounds
        def.types[15] = {15, "gameClothing", {
            Hash("ImpactSound"),
            Hash("Unk04"),            // not read by game; equals LegSwishSound in all data
            Hash("LegSwishSound")
        }};

        def.types[16] = {16, "gamePed", {
            Array("PrimaryVoices", {
                Hash("VoiceName"),
                UInt32("ReferenceCount")
            }),
            Array("MiniVoices", {
                Hash("VoiceName"),
                UInt32("ReferenceCount")
            }),
            Array("GangVoices", {
                Hash("VoiceName"),
                UInt32("ReferenceCount")
            })
        }};

        def.types[17] = {17, "gameAmbientEmitterList", {
            Array("Ambient Emitters", {{"", FieldKind::Hash}}, FieldKind::UInt16)
        }};

        def.types[18] = {18, "gameScriptedReport", {
            Array("Phrases", {
                Hash("SoundRef"),
                UInt8("WaveSlotIndex"),   // index into police scanner wave slots
                UInt16("PostDelay")       // ms gap after this phrase
            }, FieldKind::UInt16)
        }};

        def.types[19] = {19, "gameAmbientZone", {
            Float("ActivationMargin"),
            Float("MinX"),
            Float("MinY"),
            Float("MaxX"),
            Float("MaxY"),
            Float("Unknown1E"),                  // 0/1, unused by game
            UInt8("NumRulesToPlay"),             // clamped to 16
            Array("Rules", {{"", FieldKind::Hash}})
        }};

        def.types[20] = {20, "gameSoundRules", {
            Float("Weight"),
            Float("MinDist"),
            Float("MaxDist"),
            UInt8("HoursStart"),
            UInt8("HoursEnd"),
            UInt16("MinRepeatTime"),             // seconds
            Hash("SoundHash"),
            Hash("CategoryHash"),
            UInt32("LastPlayTime")               // runtime
        }};

        def.types[21] = {21, "gameAmbientZoneList", {
            Array("Zones", {{"", FieldKind::Hash}})
        }};

        // rage::TrainStation
        def.types[22] = {22, "gameTrainStation", {
            Hash("StationNameSound"),
            Hash("ApproachingExtraSound"),   // after station name when approaching; NULL except Easton
            Hash("LeavingIntroSound"),       // before ANNOUNCE_NEXT_STOP when leaving
            Hash("PlatformIntroSound"),      // platform: <this> ANNOUNCE_BOUND_FOR <terminus>
            Hash("Unk16"),                   // always NULL_SOUND, not read
            Hash("Unk20"),                   // always NULL_SOUND, not read
            String("Name")                   // display name, not read by game
        }};

        def.types[23] = {23, "gameCutscene", {
            Array("Categories", {
                Hash("Category"), 
                Int8("Volume")                       // dB, -100 = mute
            })
        }};

        def.types[24] = {24, "gameInterior", {
            Array("InteriorRooms", {
                Hash("RoomName"),
                UInt8("InteriorType"),              // 5 = warehouse room tone; 1-4 override SourceEnvironment
                Float("ReverbSize"),
                Float("ReverbWet"),
                UInt32("SourceEnvironment"),
                Hash("RoomToneSound"),
                UInt8("RainType"),
                Float("ExteriorAudibility"),
                Float("RoomOcclusionDamping"),       // sources in another room of the same interior
                Float("NonMarkedPortalOcclusion"),   // sources in the same room
                Float("PortalOcclusion"),
                UInt8("IgnorePortals"),              // 1 = skip portal occlusion
                Float("ClosedDoorOcclusion"),        // caps PortalOcclusion when the door is near-closed
                Float("PortalFadeMinDistance"),
                Float("PortalFadeMaxDistance")       // fade only runs when min != max
            })
        }};

        // gameDoor (id=25)
        def.types[25] = {25, "gameDoor", {
            Hash("BrushSound"),
            Hash("LimitSound"),
            Hash("OpenSound"),
            Hash("CloseSound"),
            Float("Unk16")
        }};

        def.types[0] = {0, "gameAutomobile", {
            Int32("MasterVolume"),
            Int32("MaxConeAttenuation"),
            Hash("EngineLowLoop"),
            Hash("EngineHighLoop"),
            Hash("ExhaustLowLoop"),
            Hash("ExhaustHighLoop"),
            Hash("RevsOffLoop"),
            Float("EngineFilter1Frequency"),
            Float("EngineFilter1Bandwidth"),
            Int32("EngineFilter1GainMin"),
            Int32("EngineFilter1GainMax"),
            Float("EngineFilter2Frequency"),
            Float("EngineFilter2Bandwidth"),
            Int32("EngineFilter2GainMin"),
            Int32("EngineFilter2GainMax"),
            Float("ExhaustFilter1Frequency"),
            Float("ExhaustFilter1Bandwidth"),
            Int32("ExhaustFilter1GainMin"),
            Int32("ExhaustFilter1GainMax"),
            Float("ExhaustFilter2Frequency"),
            Float("ExhaustFilter2Bandwidth"),
            Int32("ExhaustFilter2GainMin"),
            Int32("ExhaustFilter2GainMax"),
            Int32("ExhaustFilter2ThrottleResonanceGain"),
            Float("ExhaustFilter3Frequency"),
            Float("ExhaustFilter3Bandwidth"),
            Int32("ExhaustFilter3GainMin"),
            Int32("ExhaustFilter3GainMax"),
            Hash("EngineWaveShapeCurve"),
            Int32("EngineWaveShapeGain"),
            Hash("ExhaustWaveShapeCurve"),
            Int32("ExhaustWaveShapeGain"),
            Int32("MinPitch"),
            Int32("MaxPitch"),
            Hash("EngineIdleLoop"),
            Hash("ExhaustIdleLoop"),
            Int32("IdleMinPitch"),
            Int32("IdleMaxPitch"),
            Hash("TransmissionSound"),
            Int32("TransWhineMinPitch"),
            Int32("TransWhineMaxPitch"),
            Hash("InductionLoop"),
            Int32("InductionMinPitch"),
            Int32("InductionMaxPitch"),
            Hash("ExhaustPops"),
            Hash("TurboWhine"),
            Int32("TurboMinPitch"),
            Int32("TurboMaxPitch"),
            Hash("DumpValve"),
            Hash("StartLoop"),
            Hash("HornSounds"),
            Hash("DoorOpen"),
            Hash("DoorClose"),
            Hash("BootOpen"),
            Hash("BootClose"),
            Float("BrakeSqueekFactor"),
            Hash("SuspensionUp"),
            Hash("SuspensionDown"),
            Float("MinSuspCompThresh"),
            Float("MaxSuspCompThresh"),
            Hash("PoliceScannerManufacturer"),
            Hash("PoliceScannerModel"),
            Hash("PoliceScannerVehicleCategory"),
            Hash("GearTransLoop"),
            Int32("GearTransMinPitch"),
            Int32("GearTransMaxPitch"),
            Int32("GearTransThrottleVolume"),
            Int32("DumpValveProb"),
            Int32("TurboSpinUpSpeed"),
            Int32("VolumeBoost"),
            Int32("PlayerRevsVolumeBoost"),
            Int32("AmbientRevsVolumeBoost"),
            Hash("JumpLand"),
            Int32("JumpLandMinThresh"),
            Int32("JumpLandMaxThresh"),
            Hash("Ignition"),
            Hash("EngineShutdown"),
            UInt8("VolumeCategory"),
            UInt8("GPSType"),
            UInt8("RadioType"),
            UInt8("RadioGenre"),
            Hash("IndicatorOn"),
            Hash("IndicatorOff"),
            Hash("CoolingFan"),
            Hash("Unk324"),
            Hash("Unk328"),
            Hash("Unk332"),
            Hash("Handbrake"),
            UInt8("GpsVoice"),
            Int8("AmbientRadioVolume"),
            UInt8("RadioLeakage")

        }};

        // these objects are 4-byte aligned in the original file
        def.types[0].alignment = 4;
        def.types[23].alignment = 4;

        registry["game"] = std::move(def);
    }
} // namespace AMT
