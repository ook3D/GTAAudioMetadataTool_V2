#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include "BaseTypes.h"

namespace AMT 
{

    class HashManager
    {
        inline static std::unique_ptr<HashManager> sm_Instance;
        std::unordered_map<uint32_t, std::string> m_HashTable;

    public:
        inline static HashManager * Instance()
        {
            if (!sm_Instance)
            {
                sm_Instance = std::make_unique<HashManager>();
            }
            return sm_Instance.get();
        }

        uint32_t AddHash(const std::string &str);

        // Functions to convert between hashes and strings
        std::string HashToString(uint32_t hash) const;
        uint32_t StringToHash(const std::string &str) const;
    };

    class JoaatHash
    {
    public:
        uint32_t Hash = 0xFFFFFFFF;

        JoaatHash() = default;
        JoaatHash(const std::string &str);
        JoaatHash(uint32_t hash) : Hash (hash){};

        auto operator<=> (const JoaatHash &) const = default;

        inline std::string ToString() const
        {
            return HashManager::Instance()->HashToString(Hash);
        }

        inline void ToJson(ordered_json &j) const
        {
            j = ToString();
        }

        inline void FromJson(const ordered_json &j)
        {
            std::string hashStr = j;
            try
            {
                if (hashStr.length() == std::string("0x????????").size() && hashStr[0] == '0' && hashStr[1] == 'x')
                {
                    this->Hash = std::stoul(hashStr, nullptr, 16);
                }
                else
                {
                    *this = hashStr;
                }
            }
            catch (const std::invalid_argument &)
            {
                *this = hashStr;
            }
        }
    };

}; // namespace AMT
