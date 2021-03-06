#ifndef MODBUSSLAVE_H
#define MODBUSSLAVE_H

#include <memory>

#include <Arduino.h>

#define MODBUS_MAX_BUFFER 256
#define MODBUS_INVALID_UNIT_ADDRESS 255
#define MODBUS_DEFAULT_UNIT_ADDRESS 1
#define MODBUS_CONTROL_PIN_NONE -1

/**
 * Modbus function codes
 */
enum {
  FC_INVALID = 0,
  FC_READ_COILS = 1,
  FC_READ_DISCRETE_INPUT = 2,
  FC_READ_HOLDING_REGISTERS = 3,
  FC_READ_INPUT_REGISTERS = 4,
  FC_WRITE_COIL = 5,
  FC_WRITE_REGISTER = 6,
  FC_READ_EXCEPTION_STATUS = 7,
  FC_WRITE_MULTIPLE_COILS = 15,
  FC_WRITE_MULTIPLE_REGISTERS = 16
};

enum {
  CB_MIN = 0,
  CB_READ_COILS = CB_MIN,
  CB_READ_DISCRETE_INPUTS,
  CB_READ_HOLDING_REGISTERS,
  CB_READ_INPUT_REGISTERS,
  CB_WRITE_COILS,
  CB_WRITE_HOLDING_REGISTERS,
  CB_READ_EXCEPTION_STATUS,
  CB_MAX
};

enum {
  COIL_OFF = 0x0000,
  COIL_ON = 0xff00
};

enum {
  STATUS_OK = 0,
  STATUS_ILLEGAL_FUNCTION,
  STATUS_ILLEGAL_DATA_ADDRESS,
  STATUS_ILLEGAL_DATA_VALUE,
  STATUS_SLAVE_DEVICE_FAILURE,
  STATUS_ACKNOWLEDGE,
  STATUS_SLAVE_DEVICE_BUSY,
  STATUS_NEGATIVE_ACKNOWLEDGE,
  STATUS_MEMORY_PARITY_ERROR,
  STATUS_GATEWAY_PATH_UNAVAILABLE,
  STATUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND,
};

typedef uint8_t(*MobbusCallback)(uint8_t, uint16_t, uint16_t);

/**
 * @class Modbus
 */
class Modbus {
public:
  enum class Pattern {
    Undefined,
    TrueFalse,
    Increase,
    FixedPoints,
    Floats,
    Int32
  };

  Modbus(
    uint8_t unitAddress = MODBUS_DEFAULT_UNIT_ADDRESS,
    int transmissionControlPin = MODBUS_CONTROL_PIN_NONE);
  Modbus(
    Stream& serialStream,
    uint8_t unitAddress = MODBUS_DEFAULT_UNIT_ADDRESS,
    int transmissionControlPin = MODBUS_CONTROL_PIN_NONE);

  Modbus(
    const size_t& coilcount = 0,
    const size_t& holdingcount = 0,
    const Pattern& pattern = Pattern::Undefined);

  Modbus(
    const size_t& coilcount = 0,
    const size_t& holdingcount = 0,
    const size_t& discretecount = 0);

  void begin(uint64_t boudRate);
  void setUnitAddress(uint8_t unitAddress);
  uint8_t poll();

  bool readCoilFromBuffer(int offset);
  uint16_t readRegisterFromBuffer(int offset);
  uint8_t writeExceptionStatusToBuffer(int offset, bool status);
  uint8_t writeCoilToBuffer(int offset, bool state);
  uint8_t writeDiscreteInputToBuffer(int offset, bool state);
  uint8_t writeRegisterToBuffer(int offset, uint16_t value);
  uint8_t writeStringToBuffer(int offset, uint8_t* str, uint8_t length);

  uint8_t readFunctionCode();
  uint8_t readUnitAddress();
  bool isBroadcast();

  uint64_t getTotalBytesSent();
  uint64_t getTotalBytesReceived();

  MobbusCallback cbVector[CB_MAX];
  
  typedef std::unique_ptr<bool[]> BitBuffer;
  typedef std::unique_ptr<uint16_t[]> RegisterBuffer;

  size_t CoilCount;
  size_t DiscreteCount;
  size_t RegisterCount;

  BitBuffer Coils;
  BitBuffer Discretes;
  RegisterBuffer Registers;

  void createcoils(const size_t& coilcount);
  void creatediscretes(const size_t& discretecount);
  void createregisters(const size_t& registercount);
  void createpattern(const Pattern& pattern);

  template<typename T>
  void setregisters(const size_t& address, T& value) {
    uint16_t regvalue;
    size_t registerscount = sizeof(T) > 2 ? sizeof(T) / 2 : 1;
    char* pointer = reinterpret_cast<char*>(&value);
    for (size_t r = 0; r < registerscount; r++) {
      ::memcpy((void*)(&regvalue), pointer, 2);
      Registers[r] = regvalue;
      pointer += 2;
    }
  }
};

#endif
