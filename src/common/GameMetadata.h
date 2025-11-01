#include "MetadataMgr.h"
#include "BaseMetadata.h"
#include "HashManager.h"
#include "PlaceholderMetadata.h"
#include "BaseTypes.h"

namespace AMT
{
    enum class GameMetadataTypeId : uint8_t
    {
        GameMetadataHeader = 0,
        Collision = 1,
        AmbientEmitter = 2,
        EmitterEntity = 3,
        Heli = 4,
        MeleeCombat = 5,
        SpeechContexts = 6,
        Boat = 7,
        Weapon = 8,
        Footsteps = 9,
        RadioStationList = 10,
        RadioStation = 11,
        RadioStationTrackCategory = 12,
        RadioStationCategoryWeights = 13,
        Crime = 14,
        Clothing = 15,
        Ped = 16,
        AmbientEmitterList = 17,
        ScriptedReport = 18,
        AmbientZone = 19,
        SoundRules = 20,
        AmbientZoneList = 21,
        TrainStation = 22,
        Cutscene = 23,
        Interior = 24,
        Door = 25,
        
        // placeholders
        CutsceneCategory = 0,
        InteriorRoom = 0,
        CrimeInstruction = 0,
        CrimeDescription = 0,
        RadioStationTrackCategoryTrack = 0,
        RadioStationCategoryWeightsWeight = 0,
        PedStruct = 0,
        SpeechContextsContext = 0,
        Automobile = 0
    };

    // Ambient Zone List
    class gameAmbientZoneList : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<JoaatHash>, "Zones">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::AmbientZoneList);
        static constexpr const char* Name = "gameAmbientZoneList";
        
        gameAmbientZoneList() = default;
        gameAmbientZoneList(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Ambient Zone
    class gameAmbientZone : public SimpleBaseMetadataType
    <
        FieldType<float, "MinX">, 
        FieldType<float, "MinY">, 
        FieldType<float, "MinZ">,
        FieldType<float, "MaxX">, 
        FieldType<float, "MaxY">, 
        FieldType<float, "MaxZ">,
        FieldType<uint8_t, "RulesCount">,
        FieldType<ArrayWrapper<JoaatHash>, "Rules">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::AmbientZone);
        static constexpr const char* Name = "gameAmbientZone";
        
        gameAmbientZone() = default;
        gameAmbientZone(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Collision
    class gameCollision : public PlaceholderMetadataType<1, "gameCollision">
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Collision);
        static constexpr const char* Name = "gameCollision";
        
        gameCollision() = default;
        gameCollision(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Clothing
    class gameClothing : public PlaceholderMetadataType<15, "gameClothing"> 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Clothing);
        static constexpr const char* Name = "gameClothing";
        
        gameClothing() = default;
        gameClothing(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Door
    class gameDoor : public PlaceholderMetadataType<25, "gameDoor"> 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Door);
        static constexpr const char* Name = "gameDoor";
        
        gameDoor() = default;
        gameDoor(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Cutscene Category
    class gameCutscene_Category : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "Category">,
        FieldType<uint8_t, "Intensity">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::CutsceneCategory);
        static constexpr const char* Name = "gameCutscene_Category";
        
        gameCutscene_Category() = default;
        gameCutscene_Category(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Cutscene
    class gameCutscene : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<gameCutscene_Category>, "Categories">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Cutscene);
        static constexpr const char* Name = "gameCutscene";
        
        gameCutscene() = default;
        gameCutscene(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Ambient Emitter
    class gameAmbientEmitter : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "ChildSound">,
        FieldType<JoaatHash, "RadioStation">,
        // Position
        FieldType<float, "PosX">, 
        FieldType<float, "PosY">, 
        FieldType<float, "PosZ">,
        FieldType<uint32_t, "padding">,
        // Room and audio settings
        FieldType<uint32_t, "InteriorRoom">,
        FieldType<int32_t, "Volume">,
        FieldType<uint16_t, "LPFCutoff">, 
        FieldType<uint16_t, "HPFCutoff">,
        FieldType<uint16_t, "RolloffFactor">,
        FieldType<JoaatHash, "Interior">,
        FieldType<uint32_t, "padding">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::AmbientEmitter);
        static constexpr const char* Name = "gameAmbientEmitter";
        
        gameAmbientEmitter() = default;
        gameAmbientEmitter(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Ambient Emitter List
    class gameAmbientEmitterList : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<JoaatHash, uint16_t>, "Ambient Emitters">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::AmbientEmitterList);
        static constexpr const char* Name = "gameAmbientEmitterList";
        
        gameAmbientEmitterList() = default;
        gameAmbientEmitterList(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Emitter Entity
    class gameEmitterEntity : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "ChildSound">,
        FieldType<float, "MaxDistance">,
        FieldType<float, "BusinessHoursProbability">,
        FieldType<float, "EveningProbability">,
        FieldType<float, "NightProbability">,
        FieldType<float, "ConeInnerAngle">,
        FieldType<float, "ConeOuterAngle">,
        FieldType<float, "ConeMaxAtten">,
        FieldType<int32_t, "unk2">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::EmitterEntity);
        static constexpr const char* Name = "gameEmitterEntity";
        
        gameEmitterEntity() = default;
        gameEmitterEntity(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Footsteps
    class gameFootsteps : public PlaceholderMetadataType<9, "gameFootsteps"> 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Footsteps);
        static constexpr const char* Name = "gameFootsteps";
        
        gameFootsteps() = default;
        gameFootsteps(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Interior Room
    class gameInterior_Room : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "RoomName">,
        FieldType<int8_t, "padding">,
        FieldType<float, "ReverbLarge">,
        FieldType<float, "ReverbMedium">,
        FieldType<float, "ReverbSmall">,
        FieldType<JoaatHash, "RoomSound">,
        FieldType<int8_t, "RainType">,
        FieldType<float, "ExteriorAudibility">,
        FieldType<float, "RoomOcclusionDamping">,
        FieldType<float, "NonMarkedPortalOcclusion">,
        FieldType<float, "DistanceFromPortalForOcclusion">,
        FieldType<int8_t, "padding">,
        FieldType<float, "DistanceFromPortalFadeDistance">,
        FieldType<JoaatHash, "WeaponMetrics">,
        FieldType<JoaatHash, "InteriorWallaSoundSet">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::InteriorRoom);
        static constexpr const char* Name = "gameInterior_Room";
        
        gameInterior_Room() = default;
        gameInterior_Room(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Interior
    class gameInterior : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<gameInterior_Room>, "InteriorRooms">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Interior);
        static constexpr const char* Name = "gameInterior";
        
        gameInterior() = default;
        gameInterior(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Melee Combat
    class gameMeleeCombat : public PlaceholderMetadataType<5, "gameMeleeCombat">
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::MeleeCombat);
        static constexpr const char* Name = "gameMeleeCombat";
        
        gameMeleeCombat() = default;
        gameMeleeCombat(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Crime Instruction
    class gameCrime_Instruction : public SimpleBaseMetadataType<
        FieldType<JoaatHash, "Hash">,
        FieldType<float, "Weight">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::CrimeInstruction);
        static constexpr const char* Name = "gameCrime_Instruction";
        
        gameCrime_Instruction() = default;
        gameCrime_Instruction(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Crime Description
    class gameCrime_Description : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "Hash">,
        FieldType<float, "Weight">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::CrimeDescription);
        static constexpr const char* Name = "gameCrime_Description";
        
        gameCrime_Description() = default;
        gameCrime_Description(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Crime
    class gameCrime : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<gameCrime_Instruction>, "CrimeInstructions">,
        FieldType<ArrayWrapper<gameCrime_Description>, "CrimeDescriptions">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Crime);
        static constexpr const char* Name = "gameCrime";
        
        gameCrime() = default;
        gameCrime(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Radio Station Track Category
    #define X EnumWrapperValue
    using eRadioStationTrackCategoryType = EnumWrapper<uint8_t, 
        X{"AD", 0}, 
        X{"IDENT", 1},
        X{"MUSIC", 2},
        X{"NEWS", 3}, 
        X{"WEATHER", 4}, 
        X{"DJ_SOLO", 5},
        X{"USER_INTRO", 6}, 
        X{"USER_OUTRO", 7}, 
        X{"USER_TO_AD", 8},
        X{"USER_TO_NEWS", 9}
    >;
    #undef X

    // Radio Station Track Category Track
    class gameRadioStationTrackCategory_Track : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "Context">,
        FieldType<JoaatHash, "SoundRef">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::RadioStationTrackCategoryTrack);
        static constexpr const char* Name = "gameRadioStationTrackCategory_Track";
        
        gameRadioStationTrackCategory_Track() = default;
        gameRadioStationTrackCategory_Track(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Radio Station Track Category
    class gameRadioStationTrackCategory : public SimpleBaseMetadataType
    <
        FieldType<eRadioStationTrackCategoryType, "Type">,
        FieldType<uint32_t, "padding00">,
        FieldType<uint8_t, "padding01">,
        FieldType<ArrayWrapper<JoaatHash>, "NumHistorySpaceElems">,
        FieldType<uint16_t, "padding02">,
        FieldType<uint32_t, "padding03">,
        FieldType<ArrayWrapper<gameRadioStationTrackCategory_Track>, "Tracks">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::RadioStationTrackCategory);
        static constexpr const char* Name = "gameRadioStationTrackCategory";
        
        gameRadioStationTrackCategory() = default;
        gameRadioStationTrackCategory(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Radio Station
    class gameRadioStation : public SimpleBaseMetadataType
    <
        FieldType<int32_t, "unused">,
        FieldType<int32_t, "WheelPosition">,
        FieldType<uint8_t, "Genre">, 
        FieldType<uint8_t, "padding01">,
        FieldType<int32_t, "padding02">, 
        FieldType<int32_t, "padding03">,
        FieldType<uint8_t, "AmbientRadioVol">,
        // Station data
        FieldType<StringWrapper<>, "Name">,
        FieldType<ArrayWrapper<JoaatHash>, "TrackCategories">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::RadioStation);
        static constexpr const char* Name = "gameRadioStation";
        
        gameRadioStation() = default;
        gameRadioStation(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Radio Station Category Weights Weight
    class gameRadioStationCategoryWeights_Weight : public SimpleBaseMetadataType
    <
        FieldType<eRadioStationTrackCategoryType, "CategoryType">,
        FieldType<int, "Value">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::RadioStationCategoryWeightsWeight);
        static constexpr const char* Name = "gameRadioStationCategoryWeights_Weight";
        
        gameRadioStationCategoryWeights_Weight() = default;
        gameRadioStationCategoryWeights_Weight(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Radio Station Category Weights
    class gameRadioStationCategoryWeights : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<gameRadioStationCategoryWeights_Weight>, "Weights">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::RadioStationCategoryWeights);
        static constexpr const char* Name = "gameRadioStationCategoryWeights";
        
        gameRadioStationCategoryWeights() = default;
        gameRadioStationCategoryWeights(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Ped Voice Group Item
    class gamePed_VoiceGroupItem : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "VoiceHash">,
        FieldType<uint32_t, "ReferenceCount">
    >
    {
    public:
        static constexpr int Type = 0;
        static constexpr const char* Name = "gamePed_VoiceGroupItem";
        
        gamePed_VoiceGroupItem() = default;
        gamePed_VoiceGroupItem(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Ped
    class gamePed : public SimpleBaseMetadataType
        <
        FieldType<ArrayWrapper<gamePed_VoiceGroupItem, uint8_t>, "VoiceGroups">,
        FieldType<ArrayWrapper<gamePed_VoiceGroupItem, uint8_t>, "MiniVoiceGroups">
        >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Ped);
        static constexpr const char* Name = "gamePed";

        gamePed() = default;
        gamePed(uint8_t*& data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Train Station
    class gameTrainStation : public PlaceholderMetadataType<22, "gameTrainStation">
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::TrainStation);
        static constexpr const char* Name = "gameTrainStation";
        
        gameTrainStation() = default;
        gameTrainStation(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Vehicle Types
    class gameAutomobile : public SimpleBaseMetadataType
    <
        FieldType<int32_t, "masterVolume">,
        FieldType<int32_t, "nonPlayerFXComp">,
        FieldType<JoaatHash, "lowEngineLoop">,
        FieldType<JoaatHash, "highEngineLoop">,
        FieldType<JoaatHash, "lowExhaustLoop">,
        FieldType<JoaatHash, "highExhaustLoop">,
        FieldType<JoaatHash, "revsOffLoop">,
        FieldType<float, "minPitch">,
        FieldType<float, "maxPitch">,
        FieldType<int32_t, "idleMinPitch">,
        FieldType<int32_t, "idleMaxPitch">,
        FieldType<float, "unk0">,
        FieldType<float, "unk00">,
        FieldType<int32_t, "unk1">,
        FieldType<int32_t, "unk2">,
        FieldType<float, "dumpValveProbability">,
        FieldType<float, "turboSpinupSpeed">,
        FieldType<int32_t, "unk3">,
        FieldType<int32_t, "unk4">,
        FieldType<float, "unk5">,
        FieldType<float, "unk6">,
        FieldType<int32_t, "unk7">,
        FieldType<int32_t, "unk8">,
        FieldType<int32_t, "unk9">,
        FieldType<float, "unk10">,
        FieldType<float, "unk11">,
        FieldType<int32_t, "unk12">,
        FieldType<int32_t, "unk13">,
        FieldType<JoaatHash, "engineWaveShape">,
        FieldType<uint32_t, "unk14">,
        FieldType<JoaatHash, "exhaustWaveShape">,
        FieldType<uint32_t, "unk15">,
        FieldType<int32_t, "unk16">,
        FieldType<int32_t, "unk17">,
        FieldType<JoaatHash, "engineIdleLoopSound">,
        FieldType<JoaatHash, "exhaustIdleLoopSound">,
        FieldType<int32_t, "unk18">,
        FieldType<int32_t, "unk19">,
        FieldType<JoaatHash, "transmissionSound">,
        FieldType<int32_t, "unk20">,
        FieldType<int32_t, "unk21">,
        FieldType<JoaatHash, "InductionLoop">,
        FieldType<int32_t, "InductionMinPitch">,
        FieldType<int32_t, "InductionMaxPitch">,
        FieldType<JoaatHash, "exhaustPopSound">,
        FieldType<JoaatHash, "TurboWhine">,
        FieldType<int32_t, "TurboMinPitch">,
        FieldType<int32_t, "TurboMaxPitch">,
        FieldType<JoaatHash, "dumpValveSound">,
        FieldType<JoaatHash, "startupRevs">,
        FieldType<JoaatHash, "hornSounds">,
        FieldType<JoaatHash, "doorOpenSound">,
        FieldType<JoaatHash, "doorCloseSound">,
        FieldType<JoaatHash, "bootOpenSound">,
        FieldType<JoaatHash, "bootCloseSound">,
        FieldType<float, "rollSound">,
        FieldType<JoaatHash, "suspensionUpSound">,
        FieldType<JoaatHash, "suspensionDownSound">,
        FieldType<float, "minSuspCompThresh">,
        FieldType<float, "maxSuspCompThresh">,
        FieldType<JoaatHash, "policeScannerManufacturerSound">,
        FieldType<JoaatHash, "policeScannerModelSound">,
        FieldType<JoaatHash, "policeScannerVehicleCategorySound">,
        FieldType<JoaatHash, "gearTransmissionSound">,
        FieldType<int32_t, "unk26">,
        FieldType<int32_t, "unk27">,
        FieldType<int32_t, "unk28">,
        FieldType<int32_t, "unk29">,
        FieldType<int32_t, "unk30">,
        FieldType<int32_t, "unk31">,
        FieldType<int32_t, "unk32">,
        FieldType<int32_t, "unk33">,
        FieldType<JoaatHash, "jumpLandSound">,
        FieldType<int32_t, "JumpLandMinThresh">,
        FieldType<int32_t, "JumpLandMaxThresh">,
        FieldType<JoaatHash, "ignitionSound">,
        FieldType<JoaatHash, "engineShutDownSound">,
        FieldType<uint32_t, "unk36">,
        FieldType<JoaatHash, "indicatorOnSound">,
        FieldType<JoaatHash, "indicatorOffSound">,
        FieldType<JoaatHash, "coolingFanSound">,
        FieldType<JoaatHash, "nullSound1">,
        FieldType<JoaatHash, "nullSound2">,
        FieldType<JoaatHash, "nullSound3">,
        FieldType<JoaatHash, "handbrakeSound">,
        FieldType<int16_t, "padding00">,
        FieldType<int8_t, "padding01">
    >
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Automobile);
        static constexpr const char* Name = "gameAutomobile";
        
        gameAutomobile() = default;
        gameAutomobile(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    class gameBoat : public PlaceholderMetadataType<7, "gameBoat"> 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Boat);
        static constexpr const char* Name = "gameBoat";
        
        gameBoat() = default;
        gameBoat(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    class gameHeli : public PlaceholderMetadataType<4, "gameHeli"> 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Heli);
        static constexpr const char* Name = "gameHeli";
        
        gameHeli() = default;
        gameHeli(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Weapon
    class gameWeapon : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "FireHash">,
        FieldType<JoaatHash, "EchoHash">, 
        FieldType<JoaatHash, "CasingBounceHash">,
        FieldType<JoaatHash, "SwipeSoundHash">,
        FieldType<JoaatHash, "CollisionHash">,
        FieldType<JoaatHash, "MeleeCollisionHash">,
        FieldType<JoaatHash, "HeftHash">, 
        FieldType<JoaatHash, "PutDownHash">,
        FieldType<JoaatHash, "RattleCollisionHash">,
        FieldType<JoaatHash, "PickupSoundHash">, 
        FieldType<uint8_t, "__field00">,
        FieldType<JoaatHash, "SafetyOnSound">,
        FieldType<JoaatHash, "SafetyOffSound">,
        FieldType<JoaatHash, "SlomoSwooshSound">, 
        FieldType<uint32_t, "__field04">,
        FieldType<JoaatHash, "SlomoXfadeHash">,
        FieldType<JoaatHash, "SlomoCollisionHash">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::Weapon);
        static constexpr const char* Name = "gameWeapon";
        
        gameWeapon() = default;
        gameWeapon(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Radio Station List
    class gameRadioStationList : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<JoaatHash>, "Stations">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::RadioStationList);
        static constexpr const char* Name = "gameRadioStationList";
        
        gameRadioStationList() = default;
        gameRadioStationList(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Scripted Report
    class gameScriptedReport : public PlaceholderMetadataType<18, "gameScriptedReport"> 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::ScriptedReport);
        static constexpr const char* Name = "gameScriptedReport";
        
        gameScriptedReport() = default;
        gameScriptedReport(uint8_t *&data, uint32_t size) : PlaceholderMetadataType(data, size) {}
    };

    // Sound Rules
    class gameSoundRules : public SimpleBaseMetadataType
    <
        FieldType<float, "Weight">,
        FieldType<float, "OffsetX">, 
        FieldType<float, "OffsetY">,
        FieldType<uint8_t, "HoursStart">, 
        FieldType<uint8_t, "HoursEnd">,
        FieldType<int16_t, "__field0f">, 
        FieldType<JoaatHash, "SoundHash">,
        FieldType<JoaatHash, "CategoryHash">, 
        FieldType<uint32_t, "Unknown">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::SoundRules);
        static constexpr const char* Name = "gameSoundRules";
        
        gameSoundRules() = default;
        gameSoundRules(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Speech Contexts Context
    class gameSpeechContexts_Context : public SimpleBaseMetadataType
    <
        FieldType<JoaatHash, "ContextHash">,
        FieldType<uint32_t, "_field04">, 
        FieldType<int32_t, "_field08">,
        FieldType<uint32_t, "_field0C">, 
        FieldType<uint8_t, "_field10">,
        FieldType<JoaatHash, "UnkHash">, 
        FieldType<uint32_t, "_field15">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::SpeechContextsContext);
        static constexpr const char* Name = "gameSpeechContexts_Context";
        
        gameSpeechContexts_Context() = default;
        gameSpeechContexts_Context(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Speech Contexts
    class gameSpeechContexts : public SimpleBaseMetadataType
    <
        FieldType<ArrayWrapper<gameSpeechContexts_Context, uint16_t>, "Contexts">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::SpeechContexts);
        static constexpr const char* Name = "gameSpeechContexts";
        
        gameSpeechContexts() = default;
        gameSpeechContexts(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    // Game Metadata Header - header for game metadata files
    class GameMetadataHeader : public SimpleBaseMetadataType
    <
        FieldType<uint32_t, "nametableOffset">,
        FieldType<uint8_t, "padding">
    > 
    {
    public:
        static constexpr int Type = static_cast<int>(GameMetadataTypeId::GameMetadataHeader);
        static constexpr const char* Name = "GameMetadataHeader";
        
        GameMetadataHeader() = default;
        GameMetadataHeader(uint8_t *&data, uint32_t size) : SimpleBaseMetadataType(data, size) {}
    };

    using GameMetadataContainer = BaseMetadataContainer
    <
        GameMetadataHeader,
        true, 
        gameAmbientZoneList,
        gameAmbientZone,
        gameCollision,
        gameClothing,
        gameDoor,
        gameCutscene,
        gameAmbientEmitter,
        gameAmbientEmitterList,
        gameEmitterEntity,
        gameFootsteps, 
        gameInterior,
        gameMeleeCombat,
        gameCrime, 
        gameRadioStationTrackCategory, 
        gameRadioStation,
        gameRadioStationCategoryWeights, 
        gamePed, 
        gameTrainStation, 
        gameAutomobile,
        gameHeli,
        gameBoat, 
        gameWeapon, 
        gameRadioStationList, 
        gameSoundRules,
        gameScriptedReport, 
        gameSpeechContexts
    >;

    using GameMetadataMgr = MetadataMgr<GameMetadataContainer, 16>;
}; // namespace AMT

