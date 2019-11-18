#ifndef _AVR_EEPROM_H_
#define _AVR_EEPROM_H_ 1

#include <stddef.h>	/* size_t */
#include <stdint.h>

#include <memory>
#include <execution>


namespace avr {
namespace mock {
extern size_t size;
extern std::unique_ptr<uint8_t[]> buffer;
class AvrMockException : public std::exception {
  const char* what() const noexcept override {
    return "AVR Mock not initialized or out of range";
  }
};
}
}

      /** \ingroup avr_eeprom
          Read one byte from EEPROM address \a __p.
        */
uint8_t eeprom_read_byte(const uint8_t* __p);

/** \ingroup avr_eeprom
    Read one 16-bit word (little endian) from EEPROM address \a __p.
  */

uint16_t eeprom_read_word(const uint16_t* __p);

/** \ingroup avr_eeprom
    Read one 32-bit double word (little endian) from EEPROM address \a __p.
  */
uint32_t eeprom_read_dword(const uint32_t* __p);

/** \ingroup avr_eeprom
    Read one float value (little endian) from EEPROM address \a __p.
  */
float eeprom_read_float(const float* __p);

/** \ingroup avr_eeprom
    Read a block of \a __n bytes from EEPROM address \a __src to SRAM
    \a __dst.
  */
void eeprom_read_block(void* __dst, const void* __src, size_t __n);


/** \ingroup avr_eeprom
    Write a byte \a __value to EEPROM address \a __p.
  */
void eeprom_write_byte(uint8_t* __p, uint8_t __value);

/** \ingroup avr_eeprom
    Write a word \a __value to EEPROM address \a __p.
  */
void eeprom_write_word(uint16_t* __p, uint16_t __value);

/** \ingroup avr_eeprom
    Write a 32-bit double word \a __value to EEPROM address \a __p.
  */
void eeprom_write_dword(uint32_t* __p, uint32_t __value);

/** \ingroup avr_eeprom
    Write a float \a __value to EEPROM address \a __p.
  */
void eeprom_write_float(float* __p, float __value);

/** \ingroup avr_eeprom
    Write a block of \a __n bytes to EEPROM address \a __dst from \a __src.
    \note The argument order is mismatch with common functions like strcpy().
  */
void eeprom_write_block(const void* __src, void* __dst, size_t __n);


/** \ingroup avr_eeprom
    Update a byte \a __value to EEPROM address \a __p.
  */
void eeprom_update_byte(uint8_t* __p, uint8_t __value);

/** \ingroup avr_eeprom
    Update a word \a __value to EEPROM address \a __p.
  */
void eeprom_update_word(uint16_t* __p, uint16_t __value);

/** \ingroup avr_eeprom
    Update a 32-bit double word \a __value to EEPROM address \a __p.
  */
void eeprom_update_dword(uint32_t* __p, uint32_t __value);

/** \ingroup avr_eeprom
    Update a float \a __value to EEPROM address \a __p.
  */
void eeprom_update_float(float* __p, float __value);

/** \ingroup avr_eeprom
    Update a block of \a __n bytes to EEPROM address \a __dst from \a __src.
    \note The argument order is mismatch with common functions like strcpy().
  */
void eeprom_update_block(const void* __src, void* __dst, size_t __n);

#endif	/* !_AVR_EEPROM_H_ */
