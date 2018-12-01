#pragma once

#include <cstdint>
#include <cstddef>

template<typename T>
class BigEndian
{
    static_assert(sizeof(T) == sizeof(uint16_t));
public:
    T value() const
    {
        uint16_t swapped = (static_cast<uint16_t>(highByte) << 8)
            | static_cast<uint16_t>(lowByte);
        return reinterpret_cast<T&>(swapped);
    }

    operator T () const
    {
        return value();
    }
private:
    uint8_t highByte;
    uint8_t lowByte;
};
