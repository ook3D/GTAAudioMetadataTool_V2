#pragma once

#include <iostream>
#include <cstring>

namespace AMT 
{
    namespace IoUtils 
    {

        template <typename T>
        void ReadData(std::istream &in, T *data, uint32_t size)
        {
            in.read(reinterpret_cast<char *>(data), size);
        }

        template <typename T>
        void ReadData(std::istream &in, T *data)
        {
            in.read(reinterpret_cast<char *>(data), sizeof(T));
        }

        template <typename T>
        T ReadData(std::istream &in)
        {
            T data;
            in.read(reinterpret_cast<char *>(&data), sizeof(T));

            return data;
        }

        template <typename T>
        void WriteData(std::ostream &out, const T &data, uint32_t size)
        {
            out.write(reinterpret_cast<const char *>(&data), size);
        }

        template <typename T>
        void WriteData(std::ostream &out, const T &data)
        {
            out.write(reinterpret_cast<const char *> (&data), sizeof(T));
        }

        template <typename T>
        void ReadData(uint8_t *&in, T *data, uint32_t size)
        {
            memcpy(data, in, size);
            in += size;
        }

        template <typename T>
        void ReadData(uint8_t *&in, T *data)
        {
            memcpy(data, in, sizeof(T));
            in += sizeof (T);
        }

        template <typename T>
        T ReadData (uint8_t *&in)
        {
            T data;
            memcpy(&data, in, sizeof(T));

            in += sizeof (T);
            return data;
        }
    }; // namespace IoUtils
}; // namespace AMT
