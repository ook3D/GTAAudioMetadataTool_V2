#include "BaseTypes.h"
#include <sstream>
#include <iomanip>
#include "IoUtils.h"

namespace AMT 
{

    template <int id, FixedString name>
    class PlaceholderMetadataType
    {
        std::string Data; // Stores hex-string for the stored data
        
    public:
        inline static const char *TypeName = name;
        
        static int GetId()
        {
            return id;
        }

        PlaceholderMetadataType() = default;
        PlaceholderMetadataType(uint8_t *&data, uint32_t size)
        {
            data = Read(data, size);
        }

        uint8_t *Read (uint8_t *data, uint32_t size)
        {
            std::ostringstream ss;
            for (uint32_t i = 0; i < size; i++)
            {
                ss << std::setbase(16) << std::setfill('0') << std::setw(2) << int (data[i]) << ' ';
            }

            Data = ss.str();
            Data.pop_back();

            return data + size;
        }

        bool IsValidHexStringCharacter(char c)
        {
            return std::isdigit(c) || (c >= 'a' && c <= 'f');
        }

        bool ValidateData()
        {
            if (Data.size() < 2)
            {
                return true;
            }

            for (int i = 0; i < Data.size () - 2; i += 3)
            {
                if (!IsValidHexStringCharacter (Data[i]) || !IsValidHexStringCharacter (Data[i + 1]))
                {
                    return false;
                }

                if (i != Data.size () - 3 && Data[i + 2] != ' ')
                {
                    return false;
                }
            }
            return true;
        }

        void Write(std::ostream& out)
        {
            std::istringstream ss(Data);
            ss >> std::hex;
            int bytes = GetSize();
            for (int i = 0; i < bytes; i++)
            {
                int byte = 0;
                ss >> byte;
                IoUtils::WriteData(out, uint8_t(byte));
            }
        }

        void ToJson(ordered_json &j) const
        {
            j = Data;
        }

        void FromJson(const ordered_json &j)
        {
            Data = j.get<std::string>();
            if (!ValidateData())
            {
                throw std::runtime_error ("Invalid byte-string");
            }
        }

        uint32_t GetSize() const
        {
            return static_cast<uint32_t>((Data.size() + 1) / 3);
        }
    };

}; // namespace AMT
