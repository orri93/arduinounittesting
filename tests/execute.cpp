#include <gtest/gtest.h>
#include <gmock/gmock.h>

int wmain(int argc, wchar_t** argw) {
  ::testing::InitGoogleMock(&argc, argw);
  ::testing::InitGoogleTest(&argc, argw);
  return RUN_ALL_TESTS();
}