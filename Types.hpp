#pragma once

#include <cstdint>
#include <cstddef>

namespace MPU9255 {
namespace Register
{
namespace detail
{
    template<size_t SIZE>
    struct UnsignedIntegerTrait;

    template<>
    struct UnsignedIntegerTrait<1>
    {
        using Type = uint8_t;
    };

    template<>
    struct UnsignedIntegerTrait<2>
    {
        using Type = uint16_t;
    };

    template<>
    struct UnsignedIntegerTrait<4>
    {
        using Type = uint32_t;
    };

    template<size_t SIZE>
    using UnsignedInteger = typename UnsignedIntegerTrait<SIZE>::Type;
}

enum class Endian
{
    little,
    big
};

template<typename T, Endian ENDIAN>
class Scalar;

template<typename T>
class Scalar<T, Endian::little>
{
public:
    T value() const
    {
        return mData;
    }
private:
    T mData = 0;
};

template<typename T, size_t I>
struct SwapBytes
{
    constexpr T operator()(uint8_t const* data)
    {
        return static_cast<T>(data[I - 1]) | SwapBytes<T, I - 1>()(data) << 8;
    }
};

template<typename T>
struct SwapBytes<T, 0>
{
    constexpr T operator()(uint8_t const*)
    {
        return 0;
    }
};

template<typename T>
class Scalar<T, Endian::big>
{
public:
    T value() const
    {
        auto ret = partial<sizeof(T)>();
        return reinterpret_cast<T&>(ret);
    }
private:
    template<size_t i>
    constexpr detail::UnsignedInteger<sizeof(T)> partial() const
    {
        return SwapBytes<detail::UnsignedInteger<sizeof(T)>, sizeof(T)>()(mData);
    }
    uint8_t mData[sizeof(T)] = {};
};

template<typename T, size_t SIZE, Endian ENDIAN = Endian::little>
class Vector
{
public:
    T x() const
    {
        return mComponents[0].value();
    }

    T y() const
    {
        static_assert(SIZE >= 2, "Vector has to have SIZE >= 2");
        return mComponents[1].value();
    }

    T z() const
    {
        static_assert(SIZE >= 3, "Vector has to have SIZE >= 3");
        return mComponents[2].value();
    }
private:
    using Component = Scalar<T, ENDIAN>;
    Component mComponents[SIZE];
};

}
}
