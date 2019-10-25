#ifndef _GOS_ARDUINO_TESTING_UTILS_EXPECT_H_
#define _GOS_ARDUINO_TESTING_UTILS_EXPECT_H_

#include <Arduino.h>

#include <gtest/gtest.h>

#define GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY(v,a,c,t) { \
  EXPECT_EQ(c, v.size()); \
  size_t _gate_array_index_; \
  std::vector< t >::iterator _gate_array_it_ = v.begin(); \
  for(_gate_array_index_ = 0; \
    _gate_array_index_ < c && _gate_array_it_ != v.end(); \
    _gate_array_index_++) { \
    EXPECT_EQ(a[_gate_array_index_], *(_gate_array_it_++)); } \
  EXPECT_EQ(_gate_array_index_, c); \
  EXPECT_EQ(_gate_array_it_, v.end()); \
}

#define GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY_FLOAT(v,a,c) { \
  EXPECT_EQ(c, v.size()); \
  size_t _gate_array_index_; \
  std::vector<float>::iterator _gate_array_it_ = v.begin(); \
  for(_gate_array_index_ = 0; \
    _gate_array_index_ < c && _gate_array_it_ != v.end(); \
    _gate_array_index_++) { \
    EXPECT_FLOAT_EQ(a[_gate_array_index_], *(_gate_array_it_++)); } \
  EXPECT_EQ(_gate_array_index_, c); \
  EXPECT_EQ(_gate_array_it_, v.end()); \
}

#define GOS_ARDUINO_TESTING_EQ_VECTOR_ARRAY_REFERENCE_FLOAT(v,a,r,c) { \
  EXPECT_EQ(c, v.size()); \
  size_t _gate_array_index_; \
  std::vector<float>::iterator _gate_array_it_ = v.begin(); \
  for(_gate_array_index_ = 0; \
    _gate_array_index_ < c && _gate_array_it_ != v.end(); \
    _gate_array_index_++) { \
    EXPECT_FLOAT_EQ(a[r[_gate_array_index_]], *(_gate_array_it_++)); } \
  EXPECT_EQ(_gate_array_index_, c); \
  EXPECT_EQ(_gate_array_it_, v.end()); \
}

#define GOS_ARDUINO_TESTING_EQ_VECTOR_SET(v,s,i,t) { \
  EXPECT_EQ(s.Count, v.size()); \
  i _gate_set_index_ = 0; \
  std::vector< t >::iterator _gate_array_it_ = v.begin(); \
  for(_gate_set_index_ = 0; \
    _gate_set_index_ < s.Count && _gate_array_it_ != v.end(); \
    _gate_set_index_++) { \
    EXPECT_EQ(*(_gate_array_it_++), s.Values[_gate_set_index_]); } \
  EXPECT_EQ(_gate_set_index_, s.Count); \
  EXPECT_EQ(_gate_array_it_, v.end()); \
}

#define GOS_ARDUINO_TESTING_EQ_VECTOR_SET_FLOAT(v,s,i) { \
  EXPECT_EQ(s.Count, v.size()); \
  i _gate_set_index_; \
  std::vector<float>::iterator _gate_array_it_ = v.begin(); \
  for( _gate_set_index_ = 0; \
    _gate_set_index_ < s.Count && _gate_array_it_ != v.end(); \
    _gate_set_index_++) { \
    EXPECT_FLOAT_EQ(*(it++), s.Values[_gate_set_index_]); } \
  EXPECT_EQ(_gate_set_index_, c); \
  EXPECT_EQ(_gate_array_it_, v.end()); \
}

#define GOS_ARDUINO_TESTING_EQ_VECTOR_SET_DBL(v,s,i,t) { \
  EXPECT_EQ(s.Count, v.size()); \
  i _gate_set_index_ = 0; \
  std::vector< t >::iterator _gate_array_it_ = v.begin(); \
  for(_gate_set_index_ = 0; \
    _gate_set_index_ < s.Count && _gate_array_it_ != v.end(); \
    _gate_set_index_++) { \
    EXPECT_EQ(*(_gate_array_it_++), s.Values[_gate_set_index_]); } \
  EXPECT_EQ(_gate_set_index_, c); \
  EXPECT_EQ(_gate_array_it_, v.end()); \
}

#define GOS_ARDUINO_TESTING_EQ_FP(a,b) \
  EXPECT_EQ( ( a ).getInteger(), ( b ).getInteger()); \
  EXPECT_EQ( ( a ).getFraction(), ( b ).getFraction())

#define GOS_ARDUINO_TESTING_EQ_FP_NEAR(a,b,f) \
  EXPECT_TRUE(( ( a ) - ( b ) ) * ( ( a )  - ( b ) ) < ( f ) * ( f ) )

#endif /*_GOS_ARDUINO_TESTING_UTILS_EXPECT_H_*/
