#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatltype.h>
#include <gatlutility.h>
#include <gatlpid2.h>

namespace gatl = ::gos::atl;

class GatlPid2Fixture : public ::testing::Test {
protected:
  void SetUp() {
    //arduinomock = arduinoMockInstance();
  }

  void TearDown() {
    //releaseArduinoMock();
  }

};

TEST_F(GatlPid2Fixture, InitializeDouble) {
  gatl::pid::wiki::Variable<double> variable;
  gatl::pid::wiki::initialize<double>(variable);

  EXPECT_DOUBLE_EQ(0.0, variable.Integral);
  EXPECT_DOUBLE_EQ(0.0, variable.LastError);
}

TEST_F(GatlPid2Fixture, ComputeDouble) {
  unsigned int output;

  gatl::pid::wiki::Parameter<double, unsigned int, double> parameter;
  gatl::pid::wiki::Variable<double> variable;
  gatl::pid::Tune<double> k;

  parameter.Kp = 1.57717;
  parameter.Time = 2.0;
  parameter.Range = gatl::type::make_range<unsigned int>(0, 255);
  parameter.Setpoint = 93.0;

  k.Ki = 0.00348162;
  k.Kd = 178.615;

  gatl::pid::wiki::initialize<double>(variable);

  variable.Integral = -2445.0;
  variable.LastError = -12.0;

  output = gatl::pid::wiki::compute<double, unsigned int, double, double>(
    100.0,
    variable,
    parameter,
    k);
}
