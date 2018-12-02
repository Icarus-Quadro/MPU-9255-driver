#pragma once

#include "Types.hpp"

namespace MPU9255 {
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

enum class AccelerometerRange : uint8_t
{
    g2,
    g4,
    g8,
    g16,
};

enum class GyroscopeRange : uint8_t
{
    dps250,
    dps500,
    dps1000,
    dps2000,
};

enum class ClockSource : uint8_t
{
    internal20MHz = 0,
    bestAvailable = 1,
    stopped = 7
};

struct SampleRateDivider
{
    enum { address = 25 };
    uint8_t divider;
};

struct Configuration
{
    uint8_t digitalLowPassFilter : 3;
    uint8_t fSync : 3;
    uint8_t fifoMode : 1;
};

struct AccelerometerConfiguration1
{
    enum { address = 28 };
    uint8_t : 3;
    AccelerometerRange fullScaleSelect : 2;
    bool zAxisSelfTest : 1;
    bool yAxisSelfTest : 1;
    bool xAxisSelfTest : 1;
};

struct AccelerometerConfiguration2
{
    enum { address = 29 };
    uint8_t digitalLowPassFilterConfiguration : 3;
    bool disableLowPassFilter : 1;
};

struct GyroscopeConfiguration
{
    enum { address = 27 };
    uint8_t fChoiceB : 2;
    uint8_t : 1;
    GyroscopeRange fullScaleRange : 2;
    bool zAxisSelfTest : 1;
    bool yAxisSelfTest : 1;
    bool xAxisSelfTest : 1;
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

struct PowerManagement1
{
    enum { address = 107 };
    ClockSource clockSelect : 3;
    bool powerDownProportionalToAbsoulteTemperatureVoltageGenerator : 1;
    bool gyroscopeStandby : 1;
    bool cycle : 1;
    bool sleep : 1;
    bool hardReset : 1;
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

struct SensitivityAdjustment : Register::Vector<uint8_t, 3>
{
    enum { address = 16 };
};

}
}
