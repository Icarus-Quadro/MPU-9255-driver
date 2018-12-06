#pragma once

#include <Eigen/Core>
#include "Registers.hpp"
#include <mbed.h>


namespace MPU9255 {
template<typename Scalar, typename I2CDriver>
class Sensor
{
private:
    static constexpr uint8_t agAddress = 104;
    static constexpr uint8_t magAddress = 12;
public:
    using Vector = Eigen::Matrix<Scalar, 3, 1>;

    explicit Sensor(I2CDriver& bus) :
        mBus(bus)
    {
        namespace AG = MPU9255::AccelerometerGyroscope;
        namespace Mag = MPU9255::Magnetometer;
        // enable direct i2c access to magnetometer
        AG::InterruptBypassConfiguration config = {};
        config.enableBypass = true;
        mBus.scheduleWrite(agAddress, config, [](){});
        wait_ms(1);

        // enable continous measurement
        Mag::Control1 control = {};
        control.mode = Mag::OperationMode::continousMeasurement2;
        mBus.scheduleWrite(magAddress, control, [](){
            printf("Magnetometer init\n\r");
        });

        AG::AccelerometerConfiguration1 accConfig = {};
        accConfig.fullScaleSelect = AG::AccelerometerRange::g8;
        mBus.scheduleWrite(agAddress, accConfig, [this](){
            scheduleReads();
        });
        constexpr Scalar STANDARD_GRAVITY =  9.80665;
        mAccelerationScale = STANDARD_GRAVITY / 4096.0;

        constexpr Scalar RADIANS_PER_DEGREE = (2 * M_PI) / 360.0;
        mAngluarVelocityScale = 1.0 / 131.0 * RADIANS_PER_DEGREE;

    }

    Vector const& acceleration() const { return mAcceleration; }
    Vector const& angularVelocity() const { return mAngularVelocity; }
    Vector const& magneticStrength() const { return mMagneticStrength; }
    Scalar temperature() const { return mTemperature; } 
private:
    void scheduleReads()
    {
        namespace AG = MPU9255::AccelerometerGyroscope;
        namespace Mag = MPU9255::Magnetometer;

        mBus.template scheduleRead<AG::TemperatureRegister>(agAddress, [this](auto const& data){
            constexpr Scalar KELVINS_PER_LSB = 1.0 / 333.87;
            constexpr Scalar OFFSET_FROM_ABSOLUTE_ZERO = 294.15;
            mTemperature = static_cast<Scalar>(data.value()) * KELVINS_PER_LSB + OFFSET_FROM_ABSOLUTE_ZERO;
        });
        mBus.template scheduleRead<AG::AccelerometerMeasurements>(agAddress, [this](auto const& data){
            mAcceleration <<
                static_cast<Scalar>(data.x()),
                static_cast<Scalar>(data.y()),
                static_cast<Scalar>(data.z());
            mAcceleration *= mAccelerationScale;
        });
        mBus.template scheduleRead<AG::GyroscopeMeasurements>(agAddress, [this](auto const& data){
            mAngularVelocity <<
                static_cast<Scalar>(data.x()),
                static_cast<Scalar>(data.y()),
                static_cast<Scalar>(data.z());
            mAngularVelocity *= mAngluarVelocityScale;
        });
        mBus.template scheduleRead<Mag::Measurements>(magAddress, [this](auto const& data){            
            mMagneticStrength <<
                static_cast<Scalar>(data.x()),
                static_cast<Scalar>(data.y()),
                static_cast<Scalar>(data.z());
            constexpr Scalar TESLA_PER_LSB = 0.6E-6;
            mMagneticStrength *= TESLA_PER_LSB;
            scheduleReads();
        });
    }

    I2CDriver& mBus;
    Vector mAcceleration;
    Scalar mAccelerationScale;
    Vector mAngularVelocity;
    Scalar mAngluarVelocityScale;
    Vector mMagneticStrength;
    Scalar mTemperature;
};
}
