/**
 * Arduino String header
 */
#ifndef ARDUINO_STRING_H
#define ARDUINO_STRING_H

#include <string>

#include <Arduino.h>

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

class String;

namespace gos {
namespace arduino {
namespace mock {
class Print
{
private:
  int write_error;
  size_t printNumber(unsigned long, uint8_t);
  size_t printFloat(double, uint8_t);
protected:
  void setWriteError(int err = 1) { write_error = err; }
public:
  std::stringstream s_;

  Print() : write_error(0) {}

  int getWriteError() { return write_error; }
  void clearWriteError() { setWriteError(0); }

  virtual size_t write(uint8_t) = 0;
  size_t write(const char* str) {
    if (str == nullptr) return 0;
    return write((const uint8_t*)str, strlen(str));
  }
  virtual size_t write(const uint8_t* buffer, size_t size);
  size_t write(const char* buffer, size_t size) {
    return write((const uint8_t*)buffer, size);
  }

  // default to zero, meaning "a single write may block"
  // should be overriden by subclasses with buffering
  virtual int availableForWrite() { return 0; }

  size_t print(const String&);
  size_t print(const char[]);
  size_t print(char);
  size_t print(unsigned char, int = DEC);
  size_t print(int, int = DEC);
  size_t print(unsigned int, int = DEC);
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(double, int = 2);

  size_t println(const String& s);
  size_t println(const char[]);
  size_t println(char);
  size_t println(unsigned char, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(double, int = 2);
  size_t println(void);

  virtual void flush() { /* Empty implementation for backward compatibility */ }
};

}
}
}

class String {
  std::string s_;
public:
  String(const char* cstr = "");
  String(const String& str);

  explicit String(char c);
  explicit String(unsigned char, unsigned char base = 10);
  explicit String(int, unsigned char base = 10);
  explicit String(unsigned int, unsigned char base = 10);
  explicit String(long, unsigned char base = 10);
  explicit String(unsigned long, unsigned char base = 10);
  explicit String(float, unsigned char decimalPlaces = 2);
  explicit String(double, unsigned char decimalPlaces = 2);
  ~String(void);

  // memory management
// return true on success, false on failure (in which case, the string
// is left unchanged).  reserve(0), if successful, will validate an
// invalid string (i.e., "if (s)" will be true afterwards)
  unsigned char reserve(unsigned int size);
  inline unsigned int length(void) const { return (unsigned int)(s_.length()); }

  // creates a copy of the assigned value.  if the value is null or
  // invalid, or if the memory allocation fails, the string will be
  // marked as invalid ("if (s)" will be false).
  String& operator = (const String& rhs);
  String& operator = (const char* cstr);

  // concatenate (works w/ built-in types)

  // returns true on success, false on failure (in which case, the string
  // is left unchanged).  if the argument is null or invalid, the
  // concatenation is considered unsucessful.
  unsigned char concat(const String& str);
  unsigned char concat(const char* cstr);
  unsigned char concat(char c);
  unsigned char concat(unsigned char c);
  unsigned char concat(int num);
  unsigned char concat(unsigned int num);
  unsigned char concat(long num);
  unsigned char concat(unsigned long num);
  unsigned char concat(float num);
  unsigned char concat(double num);

  // if there's not enough memory for the concatenated value, the string
  // will be left unchanged (but this isn't signalled in any way)
  String& operator += (const String& rhs) { concat(rhs); return (*this); }
  String& operator += (const char* cstr) { concat(cstr); return (*this); }
  String& operator += (char c) { concat(c); return (*this); }
  String& operator += (unsigned char num) { concat(num); return (*this); }
  String& operator += (int num) { concat(num); return (*this); }
  String& operator += (unsigned int num) { concat(num); return (*this); }
  String& operator += (long num) { concat(num); return (*this); }
  String& operator += (unsigned long num) { concat(num); return (*this); }
  String& operator += (float num) { concat(num); return (*this); }
  String& operator += (double num) { concat(num); return (*this); }


  // comparison (only works w/ Strings and "strings")
  int compareTo(const String& s) const;
  unsigned char equals(const String& s) const;
  unsigned char equals(const char* cstr) const;
  unsigned char operator == (const String& rhs) const { return equals(rhs); }
  unsigned char operator == (const char* cstr) const { return equals(cstr); }
  unsigned char operator != (const String& rhs) const { return !equals(rhs); }
  unsigned char operator != (const char* cstr) const { return !equals(cstr); }
  unsigned char operator <  (const String& rhs) const;
  unsigned char operator >  (const String& rhs) const;
  unsigned char operator <= (const String& rhs) const;
  unsigned char operator >= (const String& rhs) const;
  unsigned char equalsIgnoreCase(const String& s) const;
  unsigned char startsWith(const String& prefix) const;
  unsigned char startsWith(const String& prefix, unsigned int offset) const;
  unsigned char endsWith(const String& suffix) const;

  // character acccess
  char charAt(unsigned int index) const;
  void setCharAt(unsigned int index, char c);
  char operator [] (unsigned int index) const;
  char& operator [] (unsigned int index);
  void getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const;
  void toCharArray(char* buf, unsigned int bufsize, unsigned int index = 0) const
  {
    getBytes((unsigned char*)buf, bufsize, index);
  }
  const char* c_str() const { return s_.c_str(); }
  //char* begin() { return buffer; }
  //char* end() { return buffer + length(); }
  const char* begin() const { return c_str(); }
  const char* end() const { return c_str() + length(); }

  // search
  int indexOf(char ch) const;
  int indexOf(char ch, unsigned int fromIndex) const;
  int indexOf(const String& str) const;
  int indexOf(const String& str, unsigned int fromIndex) const;
  int lastIndexOf(char ch) const;
  int lastIndexOf(char ch, unsigned int fromIndex) const;
  int lastIndexOf(const String& str) const;
  int lastIndexOf(const String& str, unsigned int fromIndex) const;
  String substring(unsigned int beginIndex) const { return substring(beginIndex, s_.length()); };
  String substring(unsigned int beginIndex, unsigned int endIndex) const;

  // modification
  void replace(char find, char replace);
  void replace(const String& find, const String& replace);
  void remove(unsigned int index);
  void remove(unsigned int index, unsigned int count);
  void toLowerCase(void);
  void toUpperCase(void);
  void trim(void);

  // parsing/conversion
  long toInt(void) const;
  float toFloat(void) const;
  double toDouble(void) const;

};

#endif