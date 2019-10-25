#include <memory>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlmodbus.h>

#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include <FixedPoints/SFixed.h>

#include <gos/utils/expect.h>

namespace gatl = ::gos::atl;

typedef ::FixedPoints::SQ15x16 FixedPointType;

typedef std::unique_ptr<bool[]> BooleanArray;
typedef std::unique_ptr<uint16_t[]> IntegerArray;
typedef std::unique_ptr<FixedPointType[]> FixedPointArray;
typedef std::unique_ptr<float[]> FloatArray;
typedef std::unique_ptr<int32_t[]> Int32Array;

void create(BooleanArray& boolarray, const uint8_t& count) {
  boolarray = std::make_unique<bool[]>(count);
  for (size_t i = 0; i < count; i++) {
    boolarray[i] = false;
  }
}

void create(IntegerArray& intarray, const uint8_t& count) {
  intarray = std::make_unique<uint16_t[]>(count);
  for (size_t i = 0; i < count; i++) {
    intarray[i] = 0x0000;
  }
}

void create(FixedPointArray& fparray, const uint8_t& count) {
  fparray = std::make_unique<FixedPointType[]>(count);
  for (size_t i = 0; i < count; i++) {
    fparray[i] = 0.0;
  }
}

void create(FloatArray& floatarray, const uint8_t& count) {
  floatarray = std::make_unique<float[]>(count);
  for (size_t i = 0; i < count; i++) {
    floatarray[i] = 0.0F;
  }
}

void create(Int32Array& int32array, const uint8_t& count) {
  int32array = std::make_unique<int32_t[]>(count);
  for (size_t i = 0; i < count; i++) {
    int32array[i] = 0;
  }
}

template<typename T> void reverse(
  std::unique_ptr<T[]>& typearray,
  const uint8_t& count) {
  std::unique_ptr<T[]> r = std::make_unique<T[]>(count);
  uint8_t rc = count;
  for (size_t i = 0; i < count; i++) {
    r[i] = typearray[--rc];
  }
  for (size_t i = 0; i < count; i++) {
    typearray[i] = r[i];
  }
}

void transform(BooleanArray& boolarray, const uint8_t& count) {
  for (size_t i = 0; i < count; i++) {
    boolarray[i] = boolarray[i] ? false : true;
  }
}

struct BindingTest {
  uint16_t Start;
  uint8_t Count;
};

class GatlModbusFixture : public ::testing::Test {
public:

  void testfixedpoint(
    const BindingTest& binding,
    const BindingTest& modbus,
    const BindingTest& access) {

    FixedPointType value;
    FixedPointArray fixedpoints;
    double doublevalue, doublefixedpoint;
    create(fixedpoints, binding.Count);

    size_t fixedpointsize = sizeof(FixedPointType);
    EXPECT_EQ(4, fixedpointsize);
    size_t fixedpointwords = fixedpointsize / 2;
    EXPECT_EQ(2, fixedpointwords);
    uint8_t wordcount = modbus.Count * static_cast<uint8_t>(fixedpointwords);

    Modbus slave(0, wordcount, Modbus::Pattern::FixedPoints);

    uint16_t address = binding.Start;

    uint8_t offset, count, index = 1;
    FixedPointType* pointer = fixedpoints.get();
    gatl::modbus::Bindings<FixedPointType> bindings;
    gatl::modbus::create(bindings, binding.Count, address, pointer++);
    for (size_t i = 1; i < binding.Count; i++) {
      gatl::modbus::add(bindings, index, address, pointer++);
    }

    gatl::modbus::two::assign(bindings, slave, access.Start, access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      value = gatl::utility::number::part::combine<uint16_t, FixedPointType>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      doublevalue = (double)value;
      doublefixedpoint = (double)(fixedpoints[index++]);
      EXPECT_DOUBLE_EQ(doublevalue, doublefixedpoint);
      offset += 2;
    }

    reverse<FixedPointType>(fixedpoints, binding.Count);

    gatl::modbus::two::access(bindings, slave, access.Start, access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      value = gatl::utility::number::part::combine<uint16_t, FixedPointType>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      doublevalue = (double)value;
      doublefixedpoint = (double)(fixedpoints[index++]);
      EXPECT_DOUBLE_EQ(doublevalue, doublefixedpoint);
      offset += 2;
    }

    gatl::modbus::clean(bindings);
  }
  void testfloat(
    const BindingTest& binding,
    const BindingTest& modbus,
    const BindingTest& access) {

    float value;
    FloatArray floats;
    create(floats, binding.Count);

    size_t floatsize = sizeof(float);
    EXPECT_EQ(4, floatsize);
    size_t floatwords = floatsize / 2;
    EXPECT_EQ(2, floatwords);
    uint8_t wordcount = modbus.Count * static_cast<uint8_t>(floatwords);

    Modbus slave(0, wordcount, Modbus::Pattern::Floats);

    uint16_t address = binding.Start;

    uint8_t offset, count, index = 1;
    float* pointer = floats.get();
    gatl::modbus::Bindings<float> bindings;
    gatl::modbus::create(bindings, binding.Count, address, pointer++);
    for (size_t i = 1; i < binding.Count; i++) {
      gatl::modbus::add(bindings, index, address, pointer++);
    }

    gatl::modbus::two::assign(bindings, slave, access.Start, access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      value = gatl::utility::number::part::combine<uint16_t, float>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_FLOAT_EQ(value, floats[index++]);
      offset += 2;
    }

    reverse(floats, binding.Count);

    gatl::modbus::two::access(bindings, slave, access.Start, access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      value = gatl::utility::number::part::combine<uint16_t, float>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_FLOAT_EQ(value, floats[index++]);
      offset += 2;
    }

    gatl::modbus::clean(bindings);
  }
  void testint32(
    uint8_t count,
    uint16_t startaddress,
    uint16_t length) {

    int32_t value;
    Int32Array ints;
    create(ints, count);

    size_t intsize = sizeof(int32_t);
    EXPECT_EQ(4, intsize);
    size_t intwords = intsize / 2;
    EXPECT_EQ(2, intwords);
    uint8_t wordcount = count * static_cast<uint8_t>(intwords);

    Modbus slave(0, wordcount, Modbus::Pattern::Int32);

    uint16_t address = 0;

    uint8_t offset, index = 1;
    int32_t* pointer = ints.get();
    gatl::modbus::Bindings<int32_t> bindings;
    gatl::modbus::create(bindings, count, address, pointer++);
    for (size_t i = 1; i < count; i++) {
      gatl::modbus::add(bindings, index, address, pointer++);
    }

    gatl::modbus::two::assign(bindings, slave, startaddress, length);

    count = gatl::modbus::range(
      bindings,
      startaddress,
      length,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      value = gatl::utility::number::part::combine<uint16_t, int32_t>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_EQ(value, ints[index++]);
      offset += 2;
    }

    reverse(ints, count);

    gatl::modbus::two::access(bindings, slave, startaddress, length);

    count = gatl::modbus::range(
      bindings,
      startaddress,
      length,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      value = gatl::utility::number::part::combine<uint16_t, int32_t>(
        slave.Registers[offset],
        slave.Registers[offset + 1]);
      EXPECT_EQ(value, ints[index++]);
      offset += 2;
    }

    gatl::modbus::clean(bindings);
  }
  void testholding(
    const BindingTest& binding,
    const BindingTest& modbus,
    const BindingTest& access) {
    
    IntegerArray ints;
    create(ints, binding.Count);

    Modbus slave(0, modbus.Count, Modbus::Pattern::Increase);

    uint16_t address = binding.Start;

    uint8_t offset, count, index = 1;
    uint16_t* pointer = ints.get();
    gatl::modbus::Bindings<uint16_t> bindings;
    gatl::modbus::create(bindings, binding.Count, address, pointer++);
    for (size_t i = 1; i < binding.Count; i++) {
      gatl::modbus::add(bindings, index, address, pointer++);
    }

    gatl::modbus::registers::assign(
      bindings,
      slave,
      access.Start,
      access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      EXPECT_EQ(slave.Registers[offset++], ints[index++]);
    }

    reverse(ints, binding.Count);

    gatl::modbus::registers::access(
      bindings,
      slave,
      access.Start,
      access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      EXPECT_EQ(slave.Registers[offset++], ints[index++]);
    }

    gatl::modbus::clean(bindings);
  }
  void testcoil(
    const BindingTest& binding,
    const BindingTest& modbus,
    const BindingTest& access) {

    BooleanArray bits;
    create(bits, binding.Count);

    Modbus slave(modbus.Count, 0, Modbus::Pattern::TrueFalse);

    uint16_t address = binding.Start;
    
    uint8_t offset, count, index = 1;
    bool* pointer = bits.get();
    gatl::modbus::Bindings<bool> bindings;
    gatl::modbus::create(bindings, binding.Count, address, pointer++);
    for (size_t i = 1; i < binding.Count; i++) {
      gatl::modbus::add(bindings, index, address, pointer++);
    }

    gatl::modbus::coil::assign(bindings, slave, access.Start, access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      EXPECT_EQ(slave.Coils[offset++], bits[index++]);
    }

    transform(bits, binding.Count);

    gatl::modbus::coil::access(bindings, slave, access.Start, access.Count);

    count = gatl::modbus::range(
      bindings,
      access.Start,
      access.Count,
      index,
      offset);
    for (size_t i = 0; i < count; i++) {
      EXPECT_EQ(slave.Coils[offset++], bits[index++]);
    }

    gatl::modbus::clean(bindings);
  }
};

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
  FixedPointType input_12, input_14, hold_02, hold_04;

  Modbus slave(8, 20, 8);

  gatl::modbus::Bindings<bool> coil_bindings_01, coil_bindings_02;
  gatl::modbus::Bindings<bool> discrete_bindings_01, discrete_bindings_02;
  gatl::modbus::Bindings<uint16_t> input_uint16_bindings;
  gatl::modbus::Bindings<int32_t> input_int32_bindings;
  gatl::modbus::Bindings<float> input_float_bindings;
  gatl::modbus::Bindings<FixedPointType> input_fixed_bindings;
  gatl::modbus::Bindings<uint16_t> hold_uint16_bindings;
  gatl::modbus::Bindings<int32_t> hold_int32_bindings;
  gatl::modbus::Bindings<float> hold_float_bindings;
  gatl::modbus::Bindings<FixedPointType> hold_fixed_bindings;

  uint16_t address;
  uint8_t index;

  index = 1;
  address = 0x0002;
  gatl::modbus::create(coil_bindings_01, 2, address, &coil_03);
  gatl::modbus::add(coil_bindings_01, index, address, &coil_04);

  index = 1;
  address = 0x0005;
  gatl::modbus::create(coil_bindings_02, 3, address, &coil_06);
  gatl::modbus::add(coil_bindings_02, index, address, &coil_07);
  gatl::modbus::add(coil_bindings_02, index, address, &coil_08);

  index = 1;
  address = 0x0002;
  gatl::modbus::create(discrete_bindings_01, 2, address, &discrete_03);
  gatl::modbus::add(discrete_bindings_01, index, address, &discrete_04);

  index = 1;
  address = 0x0006;
  gatl::modbus::create(discrete_bindings_02, 2, address, &discrete_07);
  gatl::modbus::add(discrete_bindings_02, index, address, &discrete_08);

  index = 1;
  address = 0x0002;
  gatl::modbus::create(input_uint16_bindings, 2, address, &input_03);
  gatl::modbus::add(input_uint16_bindings, index, address, &input_04);

  index = 1;
  address = 0x0004;
  gatl::modbus::create(input_int32_bindings, 2, address, &input_05);
  gatl::modbus::add(input_int32_bindings, index, address, &input_07);

  index = 1;
  address = 0x000b;
  gatl::modbus::create(input_fixed_bindings, 2, address, &input_12);
  gatl::modbus::add(input_fixed_bindings, index, address, &input_14);

  index = 1;
  address = 0x000f;
  gatl::modbus::create(input_float_bindings, 2, address, &input_16);
  gatl::modbus::add(input_float_bindings, index, address, &input_18);

  index = 1;
  address = 0x0001;
  gatl::modbus::create(hold_fixed_bindings, 2, address, &hold_02);
  gatl::modbus::add(hold_fixed_bindings, index, address, &hold_04);

  index = 1;
  address = 0x0005;
  gatl::modbus::create(hold_float_bindings, 2, address, &hold_06);
  gatl::modbus::add(hold_float_bindings, index, address, &hold_08);

  index = 1;
  address = 0x000c;
  gatl::modbus::create(hold_uint16_bindings, 2, address, &hold_13);
  gatl::modbus::add(hold_uint16_bindings, index, address, &hold_14);

  index = 1;
  address = 0x000e;
  gatl::modbus::create(hold_int32_bindings, 2, address, &hold_15);
  gatl::modbus::add(hold_int32_bindings, index, address, &hold_17);

  gatl::modbus::coil::assign(coil_bindings_01, slave, 0, 6);

  gatl::modbus::clean(coil_bindings_01);
  gatl::modbus::clean(coil_bindings_02);
  gatl::modbus::clean(discrete_bindings_01);
  gatl::modbus::clean(discrete_bindings_02);
  gatl::modbus::clean(input_uint16_bindings);
  gatl::modbus::clean(input_int32_bindings);
  gatl::modbus::clean(input_float_bindings);
  gatl::modbus::clean(input_fixed_bindings);
  gatl::modbus::clean(hold_uint16_bindings);
  gatl::modbus::clean(hold_int32_bindings);
  gatl::modbus::clean(hold_float_bindings);
  gatl::modbus::clean(hold_fixed_bindings);
}

TEST_F(GatlModbusFixture, FixedPointBinding) {
  BindingTest binding, modbus, access;

  binding.Start = 0;
  binding.Count = 6;
  modbus.Start = 0;
  modbus.Count = 6;
  access.Start = 0;
  access.Count = 12;
  testfixedpoint(binding, modbus, access);
}

TEST_F(GatlModbusFixture, FloatBinding) {
  BindingTest binding, modbus, access;

  binding.Start = 0;
  binding.Count = 6;
  modbus.Start = 0;
  modbus.Count = 6;
  access.Start = 0;
  access.Count = 12;
  testfloat(binding, modbus, access);
}

TEST_F(GatlModbusFixture, Int32Binding) {
  testint32(6, 0, 12);
  testint32(6, 2, 8);
}

TEST_F(GatlModbusFixture, HoldingBinding) {
  BindingTest binding, modbus, access;

  binding.Start = 0;
  binding.Count = 6;
  modbus.Start = 0;
  modbus.Count = 6;
  access.Start = 0;
  access.Count = 6;
  testholding(binding, modbus, access);

  binding.Start = 4;
  binding.Count = 6;
  modbus.Start = 0;
  modbus.Count = 10;
  access.Start = 0;
  access.Count = 6;
  testholding(binding, modbus, access);
}

TEST_F(GatlModbusFixture, CoilBinding) {
  BindingTest binding, modbus, access;
  
  binding.Start = 0;
  binding.Count = 6;
  modbus.Start = 0;
  modbus.Count = 6;
  access.Start = 0;
  access.Count = 6;
  testcoil(binding, modbus, access);

  binding.Start = 4;
  binding.Count = 6;
  modbus.Start = 0;
  modbus.Count = 10;
  access.Start = 0;
  access.Count = 6;
  testcoil(binding, modbus, access);
}
