#pragma once

#include "Types.hpp"

namespace AccelerometerGyroscope {
enum ActiveLevel : uint8_t
{
    high,
    low,
};

enum PinMode : uint8_t 
{
    pushPull,
    openDrain, 
};

struct TemperatureRegister : Register::Scalar<int16_t, Register::Endian::big>
{
    enum { address = 65 };
};

struct AccelerometerMeasurements : Register::Vector<int16_t, 3, Register::Endian::big>
{
    enum { address = 59 };
};

struct GyroscopeMeasurements : Register::Vector<int16_t, 3, Register::Endian::big>
{
    enum { address = 67 };
};

struct InterruptBypassConfiguration
{
    enum { address = 55 };

    uint8_t : 1;
    bool enableBypass : 1;
    bool enableFsyncInterrupt : 1;
    ActiveLevel fsyncActiveLevel : 1;
    bool clearInterruptOnRead : 1;
    bool latchInterrputPin : 1;
    PinMode interruptPinMode : 1;
    ActiveLevel interruptPinLevel : 1;
};
}

namespace Magnetometer {

enum class OperationMode : uint8_t
{
    powerDown = 0b0000,
    singleMeasurement = 0b0001,
    continousMeasurement1 = 0b0010,
    continousMeasurement2 = 0b0110,
    externalTriggerMeasurement = 0b0100,
    selfTest = 0b1000,
    fuseRomAccess = 0b1111,
};

enum class OutputBitSetting : uint8_t
{
    bits14 = 0,
    bits16 = 1,
};

struct Measurements : Register::Vector<int16_t, 3, Register::Endian::little>
{
    enum { address = 3 };

    uint8_t : 3;
    bool overflow : 1;
    OutputBitSetting outputBit : 1;
};

struct Control1
{
    enum { address = 0xA };
    OperationMode mode : 4;
    OutputBitSetting outputBit : 1;
};

struct WhoAmI
{
    enum { address = 0 };
    uint8_t data;
};

}