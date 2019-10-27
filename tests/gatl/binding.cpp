#include <memory>

#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatlbinding.h>

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

class GatlBindingFixture : public ::testing::Test {
public:
  void testfixedpoint(
    const uint16_t& start,    // Start address
    const uint8_t& count,     // Number of references in the binding
    const uint8_t& size) {    // The size of each reference

    gatum::FixedPointArray fixedpoints;
    gatum::create(fixedpoints, count);
    gatum::pattern::decimal(fixedpoints, count);

    gatlb::reference<gatum::FixedPointType> binding;
    uint16_t result = gatub::create(binding, fixedpoints, start, count, size);
    EXPECT_EQ(start + count * size, result);
    EXPECT_EQ(start, binding.first);
    EXPECT_EQ(count, binding.count);
    EXPECT_EQ(size, binding.size);

    gatum::FixedPointType* pointer = fixedpoints.get();
    for (uint8_t i = 0; i < count; i++) {
      bool same = gatum::address::issame<gatum::FixedPointType, uint64_t>(
        binding.pointers[i], pointer++);
      EXPECT_TRUE(same);
    }

    gatlb::testing::clean(binding);
  }
  void testfloat(
    const uint16_t& start,    // Start address
    const uint8_t& count,     // Number of references in the binding
    const uint8_t& size) {    // The size of each reference

    gatum::FloatArray floats;
    gatum::create(floats, count);
    gatum::pattern::decimal(floats, count, 0.01, true);

    gatlb::reference<float> binding;
    uint16_t result = gatub::create(binding, floats, start, count, size);
    EXPECT_EQ(start + count * size, result);
    EXPECT_EQ(start, binding.first);
    EXPECT_EQ(count, binding.count);
    EXPECT_EQ(size, binding.size);

    float* pointer = floats.get();
    for (uint8_t i = 0; i < count; i++) {
      bool same = gatum::address::issame<float, uint64_t>(
        binding.pointers[i], pointer++);
      EXPECT_TRUE(same);
    }

    gatlb::testing::clean(binding);
  }
  void testint32(
    const uint16_t& start,    // Start address
    const uint8_t& count,     // Number of references in the binding
    const uint8_t& size) {    // The size of each reference

    gatum::Int32Array ints;
    gatum::create(ints, count);
    gatum::pattern::range(ints, count);

    gatlb::reference<int32_t> binding;
    uint16_t result = gatub::create(binding, ints, start, count, size);
    EXPECT_EQ(start + count * size, result);
    EXPECT_EQ(start, binding.first);
    EXPECT_EQ(count, binding.count);
    EXPECT_EQ(size, binding.size);

    int32_t* pointer = ints.get();
    for (uint8_t i = 0; i < count; i++) {
      bool same = gatum::address::issame<int32_t, uint64_t>(
        binding.pointers[i], pointer++);
      EXPECT_TRUE(same);
    }

    gatlb::testing::clean(binding);
  }
};

TEST_F(GatlBindingFixture, FixedPointBinding) {
  testfixedpoint(0, 8, 2);
  testfixedpoint(0, 8, 4);
  testfixedpoint(2, 4, 2);
  testfixedpoint(2, 2, 4);
}

TEST_F(GatlBindingFixture, FloatBinding) {
  testfloat(0, 8, 2);
  testfloat(0, 8, 4);
  testfloat(2, 4, 2);
  testfloat(2, 2, 4);
}

TEST_F(GatlBindingFixture, Int32Binding) {
  testint32(0, 8, 2);
  testint32(0, 8, 4);
  testint32(2, 4, 2);
  testint32(2, 2, 4);
}
