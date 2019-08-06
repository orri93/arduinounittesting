#include <vector>
#include <iostream>
#include <mutex>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <FixedPoints/SFixed.h>
#include <FixedPoints.h>
#include <FixedPointsCommon.h>

#include <PID_v1.h>

#include <gos/utils/random.h>

#include <gatlpid.h>

namespace gatl = ::gos::atl;
namespace gatu = ::gos::arduino::testing::utils;

bool gatl_pid_debug = false;

template<typename Q> void expectqeq(const Q& q1, const Q& q2) {
  EXPECT_EQ(q1.getInteger(), q2.getInteger());
  EXPECT_EQ(q1.getFraction(), q2.getFraction());
}

class TestabePid : public PID {
public:
  TestabePid(
    double* Input,
    double* Output,
    double* Setpoint,
    double Kp,
    double Ki,
    double Kd,
    int POn,
    int ControllerDirection) : PID(
      Input,
      Output,
      Setpoint,
      Kp,
      Ki,
      Kd,
      POn,
      ControllerDirection) {
  }
  TestabePid(
    double* Input,
    double* Output,
    double* Setpoint,
    double Kp,
    double Ki,
    double Kd,
    int ControllerDirection) : PID(
      Input,
      Output,
      Setpoint,
      Kp,
      Ki,
      Kd,
      ControllerDirection) {
  }
  inline double innerkp() {
    return kp;
  }
  inline double innerki() {
    return ki;
  }
  inline double innerkd() {
    return kd;
  }
  inline double accessoutmin() {
    return outMin;
  }
  inline double accessoutmax() {
    return outMax;
  }
  inline double accessoutputsum() {
    return outputSum;
  }
  inline double accesslastinput() {
    return lastInput;
  }
  inline unsigned long accesssampletime() {
    return SampleTime;
  }
};

class GatlPidFixture : public ::testing::Test {
protected:

  void SetUp() {
    arduinomock = arduinoMockInstance();
  }

  void TearDown() {
    releaseArduinoMock();
  }

  template<typename I, typename O = I, typename P = I>
  void test(
    const O& minimum,
    const O& maximum,
    const P& timems,
    const P& kp,
    const P& ki,
    const P& kd,
    const I& input,
    const I& setpoint,
    const O& output,
    const bool& pone,
    const unsigned long& seed,
    const long& randommin,
    const long& randommax,
    const double& randomprecision,
    const size_t& count) {
    mutex.lock();
    ArduinoMock* arduinomock = arduinoMockInstance();

    randomSeed(seed);

    unsigned long tickinterval = static_cast<unsigned long>(timems) + 1;
    unsigned long tick = tickinterval * 2;

    double testinput = static_cast<double>(input);
    double testoutput = static_cast<double>(output);
    double testsetpoint = static_cast<double>(setpoint);
    double testkp = static_cast<double>(kp);
    double testki = static_cast<double>(ki);
    double testkd = static_cast<double>(kd);
    double testmin = static_cast<double>(minimum);
    double testmax = static_cast<double>(maximum);
    EXPECT_CALL(*arduinomock, millis()).WillOnce(testing::Return(tick));
    TestabePid pid(
      &testinput,
      &testoutput,
      &testsetpoint,
      testkp,
      testki,
      testkd,
      pone ? P_ON_E : P_ON_M,
      DIRECT);
    pid.SetSampleTime(static_cast<int>(timems));
    pid.SetOutputLimits(testmin, testmax);
    pid.SetMode(AUTOMATIC);
    unsigned long ptimems = static_cast<unsigned long>(timems);
    EXPECT_EQ(ptimems, pid.accesssampletime());
    tick += tickinterval;

    gatl::pid::Parameter<I, O, P> parameter;
    gatl::pid::initialize(
      parameter,
      minimum,
      maximum,
      timems,
      kp,
      ki,
      kd,
      pone);
    parameter.Setpoint = setpoint;
    gatl::pid::Variable<I, O> variable;
    gatl::pid::initialize(variable, parameter, input, output);

    double kitimestime = static_cast<double>(ki) * static_cast<double>(timems);
    double kddividedbytime = static_cast<double>(kd) / static_cast<double>(timems);
    EXPECT_DOUBLE_EQ(kitimestime, parameter.KiTimesTime);
    EXPECT_DOUBLE_EQ(kddividedbytime, parameter.KdDividedByTime);

    double pmin = static_cast<double>(parameter.Minimum);
    double pmax = static_cast<double>(parameter.Maximum);
    double pkp = static_cast<double>(parameter.Kp);
    double pki = static_cast<double>(parameter.KiTimesTime) / 1000.0;
    double pkd = 1000.0 * static_cast<double>(parameter.KdDividedByTime);
    EXPECT_DOUBLE_EQ(pid.accessoutmin(), pmin);
    EXPECT_DOUBLE_EQ(pid.accessoutmax(), pmax);
    EXPECT_DOUBLE_EQ(pid.innerkp(), pkp);
    EXPECT_DOUBLE_EQ(pid.innerki(), pki);
    EXPECT_DOUBLE_EQ(pid.innerkd(), pkd);

    for (size_t i = 0; i < count; i++) {
      double rinput = gatu::random::generate<double>(randommin, randommax) / randomprecision;
      testinput = rinput;

      EXPECT_CALL(*arduinomock, millis()).WillOnce(testing::Return(tick));
      bool pidresult = pid.Compute();
      EXPECT_TRUE(pidresult);

      I gatlinput = static_cast<I>(rinput);
      O gatloutput;

      gatloutput = gatl::pid::compute(gatlinput, variable, parameter);
      double gatlcastoutput = static_cast<double>(gatloutput);
      EXPECT_DOUBLE_EQ(testoutput, gatlcastoutput);

      tick += tickinterval;
    }


    releaseArduinoMock();
    mutex.unlock();
  }

  template<typename I, typename O = I, typename P = I>
  void testq(
    const O& minimum,
    const O& maximum,
    const P& timems,
    const P& kp,
    const P& ki,
    const P& kd,
    const I& input,
    const I& setpoint,
    const O& output,
    const bool& pone,
    const unsigned long& seed,
    const long& randommin,
    const long& randommax,
    const double& randomprecision,
    const size_t& count) {
    mutex.lock();
    ArduinoMock* arduinomock = arduinoMockInstance();

    randomSeed(seed);

    unsigned long tickinterval = static_cast<unsigned long>(timems.getInteger()) + 1;
    unsigned long tick = tickinterval * 2;

    double testinput = static_cast<double>(input);
    double testoutput = static_cast<double>(output);
    double testsetpoint = static_cast<double>(setpoint);
    double testkp = static_cast<double>(kp);
    double testki = static_cast<double>(ki);
    double testkd = static_cast<double>(kd);
    double testmin = static_cast<double>(minimum);
    double testmax = static_cast<double>(maximum);
    EXPECT_CALL(*arduinomock, millis()).WillOnce(testing::Return(tick));
    TestabePid pid(
      &testinput,
      &testoutput,
      &testsetpoint,
      testkp,
      testki,
      testkd,
      pone ? P_ON_E : P_ON_M,
      DIRECT);
    pid.SetSampleTime(static_cast<int>(timems.getInteger()));
    pid.SetOutputLimits(testmin, testmax);
    pid.SetMode(AUTOMATIC);
    unsigned long ptimems = static_cast<unsigned long>(timems.getInteger());
    EXPECT_EQ(ptimems, pid.accesssampletime());
    double ptimes = ptimems / 1000.0;
    EXPECT_DOUBLE_EQ(testki * ptimes, pid.innerki());
    EXPECT_DOUBLE_EQ(testkd / ptimes, pid.innerkd());
    tick += tickinterval;

    gatl::pid::Parameter<I, O, P> parameter;
    gatl::pid::initialize(
      parameter,
      minimum,
      maximum,
      timems,
      kp,
      ki,
      kd,
      pone);
    parameter.Setpoint = setpoint;
    gatl::pid::Variable<I, O> variable;
    gatl::pid::initialize(variable, parameter, input, output);

    P kitimestime = ki * timems;
    P kddividedbytime = kd / timems;
    expectqeq(kitimestime, parameter.KiTimesTime);
    expectqeq(kddividedbytime, parameter.KdDividedByTime);

    double pmin = static_cast<double>(parameter.Minimum);
    double pmax = static_cast<double>(parameter.Maximum);
    double pkp = static_cast<double>(parameter.Kp);
    double pki = static_cast<double>(parameter.KiTimesTime) / 1000.0;
    double pkd = 1000.0 * static_cast<double>(parameter.KdDividedByTime);
    EXPECT_DOUBLE_EQ(pid.accessoutmin(), pmin);
    EXPECT_DOUBLE_EQ(pid.accessoutmax(), pmax);
    EXPECT_DOUBLE_EQ(pid.innerkp(), pkp);
    EXPECT_DOUBLE_EQ(pid.innerki(), pki);
    EXPECT_NEAR(pid.innerkd(), pkd, 0.05);

    gatl_pid_debug = true;

    for (size_t i = 0; i < count; i++) {
      double rinput = gatu::random::generate<double>(randommin, randommax) / randomprecision;
      testinput = rinput;

      EXPECT_CALL(*arduinomock, millis()).WillOnce(testing::Return(tick));
      bool pidresult = pid.Compute();
      EXPECT_TRUE(pidresult);

      I gatlinput = static_cast<I>(rinput);
      O gatloutput;

      gatloutput = gatl::pid::compute(gatlinput, variable, parameter);
      double gatlcastoutput = static_cast<double>(gatloutput);
      EXPECT_NEAR(testoutput, gatlcastoutput, 1.0);

      tick += tickinterval;
    }


    releaseArduinoMock();
    mutex.unlock();
  }

  ArduinoMock* arduinomock;

  std::mutex mutex;

};

TEST_F(GatlPidFixture, ComputeDouble) {
  test<double>(     /* Types for Input, Output and Parameter */
    (double)10,     /* Output Minimum   */
    (double)1024,   /* Output Maximum   */
    (double)100,    /* Time in ms       */
    (double)2,      /* Kp               */
    (double)5,      /* Ki               */
    (double)0.5,    /* Kd               */
    11.666,         /* Initial input    */
    418.11,         /* Setpoint         */
    93.418,         /* Initial output   */
    false,          /* P on E           */
    6,              /* Random Seed      */
    10,             /* Random minimum   */
    1024,           /* Random maximum   */
    10.0,           /* Random precision */
    256);           /* Count            */

  test<double>(     /* Types for Input, Output and Parameter */
    (double)10,     /* Output Minimum   */
    (double)1024,   /* Output Maximum   */
    (double)100,    /* Time in ms       */
    (double)2,      /* Kp               */
    (double)5,      /* Ki               */
    (double)0.5,    /* Kd               */
    11.666,         /* Initial input    */
    418.11,         /* Setpoint         */
    93.418,         /* Initial output   */
    true,           /* P on E           */
    6,              /* Random Seed      */
    10,             /* Random minimum   */
    1024,           /* Random maximum   */
    10.0,           /* Random precision */
    256);           /* Count            */

  test<double>(     /* Types for Input, Output and Parameter */
    (double)0,      /* Output Minimum   */
    (double)255,    /* Output Maximum   */
    (double)50,     /* Time in ms       */
    (double)1.0,    /* Kp               */
    (double)6.2,    /* Ki               */
    (double)0.1,    /* Kd               */
    12.345,         /* Initial input    */
    673.2,          /* Setpoint         */
    5.5,            /* Initial output   */
    true,           /* P on E           */
    2,              /* Random Seed      */
   -1024,           /* Random minimum   */
    1024,           /* Random maximum   */
    100.0,          /* Random precision */
    256);           /* Count            */
}

TEST_F(GatlPidFixture, ComputeInteger) {
  typedef ::FixedPoints::SQ15x16 InputType;
  typedef ::FixedPoints::SQ15x16 OutputType;
  typedef ::FixedPoints::SQ15x16 ParameterType;

  /* Types for Input, Output and Parameter */
  testq<InputType, OutputType, ParameterType>(
    10.12,          /* Output Minimum   */
    1024.12,        /* Output Maximum   */
    50.0,           /* Time in ms       */
    2.5,            /* Kp               */
    5.1,            /* Ki               */
    0.5,            /* Kd               */
    11.666,         /* Initial input    */
    418.11,         /* Setpoint         */
    93.418,         /* Initial output   */
    false,          /* P on E           */
    6,              /* Random Seed      */
    10,             /* Random minimum   */
    1024,           /* Random maximum   */
    10.0,           /* Random precision */
    256);           /* Count            */

  /* Types for Input, Output and Parameter */
  testq<InputType, OutputType, ParameterType>(
    10,             /* Output Minimum   */
    1024,           /* Output Maximum   */
    100,            /* Time in ms       */
    2,              /* Kp               */
    5,              /* Ki               */
    0.5,            /* Kd               */
    11.666,         /* Initial input    */
    418.11,         /* Setpoint         */
    93.418,         /* Initial output   */
    true,           /* P on E           */
    6,              /* Random Seed      */
    10,             /* Random minimum   */
    1024,           /* Random maximum   */
    10.0,           /* Random precision */
    256);           /* Count            */

  /* Types for Input, Output and Parameter */
  testq<InputType, OutputType, ParameterType>(
    0x00,           /* Output Minimum   */
    0xff,           /* Output Maximum   */
    0x20,           /* Time in ms       */
    1.0,            /* Kp               */
    6.2,            /* Ki               */
    0.1,            /* Kd               */
    12.34,          /* Initial input    */
    675.93,         /* Setpoint         */
    0x10,           /* Initial output   */
    true,           /* P on E           */
    2,              /* Random Seed      */
    -1024,          /* Random minimum   */
    1024,           /* Random maximum   */
    100.0,          /* Random precision */
    256);           /* Count            */
}