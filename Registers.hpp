#pragma once

#include "Types.hpp"

class VectorRegister
{
public:
    float x() const
    {
        return static_cast<float>(mX.value());
    }

    float y() const
    {
        return static_cast<float>(mY.value());
    }

    float z() const
    {
        return static_cast<float>(mZ.value());
    }
private:
    BigEndian<int16_t> mX;
    BigEndian<int16_t> mY;
    BigEndian<int16_t> mZ;
};

class TemperatureRegister {
public:
    enum { address = 65 };
    float value() const
    {
        return static_cast<float>(mValue.value()) / 333.87f + 21.0f;
    }

private:
    BigEndian<int16_t> mValue;
};

struct AccelerometerMeasurements : VectorRegister
{
    enum { address = 59 };
};

struct GyroscopeMeasurements : VectorRegister
{
    enum { address = 67 };
};