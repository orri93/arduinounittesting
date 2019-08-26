#ifndef _GOS_ARDUINO_TESTING_MOCK_U8G2_H_
#define _GOS_ARDUINO_TESTING_MOCK_U8G2_H_

#include <gmock/gmock.h>

#ifdef U8G2_16BIT
typedef uint16_t u8g2_uint_t;	/* for pixel position only */
typedef int16_t u8g2_int_t;		/* introduced for circle calculation */
typedef int32_t u8g2_long_t;		/* introduced for ellipse calculation */
#else
typedef uint8_t u8g2_uint_t;		/* for pixel position only */
typedef int8_t u8g2_int_t;		/* introduced for circle calculation */
typedef int16_t u8g2_long_t;		/* introduced for ellipse calculation */
#endif

#define U8X8_PIN_NONE 0
#define U8G2_R0	0

typedef int u8g2_cb_t;

extern const uint8_t* u8g2_font_pressstart2p_8f;
extern const uint8_t* u8g2_font_pressstart2p_8r;
extern const uint8_t* u8g2_font_pressstart2p_8u; 
extern const uint8_t* u8g2_font_profont12_mf; 
extern const uint8_t* u8g2_font_profont12_mr; 
extern const uint8_t* u8g2_font_profont22_mf; 
extern const uint8_t* u8g2_font_profont22_mr; 
extern const uint8_t* u8g2_font_inb30_mf; 
extern const uint8_t* u8g2_font_inb30_mr; 

class U8g2 {
public:
  U8g2() {}
  U8g2(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) {}
  MOCK_METHOD0(begin, bool(void));
  MOCK_METHOD0(firstPage, void());
  MOCK_METHOD5(drawXBMP, void(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, const uint8_t *bitmap));
  MOCK_METHOD0(nextPage, uint8_t());
  MOCK_METHOD1(setFont, void(const uint8_t  *font));
  MOCK_METHOD3(drawStr, u8g2_uint_t(u8g2_uint_t x, u8g2_uint_t y, const char *s));
  MOCK_METHOD1(getStrWidth, u8g2_uint_t(const char *s));
  MOCK_METHOD1(getUTF8Width, u8g2_uint_t(const char *s));
  MOCK_METHOD4(drawLine, void(u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2));
};

typedef U8g2 U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C;

#endif
