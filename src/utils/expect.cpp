#include <gos/utils/expect.h>

namespace gos {
namespace arduino {
namespace testing {
namespace utils {
namespace expect {

void floateq(
  std::vector<float>& vector,
  float* array,
  const size_t& count) {
  size_t i;
  EXPECT_EQ(count, vector.size());
  std::vector<float>::iterator it = vector.begin();
  for (i = 0; i < count && it != vector.end(); i++) {
    EXPECT_FLOAT_EQ(array[i], *(it++));
  }
  EXPECT_EQ(i, count);
  EXPECT_EQ(it, vector.end());
}

void floateq(
  std::vector<float>& vector,
  float* array,
  const uint16_t* reference,
  const size_t& count) {
  size_t i;
  EXPECT_EQ(count, vector.size());
  std::vector<float>::iterator it = vector.begin();
  for (i = 0; i < count && it != vector.end(); i++) {
    EXPECT_FLOAT_EQ(array[reference[i]], *(it++));
  }
  EXPECT_EQ(i, count);
  EXPECT_EQ(it, vector.end());
}

}
}
}
}
}
