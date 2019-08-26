#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatldisplay.h>

#include "logo.h"

#define TEXT_LINE_1 "Testing text line 1"
#define TEXT_LINE_2 "Testing text line 2"

namespace gatl = ::gos::atl;

TEST(GatlDisplayTest, Bitmap) {
  gatl::display::Oled<> oled;
  EXPECT_CALL(*(oled.U8g2), firstPage).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), drawXBMP).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), nextPage).Times(testing::Exactly(1)).
    WillOnce(testing::Return(0));
  oled.logo(fds_logo_width, fds_logo_height, fds_logo_bits);
  delete oled.U8g2;
}

TEST(GatlDisplayTest, OneLine) {
  gatl::display::Oled<> oled;
  gatl::display::line::One<> oneline(oled);
  gatl::buffer::Holder<> buffer(TEXT_LINE_1, sizeof(TEXT_LINE_1));
  oneline.display(buffer);
  EXPECT_CALL(*(oled.U8g2), firstPage).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), setFont).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), drawStr).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), nextPage).Times(testing::Exactly(1)).
    WillOnce(testing::Return(0));
  oneline.loop();
  delete oled.U8g2;
}

TEST(GatlDisplayTest, TwoLine) {
  gatl::display::Oled<> oled;
  gatl::display::line::Two<> twoline(oled);
  gatl::buffer::Holder<> buffer1(TEXT_LINE_1, sizeof(TEXT_LINE_1));
  gatl::buffer::Holder<> buffer2(TEXT_LINE_2, sizeof(TEXT_LINE_2));
  twoline.display(buffer1, buffer2);
  EXPECT_CALL(*(oled.U8g2), firstPage).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), setFont).Times(testing::Exactly(2));
  EXPECT_CALL(*(oled.U8g2), drawStr).Times(testing::Exactly(2));
  EXPECT_CALL(*(oled.U8g2), nextPage).Times(testing::Exactly(1)).
    WillOnce(testing::Return(0));
  twoline.loop();
  delete oled.U8g2;
}

TEST(GatlDisplayTest, Graph) {
  gatl::display::Oled<> oled;
  gatl::display::Graph<> graph(oled);
  gatl::statistics::Set<gatl::display::u8g2_point_t> set(0, 128 / graph.Dx);
  randomSeed(0);
  int y = 32;
  for (int i = 0; i < set.Size; i++) {
    set.add(y);
    y += random(-4, 4);
    constrain(y, 0, 63);
  }
  graph.display(set);
  EXPECT_CALL(*(oled.U8g2), firstPage).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), drawLine).Times(testing::Exactly(set.Size - 1));
  EXPECT_CALL(*(oled.U8g2), nextPage).Times(testing::Exactly(1)).
    WillOnce(testing::Return(0));
  graph.loop();
  delete oled.U8g2;
}
