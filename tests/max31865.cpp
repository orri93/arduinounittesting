#include <mutex>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Arduino.h>
#include <SPI.h>

#include <gtest/gtest.h>

#include <PlayingWithFusion_MAX31865.h>

#include <gos/utils/order.h>

#include <gosmax31865.h>
#include <arduinosensor.h>

namespace gatub = ::gos::arduino::testing::utils::byte;

static double PlayingWithFusion(double& res, const uint16_t& raw, const uint8_t& type);
static double GosMax31865(double& res, const uint16_t& raw, const uint8_t& type);

class Max31865Test : public ::testing::Test {
protected:

  void SetUp() {
    arduinomock = arduinoMockInstance();
    spimock = spiMockInstance();
  }

  void TearDown() {
    releaseSpiMock();
    releaseArduinoMock();
  }

  ArduinoMock* arduinomock;
  SpiMock* spimock;

  std::mutex mutex;
};

TEST_F(Max31865Test, PwFusionMax31865) {

  bool popresult;
  uint8_t pop8value;

  struct var_max31865 *rtd_ptr;

  const int CS0_PIN = 9;
  const int CS1_PIN = 10;

  EXPECT_CALL(*arduinomock, digitalWrite(CS0_PIN, HIGH)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, digitalWrite(CS0_PIN, LOW)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, digitalWrite(CS1_PIN, HIGH)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, digitalWrite(CS1_PIN, LOW)).
    Times(testing::AtLeast(1));

  PWFusion_MAX31865_RTD rtd_ch0(CS0_PIN);
  PWFusion_MAX31865_RTD rtd_ch1(CS1_PIN);

  // structures for holding RTD values
  static struct var_max31865 RTD_CH0;
  static struct var_max31865 RTD_CH1;

  // configure channel 0
  rtd_ptr = &RTD_CH0;
  rtd_ch0.MAX31865_config(rtd_ptr, RTD_3_WIRE);
  RTD_CH0.RTD_type = 1;                         // un-comment for PT100 RTD
  // RTD_CH0.RTD_type = 2;                        // un-comment for PT1000 RTD

  popresult = spimock->In.popbyte(pop8value);
  EXPECT_TRUE(popresult);
  EXPECT_EQ(0x80, pop8value);               /* Send config register location to chip */
  popresult = spimock->In.popbyte(pop8value);
  EXPECT_TRUE(popresult);
  EXPECT_EQ(0xC2 | RTD_3_WIRE, pop8value);  /* wire select */
  popresult = spimock->In.popbyte(pop8value);
  EXPECT_FALSE(popresult);

  EXPECT_EQ(RTD_3_WIRE, rtd_ptr->RTD_config);
  EXPECT_EQ(1, rtd_ptr->RTD_type);

  // configure channel 1
  rtd_ptr = &RTD_CH1;
  rtd_ch1.MAX31865_config(rtd_ptr, RTD_2_WIRE);
  //RTD_CH1.RTD_type = 1;                         // un-comment for PT100 RTD
  RTD_CH1.RTD_type = 2;                        // un-comment for PT1000 RTD

  popresult = spimock->In.popbyte(pop8value);
  EXPECT_TRUE(popresult);
  EXPECT_EQ(0x80, pop8value);               /* Send config register location to chip */
  popresult = spimock->In.popbyte(pop8value);
  EXPECT_TRUE(popresult);
  EXPECT_EQ(0xC2 | RTD_2_WIRE, pop8value);  /* wire select */
  popresult = spimock->In.popbyte(pop8value);
  EXPECT_FALSE(popresult);

  EXPECT_EQ(RTD_2_WIRE, rtd_ptr->RTD_config);
  EXPECT_EQ(2, rtd_ptr->RTD_type);

  const uint8_t spiout1[] = { 0x00, 0xaf, 0x10, 0xff, 0x20, 0xfe, 0x40, 0xfd, 0x00 };
  spimock->Out.pushbytes(spiout1, sizeof(spiout1));

  rtd_ptr = &RTD_CH0;
  rtd_ch0.MAX31865_full_read(rtd_ptr);          // Update MAX31855 readings

  EXPECT_EQ(spiout1[1], rtd_ptr->conf_reg);
  EXPECT_EQ(((spiout1[2] << 8) | spiout1[3]) >> 1, rtd_ptr->rtd_res_raw);
  EXPECT_EQ(((spiout1[4] << 8) | spiout1[5]) >> 1, rtd_ptr->HFT_val);
  EXPECT_EQ((spiout1[6] << 8) | spiout1[7], rtd_ptr->LFT_val);
  EXPECT_EQ(spiout1[8], rtd_ptr->status);

  EXPECT_EQ(9, spimock->In.bytes());
  spimock->In.clear();

  const uint8_t spiout2[] = { 0x00, 0xbf, 0x20, 0xff, 0x30, 0xfe, 0x80, 0xfd, 0x00 };
  spimock->Out.pushbytes(spiout2, sizeof(spiout2));

  rtd_ptr = &RTD_CH1;
  rtd_ch1.MAX31865_full_read(rtd_ptr);          // Update MAX31855 readings 

  EXPECT_EQ(spiout2[1], rtd_ptr->conf_reg);
  EXPECT_EQ(((spiout2[2] << 8) | spiout2[3]) >> 1, rtd_ptr->rtd_res_raw);
  EXPECT_EQ(((spiout2[4] << 8) | spiout2[5]) >> 1, rtd_ptr->HFT_val);
  EXPECT_EQ((int16_t)(spiout2[6] << 8) | spiout2[7], rtd_ptr->LFT_val);
  EXPECT_EQ(spiout2[8], rtd_ptr->status);
  EXPECT_EQ(9, spimock->In.bytes());
  spimock->In.clear();
}

TEST_F(Max31865Test, byteorder) {
  uint16_t word;
  const uint8_t spiout1[] = { 0x00, 0xaf, 0x10, 0xff, 0x20, 0xfe, 0x40, 0xfd, 0x00 };
  EXPECT_EQ(0x10ff, (spiout1[2] << 8) | spiout1[3]);
  EXPECT_EQ(0x20fe, (spiout1[4] << 8) | spiout1[5]);
  EXPECT_EQ(0x40fd, (spiout1[6] << 8) | spiout1[7]);

  word = gatub::combine(spiout1[2], spiout1[3]);
  EXPECT_EQ(0x10ff, word);
  word = gatub::combine(spiout1[4], spiout1[5]);
  EXPECT_EQ(0x20fe, word);
  word = gatub::combine(spiout1[6], spiout1[7]);
  EXPECT_EQ(0x40fd, word);
}

TEST_F(Max31865Test, Behavior) {

  mutex.lock();

  const uint8_t pin = 8;
  const uint32_t clock = 16000000 / 64;

  const uint8_t wire = RTD_3_WIRE;
  const uint8_t type = RTD_TYPE_PT1000;

  bool popresult, readresult;
  uint8_t pop8value;
  double readvalue;

  ::gos::Max31865 max31865(pin, clock, SPI_MODE1, MSBFIRST);
  EXPECT_CALL(*arduinomock, digitalWrite(pin, HIGH)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, LOW)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, pinMode(pin, OUTPUT));
  max31865.initialize(type, wire);

  popresult = spimock->In.popbyte(pop8value);
  EXPECT_TRUE(popresult);
  EXPECT_EQ(0x80, pop8value);               /* Send config register location to chip */
  popresult = spimock->In.popbyte(pop8value);
  EXPECT_TRUE(popresult);
  EXPECT_EQ(0xC2 | RTD_3_WIRE, pop8value);  /* wire select */
  popresult = spimock->In.popbyte(pop8value);
  EXPECT_FALSE(popresult);

  const uint8_t spioutbytes[] = { 0x00, 0xaf };
  spimock->Out.pushbytes(spioutbytes, sizeof(spioutbytes));
  EXPECT_EQ(2, spimock->Out.bytes());
  const uint16_t spioutwords[] = { 0x10ff, 0x20fe, 0x40fd };
  spimock->Out.pushwords(spioutwords, sizeof(spioutwords) / 2);
  EXPECT_EQ(8, spimock->Out.bytes());
  spimock->Out.pushbyte(0x00); /* Status */
  EXPECT_EQ(9, spimock->Out.bytes());

  readresult = max31865.read(readvalue);
  EXPECT_TRUE(readresult);
  EXPECT_EQ(0, spimock->Out.bytes());
  EXPECT_EQ(9, spimock->In.bytes());
  spimock->In.clear();

  uint16_t raw = 0x10ff >> 1;
  double pwfvalue, pwfres;

  pwfvalue = PlayingWithFusion(pwfres, raw, RTD_TYPE_PT1000);

  EXPECT_DOUBLE_EQ(pwfvalue, readvalue);
  EXPECT_DOUBLE_EQ(-188.03125, readvalue);

  mutex.unlock();
}

TEST_F(Max31865Test, error) {
  spimock->Out.pushword(0x0004);
  spimock->Out.pushword(0x0400);

  const uint8_t pin = 8;
  const char* error = nullptr;
  uint8_t length;
  double value;
  bool readresult;
  ::gos::Max31865 max31865(pin);
  EXPECT_CALL(*arduinomock, pinMode(pin, OUTPUT)).
    Times(testing::Exactly(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, HIGH)).
    Times(testing::AtLeast(1));
  EXPECT_CALL(*arduinomock, digitalWrite(pin, LOW)).
    Times(testing::AtLeast(1));
  max31865.initialize();
  readresult = max31865.read(value);
  error = max31865.error(length);
  EXPECT_TRUE(error == nullptr);
  EXPECT_EQ(0, length);
}

TEST_F(Max31865Test, calculation) {
  uint8_t type;
  double pwfres, gosres, pwfcelsius, goscelsius;

  type = RTD_TYPE_PT100;
  for (uint16_t raw = 1; raw <= 0x0fff; raw += 8) {
    pwfcelsius = PlayingWithFusion(pwfres, raw, type);
    goscelsius = GosMax31865(gosres, raw, type);
    EXPECT_DOUBLE_EQ(pwfcelsius, goscelsius);
    EXPECT_DOUBLE_EQ(pwfres, gosres);
  }
  
  type = RTD_TYPE_PT1000;
  for (uint16_t raw = 1; raw <= 0x0fff; raw += 8) {
    pwfcelsius = PlayingWithFusion(pwfres, raw, type);
    goscelsius = GosMax31865(gosres, raw, type);
    EXPECT_DOUBLE_EQ(pwfcelsius, goscelsius);
    EXPECT_DOUBLE_EQ(pwfres, gosres);
  }
}

double PlayingWithFusion(double& res, const uint16_t& raw, const uint8_t& type) {
  switch (type) {
  case RTD_TYPE_PT100:
    res = (double)raw * 400 / 32768;
    return ((double)raw/32) - 256;
  case RTD_TYPE_PT1000:
    res = (double)raw * 4000 / 32768;
    return ((double)raw / 32) - 256;;
  default:
    return NAN;
  }
}
double GosMax31865(double& res, const uint16_t& raw, const uint8_t& type) {
  switch (type) {
  case RTD_TYPE_PT100:
    res = 400.0 * static_cast<double>(raw) / 32768.0;
    return -256.0 + static_cast<double>(raw) / 32.0;
  case RTD_TYPE_PT1000:
    res = 4000.0 * static_cast<double>(raw) / 32768.0;
    return -256.0 + static_cast<double>(raw) / 32.0;
  default:
    return NAN;
  }

}
