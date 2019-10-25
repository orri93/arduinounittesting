#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlsensor.h>

#include <gosmax31865.h>
#include <gosmax6675.h>

namespace gatl = ::gos::atl;

template<typename T = double>
class TestSensor : public gatl::Sensor<T> {
public:
  gatl::Sensor<T>::Status measure() {
    Value = 93.0;
    Code = 11;
    return gatl::Sensor<T>::Status::Operational;
  }
  const char* error(uint8_t& length) {
    return nullptr;
  }
};

class MaxSensor31865 : public gatl::Sensor<double> {
public:
  MaxSensor31865(
    /* PIN for Chip Select CS is often 10 */
    const uint8_t&  pincs = 10,
    /* SPI CLOCK 1000000 (1MHz) */
    const uint32_t& clock = SPI_CLOCK_MAX_31865,
    /* SPI bit order MSB FIRST */
    const uint8_t&  order = SPI_BIT_ORDER_MAX_31865,
    /* MAX31865 works in MODE1 or MODE3 */
    const uint8_t&  mode = SPI_MODE_MAX_31865,
    /* RTD type either PT100 or PT1000 */
    const uint8_t& rtdtype = RTD_TYPE_PT100,
    /* Often 3 wires rather than 4 */
    const uint8_t& wires = RTD_3_WIRE) :
    max31865(pincs, clock, order, mode),
    rtdtype_(rtdtype),
    wires_(wires) {
  }

  void begin() {
    max31865.initialize(rtdtype_, wires_);
  }

  gatl::Sensor<double>::Status measure() {
    if (max31865.read(Value)) {
      return gatl::Sensor<double>::check();
    } else {
      Code = max31865.Status;
      return gatl::Sensor<double>::Status::Fault;
    }
  }

  const char* error(uint8_t& length) {
    return max31865.error(length);
  }

private:
  ::gos::Max31865 max31865;
  uint8_t rtdtype_;
  uint8_t wires_;
};

TEST(GatlSensorTest, SingleSensor) {
  TestSensor<> sensor;
  TestSensor<>::Status status = sensor.measure();
  EXPECT_EQ(TestSensor<>::Status::Operational, status);
  double value = sensor.Value;
  EXPECT_DOUBLE_EQ(93.0, value);
  uint8_t code = sensor.Code;
  EXPECT_EQ(11, code);
}

TEST(GatlSensorTest, SensorArray) {
  typedef gatl::Sensor<>* SensorPointer;
  //SensorPointer pointer[2];

  TestSensor<> sensor;
  TestSensor<>::Status status = sensor.measure();
  EXPECT_EQ(TestSensor<>::Status::Operational, status);
  double value = sensor.Value;
  EXPECT_DOUBLE_EQ(93.0, value);
  uint8_t code = sensor.Code;
  EXPECT_EQ(11, code);
}
