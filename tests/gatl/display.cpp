#include <gtest/gtest.h>

#include <Arduino.h>

#include <gatldisplay.h>

#include "logo.h"

#define TEXT_LINE_1 "Testing text line 1"
#define TEXT_LINE_2 "Testing text line 2"

TEST(GatlDisplayTest, Bitmap) {
  gos::atl::display::Oled<> oled;
  EXPECT_CALL(*(oled.U8g2), firstPage).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), drawXBMP).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), nextPage).Times(testing::Exactly(1)).
    WillOnce(testing::Return(0));
  oled.logo(fds_logo_width, fds_logo_height, fds_logo_bits);
  delete oled.U8g2;
}

TEST(GatlDisplayTest, OneLine) {
  gos::atl::display::Oled<> oled;
  gos::atl::display::line::One<> oneline(oled);
  gos::atl::buffer::Holder<> buffer(TEXT_LINE_1, sizeof(TEXT_LINE_1));
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
  gos::atl::display::Oled<> oled;
  gos::atl::display::line::Two<> twoline(oled);
  gos::atl::buffer::Holder<> buffer1(TEXT_LINE_1, sizeof(TEXT_LINE_1));
  gos::atl::buffer::Holder<> buffer2(TEXT_LINE_2, sizeof(TEXT_LINE_2));
  twoline.display(buffer1, buffer2);
  EXPECT_CALL(*(oled.U8g2), firstPage).Times(testing::Exactly(1));
  EXPECT_CALL(*(oled.U8g2), setFont).Times(testing::Exactly(2));
  EXPECT_CALL(*(oled.U8g2), drawStr).Times(testing::Exactly(2));
  EXPECT_CALL(*(oled.U8g2), nextPage).Times(testing::Exactly(1)).
    WillOnce(testing::Return(0));
  twoline.loop();
  delete oled.U8g2;
}