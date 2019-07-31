/**
 * SPI mock header
 */
#ifndef SPI_H
#define SPI_H

#include <Arduino.h>

#include <gmock/gmock.h>

 // SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
 // usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

// define SPI_AVR_EIMSK for AVR boards with external interrupt pins
#if defined(EIMSK)
#define SPI_AVR_EIMSK  EIMSK
#elif defined(GICR)
#define SPI_AVR_EIMSK  GICR
#elif defined(GIMSK)
#define SPI_AVR_EIMSK  GIMSK
#endif

class SpiMock;

class SPISettings {
public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
  SPISettings();
private:
  void init(uint32_t clock, uint8_t bitOrder, uint8_t dataMode);
  uint32_t clock_;
  uint8_t bitOrder_;
  uint8_t dataMode_;
  friend class SpiMock;
};

class SpiMock {
public:
  // Initialize the SPI library
  //static void begin();
  MOCK_METHOD0(begin, void());

  // If SPI is used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can
  // prevent conflicts.  The input interruptNumber is the number used
  // with attachInterrupt.  If SPI is used from a different interrupt
  // (eg, a timer), interruptNumber should be 255.
  //static void usingInterrupt(uint8_t interruptNumber);
  MOCK_METHOD1(usingInterrupt, void(uint8_t interruptNumber));

  // And this does the opposite.
  //static void notUsingInterrupt(uint8_t interruptNumber);
  MOCK_METHOD1(notUsingInterrupt, void(uint8_t interruptNumber));


  // Note: the usingInterrupt and notUsingInterrupt functions should
  // not to be called from ISR context or inside a transaction.
  // For details see:
  // https://github.com/arduino/Arduino/pull/2381
  // https://github.com/arduino/Arduino/pull/2449

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
  // inline static void beginTransaction(SPISettings settings)
  MOCK_METHOD1(beginTransaction, void(SPISettings settings));

  //inline static uint8_t transfer(uint8_t data);
  MOCK_METHOD1(transfer, uint8_t(uint8_t data));

  //inline static uint16_t transfer16(uint16_t data);
  MOCK_METHOD1(transfer16, uint16_t(uint16_t data));

  //inline static void transfer(void *buf, size_t count);
  MOCK_METHOD2(transfer, void(void *buf, size_t count));

  //inline static void endTransaction(void);
  MOCK_METHOD0(endTransaction, void(void));

  // Disable the SPI bus
  //static void end();
  MOCK_METHOD0(end, void(void));

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  //inline static void setBitOrder(uint8_t bitOrder);
  MOCK_METHOD1(setBitOrder, void(uint8_t bitOrder));

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  //inline static void setDataMode(uint8_t dataMode);
  MOCK_METHOD1(setDataMode, void(uint8_t dataMode));

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  //inline static void setClockDivider(uint8_t clockDiv);
  MOCK_METHOD1(setClockDivider, void(uint8_t clockDiv));

  // These undocumented functions should not be used.  SPI.transfer()
  // polls the hardware flag which is automatically cleared as the
  // AVR responds to SPI's interrupt
  //inline static void attachInterrupt();
  //inline static void detachInterrupt();
  MOCK_METHOD0(attachInterrupt, void(void));
  MOCK_METHOD0(detachInterrupt, void(void));
};

class Spi_ {
public:
  static void begin();
  static void usingInterrupt(uint8_t interruptNumber);
  static void notUsingInterrupt(uint8_t interruptNumber);
  static void beginTransaction(SPISettings settings);
  static uint8_t transfer(uint8_t data);
  static uint16_t transfer16(uint16_t data);
  static void transfer(void *buf, size_t count);
  static void endTransaction(void);
  static void end();
  static void setBitOrder(uint8_t bitOrder);
  static void setDataMode(uint8_t dataMode);
  static void setClockDivider(uint8_t clockDiv);
  static void attachInterrupt();
  static void detachInterrupt();
};
extern Spi_ SPI;

SpiMock* spiMockInstance();
void releaseSpiMock();

#endif // SPI_H
