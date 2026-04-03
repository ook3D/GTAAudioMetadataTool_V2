#include "pch.h"
#include "common/MetadataFile.h"
#include "common/MetadataRegistry.h"
#include "common/HashManager.h"
#include "common/SpeechMetadata.h"

#include <algorithm>
#include <filesystem>
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

std::vector<std::string> GlobFiles(const std::string& extension)
{
    std::vector<std::string> results;
    std::string extLower = extension;
    std::transform(extLower.begin(), extLower.end(), extLower.begin(), ::tolower);

    for (const auto& entry : std::filesystem::directory_iterator("."))
    {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        std::string filenameLower = filename;
        std::transform(filenameLower.begin(), filenameLower.end(), filenameLower.begin(), ::tolower);

        if (filenameLower.size() >= extLower.size() && filenameLower.compare(filenameLower.size() - extLower.size(), extLower.size(), extLower) == 0)
        {
            results.push_back(filename);
        }
    }
    return results;
}

void ProcessMetadataFiles(bool generateMode, bool debugMode)
{
    const std::vector<std::pair<std::string, std::string>> extensionSchemaMap = {
        {".dat15",  "categories"},
        {".dat11",  "effects"},
        {".dat12",  "curves"},
    };

    const std::vector<std::pair<std::string, std::string>> suffixSchemaMap = {
        {"categories.dat15", "categories"},
        {"sounds.dat15",     "sounds"},
        {"game.dat16",       "game"},
        {"effects.dat11",    "effects"},
        {"curves.dat12",     "curves"},
    };

    auto getSchemaForFile = [&](const std::string& filename) -> std::string {
        std::string lower = filename;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        for (const auto& [suffix, schema] : suffixSchemaMap)
        {
            if (lower.size() >= suffix.size() &&
                lower.compare(lower.size() - suffix.size(), suffix.size(), suffix) == 0)
            {
                return schema;
            }
        }
        return {};
    };

    const std::vector<std::string> extensions = {".dat15", ".dat16", ".dat11", ".dat12"};
    for (const auto& ext : extensions)
    {
        for (const auto& filename : GlobFiles(ext))
        {
            std::string schema = getSchemaForFile(filename);
            if (schema.empty()) continue;

            if (generateMode)
            {
                SerialiseMetadata(filename, schema);
            }
            else
            {
                DeserialiseMetadata(filename, schema, debugMode);
            }
        }
    }

    // Process speech files
    for (const auto& filename : GlobFiles(".dat"))
    {
        std::string lower = filename;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        // Only match files ending in "speech.dat" to avoid matching other .dat files
        if (lower.size() >= 10 && lower.compare(lower.size() - 10, 10, "speech.dat") == 0)
        {
            if (generateMode)
            {
                SerialiseMetadataLegacy<AMT::SpeechMetadataMgr>(filename);
            }
            else
            {
                DeserialiseMetadataLegacy<AMT::SpeechMetadataMgr>(filename);
            }
        }
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
        {
            generateMode = true;
        }
        else if (arg == "debug")
        {
            debugMode = true;
        }
    }

    ProcessMetadataFiles(generateMode, debugMode);
    return 0;
}