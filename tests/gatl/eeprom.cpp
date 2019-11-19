#include <memory>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <EEPROM.h>

#include <gatleeprom.h>

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
namespace gatle = ::gos::atl::eeprom;

namespace gatlunp = ::gos::atl::utility::number::part;

class GatlEepromFixture : public ::testing::Test {
public:
  void testfixedpoint(uint8_t count, int startaddress) {

    int address;

    gatum::FixedPointArray fixedpoints, reversed;
    gatum::create(fixedpoints, count);
    gatum::create(reversed, count);
    gatum::pattern::decimal(fixedpoints, count);

    uint8_t size = gatub::size<gatum::FixedPointType, uint8_t, uint8_t>();
    EXPECT_EQ(4, size);

    gatlb::reference<gatum::FixedPointType, int> binding;
    uint16_t result = gatub::create<gatum::FixedPointType, int>(
      binding,
      fixedpoints,
      startaddress,
      count,
      size);

#ifdef GOS_ATL_USE_ARDUINO_EEPROM
    EEPROMMock* eeprommock = EEPROMMockInstance();
#endif

    gatle::write(binding);

    address = binding.first;
    for (uint8_t i = 0; i < count; i++) {
      uint8_t* pointer = reinterpret_cast<uint8_t*>(binding.pointers[i]);
      for (uint8_t j = 0; j < size; j++) {
        EXPECT_EQ(eeprommock->Buffer[address++], *(pointer++));
        EXPECT_EQ(eeprommock->Buffer[address++], *(pointer++));
      }
    }

    gatum::pattern::reverse(reversed, fixedpoints, count);
    address = startaddress;
    for (uint8_t i = 0; i < count; i++) {
      uint8_t* pointer = reinterpret_cast<uint8_t*>(&(reversed[i]));
      for (uint8_t j = 0; j < size; j++) {
        eeprommock->Buffer[address++] = *(pointer++);
      }
    }

    gatle::read(binding);
    address = binding.first;
    for (uint8_t i = 0; i < count; i++) {
      EXPECT_EQ(reversed[i], fixedpoints[i]);
    }

    gatlb::testing::clean(binding);
#ifdef GOS_ATL_USE_ARDUINO_EEPROM
    releaseEEPROMMock();
#endif
  }
  void testfloat(uint8_t count, int startaddress) {

    gatum::FloatArray floats, reversed;
    gatum::create(floats, count);
    gatum::create(reversed, count);
    gatum::pattern::decimal(floats, count);

    uint8_t size = gatub::size<float, uint8_t, uint8_t>();
    EXPECT_EQ(4, size);

    gatlb::reference<float, int> binding;
    uint16_t result = gatub::create<float, int>(
      binding,
      floats,
      startaddress,
      count,
      size);

#ifdef GOS_ATL_USE_ARDUINO_EEPROM
    EEPROMMock* eeprommock = EEPROMMockInstance();
#endif

    gatle::write(binding);

    int address = binding.first;
    for (uint8_t i = 0; i < count; i++) {
      uint8_t* pointer = reinterpret_cast<uint8_t*>(binding.pointers[i]);
      for (uint8_t j = 0; j < size; j++) {
        EXPECT_EQ(eeprommock->Buffer[address++], *(pointer++));
      }
    }

    gatum::pattern::reverse(reversed, floats, count);
    address = startaddress;
    for (uint8_t i = 0; i < count; i++) {
      uint8_t* pointer = reinterpret_cast<uint8_t*>(&(reversed[i]));
      for (uint8_t j = 0; j < size; j++) {
        eeprommock->Buffer[address++] = *(pointer++);
      }
    }

    gatle::read(binding);
    address = binding.first;
    for (uint8_t i = 0; i < count; i++) {
      EXPECT_EQ(reversed[i], floats[i]);
    }

    gatlb::testing::clean(binding);
#ifdef GOS_ATL_USE_ARDUINO_EEPROM
    releaseEEPROMMock();
#endif
  }
  void testint32(uint8_t count, int startaddress) {

    gatum::Int32Array ints, reversed;
    gatum::create(ints, count);
    gatum::create(reversed, count);
    gatum::pattern::range(ints, count);

    uint8_t size = gatub::size<int32_t, uint8_t, uint8_t>();
    EXPECT_EQ(4, size);

    gatlb::reference<int32_t, int> binding;
    uint16_t result = gatub::create<int32_t, int>(
      binding,
      ints,
      startaddress,
      count,
      size);

#ifdef GOS_ATL_USE_ARDUINO_EEPROM
    EEPROMMock* eeprommock = EEPROMMockInstance();
#endif

    gatle::write(binding);

    int address = binding.first;
    for (uint8_t i = 0; i < count; i++) {
      uint8_t* pointer = reinterpret_cast<uint8_t*>(binding.pointers[i]);
      for (uint8_t j = 0; j < size; j++) {
        EXPECT_EQ(eeprommock->Buffer[address++], *(pointer++));
      }
    }

    gatum::pattern::reverse(reversed, ints, count);
    address = startaddress;
    for (uint8_t i = 0; i < count; i++) {
      uint8_t* pointer = reinterpret_cast<uint8_t*>(&(reversed[i]));
      for (uint8_t j = 0; j < size; j++) {
        eeprommock->Buffer[address++] = *(pointer++);
      }
    }

    gatle::read(binding);
    address = binding.first;
    for (uint8_t i = 0; i < count; i++) {
      EXPECT_EQ(reversed[i], ints[i]);
    }

    gatlb::testing::clean(binding);
#ifdef GOS_ATL_USE_ARDUINO_EEPROM
    releaseEEPROMMock();
#endif
  }
};


TEST_F(GatlEepromFixture, MixedBinding) {
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

TEST_F(GatlEepromFixture, FixedPointBinding) {
  testfixedpoint(6, 0);
  testfixedpoint(6, 2);
}

TEST_F(GatlEepromFixture, FloatBinding) {
  testfloat(6, 4);
}

TEST_F(GatlEepromFixture, Int32Binding) {
  testint32(6, 12);
  testint32(6, 8);
}
