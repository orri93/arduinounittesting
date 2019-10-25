#include <gtest/gtest.h>

#include <ModbusSlave.h>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <FixedPoints/SFixed.h>

#include <gatlutility.h>

namespace gatl = ::gos::atl;

typedef ::FixedPoints::SQ15x16 FixedPointType;

Modbus::Modbus(uint8_t unitAddress, int transmissionControlPin) {
}

Modbus::Modbus(
  Stream& serialStream,
  uint8_t unitAddress,
  int transmissionControlPin) {
}

Modbus::Modbus(
  const size_t& coilcount,
  const size_t& holdingcount,
  const Pattern& pattern) {
  createcoils(coilcount);
  createregisters(holdingcount);
  createpattern(pattern);
}

Modbus::Modbus(
  const size_t& coilcount,
  const size_t& holdingcount,
  const size_t& discretecount) {
  createcoils(coilcount);
  createregisters(holdingcount);
  creatediscretes(discretecount);
}

void Modbus::begin(uint64_t boudRate) {}
void Modbus::setUnitAddress(uint8_t unitAddress) {}
uint8_t Modbus::poll() { return 0; }

bool Modbus::readCoilFromBuffer(int offset) {
  EXPECT_TRUE(offset < CoilCount);
  return Coils[offset];
}

uint16_t Modbus::readRegisterFromBuffer(int offset) {
  EXPECT_TRUE(offset < RegisterCount);
  return Registers[offset];
}

uint8_t Modbus::writeExceptionStatusToBuffer(int offset, bool status) {
  return STATUS_OK;
}

uint8_t Modbus::writeCoilToBuffer(int offset, bool state) {
  EXPECT_TRUE(offset < CoilCount);
  Coils[offset] = state;
  return STATUS_OK;
}

uint8_t Modbus::writeDiscreteInputToBuffer(int offset, bool state) {
  return STATUS_OK;
}

uint8_t Modbus::writeRegisterToBuffer(int offset, uint16_t value) {
  EXPECT_TRUE(offset < RegisterCount);
  Registers[offset] = value;
  return STATUS_OK;
}

uint8_t Modbus::writeStringToBuffer(int offset, uint8_t* str, uint8_t length) {
  return STATUS_OK;
}


uint8_t Modbus::readFunctionCode() {
  return 0;
}
uint8_t Modbus::readUnitAddress() {
  return 0;
}
bool Modbus::isBroadcast() {
  return true;
}

uint64_t Modbus::getTotalBytesSent() {
  return 0;
}
uint64_t Modbus::getTotalBytesReceived() {
  return 0;
}

void Modbus::createcoils(const size_t& coilcount) {
  CoilCount = coilcount;
  if (CoilCount > 0) {
    Coils = std::make_unique<bool[]>(CoilCount);
    for (size_t i = 0; i < CoilCount; i++) {
      Coils[i] = false;
    }
  }
}
void Modbus::creatediscretes(const size_t& discretecount) {
  DiscreteCount = discretecount;
  if (DiscreteCount > 0) {
    Discretes = std::make_unique<bool[]>(DiscreteCount);
    for (size_t i = 0; i < DiscreteCount; i++) {
      Discretes[i] = false;
    }
  }
}
void Modbus::createregisters(const size_t& registercount) {
  RegisterCount = registercount;
  if (RegisterCount > 0) {
    Registers = std::make_unique<uint16_t[]>(RegisterCount);
    for (size_t i = 0; i < RegisterCount; i++) {
      Registers[i] = 0;
    }
  }
}

void Modbus::createpattern(const Pattern& pattern) {
  switch (pattern) {
  case Pattern::TrueFalse:
  {
    bool value = true;
    for (size_t i = 0; i < CoilCount; i++) {
      Coils[i] = value;
      value = value ? false : true;
    }
    break;
  }
  case Pattern::Increase:
  {
    uint16_t value = 0;
    for (size_t i = 0; i < RegisterCount; i++) {
      Registers[i] = value++;
    }
    break;
  }
  case Pattern::FixedPoints:
  {
    double doublecast, doublevalue;
    size_t i = 0;
    uint16_t word;
    FixedPointType value;
    size_t count = RegisterCount / 2;
    for (size_t n = 0; n < count; n++) {
      doublecast = static_cast<double>(n);
      doublevalue = 0.01 + doublecast + doublecast / 10.0;
      value = doublevalue;
      word = gatl::utility::number::part::first<uint16_t>(value);
      Registers[i++] = word;
      word = gatl::utility::number::part::second<uint16_t>(value);
      Registers[i++] = word;
    }
    break;
  }
  case Pattern::Floats:
  {
    float floatcast, value;
    size_t i = 0;
    uint16_t word;
    size_t count = RegisterCount / 2;
    for (size_t n = 0; n < count; n++) {
      floatcast = static_cast<float>(n);
      value = 0.01F + floatcast + floatcast / 10.0F;
      word = gatl::utility::number::part::first<uint16_t>(value);
      Registers[i++] = word;
      word = gatl::utility::number::part::second<uint16_t>(value);
      Registers[i++] = word;
    }
    break;
  }
  case Pattern::Int32:
  {
    size_t count = RegisterCount / 2;
    int32_t value = -static_cast<int32_t>(count);
    size_t i = 0;
    uint16_t word;
    for (size_t n = 0; n < count; n++) {
      word = gatl::utility::number::part::first<uint16_t>(value);
      Registers[i++] = word;
      word = gatl::utility::number::part::second<uint16_t>(value);
      Registers[i++] = word;
      value += 2;
    }
    break;
  }
  }
}