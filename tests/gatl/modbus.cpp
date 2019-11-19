#include <memory>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlmodbus.h>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <FixedPoints/SFixed.h>

#include <gos/utils/expect.h>
#include <gos/utils/memory.h>
#include <gos/utils/binding.h>

namespace gatum = ::gos::arduino::testing::utils::memory;
namespace gatub = ::gos::arduino::testing::utils::binding;

namespace gatl = ::gos::atl;
namespace gatlb = ::gos::atl::binding;

namespace gatl = ::gos::atl;
namespace gatlm = ::gos::atl::modbus;
namespace gatlmd = ::gos::atl::modbus::detail;

namespace gatlunp = ::gos::atl::utility::number::part;

class GatlModbusFixture : public ::testing::Test {
public:
  void testfixedpoint(
    uint8_t count,
    uint16_t startaddress,
    uint16_t length) {

    gatum::FixedPointType value;
    gatum::FixedPointArray fixedpoints;
    gatum::create(fixedpoints, count);

    double doublevalue, doublefixedpoint;

    uint8_t size = gatub::size<gatum::FixedPointType, uint16_t, uint8_t>();
    EXPECT_EQ(2, size);
    
    gatlb::reference<gatum::FixedPointType> binding;
    uint16_t result = gatub::create(
      binding,
      fixedpoints,
      startaddress,
      count,
      size);

    uint8_t wordcount = count * size;
    Modbus slave(0, wordcount, Modbus::Pattern::FixedPoints);

    uint8_t offset, from, to, index = 0;

    gatlm::two::assign(binding, slave, startaddress, count, from, to);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      value = gatlunp::combine<uint16_t, gatum::FixedPointType>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      doublevalue = (double)value;
      doublefixedpoint = (double)(fixedpoints[from++]);
      EXPECT_DOUBLE_EQ(doublevalue, doublefixedpoint);
      offset += 2;
    }

    gatum::pattern::reverse(fixedpoints, count);

    gatlm::two::access(binding, slave, startaddress, count);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      value = gatlunp::combine<uint16_t, gatum::FixedPointType>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      doublevalue = (double)value;
      doublefixedpoint = (double)(fixedpoints[from++]);
      EXPECT_DOUBLE_EQ(doublevalue, doublefixedpoint);
      offset += 2;
    }

    gatlb::testing::clean(binding);
  }
  void testfloat(
    uint8_t count,
    uint16_t startaddress,
    uint16_t length) {

    float value;
    gatum::FloatArray floats;
    gatum::create(floats, count);

    uint8_t size = gatub::size<float, uint16_t, uint8_t>();
    EXPECT_EQ(2, size);

    gatlb::reference<float> binding;
    uint16_t result = gatub::create(
      binding,
      floats,
      startaddress,
      count,
      size);

    uint8_t wordcount = count * size;
    Modbus slave(0, wordcount, Modbus::Pattern::Floats);

    uint8_t offset, from, to, index = 0;

    gatlm::two::assign(binding, slave, startaddress, count, from, to);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      value = gatlunp::combine<uint16_t, float>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_FLOAT_EQ(value, floats[from++]);
      offset += 2;
    }

    gatum::pattern::reverse(floats, count);

    gatlm::two::access(binding, slave, startaddress, count);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      value = gatlunp::combine<uint16_t, float>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_FLOAT_EQ(value, floats[from++]);
      offset += 2;
    }

    gatlb::testing::clean(binding);
  }
  void testint32(
    uint8_t count,
    uint16_t startaddress,
    uint16_t length) {

    int32_t value;
    gatum::Int32Array ints;
    gatum::create(ints, count);

    uint8_t size = gatub::size<int32_t, uint16_t, uint8_t>();
    EXPECT_EQ(2, size);

    gatlb::reference<int32_t> binding;
    uint16_t result = gatub::create(
      binding,
      ints,
      startaddress,
      count,
      size);

    uint8_t wordcount = count * size;
    Modbus slave(0, wordcount, Modbus::Pattern::Int32);

    uint8_t offset, from, to, index = 0;

    gatlm::two::assign(binding, slave, startaddress, length, from, to);

    gatlmd::initialize(binding, startaddress, length, offset, from, to);
    while (from < to) {
      value = gatl::utility::number::part::combine<uint16_t, int32_t>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_EQ(value, ints[from++]);
      offset += 2;
    }

    gatum::pattern::reverse(ints, count);

    gatlm::two::access(binding, slave, startaddress, length);
    gatlmd::initialize(binding, startaddress, length, offset, from, to);
    while (from < to) {
      value = gatl::utility::number::part::combine<uint16_t, int32_t>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_EQ(value, ints[from++]);
      offset += 2;
    }

    gatlb::testing::clean(binding);
  }
  void testholding(
    uint8_t count,
    uint16_t startaddress,
    uint16_t length) {
    
    gatum::Uint16Array ints;
    gatum::create(ints, count);

    uint8_t size = 1;
    gatlb::reference<uint16_t> binding;
    uint16_t result = gatub::create(
      binding,
      ints,
      startaddress,
      count,
      size);

    Modbus slave(0, count, Modbus::Pattern::Increase);

    uint8_t offset, from, to, index = 0;

    gatlm::registers::assign(binding, slave, startaddress, count, from, to);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      EXPECT_EQ(slave.Registers[offset++], ints[from++]);
    }

    gatum::pattern::reverse(ints, count);

    gatlm::registers::access(binding, slave, startaddress, count);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      EXPECT_EQ(slave.Registers[offset++], ints[from++]);
    }

    gatlb::testing::clean(binding);
  }
  void testcoil(
    uint8_t count,
    uint16_t startaddress,
    uint16_t length) {

    gatum::BooleanArray bits;
    gatum::create(bits, count);

    uint8_t size = 1;
    gatlb::reference<bool> binding;
    uint16_t result = gatub::create(
      binding,
      bits,
      startaddress,
      count,
      size);

    Modbus slave(count, 0, Modbus::Pattern::TrueFalse);
    
    uint8_t offset, from, to, index = 0;

    gatlm::coil::assign(binding, slave, startaddress, count, from, to);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      EXPECT_EQ(slave.Coils[offset++], bits[from++]);
    }

    gatum::transform(bits, count);

    gatlm::coil::access(binding, slave, startaddress, count);
    gatlmd::initialize(binding, startaddress, count, offset, from, to);
    while (from < to) {
      EXPECT_EQ(slave.Coils[offset++], bits[from++]);
    }

    gatlb::testing::clean(binding);
  }
  
  void testinitialize(uint16_t start, uint16_t lastlength) {

    bool coil_01, coil_02, coil_03, coil_04;
    bool discrete_01, discrete_02, discrete_03, discrete_04;
    uint16_t hold_01, hold_02;
    int32_t hold_03, hold_05;
    float hold_07, hold_09;
    gatum::FixedPointType hold_11, hold_13;

    //uint8_t offset, from, to;

    gatl::binding::reference<bool> coil_binding;
    gatl::binding::reference<bool> discrete_binding;
    gatl::binding::reference<uint16_t> uint16_binding;
    gatl::binding::reference<int32_t> int32_binding;
    gatl::binding::reference<float> float_binding;
    gatl::binding::reference<gatum::FixedPointType> fixed_binding;

    uint8_t size;
    uint16_t address;

    size = 1;
    address = 0x0000;
    address = gatub::createfour(
      coil_binding, address, size,
      &coil_01, &coil_02, &coil_03, &coil_04);
    EXPECT_EQ(0x0004, address);
    address = 0x0000;
    address = gatub::createfour(discrete_binding, address, size,
      &discrete_01, &discrete_02, &discrete_03, &discrete_04);
    EXPECT_EQ(0x0004, address);

    address = 0x0000;
    address = gatub::createtwo(
      uint16_binding, address, size,
      &hold_01, &hold_02);
    EXPECT_EQ(0x0002, address);
    size = 2;
    address = gatub::createtwo(
      int32_binding, address, size,
      &hold_03, &hold_05);
    EXPECT_EQ(0x0006, address);
    address = gatub::createtwo(
      float_binding, address, size,
      &hold_07, &hold_09);
    EXPECT_EQ(0x000a, address);
    address = gatub::createtwo(
      fixed_binding, address, size,
      &hold_11, &hold_13);
    EXPECT_EQ(0x000e, address);

    //gatl::modbus::detail::initialize(coil_bindings, start, lastlength, offset, from, to);

    gatlb::testing::clean(coil_binding);
    gatlb::testing::clean(discrete_binding);
    gatlb::testing::clean(uint16_binding);
    gatlb::testing::clean(int32_binding);
    gatlb::testing::clean(float_binding);
    gatlb::testing::clean(fixed_binding);
  }
  
  void testinitialize(
    uint16_t start,       // Start address for the modbus function
    uint16_t length,      // Length for the modbus function
    uint16_t first,       // Fist address in the binding
    uint8_t count,       // Number of bindings
    uint8_t size,         // Size for each binding
    uint8_t& offset,      // Offset in the modbus buffer
    uint8_t& from,        // First binding index
    uint8_t& to) {        // Last binding index + 1
    uint8_t index = 0;
    offset = 0;
    from = 1;
    to = 0;
    switch (size) {
    case 0: {
      size = 1;
      gatum::BooleanArray bits;
      gatum::create(bits, count);
      uint16_t address = first;
      gatl::binding::reference<bool> binding;
      address = gatub::create(
        binding,
        bits,
        address,
        count,
        size);
      EXPECT_EQ(first + count, address);

      gatl::modbus::detail::initialize(
        binding,
        start,
        length,
        offset,
        from,
        to);
      break;
    }
    case 1: {
      gatum::Uint16Array ints;
      gatum::create(ints, count);
      uint16_t address = first;
      gatl::binding::reference<uint16_t> binding;
      address = gatub::create(
        binding,
        ints,
        address,
        count,
        size);
      EXPECT_EQ(first + count, address);

      gatl::modbus::detail::initialize(
        binding,
        start,
        length,
        offset,
        from,
        to);
      break;
    }
    case 2: {
      gatum::FloatArray floats;
      gatum::create(floats, count);
      uint16_t address = first;
      gatl::binding::reference<float> binding;
      address = gatub::create(
        binding,
        floats,
        address,
        count,
        size);
      EXPECT_EQ(first + (count * size), address);

      gatl::modbus::detail::initialize(
        binding,
        start,
        length,
        offset,
        from,
        to);
      break;
    }
    }
  }

  void conceptmethod1(
    uint16_t start,       // Start address for the modbus function
    uint16_t length,      // Length for the modbus function
    uint16_t first,       // Fist address in the binding
    uint16_t count,       // Number of bindings
    uint8_t size,         // Size for each binding
    uint8_t& offset,      // Offset in the modbus buffer
    uint8_t& from,        // First binding index
    uint8_t& number) {    // Number of binding covered by the function
    number = 0;

    uint16_t end = start + length - 1;        // End address for the modbus
    uint16_t last = first - 1 + count * size; // Last address for the binging
    uint16_t delta;

    if (first >= start) {
      if (first <= end) {
        //    smmmme
        //    fbbbbbbl
        //
        //    smmmme
        //      fbbbbl
        from = 0;
        offset = first - start;
        delta = 1 + end - first;
        number = delta / size;
      }
    }
    else {
      if (last >= start) {
        //      smmmme
        //    fbbbbl
        offset = 0;
        delta = start - first;
        from = delta / size;
        number = 1 + (last - start) / size;
        if (delta % size > 0) {
          from++;
          number--;
        }
      }
    }
  }

  void conceptmethod2(
    const uint16_t& start,        // Start address for the modbus function
    const uint16_t& length,       // Length for the modbus function
    const uint16_t& first,        // Fist address in the binding
    const uint16_t& count,        // Number of bindings
    const uint8_t& size,          // Size for each binding
    uint8_t& offset,              // Offset in the modbus buffer
    uint8_t& from,                // First binding index
    uint8_t& number) {            // Number of binding covered by the function
    number = 0;

    if (first >= start) {
      if (first < (start + length)) {
        //    smmmme
        //    fbbbbbbl
        //
        //    smmmme
        //      fbbbbl
        from = 0;
        offset = first - start;
        number = (start + length - first) / size;
      }
    } else if ((first + count * size) > start) {
      //      smmmme
      //    fbbbbl
      offset = 0;
      from = ((start - first) % size > 0) + (start - first) / size;
      number = ((start - first) % size == 0) +
        ((first - 1 + count * size) - start) / size;
    }
  }

  void testconcept(
    uint16_t start,       // Start address for the modbus function
    uint16_t length,      // Length for the modbus function
    uint16_t first,       // Fist address in the binding
    uint16_t count,       // Number of bindings
    uint8_t size,         // Size for each binding
    uint8_t& offset,      // Offset in the modbus buffer
    uint8_t& from,        // First binding index
    uint8_t& number) {    // Number of binding covered by the function
    conceptmethod2(start, length, first, count, size, offset, from, number);
  }
};

TEST_F(GatlModbusFixture, InitializeOneRegistry) {
  uint16_t start;       // Start address for the modbus function
  uint16_t length;      // Length for the modbus function
  uint16_t first;       // Fist address in the binding
  uint16_t count;       // Number of bindings
  uint8_t size;         // Size for each binding (number of registry)
  uint8_t offset;       // Offset in the modbus buffer
  uint8_t from;         // First binding index
  uint8_t to;           // Last binding index + 1
  uint8_t i = 0;

  // smmmme
  // fbbbbl
  start = 0; length = 8; first = 0; count = 8; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(8, to);
  start = 0; length = 8; first = 0; count = 8; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(8, to);

  // smmmme
  //   fbbbbl
  start = 0; length = 8; first = 2; count = 8; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(2, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(6, to);
  start = 0; length = 8; first = 2; count = 8; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(2, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(6, to);

  // smmmme
  //      fbbbbl
  start = 0; length = 4; first = 3; count = 8; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(3, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(1, to);
  start = 0; length = 4; first = 3; count = 8; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(3, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(1, to);

  // smmmme
  //       fbbbbl
  start = 0; length = 4; first = 4; count = 8; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);
  start = 0; length = 4; first = 4; count = 8; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  // smmmme
  //         fbbbbl
  start = 0; length = 4; first = 6; count = 8; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);
  start = 0; length = 4; first = 6; count = 8; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  //   smmmme
  // fbbbbl
  start = 2; length = 8; first = 0; count = 8; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(2, from);
  EXPECT_EQ(8, to);
  start = 2; length = 8; first = 0; count = 8; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(2, from);
  EXPECT_EQ(8, to);

  //      smmmme
  // fbbbbl
  start = 3; length = 4; first = 0; count = 4; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(3, from);
  EXPECT_EQ(4, to);
  start = 3; length = 4; first = 0; count = 4; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(3, from);
  EXPECT_EQ(4, to);

  //       smmmme
  // fbbbbl
  start = 4; length = 4; first = 0; count = 4; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);
  start = 4; length = 4; first = 0; count = 4; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  //         smmmme
  // fbbbbl
  start = 6; length = 4; first = 0; count = 4; size = 0;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);
  start = 6; length = 4; first = 0; count = 4; size = 1;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);
}

TEST_F(GatlModbusFixture, InitializeTwoRegistry) {
  uint16_t start;       // Start address for the modbus function
  uint16_t length;      // Length for the modbus function
  uint16_t first;       // Fist address in the binding
  uint16_t count;       // Number of bindings
  uint8_t size;         // Size for each binding (number of registry)
  uint8_t offset;       // Offset in the modbus buffer
  uint8_t from;         // First binding index
  uint8_t to;           // Last binding index + 1
  uint8_t i = 0;

  // smmmme
  // fbbbbl
  start = 0; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(4, to);

  // smmmme
  //  fbbbbl
  start = 0; length = 8; first = 1; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(1, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(3, to);

  // smmmme
    //   fbbbbl
  start = 0; length = 8; first = 2; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(2, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(3, to);

  // smmmme
  //    fbbbbl
  start = 0; length = 8; first = 3; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(3, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(2, to);

  // smmmme
  //    fbbbbl
  start = 0; length = 8; first = 4; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(4, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(2, to);

  // smmmme
  //     fbbbbl
  start = 0; length = 8; first = 6; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(6, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(1, to);

  // smmmme
  //      fbbbbl
  start = 0; length = 8; first = 7; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  // smmmme
  //       fbbbbl
  start = 0; length = 8; first = 8; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  // smmmme
  //        fbbbbl
  start = 0; length = 8; first = 9; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_TRUE(from > to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  //  smmmme
  // fbbbbl
  start = 1; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(1, from);
  EXPECT_EQ(4, to);

  //   smmmme
  // fbbbbl
  start = 2; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(1, from);
  EXPECT_EQ(4, to);

  //     smmmme
  // fbbbbl
  start = 6; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(3, from);
  EXPECT_EQ(4, to);

  //      smmmme
  // fbbbbl
  start = 7; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  //       smmmme
  // fbbbbl
  start = 8; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);

  //        smmmme
  // fbbbbl
  start = 9; length = 8; first = 0; count = 4; size = 2;
  testinitialize(start, length, first, count, size, offset, from, to);
  while (from < to) { i++; from++; }
  EXPECT_EQ(0, i);
}

TEST_F(GatlModbusFixture, ConceptOneRegistry) {
  uint16_t start;       // Start address for the modbus function
  uint16_t length;      // Length for the modbus function
  uint16_t first;       // Fist address in the binding
  uint16_t count;       // Number of bindings
  uint8_t size;         // Size for each binding (number of registry)
  uint8_t offset;       // Offset in the modbus buffer
  uint8_t from;         // First binding index
  uint8_t number;       // Number of binding covered by the function

  // smmmme
  // fbbbbl
  start = 0; length = 8; first = 0; count = 8; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(8, number);

  // smmmme
  //   fbbbbl
  start = 0; length = 8; first = 2; count = 8; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(2, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(6, number);

  // smmmme
  //      fbbbbl
  start = 0; length = 4; first = 3; count = 8; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(3, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(1, number);

  // smmmme
  //       fbbbbl
  start = 0; length = 4; first = 4; count = 8; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  // smmmme
  //         fbbbbl
  start = 0; length = 4; first = 6; count = 8; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  //   smmmme
  // fbbbbl
  start = 2; length = 8; first = 0; count = 8; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(2, from);
  EXPECT_EQ(6, number);

  //      smmmme
  // fbbbbl
  start = 3; length = 4; first = 0; count = 4; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(3, from);
  EXPECT_EQ(1, number);

  //       smmmme
  // fbbbbl
  start = 4; length = 4; first = 0; count = 4; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  //         smmmme
  // fbbbbl
  start = 6; length = 4; first = 0; count = 4; size = 1;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);
}

TEST_F(GatlModbusFixture, ConceptTwoRegistry) {
  uint16_t start;       // Start address for the modbus function
  uint16_t length;      // Length for the modbus function
  uint16_t first;       // Fist address in the binding
  uint16_t count;       // Number of bindings
  uint8_t size;         // Size for each binding (number of registry)
  uint8_t offset;       // Offset in the modbus buffer
  uint8_t from;         // First binding index
  uint8_t number;       // Number of binding covered by the function

  // smmmme
  // fbbbbl
  start = 0; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(4, number);

  // smmmme
  //  fbbbbl
  start = 0; length = 8; first = 1; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(1, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(3, number);

  // smmmme
  //   fbbbbl
  start = 0; length = 8; first = 2; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(2, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(3, number);

  // smmmme
  //    fbbbbl
  start = 0; length = 8; first = 3; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(3, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(2, number);

  // smmmme
  //    fbbbbl
  start = 0; length = 8; first = 4; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(4, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(2, number);

  // smmmme
  //     fbbbbl
  start = 0; length = 8; first = 6; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(6, offset);
  EXPECT_EQ(0, from);
  EXPECT_EQ(1, number);

  // smmmme
  //      fbbbbl
  start = 0; length = 8; first = 7; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  // smmmme
  //       fbbbbl
  start = 0; length = 8; first = 8; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  // smmmme
  //        fbbbbl
  start = 0; length = 8; first = 9; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  //  smmmme
  // fbbbbl
  start = 1; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(1, from);
  EXPECT_EQ(3, number);

  //   smmmme
  // fbbbbl
  start = 2; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(1, from);
  EXPECT_EQ(3, number);

  //     smmmme
  // fbbbbl
  start = 6; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, offset);
  EXPECT_EQ(3, from);
  EXPECT_EQ(1, number);

  //      smmmme
  // fbbbbl
  start = 7; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  //       smmmme
  // fbbbbl
  start = 8; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);

  //        smmmme
  // fbbbbl
  start = 9; length = 8; first = 0; count = 4; size = 2;
  testconcept(start, length, first, count, size, offset, from, number);
  EXPECT_EQ(0, number);
}

TEST_F(GatlModbusFixture, MixedBinding) {
  /*
   *       coil_01  00001  0x0000  bool    Unbound
   *       coil_02  00002  0x0001  bool    Unbound
   *       coil_03  00003  0x0002  bool      Bound
   *       coil_04  00004  0x0003  bool      Bound
   *       coil_05  00005  0x0004  bool    Unbound
   *       coil_06  00006  0x0005  bool      Bound
   *       coil_07  00007  0x0006  bool      Bound
   *       coil_08  00008  0x0007  bool      Bound
   *
   *   discrete_01  10001  0x0000  bool    Unbound
   *   discrete_02  10002  0x0001  bool    Unbound
   *   discrete_03  10003  0x0002  bool      Bound
   *   discrete_04  10004  0x0003  bool      Bound
   *   discrete_05  10005  0x0004  bool    Unbound
   *   discrete_06  10006  0x0005  bool    Unbound
   *   discrete_07  10007  0x0006  bool      Bound
   *   discrete_08  10008  0x0007  bool      Bound
   *
   *      input_01  30001  0x0000  uint16  Unbound
   *      input_02  30002  0x0001  uint16  Unbound
   *      input_03  30003  0x0002  uint16    Bound
   *      input_04  30004  0x0003  uint16    Bound
   *      input_05  30005  0x0004  int32     Bound
   *      input_07  30007  0x0006  int32     Bound
   *      input_09  30009  0x0008  uint16  Unbound
   *      input_10  30010  0x0009  uint16  Unbound
   *      input_11  30011  0x000a  uint16  Unbound
   *      input_12  30012  0x000b  fixed     Bound
   *      input_14  30014  0x000d  fixed     Bound
   *      input_16  30016  0x000f  float     Bound
   *      input_18  30018  0x0011  float     Bound
   *      input_20  30020  0x0013  uint16  Unbound
   *
   *       hold_01  30001  0x0000  uint16  Unbound
   *       hold_02  30002  0x0001  fixed     Bound
   *       hold_04  30004  0x0003  fixed     Bound
   *       hold_06  30006  0x0005  float     Bound
   *       hold_08  30008  0x0007  float     Bound
   *       hold_10  30010  0x0009  uint16  Unbound
   *       hold_11  30011  0x000a  uint16  Unbound
   *       hold_12  30012  0x000b  uint16  Unbound
   *       hold_13  30013  0x000c  uint16    Bound
   *       hold_14  30014  0x000d  uint16    Bound
   *       hold_15  30015  0x000e  int32     Bound
   *       hold_17  30017  0x0010  int32     Bound
   *       hold_19  30019  0x0012  uint16  Unbound
   *       hold_20  30020  0x0013  uint16  Unbound
   *
   */

  bool coil_03, coil_04, coil_06, coil_07, coil_08;
  bool discrete_03, discrete_04, discrete_07, discrete_08;
  uint16_t input_03, input_04, hold_13, hold_14;
  int32_t input_05, input_07, hold_15, hold_17;
  float input_16, input_18, hold_06, hold_08;
  gatum::FixedPointType input_12, input_14, hold_02, hold_04;

  Modbus slave(8, 20, 8);

  gatlb::reference<bool> coil_binding_01, coil_binding_02;
  gatlb::reference<bool> discrete_binding_01, discrete_binding_02;
  gatlb::reference<uint16_t> input_uint16_binding;
  gatlb::reference<int32_t> input_int32_binding;
  gatlb::reference<float> input_float_binding;
  gatlb::reference<gatum::FixedPointType> input_fixed_binding;
  gatlb::reference<uint16_t> hold_uint16_binding;
  gatlb::reference<int32_t> hold_int32_binding;
  gatlb::reference<float> hold_float_binding;
  gatlb::reference<gatum::FixedPointType> hold_fixed_binding;

  uint8_t size;
  uint16_t address;

  size = 1;
  address = 0x0002;
  gatub::createtwo(
    coil_binding_01, address, size,
    &coil_03, &coil_04);

  address = 0x0005;
  gatub::createthree(
    coil_binding_02, address, size,
    &coil_06, &coil_07, &coil_08);

  address = 0x0002;
  gatub::createtwo(
    discrete_binding_01, address, size,
    &discrete_03, &discrete_04);

  address = 0x0006;
  gatub::createtwo(
    discrete_binding_02, address, size,
    &discrete_07, &discrete_08);

  address = 0x0002;
  gatub::createtwo(
    input_uint16_binding, address, size,
    &input_03, &input_04);

  size = 2;
  address = 0x0004;
  gatub::createtwo(
    input_int32_binding, address, size,
    &input_05, &input_07);

  address = 0x000b;
  gatub::createtwo(
    input_fixed_binding, address, size,
    &input_12, &input_14);

  address = 0x000f;
  gatub::createtwo(
    input_float_binding, address, size,
    &input_16, &input_18);

  address = 0x0001;
  gatub::createtwo(
    hold_fixed_binding, address, size,
    &hold_02, &hold_04);

  address = 0x0005;
  gatub::createtwo(
    hold_float_binding, address, size,
    &hold_06, &hold_08);

  size = 1;
  address = 0x000c;
  gatub::createtwo(
    hold_uint16_binding, address, size,
    &hold_13, &hold_14);

  size = 2;
  address = 0x000e;
  gatub::createtwo(
    hold_int32_binding, address, size,
    &hold_15, &hold_17);

  //gatl::modbus::coil::assign(coil_binding_01, slave, 0, 6);

  gatlb::testing::clean(coil_binding_01);
  gatlb::testing::clean(coil_binding_02);
  gatlb::testing::clean(discrete_binding_01);
  gatlb::testing::clean(discrete_binding_02);
  gatlb::testing::clean(input_uint16_binding);
  gatlb::testing::clean(input_int32_binding);
  gatlb::testing::clean(input_float_binding);
  gatlb::testing::clean(input_fixed_binding);
  gatlb::testing::clean(hold_uint16_binding);
  gatlb::testing::clean(hold_int32_binding);
  gatlb::testing::clean(hold_float_binding);
  gatlb::testing::clean(hold_fixed_binding);
}

TEST_F(GatlModbusFixture, FixedPointBinding) {
  testfixedpoint(6, 0, 12);
}

TEST_F(GatlModbusFixture, FloatBinding) {
  testfloat(6, 0, 12);
}

TEST_F(GatlModbusFixture, Int32Binding) {
  testint32(6, 0, 12);
  testint32(6, 2, 8);
}

TEST_F(GatlModbusFixture, HoldingBinding) {
  testholding(6, 0, 6);
  testholding(6, 2, 10);
}

TEST_F(GatlModbusFixture, CoilBinding) {
  testcoil(6, 0, 6);
  testcoil(6, 2, 10);
}

TEST_F(GatlModbusFixture, OnlyReadingOneRegistry) {
  typedef int16_t Integer;
  typedef float Real;

  const uint8_t IntegerSize = 1;
  const uint8_t RealSize = 2;

  Integer man;
  Integer hold1, hold3;
  Real sensor1, setpoint;

  Modbus slave(8, 20, 8);

  gatl::binding::reference<Integer, uint16_t> manual;
  gatl::binding::reference<Real, uint16_t> real;

  gatl::binding::reference<Integer, uint16_t> output;
  gatl::binding::reference<Real, uint16_t> sensor;
  
  uint16_t address;
  uint8_t count;
  bool result;

  count = 1;
  address = gatl::binding::create<Integer, uint16_t, uint8_t>(
    manual, 0, count, IntegerSize);
  gatl::binding::set<Integer, uint16_t, uint8_t>(manual, 0, &man);
  address = gatl::binding::create<Real, uint16_t, uint8_t>(
    real, address, 2, RealSize);
  gatl::binding::set< Real, uint16_t, uint8_t>(real, 0, &sensor1);
  gatl::binding::set< Real, uint16_t, uint8_t>(real, 1, &setpoint);

  result = gatl::modbus::registers::access(manual, slave, 0, 1);
  EXPECT_TRUE(result);
}