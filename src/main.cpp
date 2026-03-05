#include "pch.h"
#include "common/MetadataFile.h"
#include "common/MetadataRegistry.h"
#include "common/HashManager.h"
#include "common/SpeechMetadata.h"

#include <fstream>
#include <functional>
#include <vector>

static AMT::MetadataRegistry g_Registry;

void DeserialiseMetadata(const std::string& file, const std::string& schemaKey, bool debugMode = false)
{
    const AMT::MetadataFileDef* def = g_Registry.GetFileDef(schemaKey);
    if (!def) return;

    std::ifstream input(file, std::ios_base::binary);
    if (!input.good()) return;

    AMT::MetadataFile mgr(def, debugMode);
    mgr.Read(input);

    AMT::ordered_json j;
    mgr.ToJson(j);

    std::ofstream out(file + ".json");
    out << j.dump(4);
}

void SerialiseMetadata(const std::string& file, const std::string& schemaKey)
{
    const AMT::MetadataFileDef* def = g_Registry.GetFileDef(schemaKey);
    if (!def) return;

    std::ifstream input(file + ".json");
    if (!input.good()) return;

    AMT::ordered_json j;
    input >> j;

    AMT::MetadataFile mgr(def);
    mgr.FromJson(j);

    std::ofstream out(file + ".GEN", std::ios_base::binary);
    mgr.Write(out);
}

template <typename T>
void DeserialiseMetadataLegacy(const std::string& file)
{
    T mgr;
    std::ifstream input(file, std::ios_base::binary);
    if (!input.good()) return;

    mgr.Read(input);

    AMT::ordered_json j;
    mgr.ToJson(j);
    std::ofstream out(file + ".json");
    out << j.dump(4);
}

template <typename T>
void SerialiseMetadataLegacy(const std::string& file)
{
    std::ifstream input(file + ".json");
    if (!input.good()) return;

    AMT::ordered_json j;
    input >> j;

    T mgr;
    mgr.FromJson(j);
    std::ofstream out(file + ".GEN", std::ios_base::binary);
    mgr.Write(out);
}

void ReadHashes(const std::string& file)
{
    std::ifstream input(file);
    std::string key;
    while (std::getline(input, key))
    {
        AMT::HashManager::Instance()->AddHash(key);
    }
}

void ProcessMetadataFiles(bool generateMode, bool debugMode)
{
    const std::vector<std::pair<std::string, std::string>> categoriesFiles = {
        {"CATEGORIES.DAT15", "categories"},
        {"EP1_CATEGORIES.DAT15", "categories"},
        {"EP2_CATEGORIES.DAT15", "categories"}
    };

    const std::vector<std::pair<std::string, std::string>> effectsFiles = {
        {"EFFECTS.DAT11", "effects"},
        {"EP1_EFFECTS.DAT11", "effects"},
        {"EP2_EFFECTS.DAT11", "effects"}
    };

    const std::vector<std::pair<std::string, std::string>> curvesFiles = {
        {"CURVES.DAT12", "curves"},
        {"EP1_CURVES.DAT12", "curves"},
        {"EP2_CURVES.DAT12", "curves"}
    };

    const std::vector<std::pair<std::string, std::string>> soundFiles = {
        {"SOUNDS.DAT15", "sounds"},
        {"EP1_RADIO_SOUNDS.DAT15", "sounds"},
        {"EP2_RADIO_SOUNDS.DAT15", "sounds"},
        {"EP1_SOUNDS.DAT15", "sounds"},
        {"EP2_SOUNDS.DAT15", "sounds"}
    };

    const std::vector<std::pair<std::string, std::string>> gameFiles = {
        {"GAME.DAT16", "game"},
        {"EP1_GAME.DAT16", "game"},
        {"EP1_RADIO_GAME.DAT16", "game"},
        {"EP2_GAME.DAT16", "game"},
        {"EP2_RADIO_GAME.DAT16", "game"}
    };

    const std::vector<std::string> speechFiles = {
        "SPEECH.DAT", "EP1_SPEECH.DAT", "EP2_SPEECH.DAT"
    };

    auto processFiles = [&](const std::vector<std::pair<std::string, std::string>>& files) {
        for (const auto& [filename, schema] : files)
        {
            if (generateMode)
                SerialiseMetadata(filename, schema);
            else
                DeserialiseMetadata(filename, schema, debugMode);
        }
    };

    processFiles(categoriesFiles);
    processFiles(effectsFiles);
    processFiles(curvesFiles);
    processFiles(soundFiles);
    processFiles(gameFiles);

    for (const auto& filename : speechFiles)
    {
        if (generateMode)
            SerialiseMetadataLegacy<AMT::SpeechMetadataMgr>(filename);
        else
            DeserialiseMetadataLegacy<AMT::SpeechMetadataMgr>(filename);
    }
}

int main(int argc, char** argv)
{
    ReadHashes("Hashes.txt");

    g_Registry.RegisterAll();

    bool generateMode = false;
    bool debugMode = false;

    if (argc >= 2)
    {
        std::string arg(argv[1]);
        if (arg == "gen")
            generateMode = true;
        else if (arg == "debug")
            debugMode = true;
    }

    ProcessMetadataFiles(generateMode, debugMode);

    return 0;
}
