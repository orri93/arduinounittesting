#include <memory>
#include <mutex>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlmodbus.h>
#include <gatlbuffer.h>

#include <ArduinoModbusSlave/src/ModbusSlave.h>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <FixedPoints/SFixed.h>

#include <gos/utils/expect.h>
#include <gos/utils/memory.h>
#include <gos/utils/binding.h>

#define MODBUS_BUFFER_SIZE 64

namespace gatum = ::gos::arduino::testing::utils::memory;
namespace gatub = ::gos::arduino::testing::utils::binding;

namespace gatl = ::gos::atl;
namespace gatlb = ::gos::atl::binding;

namespace gatl = ::gos::atl;
namespace gatlm = ::gos::atl::modbus;

gatl::buffer::Holder<uint16_t, MODBUS_TYPE_BUFFER> request(MODBUS_BUFFER_SIZE);
gatl::buffer::Holder<uint16_t, MODBUS_TYPE_BUFFER> response(MODBUS_BUFFER_SIZE);

class ModbusAccess : public Modbus {
public:
  ModbusAccess();
  uint16_t CalculateCrc(uint8_t* buffer, int length);
};

class GatlModbusHandler : public virtual gatlm::Handler<> {
public:
  GatlModbusHandler();
  MODBUS_TYPE_RESULT ReadCoils(
    const MODBUS_TYPE_DEFAULT& address,
    const MODBUS_TYPE_DEFAULT& length);
  MODBUS_TYPE_RESULT ReadHoldingRegisters(
    const MODBUS_TYPE_DEFAULT& address,
    const MODBUS_TYPE_DEFAULT& length);
  MODBUS_TYPE_RESULT ReadInputRegisters(
    const MODBUS_TYPE_DEFAULT& address,
    const MODBUS_TYPE_DEFAULT& length);
  MODBUS_TYPE_RESULT WriteCoils(
    const MODBUS_TYPE_FUNCTION& function,
    const MODBUS_TYPE_DEFAULT& address,
    const MODBUS_TYPE_DEFAULT& length);
  MODBUS_TYPE_RESULT WriteHoldingRegisters(
    const MODBUS_TYPE_FUNCTION& function,
    const MODBUS_TYPE_DEFAULT& address,
    const MODBUS_TYPE_DEFAULT& length);
  MODBUS_TYPE_RESULT ReadExceptionStatus();

  int readcoilscount;
  int readholdingregisterscount;
  int readinputregisterscount;
  int writecoilscount;
  int writeholdingregisterscount;
  int readexceptionstatuscount;
};

GatlModbusHandler::GatlModbusHandler() :
  readcoilscount(0),
  readholdingregisterscount(0),
  readinputregisterscount(0),
  writecoilscount(0),
  writeholdingregisterscount(0),
  readexceptionstatuscount(0) {
}

class GatlModbusFixture : public ::testing::Test {
public:
  void SetUp() override {
    arduinomock = arduinoMockInstance();
    serial = serialMockInstance();
  }

  void TearDown() override {
    releaseSerialMock();
    releaseArduinoMock();
  }

  ArduinoMock* arduinomock;
  HardwareSerialMock* serial;

  std::mutex mutex;

  gatlm::structures::Parameter<> parameter;
  gatlm::structures::Variable<> variable;

  const MODBUS_TYPE_RATE Rate = 9600;
  const MODBUS_TYPE_DEFAULT Available = 93;
  const MODBUS_TYPE_TIME Time = 666;

  void begin();
  void crc(const uint8_t* buffer, const uint16_t& length, const uint16_t& crc);
  uint16_t swap(const uint16_t& value);
};

#define CRC_TEST_TEXT "Abc123"

void GatlModbusFixture::begin() {
  parameter.Control = 1;
  parameter.Id = 1;

  EXPECT_CALL(*arduinomock, pinMode(parameter.Control, OUTPUT))
    .Times(testing::Exactly(1));

  EXPECT_CALL(*arduinomock, digitalWrite(parameter.Control, LOW))
    .Times(testing::Exactly(1));

  EXPECT_CALL(*serial, setTimeout(0)).Times(testing::Exactly(1));
  EXPECT_CALL(*serial, flush()).Times(testing::Exactly(1));
  EXPECT_CALL(*serial, availableForWrite())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(Available));

  EXPECT_CALL(*arduinomock, micros())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(Time));

  gatlm::begin<>(Serial, parameter, variable, Rate);
}

void GatlModbusFixture::crc(
  const uint8_t* buffer,
  const uint16_t& length,
  const uint16_t& crc) {
  uint16_t swapedcrc = swap(crc);
  ModbusAccess ma;
  uint16_t crcams = ma.CalculateCrc(
    const_cast<uint8_t*>(buffer),
    length);
  gatl::buffer::Holder<uint16_t, MODBUS_TYPE_BUFFER> holder(length);
  ::memcpy(holder.Buffer, buffer, length);
  uint16_t gatlmcrc = gatlm::details::crc::calculate<uint16_t>(
    holder, length);
  EXPECT_EQ(swapedcrc, crcams);
  EXPECT_EQ(swapedcrc, gatlmcrc);
}

uint16_t GatlModbusFixture::swap(const uint16_t& value) {
  uint8_t fb = value & 0xff;
  uint8_t sb = value >> 8;

  uint16_t word = static_cast<uint16_t>(fb) << 8;
  word |= sb;

  return word;
}

TEST_F(GatlModbusFixture, CRC) {
  ModbusAccess ma;
  char* text = const_cast<char*>(CRC_TEST_TEXT);
  uint8_t* buffer = reinterpret_cast<uint8_t*>(text);
  uint16_t crcams = ma.CalculateCrc(buffer, sizeof(CRC_TEST_TEXT));

  gatl::buffer::Holder<uint16_t, MODBUS_TYPE_BUFFER> holder(
    buffer, sizeof(CRC_TEST_TEXT));

  uint16_t gatlmcrc = gatlm::details::crc::calculate<uint16_t>(
    holder, sizeof(CRC_TEST_TEXT));

  EXPECT_EQ(crcams, gatlmcrc);

  const uint8_t bytebuffera[] = { 0x01, 0x01, 0x00, 0x00, 0x00, 0x01 };
  crc(bytebuffera, 6, 0xfdca);

  const uint8_t bytebufferb[] = { 0x01, 0x01, 0x01, 0x00 };
  crc(bytebufferb, 4, 0x5188);
}

TEST_F(GatlModbusFixture, Begin) {
  begin();

  EXPECT_EQ(0, variable.Index.Write);
  EXPECT_EQ(0, variable.Length.Response);
  EXPECT_EQ(false, variable.Reading);
  EXPECT_EQ(false, variable.Writing);

  EXPECT_EQ(93, variable.Length.Transmission);

  EXPECT_EQ(0, variable.Length.Request);
}

TEST_F(GatlModbusFixture, Loop) {

  MODBUS_TYPE_DEFAULT result;

  begin();

  GatlModbusHandler handler;

  EXPECT_CALL(*serial, available())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(0));

  result  = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);

  EXPECT_EQ(0, result);

  const uint8_t ReadCoilRequest[] = {
    0x01, 0x01, 0x00, 0x0B, 0x00, 0x01, 0x8C, 0x08 };

  EXPECT_CALL(*serial, available())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(8));
  EXPECT_CALL(*serial, readBytes(request.Buffer, 8))
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(8));
  ::memcpy(request.Buffer, ReadCoilRequest, 8);
  EXPECT_CALL(*arduinomock, micros())
    .Times(testing::Exactly(2))
    .WillRepeatedly(testing::Return(0));

  result = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
  EXPECT_EQ(0, result);

  EXPECT_CALL(*serial, available())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(0));
  EXPECT_CALL(*arduinomock, micros())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(
      1 + variable.Time.Half * MODBUS_HALF_SILENCE_MULTIPLIER));
  result = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
  EXPECT_EQ(0, result);

  EXPECT_CALL(*serial, available())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(0));

  result = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
}

MODBUS_TYPE_RESULT GatlModbusHandler::ReadCoils(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++readcoilscount;
  return MODBUS_STATUS_OK;
}

MODBUS_TYPE_RESULT GatlModbusHandler::ReadHoldingRegisters(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++readholdingregisterscount;
  return MODBUS_STATUS_OK;
}

MODBUS_TYPE_RESULT GatlModbusHandler::ReadInputRegisters(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++readinputregisterscount;
  return MODBUS_STATUS_OK;
}

MODBUS_TYPE_RESULT GatlModbusHandler::WriteCoils(
  const MODBUS_TYPE_FUNCTION& function,
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++writecoilscount;
  return MODBUS_STATUS_OK;
}

MODBUS_TYPE_RESULT GatlModbusHandler::WriteHoldingRegisters(
  const MODBUS_TYPE_FUNCTION& function,
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++writeholdingregisterscount;
  return MODBUS_STATUS_OK;
}

MODBUS_TYPE_RESULT GatlModbusHandler::ReadExceptionStatus() {
  ++readexceptionstatuscount;
  return MODBUS_STATUS_OK;
}

ModbusAccess::ModbusAccess() : Modbus(Serial) {
}

uint16_t ModbusAccess::CalculateCrc(uint8_t* buffer, int length) {
  return calculateCRC(buffer, length);
}
