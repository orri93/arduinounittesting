#include <sstream>

#include <mock/WAString.h>

namespace gos {
namespace arduino {
namespace mock {
/* default implementation: may be overridden */
size_t Print::write(const uint8_t* buffer, size_t size)
{
  size_t n = 0;
  while (size--) {
    if(*buffer) {
      s_ << (char)(*buffer);
      buffer++;
      n++;
    } else {
      break;
    }
  }
  return n;
}

size_t Print::print(const String& s)
{
  return write(s.c_str(), s.length());
}

size_t Print::print(const char str[])
{
  return write(str);
}

size_t Print::print(char c)
{
  return write(c);
}

size_t Print::print(unsigned char b, int base)
{
  return print((unsigned long)b, base);
}

size_t Print::print(int n, int base)
{
  return print((long)n, base);
}

size_t Print::print(unsigned int n, int base)
{
  return print((unsigned long)n, base);
}

size_t Print::print(long n, int base)
{
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    if (n < 0) {
      int t = print('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, base);
  }
}

size_t Print::print(unsigned long n, int base)
{
  if (base == 0) return write(n);
  else return printNumber(n, base);
}

size_t Print::print(double n, int digits)
{
  return printFloat(n, digits);
}

size_t Print::println(void)
{
  return write("\r\n");
}

size_t Print::println(const String& s)
{
  size_t n = print(s);
  n += println();
  return n;
}

size_t Print::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t Print::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t Print::println(int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t Print::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base)
{
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char* str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  return write(str);
}

size_t Print::printFloat(double number, uint8_t digits)
{
  size_t n = 0;

  if (isnan(number)) return print("nan");
  if (isinf(number)) return print("inf");
  if (number > 4294967040.0) return print("ovf");  // constant determined empirically
  if (number < -4294967040.0) return print("ovf");  // constant determined empirically

  // Handle negative numbers
  if (number < 0.0)
  {
    n += print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print('.');
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}

typedef std::stringstream Stream;


}
}
}

namespace gam = ::gos::arduino::mock;


String::String(const char* cstr) : s_(cstr) {
}

String::String(const String& str) : s_(str.s_) {
}

String::String(char c) {
  gam::Stream s;
  s << c;
  s_ = s.str();
}

String::String(int value, unsigned char base) {
  gam::Print print;
  print.println(value, base);
  s_ = print.s_.str();
}
String::String(unsigned int value, unsigned char base) {
  gam::Print print;
  print.println(value, base);
  s_ = print.s_.str();
}
String::String(long value, unsigned char base) {
  gam::Print print;
  print.println(value, base);
  s_ = print.s_.str();
}
String::String(unsigned long value, unsigned char base) {
  gam::Print print;
  print.println(value, base);
  s_ = print.s_.str();
}
String::String(float value, unsigned char decimalPlaces) {
  gam::Print print;
  print.println(value, decimalPlaces);
  s_ = print.s_.str();
}
String::String(double value, unsigned char decimalPlaces) {
  gam::Print print;
  print.println(value, decimalPlaces);
  s_ = print.s_.str();
}

String::~String(void) {}

unsigned char reserve(unsigned int size) {
  return size;
}

String& String::operator = (const String& rhs) {
  if (&rhs != this) {
    s_ = rhs.s_;
  }
  return *this;
}
String& String::operator = (const char* cstr) {
  s_ = cstr;
}

unsigned char String::concat(const String& str) {
  s_ += str.s_;
  return 1;
}
unsigned char String::concat(const char* cstr) {
  s_ += cstr;
  return 1;
}
unsigned char String::concat(char c) {
  return concat(String(c));
}
unsigned char String::concat(unsigned char c) {
  return concat(String(c));
}
unsigned char String::concat(int num) {
  return concat(String(num));
}
unsigned char String::concat(unsigned int num) {
  return concat(String(num));
}
unsigned char String::concat(long num) {
  return concat(String(num));
}
unsigned char String::concat(unsigned long num) {
  return concat(String(num));
}
unsigned char String::concat(float num) {
  return concat(String(num));
}
unsigned char String::concat(double num) {
  return concat(String(num));
}

int String::compareTo(const String& s) const {
  s_.compare(s.s_);
}
unsigned char String::equals(const String& s) const {
  return s_.compare(s.s_) == 0 ? 1 : 0;
}
unsigned char String::equals(const char* cstr) const {
  return s_.compare(cstr) == 0 ? 1 : 0;
}

unsigned char String::operator <  (const String& rhs) const {
  return s_.compare(rhs.s_) < 0;

}
unsigned char String::operator >  (const String& rhs) const {
  return s_.compare(rhs.s_) > 0;
}
unsigned char String::operator <= (const String& rhs) const {
  return s_.compare(rhs.s_) <= 0;
}
unsigned char String::operator >= (const String& rhs) const {
  return s_.compare(rhs.s_) >= 0;
}

unsigned char String::equalsIgnoreCase(const String& s) const {
  
}
unsigned char String::startsWith(const String& prefix) const {

}
unsigned char String::startsWith(const String& prefix, unsigned int offset) const {

}
unsigned char String::endsWith(const String& suffix) const {

}

// character acccess
char String::charAt(unsigned int index) const {
  return s_.at(index);
}
void String::setCharAt(unsigned int index, char c) {
  s_[index] = c;
}
char String::operator [] (unsigned int index) const {
  return s_.at(index);
}
char& String::operator [] (unsigned int index) {
  return s_.at(index);
}
void String::getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const {
  unsigned int size = min(bufsize, s_.length() + 1);
  ::memcpy(buf, s_.c_str(), size);
}

// search
int String::indexOf(char ch) const;
int String::indexOf(char ch, unsigned int fromIndex) const;
int String::indexOf(const String& str) const;
int String::indexOf(const String& str, unsigned int fromIndex) const;
int String::lastIndexOf(char ch) const;
int String::lastIndexOf(char ch, unsigned int fromIndex) const;
int String::lastIndexOf(const String& str) const;
int String::lastIndexOf(const String& str, unsigned int fromIndex) const;
