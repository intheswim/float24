/* float24.h, Copyright (c) 2020 Yuriy Yakimenko
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#pragma once 

#include <stdint.h> // unit8_t
#include <math.h>   // ldexp
#include <string.h> // memset

// FP24 impl. unsigned 24-bit float. Range: 0 to 65535 (when  FP24_EXP_BITS set to 5).
// Epsilon value with MANTISSA bits equal 19 is 0.000002 (2^-19).

#define FP24_EXP_BITS  5
#define FP24_MANT_BITS (24 - FP24_EXP_BITS) // 19: 24 bits in 3 bytes minus exp. bits.
#define FP24_EXP_SHIFT ((1 << (FP24_EXP_BITS - 1)) - 1)  // 15
#define FP24_MAX_EXP   ((1 << FP24_EXP_BITS) - 1)       // 31

typedef union {

  float f;
  struct
  {
    // Order is important. 
    unsigned int mantissa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;

  } raw;
} IEEE_float;

typedef union {
  unsigned char buffer[4];
  struct
  {
    unsigned int mantissa : FP24_MANT_BITS;
    unsigned int exponent : FP24_EXP_BITS;
    unsigned int unused : 8;
  } raw;
} float24_cast; // supports only non-negative values.

static void set24bit (float24_cast & value, float input);
static float get24bit (const float24_cast value);


struct float24_u
{
  float24_cast data;

  float24_u ()
  {
    set (0);
  }

  float24_u (float input)
  {
    set (input);
  }
  void set(float input)
  {
    set24bit(data, input);
  }
  float get() const
  {
    return get24bit(data);
  }

  operator float() const
  {
    return get();
  }
};

static bool isFloatBigEndian()
{
  float floatNumber = -1.0;
  uint8_t* numPtr = (uint8_t*)&floatNumber;
  return (numPtr[0] != 0);
}

static float ReverseFloat(const float inFloat)
{
  float retVal;
  char* floatToConvert = (char*)&inFloat;
  char* returnFloat = (char*)&retVal;

  // swap the bytes into a temporary buffer
  returnFloat[0] = floatToConvert[3];
  returnFloat[1] = floatToConvert[2];
  returnFloat[2] = floatToConvert[1];
  returnFloat[3] = floatToConvert[0];

  return retVal;
}

static void set24bit (float24_cast & value, float input)
{
  if (isFloatBigEndian())
  {
    input = ReverseFloat(input);
  } 

  IEEE_float temp;
  temp.f = input;

  if (input <= 0)
  {
    memset(value.buffer, 0, 3);
    return;
  }

  int exp = (int)temp.raw.exponent - 127 + FP24_EXP_SHIFT + 1;

  if (exp <= 0) // number betwen 0 and 1/(2^15)
  {
    value.raw.unused = 0;

    input = ldexp(input, FP24_EXP_SHIFT + FP24_MANT_BITS); // input* pow(2, 15 + 19); // result number between 0 and 2^19-1
    value.raw.mantissa = (int)round(input);
    value.raw.exponent = 0;

    return;
  }
  else if (exp > FP24_MAX_EXP)
  {
    value.raw.unused = 0;
    // value.raw.exponent = 31;
    // value.raw.mantissa = 0x7ffff;
    memset(value.buffer, 0xFF, 3);
    return;
  }

  value.raw.exponent = exp;
  value.raw.mantissa = temp.raw.mantissa >> (23 - FP24_MANT_BITS);
  value.raw.unused = 0;
}

static float get24bit (const float24_cast value)
{
  IEEE_float temp;
  if (value.raw.exponent == 0 && value.raw.mantissa == 0) return 0;

  if (value.raw.exponent == 0) // values between 0 and 1/(2^15)
  {
    double ret = ldexp(value.raw.mantissa, -(FP24_EXP_SHIFT + FP24_MANT_BITS)); // *pow(2, -34); // 34 = 15 + 19
    return (float)ret;
  }

  temp.raw.sign = 0;
  temp.raw.exponent = value.raw.exponent - FP24_EXP_SHIFT + 127 - 1;
  temp.raw.mantissa = value.raw.mantissa << (23 - FP24_MANT_BITS);

  if (isFloatBigEndian())
  {
    return ReverseFloat(temp.f);
  }
  return temp.f;
}