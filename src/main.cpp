#include "pch.h"
#include "common/SoundsMetadata.h"
#include "common/MetadataMgr.h"
#include "common/HashManager.h"
#include "common/CategoryMetadata.h"
#include "common/GameMetadata.h"
#include "common/EffectsMetadata.h"
#include "common/CurvesMetadata.h"

#include <fstream>
#include <functional>
#include <vector>

template <typename T>
void DeserialiseMetadata(const std::string &file)
{
    T mgr;
    std::ifstream input(file, std::ios_base::binary);

    if (!input.good())
    {
        return;
    }

    mgr.Read(input);

    AMT::ordered_json j;
    mgr.ToJson(j);
    std::ofstream out(file + ".json");

    out << j.dump(4);
}

template <typename T>
void SerialiseMetadata(const std::string &file)
{
    std::ifstream input(file + ".json");
    if (!input.good())
    {
        return;
    }

    AMT::ordered_json j;
    input >> j;

    T mgr = j;
    std::ofstream out(file + ".GEN", std::ios_base::binary);
    mgr.Write(out);
}

void ReadHashes(const std::string &file)
{
    std::ifstream input(file);
    std::string key;
    while (std::getline(input, key))
    {
        AMT::HashManager::Instance()->AddHash(key);
    }
}

// Configuration for metadata files
struct MetadataFileConfig 
{
    std::string filename;
    std::function<void(const std::string&)> serializer;
    std::function<void(const std::string&)> deserializer;
};

void ProcessMetadataFiles(bool generateMode) 
{
    const std::vector<std::string> categoriesFiles = 
    {
        "CATEGORIES.DAT15", "EP1_CATEGORIES.DAT15", "EP2_CATEGORIES.DAT15"
    };

    const std::vector<std::string> effectsFiles = 
    {
        "EFFECTS.DAT11", "EP1_EFFECTS.DAT11", "EP2_EFFECTS.DAT11"
    };

    const std::vector<std::string> curvesFiles = 
    {
        "CURVES.DAT12", "EP1_CURVES.DAT12", "EP2_CURVES.DAT12"
    };

    const std::vector<std::string> soundFiles = 
    {
        "SOUNDS.DAT15", "EP1_RADIO_SOUNDS.DAT15", "EP2_RADIO_SOUNDS.DAT15", "EP1_SOUNDS.DAT15", "EP2_SOUNDS.DAT15"
    };

    const std::vector<std::string> gameFiles = 
    {
        "GAME.DAT16", "EP1_GAME.DAT16", "EP1_RADIO_GAME.DAT16", "EP2_GAME.DAT16", "EP2_RADIO_GAME.DAT16"
    };

    for (const auto& filename : categoriesFiles) 
    {
        if (generateMode) 
        {
            SerialiseMetadata<AMT::CategoriesMetadataMgr>(filename);
        } 
        else 
        {
            DeserialiseMetadata<AMT::CategoriesMetadataMgr>(filename);
        }
    }

    for (const auto& filename : effectsFiles) 
    {
        if (generateMode) 
        {
            SerialiseMetadata<AMT::EffectsMetadataMgr>(filename);
        } 
        else 
        {
            DeserialiseMetadata<AMT::EffectsMetadataMgr>(filename);
        }
    }

    for (const auto& filename : curvesFiles) 
    {
        if (generateMode) 
        {
            SerialiseMetadata<AMT::CurvesMetadataMgr>(filename);
        } 
        else 
        {
            DeserialiseMetadata<AMT::CurvesMetadataMgr>(filename);
        }
    }

    for (const auto& filename : soundFiles) 
    {
        if (generateMode) 
        {
            SerialiseMetadata<AMT::SoundsMetadataMgr>(filename);
        } 
        else 
        {
            DeserialiseMetadata<AMT::SoundsMetadataMgr>(filename);
        }
    }

    for (const auto& filename : gameFiles) 
    {
        if (generateMode) 
        {
            SerialiseMetadata<AMT::GameMetadataMgr>(filename);
        } 
        else
        {
            DeserialiseMetadata<AMT::GameMetadataMgr>(filename);
        }
    }
}

int main(int argc, char **argv) 
{
    ReadHashes("Hashes.txt");
    
    const bool generateMode = (argc == 2 && std::string(argv[1]) == "gen");
    ProcessMetadataFiles(generateMode);

    return 0;
}
