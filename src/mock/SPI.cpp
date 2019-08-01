#include <SPI.h>

#include <iostream>

SPISettings::SPISettings() {
  init(4000000, MSBFIRST, SPI_MODE0);
}

SPISettings::SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
  init(clock, bitOrder, dataMode);
}

SPISettings::SPISettings(const SPISettings& settings) {
  init(settings.clock_, settings.bitOrder_, settings.dataMode_);
}

bool SPISettings::operator==(const SPISettings& rhs) const {
  return clock_ == rhs.clock_ && bitOrder_ == rhs.bitOrder_ && dataMode_ == rhs.dataMode_;
}

void SPISettings::init(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
  clock_ = clock;
  bitOrder_ = bitOrder;
  dataMode_ = dataMode;

  // Clock settings are defined as follows. Note that this shows SPI2X
// inverted, so the bits form increasing numbers. Also note that
// fosc/64 appears twice
// SPR1 SPR0 ~SPI2X Freq
//   0    0     0   fosc/2
//   0    0     1   fosc/4
//   0    1     0   fosc/8
//   0    1     1   fosc/16
//   1    0     0   fosc/32
//   1    0     1   fosc/64
//   1    1     0   fosc/64
//   1    1     1   fosc/128

// We find the fastest clock that is less than or equal to the
// given clock rate. The clock divider that results in clock_setting
// is 2 ^^ (clock_div + 1). If nothing is slow enough, we'll use the
// slowest (128 == 2 ^^ 7, so clock_div = 6).
  //uint8_t clockDiv;

  // When the clock is known at compiletime, use this if-then-else
  // cascade, which the compiler knows how to completely optimize
  // away. When clock is not known, use a loop instead, which generates
  // shorter code.
  /*
  if (__builtin_constant_p(clock)) {
    if (clock >= F_CPU / 2) {
      clockDiv = 0;
    }
    else if (clock >= F_CPU / 4) {
      clockDiv = 1;
    }
    else if (clock >= F_CPU / 8) {
      clockDiv = 2;
    }
    else if (clock >= F_CPU / 16) {
      clockDiv = 3;
    }
    else if (clock >= F_CPU / 32) {
      clockDiv = 4;
    }
    else if (clock >= F_CPU / 64) {
      clockDiv = 5;
    }
    else {
      clockDiv = 6;
    }
  }
  else {
    uint32_t clockSetting = F_CPU / 2;
    clockDiv = 0;
    while (clockDiv < 6 && clock < clockSetting) {
      clockSetting /= 2;
      clockDiv++;
    }
  }

  // Compensate for the duplicate fosc/64
  if (clockDiv == 6)
    clockDiv = 7;

  // Invert the SPI2X bit
  clockDiv ^= 0x1;

  // Pack into the SPISettings class
  spcr = _BV(SPE) | _BV(MSTR) | ((bitOrder == LSBFIRST) ? _BV(DORD) : 0) |
    (dataMode & SPI_MODE_MASK) | ((clockDiv >> 1) & SPI_CLOCK_MASK);
  spsr = clockDiv & SPI_2XCLOCK_MASK;
  */
}

uint8_t SpiMock::transfer(uint8_t data) {
  uint8_t result = 0x00;
  In.pushbyte(data);
  if (Out.bytes() > 0) {
    Out.popbyte(result);
  }
  return result;
}

uint16_t SpiMock::transfer16(uint16_t data) {
  uint16_t result = 0x0000;
  In.pushword(data);
  if (Out.bytes() > 1) {
    Out.popword(result);
  }
  return result;
}

void SpiMock::transfer(void *buf, size_t count) {
  In.pushbytes(static_cast<uint8_t*>(buf), count);
}

void SpiMock::beginTransaction(SPISettings settings) {
  if (!TransactionQueue.empty()) {
    std::cout << "Warning: Starting multiple SPI Transactions" << std::endl;
  }
  TransactionQueue.push(settings);
}

void SpiMock::endTransaction() {
  if (!TransactionQueue.empty()) {
    TransactionQueue.pop();
  } else {
    std::cout << "Warning: Ending SPI Transaction without beginning one" << std::endl;
  }
}

static SpiMock* gSpiMock = nullptr;
SpiMock* spiMockInstance() {
  if (!gSpiMock) {
    gSpiMock = new SpiMock();
  } else {
    std::cout << "Warning: SPI Mock Instance requested multiple times" << std::endl;
  }
  return gSpiMock;
}

void releaseSpiMock() {
  if (gSpiMock) {
    delete gSpiMock;
    gSpiMock = nullptr;
  } else {
    std::cout << "Warning: Trying to release undefined SPI Mock Instance" << std::endl;
  }
}

void Spi_::begin() {
  gSpiMock->begin();
}

void Spi_::usingInterrupt(uint8_t interruptNumber) {
  gSpiMock->usingInterrupt(interruptNumber);
}
void Spi_::notUsingInterrupt(uint8_t interruptNumber) {
  gSpiMock->notUsingInterrupt(interruptNumber);
}
void Spi_::beginTransaction(SPISettings settings) {
  gSpiMock->beginTransaction(settings);
}
uint8_t Spi_::transfer(uint8_t data) {
  return gSpiMock->transfer(data);
}
uint16_t Spi_::transfer16(uint16_t data) {
  return gSpiMock->transfer16(data);
}
void Spi_::transfer(void *buf, size_t count) {
  return gSpiMock->transfer(buf, count);
}
void Spi_::endTransaction(void) {
  gSpiMock->endTransaction();
}
void Spi_::end() {
  gSpiMock->end();
}
void Spi_::setBitOrder(uint8_t bitOrder) {
  gSpiMock->setBitOrder(bitOrder);
}
void Spi_::setDataMode(uint8_t dataMode) {
  gSpiMock->setDataMode(dataMode);
}
void Spi_::setClockDivider(uint8_t clockDiv) {
  gSpiMock->setClockDivider(clockDiv);
}
void Spi_::attachInterrupt() {
  gSpiMock->attachInterrupt();
}
void Spi_::detachInterrupt() {
  gSpiMock->detachInterrupt();
}

// Preinstantiate Objects
Spi_ SPI;
