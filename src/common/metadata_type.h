#include <cstdint>
#include <tuple>
#include <string_view>
#include <cstring>

template <unsigned N> struct FixedString
{
    char buf[N + 1]{};
    constexpr FixedString(char const *s)
    {
        for (unsigned i = 0; i != N; ++i)
        {
            buf[i] = s[i];
        }
    }
    constexpr operator char const * () const { return buf; }
};

template <unsigned N> FixedString (char const (&)[N]) -> FixedString<N - 1>;

template <typename T, FixedString name, bool isArchive = false, bool isHash = false>
struct ArgumentType
{
    using type_name = T;
    inline static const char *Name = name;
    inline static bool IsArchive = isArchive;
    inline static bool IsHash = isHash;

    T Data;
};

template <int id, FixedString, typename... Args> class BaseMetadataType
{
public:
    std::tuple<(decltype(Args)::data, ...)> Data;

    int GetId()
    {
        return id;
    }

    uint32_t GetSize();

protected:
    template <typename... T>
    uint8_t *Read(uint8_t *data, ArgumentType<T...> &out)
    {
        if constexpr (std::is_standard_layout_v<decltype(out.Data)>)
        {
            memcpy(&out.Data, data, sizeof (decltype(out.Data)));
            data += sizeof (decltype(out.Data));
        }
        else
        {
            data = out.Data.Read(data);
        }

        return data;
    }

    template <int i = sizeof...(Args)>
    uint8_t *Read(uint8_t *data)
    {
        if constexpr (i == 0)
        {
            return data;
        }
        
        data = Read(data, std::get<i>(Data));
        return Read<i - 1>(data);
    }
};

using audSimpleSound = BaseMetadataType
<
    1,
    "audSimpleSound",
    ArgumentType<uint32_t, "__field00">,
    ArgumentType<uint32_t, "ArchiveHash", true>,
    ArgumentType<uint32_t, "SoundHash">
>;

template <typename... Types> class MetadataMgr
{
    std::vector<std::variant<Types>> Datas;
}
