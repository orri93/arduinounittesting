#include <avr/eeprom.h>

namespace avr {
namespace mock {
size_t size = 0;
std::unique_ptr<uint8_t[]> buffer;
void initialize(const size_t& size) {
  ::avr::mock::size = size;
  buffer = std::make_unique<uint8_t[]>(size);
}
}
}

/** \ingroup avr_eeprom
    Read one byte from EEPROM address \a __p.
  */
uint8_t eeprom_read_byte(const uint8_t* __p) {
  size_t index = reinterpret_cast<size_t>(__p);
  if (::avr::mock::buffer && index < ::avr::mock::size) {
    return ::avr::mock::buffer[index];
  } else {
    throw ::avr::mock::AvrMockException();
  }
}

/** \ingroup avr_eeprom
    Read a block of \a __n bytes from EEPROM address \a __src to SRAM
    \a __dst.
  */
void eeprom_read_block(void* __dst, const void* __src, size_t __n) {
  size_t index = reinterpret_cast<size_t>(__src);
  if (
    ::avr::mock::buffer &&
    index < ::avr::mock::size &&
    index + __n < ::avr::mock::size) {
    ::memcpy(
      __dst,
      static_cast<const void*>(::avr::mock::buffer.get() + index),
      __n);
  } else {
    throw ::avr::mock::AvrMockException();
  }
}


/** \ingroup avr_eeprom
    Write a byte \a __value to EEPROM address \a __p.
  */
void eeprom_write_byte(uint8_t* __p, uint8_t __value) {
  size_t index = reinterpret_cast<size_t>(__p);
  if (::avr::mock::buffer && index < ::avr::mock::size) {
    ::avr::mock::buffer[index] = __value;
  } else {
    throw ::avr::mock::AvrMockException();
  }
}

/** \ingroup avr_eeprom
    Write a block of \a __n bytes to EEPROM address \a __dst from \a __src.
    \note The argument order is mismatch with common functions like strcpy().
  */
void eeprom_write_block(const void* __src, void* __dst, size_t __n) {
  size_t index = reinterpret_cast<size_t>(__dst);
  if (
    ::avr::mock::buffer &&
    index < ::avr::mock::size &&
    index + __n < ::avr::mock::size) {
    ::memcpy(__dst, __src, __n);
  }
}
