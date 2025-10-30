#include "pch.h"
#include "HashManager.h"
#include <iomanip>
#include <sstream>

namespace AMT 
{
    // https://en.wikipedia.org/wiki/Jenkins_hash_function
    uint32_t joaat(const uint8_t *key, size_t length)
    {
        size_t i = 0;
        uint32_t hash = 0;
        while (i != length)
        {
            hash += key[i++];
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }

    uint32_t HashString(const std::string &str)
    {
        return joaat(reinterpret_cast<const uint8_t *>(str.c_str()), str.length());
    }


    uint32_t HashStringLowercase (const std::string &str)
    {
        std::string key = str;
        std::transform(key.begin(), key.end(), key.begin(), [] (unsigned char c) { return std::tolower(c); });
        return HashString(key);
    }


    uint32_t HashManager::AddHash(const std::string &str)
    {
        uint32_t hash = StringToHash(str);
        m_HashTable[hash] = str;

        return hash;
    }


    uint32_t HashManager::StringToHash(const std::string &str) const
    {
        return HashStringLowercase(str);
    }


    std::string HashManager::HashToString(uint32_t hash) const
    {
        if (m_HashTable.count(hash))
        {
            return m_HashTable.at(hash);
        }

        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << hash;

        return ss.str();
    }


    JoaatHash::JoaatHash(const std::string &str)
    {
        this->Hash = HashManager::Instance()->AddHash(str);
    }

}; // namespace AMT
