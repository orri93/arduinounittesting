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

#ifdef MODBUS_HANDLER_INTERFACE
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
#else
namespace callback {
namespace read {
static int count = 0;
static MODBUS_TYPE_RESULT coils(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length);
namespace discrete {
static int count = 0;
static MODBUS_TYPE_RESULT inputs(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length);
}
namespace holding {
static int count = 0;
static MODBUS_TYPE_RESULT registers(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length);
}
namespace input {
static int count = 0;
static MODBUS_TYPE_RESULT registers(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length);
}
namespace exception {
static int count = 0;
static MODBUS_TYPE_RESULT status();
}
}
namespace write {
static int count = 0;
static MODBUS_TYPE_RESULT coils(
  const MODBUS_TYPE_FUNCTION& function,
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length);
namespace holding {
static int count = 0;
static MODBUS_TYPE_RESULT registers(
  const MODBUS_TYPE_FUNCTION& function,
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length);
}
}
}
#endif

#ifdef MODBUS_HANDLER_INTERFACE
GatlModbusHandler::GatlModbusHandler() :
  readcoilscount(0),
  readholdingregisterscount(0),
  readinputregisterscount(0),
  writecoilscount(0),
  writeholdingregisterscount(0),
  readexceptionstatuscount(0) {
}
#endif

class GatlModbusFixture : public ::testing::Test {
public:
  void SetUp() override {
    arduinomock = arduinoMockInstance();
    serial = serialMockInstance();
    parameter.Control = 0;
    parameter.Id = 0;

    variable.Index.Write = 0;
    variable.Length.Request = 0;
    variable.Length.Response = 0;
    variable.Length.Transmission = 0;
    variable.Reading = false;
    variable.Writing = false;
    variable.Time.Half = 0;
    variable.Time.Last = 0;
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

  void begin(const int& ftimes = 1, const int& afwtimes = 1);
  void crc(const uint8_t* buffer, const uint16_t& length, const uint16_t& crc);
  uint16_t swap(const uint16_t& value);
};

#define CRC_TEST_TEXT "Abc123"

void GatlModbusFixture::begin(const int& ftimes, const int& afwtimes) {
  parameter.Control = 1;
  parameter.Id = 1;

  EXPECT_CALL(*arduinomock, pinMode(parameter.Control, OUTPUT))
    .Times(testing::Exactly(1));

  EXPECT_CALL(*arduinomock, digitalWrite(parameter.Control, LOW))
    .Times(testing::Exactly(1));

  EXPECT_CALL(*serial, setTimeout(0)).Times(testing::Exactly(1));
  if (ftimes == 1) {
    EXPECT_CALL(*serial, flush()).Times(testing::Exactly(1));
  } else if (ftimes > 1) {
    EXPECT_CALL(*serial, flush()).Times(testing::Exactly(ftimes));
  }
  if (afwtimes == 1) {
    EXPECT_CALL(*serial, availableForWrite())
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(Time));
  } else if (afwtimes > 1) {
    EXPECT_CALL(*serial, availableForWrite())
      .Times(testing::Exactly(afwtimes))
      .WillRepeatedly(testing::Return(Time));
  }

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

  EXPECT_EQ(666, variable.Length.Transmission);

  EXPECT_EQ(0, variable.Length.Request);
}

TEST_F(GatlModbusFixture, Loop) {

  MODBUS_TYPE_DEFAULT result;

  begin(3, 5);

#ifdef MODBUS_HANDLER_INTERFACE
  GatlModbusHandler handler;
#endif

  EXPECT_CALL(*serial, available())
    .Times(testing::Exactly(1))
    .WillOnce(testing::Return(0));

#ifdef MODBUS_HANDLER_INTERFACE
  result  = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
#else
  result = gatlm::loop<>(Serial, parameter, variable, request, response);
#endif

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

#ifdef MODBUS_HANDLER_INTERFACE
  result = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
#else
  result = gatlm::loop<>(Serial, parameter, variable, request, response);
#endif
  EXPECT_EQ(0, result);

  EXPECT_CALL(*serial, available())
    .Times(testing::Exactly(1))
    .WillRepeatedly(testing::Return(0));
  EXPECT_CALL(*arduinomock, micros())
    .Times(testing::Exactly(3))
    .WillRepeatedly(testing::Return(
      1 + variable.Time.Half * MODBUS_HALF_SILENCE_MULTIPLIER));
  EXPECT_CALL(*arduinomock, digitalWrite(parameter.Control, HIGH))
    .Times(testing::Exactly(2));
  EXPECT_CALL(*serial, write(response.Buffer, 6))
    .Times(testing::Exactly(2));

#ifdef MODBUS_HANDLER_INTERFACE
  result = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
#else
  result = gatlm::loop<>(Serial, parameter, variable, request, response);
#endif
  EXPECT_EQ(0, result);

#ifdef MODBUS_HANDLER_INTERFACE
  result = gatlm::loop<>(
    Serial, parameter, handler, variable, request, response);
#else
  result = gatlm::loop<>(Serial, parameter, variable, request, response);
#endif
}

TEST_F(GatlModbusFixture, ProvideDigitalIndex) {
  MODBUS_TYPE_DEFAULT index;
  MODBUS_TYPE_BIT_INDEX bitindex;
  MODBUS_TYPE_RESULT result;
  MODBUS_TYPE_DEFAULT offset;

  gatl::buffer::clear(request);
  gatl::buffer::clear(response);

  /* Negative test */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_WRITE_REGISTER;
  offset = 0;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  variable.Length.Response = 4;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_COILS;

  offset = 0;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  /* Test read coils */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_COILS;

  variable.Length.Response = 8;

  offset = 0;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 1, index);
  EXPECT_EQ(0, bitindex);

  offset = 1;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 1, index);
  EXPECT_EQ(1, bitindex);

  offset = 6;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 1, index);
  EXPECT_EQ(6, bitindex);

  /* Test discrete inputs */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_DISCRETE_INPUT;

  offset = 8;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 2, index);
  EXPECT_EQ(0, bitindex);

  offset = 12;
  result = gatlm::index::provide::digital<>(
    variable,
    request,
    response,
    offset,
    index,
    bitindex);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 2, index);
  EXPECT_EQ(4, bitindex);
}

TEST_F(GatlModbusFixture, ProvideRegistersIndex) {
  MODBUS_TYPE_DEFAULT index;
  MODBUS_TYPE_RESULT result;
  MODBUS_TYPE_DEFAULT offset;

  gatl::buffer::clear(request);

  /* Negative test */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_COILS;
  offset = 0;
  result = gatlm::index::provide::registers<>(variable, request, offset, index);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  variable.Length.Response = 4;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_HOLDING_REGISTERS;

  offset = 0;
  result = gatlm::index::provide::registers<>(variable, request, offset, index);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  /* Test holding registers */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_HOLDING_REGISTERS;

  variable.Length.Response = 16;

  offset = 0;
  result = gatlm::index::provide::registers<>(variable, request, offset, index);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 1, index);

  offset = 1;
  result = gatlm::index::provide::registers<>(variable, request, offset, index);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 1 + 2, index);

  offset = 2;
  result = gatlm::index::provide::registers<>(variable, request, offset, index);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(MODBUS_DATA_INDEX + 1 + 4, index);

  /* Last negative test */
  offset = 6;
  result = gatlm::index::provide::registers<>(variable, request, offset, index);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);
  /* EXPECT_EQ(MODBUS_DATA_INDEX + 1 + 12, index); */
}

TEST_F(GatlModbusFixture, AccessFunction) {
  MODBUS_TYPE_FUNCTION result;

  gatl::buffer::clear(request);

  /* Negative test */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = 0;

  variable.Reading = false;
  variable.Length.Request = 2;
  result = gatlm::access::function<>(variable, request);
  EXPECT_EQ(MODBUS_FC_INVALID, result);
  
  variable.Reading = true;
  variable.Length.Request = 8;
  result = gatlm::access::function<>(variable, request);
  EXPECT_EQ(MODBUS_FC_INVALID, result);

  /* Positive test */
  variable.Reading = false;
  variable.Length.Request = 8;
  for (uint8_t i = 0; i < 255; ++i) {
    request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = i;
    result = gatlm::access::function<>(variable, request);
    EXPECT_EQ(i, result);
  }
}

TEST_F(GatlModbusFixture, AccessUnitAddress) {
  MODBUS_TYPE_RESULT result;

  gatl::buffer::clear(request);

  /* Negative test */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = 0;

  variable.Reading = false;
  variable.Length.Request = 2;
  result = gatlm::access::unit::address<>(variable, request);
  EXPECT_EQ(MODBUS_INVALID_UNIT_ADDRESS, result);

  variable.Reading = true;
  variable.Length.Request = 8;
  result = gatlm::access::unit::address<>(variable, request);
  EXPECT_EQ(MODBUS_INVALID_UNIT_ADDRESS, result);

  /* Positive test */
  variable.Reading = false;
  variable.Length.Request = 8;
  for (uint8_t i = 0; i < 255; ++i) {
    request.Buffer[MODBUS_ADDRESS_INDEX] = i;
    result = gatlm::access::unit::address<>(variable, request);
    EXPECT_EQ(i, result);
  }
}

TEST_F(GatlModbusFixture, AccessBroadcastIs) {
  bool result;

  gatl::buffer::clear(request);

  /* Negative test */
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = 0;

  variable.Reading = false;
  variable.Length.Request = 2;
  result = gatlm::access::broadcast::is<>(variable, request);
  EXPECT_FALSE(result);

  variable.Reading = true;
  variable.Length.Request = 8;
  result = gatlm::access::broadcast::is<>(variable, request);
  EXPECT_FALSE(result);

  /* Positive test */
  variable.Reading = false;
  variable.Length.Request = 8;
  request.Buffer[MODBUS_ADDRESS_INDEX] = MODBUS_BROADCAST_ADDRESS;
  result = gatlm::access::broadcast::is<>(variable, request);
  EXPECT_TRUE(result);

  /* Last negative test */
  request.Buffer[MODBUS_ADDRESS_INDEX] = MODBUS_BROADCAST_ADDRESS + 1;
  result = gatlm::access::broadcast::is<>(variable, request);
  EXPECT_FALSE(result);
}

TEST_F(GatlModbusFixture, AccessCoil) {
  MODBUS_TYPE_DEFAULT offset;
  bool result;

  gatl::buffer::clear(request);
  
  /* Negative test */
  offset = 0;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_FALSE(result);

  /* Write coil */
  offset = 0;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_WRITE_COIL;
  request.Buffer[MODBUS_DATA_INDEX + 2] = 0xff;
  request.Buffer[MODBUS_DATA_INDEX + 2 + 1] = 0x00;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_TRUE(result);

  offset = 1;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_FALSE(result);

  /* Write multiple coils */
  variable.Length.Request = 10;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_WRITE_MULTIPLE_COILS;

  offset = 0;
  request.Buffer[MODBUS_DATA_INDEX + 5] = 0x00;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_FALSE(result);
  request.Buffer[MODBUS_DATA_INDEX + 5] = 0x01;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_TRUE(result);

  offset = 2;
  request.Buffer[MODBUS_DATA_INDEX + 5] = 0x00;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_FALSE(result);
  request.Buffer[MODBUS_DATA_INDEX + 5] = 0x04;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_TRUE(result);

  variable.Length.Request = 12;

  offset = 8;
  request.Buffer[MODBUS_DATA_INDEX + 5 + 1] = 0x00;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_FALSE(result);
  request.Buffer[MODBUS_DATA_INDEX + 5 + 1] = 0x01;
  result = gatlm::access::coil<>(variable, request, offset);
  EXPECT_TRUE(result);
}

TEST_F(GatlModbusFixture, AccessRegisters) {
  MODBUS_TYPE_DEFAULT offset;
  MODBUS_TYPE_DEFAULT result;

  gatl::buffer::clear(request);

  /* Negative test */
  offset = 0;
  result = gatlm::access::registers<>(variable, request, offset);
  EXPECT_EQ(0, result);

  /* Write Register */
  offset = 0;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_WRITE_REGISTER;
  request.Buffer[MODBUS_DATA_INDEX + 2] = 0x12;
  request.Buffer[MODBUS_DATA_INDEX + 2 + 1] = 0x34;
  result = gatlm::access::registers<>(variable, request, offset);
  EXPECT_EQ(0x1234, result);

  offset = 1;
  result = gatlm::access::registers<>(variable, request, offset);
  EXPECT_EQ(0x0000, result);

  /* Write multiple registers */
  variable.Length.Request = 10;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] =
    MODBUS_FC_WRITE_MULTIPLE_REGISTERS;

  offset = 0;
  request.Buffer[MODBUS_DATA_INDEX + 5] = 0x12;
  request.Buffer[MODBUS_DATA_INDEX + 5 + 1] = 0x34;
  result = gatlm::access::registers<>(variable, request, offset);
  EXPECT_EQ(0x1234, result);

  variable.Length.Request = 12;

  offset = 1;
  request.Buffer[MODBUS_DATA_INDEX + 5 + 2] = 0x23;
  request.Buffer[MODBUS_DATA_INDEX + 5 + 2 + 1] = 0x45;
  result = gatlm::access::registers<>(variable, request, offset);
  EXPECT_EQ(0x2345, result);
}

TEST_F(GatlModbusFixture, ProvideException) {
  MODBUS_TYPE_DEFAULT offset;
  MODBUS_TYPE_RESULT result;
  bool status;

  gatl::buffer::clear(request);
  gatl::buffer::clear(response);

  /* Negative test */
  result = gatlm::provide::mexception<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  variable.Length.Response = 4;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_EXCEPTION_STATUS;
  result = gatlm::provide::mexception<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  /* Test read exception */
  variable.Length.Response = 8;

  offset = 0;
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::mexception<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x01, response.Buffer[2]);

  offset = 0;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::mexception<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[2]);

  offset = 3;
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::mexception<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x08, response.Buffer[2]);

  offset = 3;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::mexception<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[2]);
}

TEST_F(GatlModbusFixture, ProvideCoil) {
  MODBUS_TYPE_DEFAULT offset;
  MODBUS_TYPE_RESULT result;
  bool status;

  gatl::buffer::clear(request);
  gatl::buffer::clear(response);

  /* Negative test */
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  variable.Length.Response = 4;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_EXCEPTION_STATUS;
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  /* Test read coils */
  variable.Length.Response = 8;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_COILS;

  offset = 0;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x01, response.Buffer[MODBUS_DATA_INDEX + 1]);

  offset = 1;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x02, response.Buffer[MODBUS_DATA_INDEX + 1]);

  offset = 6;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x40, response.Buffer[MODBUS_DATA_INDEX + 1]);

  offset = 8;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 2]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x01, response.Buffer[MODBUS_DATA_INDEX + 2]);

  offset = 12;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 2]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::coil<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x10, response.Buffer[MODBUS_DATA_INDEX + 2]);
}

TEST_F(GatlModbusFixture, ProvideDiscrete) {
  MODBUS_TYPE_DEFAULT offset;
  MODBUS_TYPE_RESULT result;
  bool status;

  gatl::buffer::clear(request);
  gatl::buffer::clear(response);

  /* Negative test */
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  variable.Length.Response = 4;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_EXCEPTION_STATUS;
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  /* Test read discrete input */
  variable.Length.Response = 8;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_DISCRETE_INPUT;

  offset = 0;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x01, response.Buffer[MODBUS_DATA_INDEX + 1]);

  offset = 1;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x02, response.Buffer[MODBUS_DATA_INDEX + 1]);

  offset = 6;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x40, response.Buffer[MODBUS_DATA_INDEX + 1]);

  offset = 8;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 2]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x01, response.Buffer[MODBUS_DATA_INDEX + 2]);

  offset = 12;
  status = false;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 2]);
  status = true;
  gatl::buffer::clear(response);
  result = gatlm::provide::discrete<>(
    variable,
    request,
    response,
    offset,
    status);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x10, response.Buffer[MODBUS_DATA_INDEX + 2]);
}

TEST_F(GatlModbusFixture, ProvideRegisters) {
  MODBUS_TYPE_DEFAULT offset;
  MODBUS_TYPE_DEFAULT value;
  MODBUS_TYPE_RESULT result;

  gatl::buffer::clear(request);
  gatl::buffer::clear(response);

  /* Negative test */
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  variable.Length.Response = 4;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_INPUT_REGISTERS;
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

  /* Test read input registers */
  variable.Length.Response = 8;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_INPUT_REGISTERS;

  offset = 0;
  value = 0x0000;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1 + 1]);
  value = 0x1234;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x12, response.Buffer[MODBUS_DATA_INDEX + 1]);
  EXPECT_EQ(0x34, response.Buffer[MODBUS_DATA_INDEX + 1 + 1]);

  variable.Length.Response = 12;

  offset = 1;
  value = 0x0000;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 3]);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 3 + 1]);
  value = 0x2345;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x23, response.Buffer[MODBUS_DATA_INDEX + 3]);
  EXPECT_EQ(0x45, response.Buffer[MODBUS_DATA_INDEX + 3 + 1]);

  offset = 2;
  value = 0x0000;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 5]);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 5 + 1]);
  value = 0x3456;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x34, response.Buffer[MODBUS_DATA_INDEX + 5]);
  EXPECT_EQ(0x56, response.Buffer[MODBUS_DATA_INDEX + 5 + 1]);

  /* Last negative test */
  offset = 6;
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);


  /* Test read holding registers */
  variable.Length.Response = 8;
  request.Buffer[MODBUS_FUNCTION_CODE_INDEX] = MODBUS_FC_READ_HOLDING_REGISTERS;

  offset = 0;
  value = 0x0000;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1]);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 1 + 1]);
  value = 0x1234;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x12, response.Buffer[MODBUS_DATA_INDEX + 1]);
  EXPECT_EQ(0x34, response.Buffer[MODBUS_DATA_INDEX + 1 + 1]);

  variable.Length.Response = 12;

  offset = 1;
  value = 0x0000;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 3]);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 3 + 1]);
  value = 0x2345;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x23, response.Buffer[MODBUS_DATA_INDEX + 3]);
  EXPECT_EQ(0x45, response.Buffer[MODBUS_DATA_INDEX + 3 + 1]);

  offset = 2;
  value = 0x0000;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 5]);
  EXPECT_EQ(0x00, response.Buffer[MODBUS_DATA_INDEX + 5 + 1]);
  value = 0x3456;
  gatl::buffer::clear(response);
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_OK, result);
  EXPECT_EQ(0x34, response.Buffer[MODBUS_DATA_INDEX + 5]);
  EXPECT_EQ(0x56, response.Buffer[MODBUS_DATA_INDEX + 5 + 1]);

  /* Last negative test */
  offset = 6;
  result = gatlm::provide::registers<>(
    variable,
    request,
    response,
    offset,
    value);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);
}

TEST_F(GatlModbusFixture, ProvideString) {
  MODBUS_TYPE_DEFAULT offset;
  MODBUS_TYPE_DEFAULT length;
  MODBUS_TYPE_RESULT result;

  gatl::buffer::clear(response);

  /* Negative test */
  result = gatlm::provide::string<>(
    variable,
    response,
    offset,
    nullptr,
    length);
  EXPECT_EQ(MODBUS_STATUS_ILLEGAL_DATA_ADDRESS, result);

}

#ifdef MODBUS_HANDLER_INTERFACE

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
#else
namespace callback {
namespace read {
MODBUS_TYPE_RESULT coils(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++count;
  return MODBUS_STATUS_OK;
}
namespace discrete {
static MODBUS_TYPE_RESULT inputs(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++count;
  return MODBUS_STATUS_OK;
}
}
namespace holding {
static MODBUS_TYPE_RESULT registers(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++count;
  return MODBUS_STATUS_OK;
}
}
namespace input {
static MODBUS_TYPE_RESULT registers(
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++count;
  return MODBUS_STATUS_OK;
}
}
namespace exception {
static MODBUS_TYPE_RESULT status() {
  ++count;
  return MODBUS_STATUS_OK;
}
}
}
namespace write {
static MODBUS_TYPE_RESULT coils(
  const MODBUS_TYPE_FUNCTION& function,
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++count;
  return MODBUS_STATUS_OK;
}
namespace holding {
static MODBUS_TYPE_RESULT registers(
  const MODBUS_TYPE_FUNCTION& function,
  const MODBUS_TYPE_DEFAULT& address,
  const MODBUS_TYPE_DEFAULT& length) {
  ++count;
  return MODBUS_STATUS_OK;
}
}
}
}
#endif

ModbusAccess::ModbusAccess() : Modbus(Serial) {
}

uint16_t ModbusAccess::CalculateCrc(uint8_t* buffer, int length) {
  return calculateCRC(buffer, length);
}
