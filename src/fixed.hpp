//#!/usr/bin/tcc -run 
/****************************************************************
 * $ID: fixed.hpp      Wed, 05 Apr 2006 13:59:29 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  ������(MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it   	        *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/
#ifndef FIXED_HPP
#define FIXED_HPP

#include "common.h"

#if 1

typedef  int8_t	  f8_2_t;
typedef  int8_t	  f8_4_t;
typedef  int8_t	  f8_6_t;
typedef  int16_t  f16_4_t;
typedef  int16_t  f16_8_t;
typedef  int32_t  f32_8_t;
typedef  int16_t  f16_12_t;
typedef  int32_t  f32_16_t;
typedef  int32_t  f32_24_t;
typedef  int64_t  f64_16_t;
typedef  int64_t  f64_32_t;
typedef  int64_t  f64_48_t;

typedef uint8_t	 uf8_2_t;
typedef uint8_t	 uf8_4_t;
typedef uint8_t	 uf8_6_t;
typedef uint16_t uf16_4_t;
typedef uint16_t uf16_8_t;
typedef uint32_t uf32_8_t;
typedef uint16_t uf16_12_t;
typedef uint32_t uf32_16_t;
typedef uint32_t uf32_24_t;
typedef uint64_t uf64_16_t;
typedef uint64_t uf64_32_t;
typedef uint64_t uf64_48_t;

#else// XXX:

template <int v> struct Int2Type { enum { val = v }; };

template <typename T, uint8_t p>
inline T float2fixed(const double f) { return (T)(f * (1ULL << p)); }
template <typename T, uint8_t p>
inline double fixed2float(const T v) {
    return (double)v / (p < BITSOF(T)  ? (1ULL << p) : (T)(~0ULL));
}

template <typename T, uint8_t p>
inline T fixed_mul(T a, T b) {
    if (sizeof(T) < 2)	return (T)(( (int16_t)a * b) >> p);	else
    if (sizeof(T) < 3)	return (T)(( (int32_t)a * b) >> p);	else
			return (T)(( (int64_t)a * b) >> p);
}
template <typename T, uint8_t p>
inline T fixed_div(T a, T b) {
    if (sizeof(T) < 2)	return (T)((((int16_t)a) << p) / b);	else
    if (sizeof(T) < 3)	return (T)((((int32_t)a) << p) / b);	else
			return (T)((((int64_t)a) << p) / b);
}

template <typename T, uint8_t p>	// XXX: uint8_t -> int ?
struct FixedPointType {		//assert(v <= BITSOF(T));
    FixedPointType() /*: fixed(0ULL)*/ { }
    FixedPointType(const FixedPointType& v) : fixed(v.fixed) { }

#if 1
    static T zero() { return  0; }
    static T one()  { return (0x01 << p); }
#else// XXX:
    static FixedPointType one()  { return (0x01 << p); }
    static FixedPointType zero() { return  0; }
#endif
    static uint8_t log2() { return p; }

    FixedPointType(T v) : fixed(v << p) { }
    operator T() const { return fixed >> p; }

    operator double() const { return fixed2float(fixed); }
    FixedPointType(double v) : fixed(float2fixed<T, p>(v)) { }

    FixedPointType& operator+=(const FixedPointType& a) {
	fixed += a.fixed;	return *this;
    }
    FixedPointType& operator-=(const FixedPointType& a) {
	fixed -= a.fixed;	return *this;
    }
    FixedPointType& operator*=(const FixedPointType& a) {
	fixed = fixed_mul<T, p>(fixed, a.fixed);	return *this;
    }
    FixedPointType& operator/=(const FixedPointType& a) {
	fixed = fixed_div<T, p>(fixed, a.fixed);	return *this;
    }

    FixedPointType& operator*=(const T a) { fixed *= a;	return *this; }
    FixedPointType& operator/=(const T a) { fixed /= a;	return *this; }

    FixedPointType  operator- () const {
	FixedPointType x;	 x.fixed  =  -fixed;	return x;
    }
    FixedPointType  operator+ (const FixedPointType& a) const {
	FixedPointType x(*this); x += a;	return x;
    }
    FixedPointType  operator- (const FixedPointType& a) const {
	FixedPointType x(*this); x -= a;	return x;
    }
    FixedPointType  operator* (const FixedPointType& a) const {
	FixedPointType x(*this); x *= a;	return x;
    }
    FixedPointType  operator/ (const FixedPointType& a) const {
	FixedPointType x(*this); x /= a;	return x;
    }

    bool operator< (const FixedPointType& a) const { return fixed <  a.fixed; }
    bool operator==(const FixedPointType& a) const { return fixed == a.fixed; }
    bool operator!=(const FixedPointType& a) const { return fixed != a.fixed; }
    bool operator<=(const FixedPointType& a) const { return fixed <= a.fixed; }
    bool operator>=(const FixedPointType& a) const { return fixed >= a.fixed; }
    bool operator> (const FixedPointType& a) const { return fixed >  a.fixed; }

private:
    T fixed;
};


typedef	FixedPointType<  int8_t,  2>  f8_2_t;
typedef	FixedPointType<  int8_t,  4>  f8_4_t;
typedef	FixedPointType<  int8_t,  6>  f8_6_t;
typedef	FixedPointType< int16_t,  4>  f16_4_t;
typedef	FixedPointType< int16_t,  8>  f16_8_t;
typedef	FixedPointType< int32_t,  8>  f32_8_t;
typedef	FixedPointType< int16_t, 12>  f16_12_t;
typedef	FixedPointType< int32_t, 16>  f32_16_t;
typedef	FixedPointType< int32_t, 24>  f32_24_t;
typedef	FixedPointType< int64_t, 16>  f64_16_t;
typedef	FixedPointType< int64_t, 32>  f64_32_t;
typedef	FixedPointType< int64_t, 48>  f64_48_t;

typedef	FixedPointType< uint8_t,  2> uf8_2_t;
typedef	FixedPointType< uint8_t,  4> uf8_4_t;
typedef	FixedPointType< uint8_t,  6> uf8_6_t;
typedef	FixedPointType<uint16_t,  4> uf16_4_t;
typedef	FixedPointType<uint16_t,  8> uf16_8_t;
typedef	FixedPointType<uint32_t,  8> uf32_8_t;
typedef	FixedPointType<uint16_t, 12> uf16_12_t;
typedef	FixedPointType<uint32_t, 16> uf32_16_t;
typedef	FixedPointType<uint32_t, 24> uf32_24_t;
typedef	FixedPointType<uint64_t, 16> uf64_16_t;
typedef	FixedPointType<uint64_t, 32> uf64_32_t;
typedef	FixedPointType<uint64_t, 48> uf64_48_t;


template <typename T, uint8_t p>
inline FixedPointType<T, p> operator+ (const T a,
	const FixedPointType<T, p>& b) { return  b + a; }
template <typename T, uint8_t p>
inline FixedPointType<T, p> operator- (const T a,
	const FixedPointType<T, p>& b) { return -b + a; }
template <typename T, uint8_t p>
inline FixedPointType<T, p> operator* (const T a,
	const FixedPointType<T, p>& b) { return  b * a; }
template <typename T, uint8_t p>
inline FixedPointType<T, p> operator/ (const T a,
	const FixedPointType<T, p>& b) {
    FixedPointType<T, p> r(a);		 return r /= b;
}

template <typename T, uint8_t p>
inline FixedPointType<T, p> sin(const FixedPointType<T, p>& a) {
    FixedPointType<T, p> r;
    r.fixed = fixed_sincos<T, p, false>;	return r;
}
template <typename T, uint8_t p>
inline FixedPointType<T, p> cos(const FixedPointType<T, p>& a) {
    FixedPointType<T, p> r;
    r.fixed = fixed_sincos<T, p, true>;		return r;
}

template <typename T, uint8_t p>
inline FixedPointType<T, p> tan(const FixedPointType<T, p>& a) {
    T s = fixed_sincos(T, p, false), c = fixed_sincos(T, p, true);
    if (!c) return (0 < s) ? std::numeric_limits<T>::max() :
			     std::numeric_limits<T>::min(); else
	    return (((s << (p - 1)) / c) << 1);
    //fixed_div<T, p>(s, c);
}

template <typename T, uint8_t p>
inline FixedPointType<T, p> sqrt(const FixedPointType<T, p>& a);
template <typename T, uint8_t p>
inline FixedPointType<T, p> rsqrt(const FixedPointType<T, p>& a);

template <typename T, uint8_t p>
inline FixedPointType<T, p> inv(const FixedPointType<T, p>& a);
template <typename T, uint8_t p>
inline FixedPointType<T, p> abs(const FixedPointType<T, p>& a) {
    FixedPointType<T, p> r;
    r.fixed = 0 < a.fixed ? a.fixed : -a.fixed;
    return r;
}


static const uint8_t LOG2_SIN = 10u, LOG2_FIXED = 16u;
static const uint16_t ANGLE_0 = 0u;
static const uint16_t ANGLE_90  = (0x01 << LOG2_SIN);
static const uint16_t ANGLE_180 = (ANGLE_90 << 1);
static const uint16_t ANGLE_360 = (ANGLE_90 << 2);
static const uint16_t ANGLE_270 = (ANGLE_90 + ANGLE_180);
static const uint16_t sin16_table[0x01 << LOG2_SIN] = {
/* Construction:
tcc -run - << EOF
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define LOG2_SIN    10u
#define LOG2_FIXED  16u

int main(int argc, char* argv[])
{
    uint16_t i;

    for (i = 0; i < (0x01 << LOG2_SIN); ++i) {
	uint32_t v = floor(sin(M_PI * i /
		(0x01 << (LOG2_SIN + 1))) * (0x01 << LOG2_FIXED) + 0.5);
	if (!(i % 8)) fprintf(stdout, "\n    ");
	fprintf(stdout, " 0x%04x,", UINT16_MAX < v ? UINT16_MAX : v);
    }	fprintf(stdout, "\n");

    return 0;
}
EOF
 */
    0x0000, 0x0065, 0x00c9, 0x012e, 0x0192, 0x01f7, 0x025b, 0x02c0,
    0x0324, 0x0389, 0x03ed, 0x0452, 0x04b6, 0x051b, 0x057f, 0x05e4,
    0x0648, 0x06ad, 0x0711, 0x0776, 0x07da, 0x083f, 0x08a3, 0x0908,
    0x096c, 0x09d1, 0x0a35, 0x0a9a, 0x0afe, 0x0b62, 0x0bc7, 0x0c2b,
    0x0c90, 0x0cf4, 0x0d59, 0x0dbd, 0x0e21, 0x0e86, 0x0eea, 0x0f4e,
    0x0fb3, 0x1017, 0x107b, 0x10e0, 0x1144, 0x11a8, 0x120d, 0x1271,
    0x12d5, 0x1339, 0x139e, 0x1402, 0x1466, 0x14ca, 0x152e, 0x1593,
    0x15f7, 0x165b, 0x16bf, 0x1723, 0x1787, 0x17eb, 0x1850, 0x18b4,
    0x1918, 0x197c, 0x19e0, 0x1a44, 0x1aa8, 0x1b0c, 0x1b70, 0x1bd4,
    0x1c38, 0x1c9b, 0x1cff, 0x1d63, 0x1dc7, 0x1e2b, 0x1e8f, 0x1ef3,
    0x1f56, 0x1fba, 0x201e, 0x2082, 0x20e5, 0x2149, 0x21ad, 0x2210,
    0x2274, 0x22d7, 0x233b, 0x239f, 0x2402, 0x2466, 0x24c9, 0x252d,
    0x2590, 0x25f4, 0x2657, 0x26ba, 0x271e, 0x2781, 0x27e4, 0x2848,
    0x28ab, 0x290e, 0x2971, 0x29d5, 0x2a38, 0x2a9b, 0x2afe, 0x2b61,
    0x2bc4, 0x2c27, 0x2c8a, 0x2ced, 0x2d50, 0x2db3, 0x2e16, 0x2e79,
    0x2edc, 0x2f3f, 0x2fa1, 0x3004, 0x3067, 0x30ca, 0x312c, 0x318f,
    0x31f1, 0x3254, 0x32b7, 0x3319, 0x337c, 0x33de, 0x3440, 0x34a3,
    0x3505, 0x3568, 0x35ca, 0x362c, 0x368e, 0x36f1, 0x3753, 0x37b5,
    0x3817, 0x3879, 0x38db, 0x393d, 0x399f, 0x3a01, 0x3a63, 0x3ac5,
    0x3b27, 0x3b88, 0x3bea, 0x3c4c, 0x3cae, 0x3d0f, 0x3d71, 0x3dd2,
    0x3e34, 0x3e95, 0x3ef7, 0x3f58, 0x3fba, 0x401b, 0x407c, 0x40de,
    0x413f, 0x41a0, 0x4201, 0x4262, 0x42c3, 0x4324, 0x4385, 0x43e6,
    0x4447, 0x44a8, 0x4509, 0x456a, 0x45cb, 0x462b, 0x468c, 0x46ec,
    0x474d, 0x47ae, 0x480e, 0x486f, 0x48cf, 0x492f, 0x4990, 0x49f0,
    0x4a50, 0x4ab0, 0x4b10, 0x4b71, 0x4bd1, 0x4c31, 0x4c90, 0x4cf0,
    0x4d50, 0x4db0, 0x4e10, 0x4e70, 0x4ecf, 0x4f2f, 0x4f8e, 0x4fee,
    0x504d, 0x50ad, 0x510c, 0x516c, 0x51cb, 0x522a, 0x5289, 0x52e8,
    0x5348, 0x53a7, 0x5406, 0x5464, 0x54c3, 0x5522, 0x5581, 0x55e0,
    0x563e, 0x569d, 0x56fc, 0x575a, 0x57b9, 0x5817, 0x5875, 0x58d4,
    0x5932, 0x5990, 0x59ee, 0x5a4c, 0x5aaa, 0x5b08, 0x5b66, 0x5bc4,
    0x5c22, 0x5c80, 0x5cde, 0x5d3b, 0x5d99, 0x5df6, 0x5e54, 0x5eb1,
    0x5f0f, 0x5f6c, 0x5fc9, 0x6026, 0x6084, 0x60e1, 0x613e, 0x619b,
    0x61f8, 0x6254, 0x62b1, 0x630e, 0x636b, 0x63c7, 0x6424, 0x6480,
    0x64dd, 0x6539, 0x6595, 0x65f2, 0x664e, 0x66aa, 0x6706, 0x6762,
    0x67be, 0x681a, 0x6876, 0x68d1, 0x692d, 0x6989, 0x69e4, 0x6a40,
    0x6a9b, 0x6af6, 0x6b52, 0x6bad, 0x6c08, 0x6c63, 0x6cbe, 0x6d19,
    0x6d74, 0x6dcf, 0x6e2a, 0x6e85, 0x6edf, 0x6f3a, 0x6f94, 0x6fef,
    0x7049, 0x70a3, 0x70fe, 0x7158, 0x71b2, 0x720c, 0x7266, 0x72c0,
    0x731a, 0x7373, 0x73cd, 0x7427, 0x7480, 0x74da, 0x7533, 0x758d,
    0x75e6, 0x763f, 0x7698, 0x76f1, 0x774a, 0x77a3, 0x77fc, 0x7855,
    0x78ad, 0x7906, 0x795f, 0x79b7, 0x7a10, 0x7a68, 0x7ac0, 0x7b18,
    0x7b70, 0x7bc8, 0x7c20, 0x7c78, 0x7cd0, 0x7d28, 0x7d7f, 0x7dd7,
    0x7e2f, 0x7e86, 0x7edd, 0x7f35, 0x7f8c, 0x7fe3, 0x803a, 0x8091,
    0x80e8, 0x813f, 0x8195, 0x81ec, 0x8243, 0x8299, 0x82f0, 0x8346,
    0x839c, 0x83f2, 0x8449, 0x849f, 0x84f5, 0x854a, 0x85a0, 0x85f6,
    0x864c, 0x86a1, 0x86f7, 0x874c, 0x87a1, 0x87f6, 0x884c, 0x88a1,
    0x88f6, 0x894a, 0x899f, 0x89f4, 0x8a49, 0x8a9d, 0x8af2, 0x8b46,
    0x8b9a, 0x8bef, 0x8c43, 0x8c97, 0x8ceb, 0x8d3f, 0x8d93, 0x8de6,
    0x8e3a, 0x8e8d, 0x8ee1, 0x8f34, 0x8f88, 0x8fdb, 0x902e, 0x9081,
    0x90d4, 0x9127, 0x9179, 0x91cc, 0x921f, 0x9271, 0x92c4, 0x9316,
    0x9368, 0x93ba, 0x940c, 0x945e, 0x94b0, 0x9502, 0x9554, 0x95a5,
    0x95f7, 0x9648, 0x969a, 0x96eb, 0x973c, 0x978d, 0x97de, 0x982f,
    0x9880, 0x98d0, 0x9921, 0x9972, 0x99c2, 0x9a12, 0x9a63, 0x9ab3,
    0x9b03, 0x9b53, 0x9ba3, 0x9bf2, 0x9c42, 0x9c92, 0x9ce1, 0x9d31,
    0x9d80, 0x9dcf, 0x9e1e, 0x9e6d, 0x9ebc, 0x9f0b, 0x9f5a, 0x9fa8,
    0x9ff7, 0xa045, 0xa094, 0xa0e2, 0xa130, 0xa17e, 0xa1cc, 0xa21a,
    0xa268, 0xa2b5, 0xa303, 0xa350, 0xa39e, 0xa3eb, 0xa438, 0xa485,
    0xa4d2, 0xa51f, 0xa56c, 0xa5b8, 0xa605, 0xa652, 0xa69e, 0xa6ea,
    0xa736, 0xa782, 0xa7ce, 0xa81a, 0xa866, 0xa8b2, 0xa8fd, 0xa949,
    0xa994, 0xa9df, 0xaa2a, 0xaa76, 0xaac1, 0xab0b, 0xab56, 0xaba1,
    0xabeb, 0xac36, 0xac80, 0xacca, 0xad14, 0xad5e, 0xada8, 0xadf2,
    0xae3c, 0xae85, 0xaecf, 0xaf18, 0xaf62, 0xafab, 0xaff4, 0xb03d,
    0xb086, 0xb0ce, 0xb117, 0xb160, 0xb1a8, 0xb1f0, 0xb239, 0xb281,
    0xb2c9, 0xb311, 0xb358, 0xb3a0, 0xb3e8, 0xb42f, 0xb477, 0xb4be,
    0xb505, 0xb54c, 0xb593, 0xb5da, 0xb620, 0xb667, 0xb6ad, 0xb6f4,
    0xb73a, 0xb780, 0xb7c6, 0xb80c, 0xb852, 0xb898, 0xb8dd, 0xb923,
    0xb968, 0xb9ae, 0xb9f3, 0xba38, 0xba7d, 0xbac1, 0xbb06, 0xbb4b,
    0xbb8f, 0xbbd4, 0xbc18, 0xbc5c, 0xbca0, 0xbce4, 0xbd28, 0xbd6b,
    0xbdaf, 0xbdf2, 0xbe36, 0xbe79, 0xbebc, 0xbeff, 0xbf42, 0xbf85,
    0xbfc7, 0xc00a, 0xc04c, 0xc08f, 0xc0d1, 0xc113, 0xc155, 0xc197,
    0xc1d8, 0xc21a, 0xc25c, 0xc29d, 0xc2de, 0xc31f, 0xc360, 0xc3a1,
    0xc3e2, 0xc423, 0xc463, 0xc4a4, 0xc4e4, 0xc524, 0xc564, 0xc5a4,
    0xc5e4, 0xc624, 0xc663, 0xc6a3, 0xc6e2, 0xc721, 0xc761, 0xc7a0,
    0xc7de, 0xc81d, 0xc85c, 0xc89a, 0xc8d9, 0xc917, 0xc955, 0xc993,
    0xc9d1, 0xca0f, 0xca4d, 0xca8a, 0xcac7, 0xcb05, 0xcb42, 0xcb7f,
    0xcbbc, 0xcbf9, 0xcc35, 0xcc72, 0xccae, 0xcceb, 0xcd27, 0xcd63,
    0xcd9f, 0xcddb, 0xce17, 0xce52, 0xce8e, 0xcec9, 0xcf04, 0xcf3f,
    0xcf7a, 0xcfb5, 0xcff0, 0xd02a, 0xd065, 0xd09f, 0xd0d9, 0xd113,
    0xd14d, 0xd187, 0xd1c1, 0xd1fa, 0xd234, 0xd26d, 0xd2a6, 0xd2df,
    0xd318, 0xd351, 0xd38a, 0xd3c2, 0xd3fb, 0xd433, 0xd46b, 0xd4a3,
    0xd4db, 0xd513, 0xd54b, 0xd582, 0xd5ba, 0xd5f1, 0xd628, 0xd65f,
    0xd696, 0xd6cd, 0xd703, 0xd73a, 0xd770, 0xd7a6, 0xd7dc, 0xd812,
    0xd848, 0xd87e, 0xd8b4, 0xd8e9, 0xd91e, 0xd954, 0xd989, 0xd9be,
    0xd9f2, 0xda27, 0xda5c, 0xda90, 0xdac4, 0xdaf8, 0xdb2c, 0xdb60,
    0xdb94, 0xdbc8, 0xdbfb, 0xdc2f, 0xdc62, 0xdc95, 0xdcc8, 0xdcfb,
    0xdd2d, 0xdd60, 0xdd92, 0xddc5, 0xddf7, 0xde29, 0xde5b, 0xde8c,
    0xdebe, 0xdef0, 0xdf21, 0xdf52, 0xdf83, 0xdfb4, 0xdfe5, 0xe016,
    0xe046, 0xe077, 0xe0a7, 0xe0d7, 0xe107, 0xe137, 0xe167, 0xe196,
    0xe1c6, 0xe1f5, 0xe224, 0xe253, 0xe282, 0xe2b1, 0xe2df, 0xe30e,
    0xe33c, 0xe36b, 0xe399, 0xe3c7, 0xe3f4, 0xe422, 0xe450, 0xe47d,
    0xe4aa, 0xe4d7, 0xe504, 0xe531, 0xe55e, 0xe58b, 0xe5b7, 0xe5e3,
    0xe610, 0xe63c, 0xe667, 0xe693, 0xe6bf, 0xe6ea, 0xe716, 0xe741,
    0xe76c, 0xe797, 0xe7c2, 0xe7ec, 0xe817, 0xe841, 0xe86b, 0xe895,
    0xe8bf, 0xe8e9, 0xe913, 0xe93c, 0xe966, 0xe98f, 0xe9b8, 0xe9e1,
    0xea0a, 0xea32, 0xea5b, 0xea83, 0xeaab, 0xead4, 0xeafc, 0xeb23,
    0xeb4b, 0xeb73, 0xeb9a, 0xebc1, 0xebe8, 0xec0f, 0xec36, 0xec5d,
    0xec83, 0xecaa, 0xecd0, 0xecf6, 0xed1c, 0xed42, 0xed68, 0xed8d,
    0xedb3, 0xedd8, 0xedfd, 0xee22, 0xee47, 0xee6b, 0xee90, 0xeeb4,
    0xeed9, 0xeefd, 0xef21, 0xef45, 0xef68, 0xef8c, 0xefaf, 0xefd2,
    0xeff5, 0xf018, 0xf03b, 0xf05e, 0xf080, 0xf0a3, 0xf0c5, 0xf0e7,
    0xf109, 0xf12b, 0xf14c, 0xf16e, 0xf18f, 0xf1b1, 0xf1d2, 0xf1f3,
    0xf213, 0xf234, 0xf254, 0xf275, 0xf295, 0xf2b5, 0xf2d5, 0xf2f5,
    0xf314, 0xf334, 0xf353, 0xf372, 0xf391, 0xf3b0, 0xf3cf, 0xf3ed,
    0xf40c, 0xf42a, 0xf448, 0xf466, 0xf484, 0xf4a2, 0xf4bf, 0xf4dd,
    0xf4fa, 0xf517, 0xf534, 0xf551, 0xf56e, 0xf58a, 0xf5a6, 0xf5c3,
    0xf5df, 0xf5fb, 0xf616, 0xf632, 0xf64e, 0xf669, 0xf684, 0xf69f,
    0xf6ba, 0xf6d5, 0xf6ef, 0xf70a, 0xf724, 0xf73e, 0xf758, 0xf772,
    0xf78c, 0xf7a5, 0xf7bf, 0xf7d8, 0xf7f1, 0xf80a, 0xf823, 0xf83b,
    0xf854, 0xf86c, 0xf885, 0xf89d, 0xf8b4, 0xf8cc, 0xf8e4, 0xf8fb,
    0xf913, 0xf92a, 0xf941, 0xf958, 0xf96e, 0xf985, 0xf99b, 0xf9b2,
    0xf9c8, 0xf9de, 0xf9f3, 0xfa09, 0xfa1f, 0xfa34, 0xfa49, 0xfa5e,
    0xfa73, 0xfa88, 0xfa9c, 0xfab1, 0xfac5, 0xfad9, 0xfaed, 0xfb01,
    0xfb15, 0xfb28, 0xfb3c, 0xfb4f, 0xfb62, 0xfb75, 0xfb88, 0xfb9a,
    0xfbad, 0xfbbf, 0xfbd1, 0xfbe3, 0xfbf5, 0xfc07, 0xfc18, 0xfc2a,
    0xfc3b, 0xfc4c, 0xfc5d, 0xfc6e, 0xfc7f, 0xfc8f, 0xfca0, 0xfcb0,
    0xfcc0, 0xfcd0, 0xfcdf, 0xfcef, 0xfcfe, 0xfd0e, 0xfd1d, 0xfd2c,
    0xfd3b, 0xfd49, 0xfd58, 0xfd66, 0xfd74, 0xfd83, 0xfd90, 0xfd9e,
    0xfdac, 0xfdb9, 0xfdc7, 0xfdd4, 0xfde1, 0xfdee, 0xfdfa, 0xfe07,
    0xfe13, 0xfe1f, 0xfe2b, 0xfe37, 0xfe43, 0xfe4f, 0xfe5a, 0xfe66,
    0xfe71, 0xfe7c, 0xfe87, 0xfe91, 0xfe9c, 0xfea6, 0xfeb0, 0xfeba,
    0xfec4, 0xfece, 0xfed8, 0xfee1, 0xfeeb, 0xfef4, 0xfefd, 0xff06,
    0xff0e, 0xff17, 0xff1f, 0xff28, 0xff30, 0xff38, 0xff3f, 0xff47,
    0xff4e, 0xff56, 0xff5d, 0xff64, 0xff6b, 0xff71, 0xff78, 0xff7e,
    0xff85, 0xff8b, 0xff91, 0xff96, 0xff9c, 0xffa2, 0xffa7, 0xffac,
    0xffb1, 0xffb6, 0xffbb, 0xffbf, 0xffc4, 0xffc8, 0xffcc, 0xffd0,
    0xffd4, 0xffd7, 0xffdb, 0xffde, 0xffe1, 0xffe4, 0xffe7, 0xffea,
    0xffec, 0xffef, 0xfff1, 0xfff3, 0xfff5, 0xfff7, 0xfff8, 0xfffa,
    0xfffb, 0xfffc, 0xfffd, 0xfffe, 0xffff, 0xffff, 0xffff, 0xffff
};

#define FIXED_2PI  float2fixed<T, p>(M_PI)
#define FIXED_R2PI float2fixed<T, p>(1.f/2/M_PI)
inline int16_t degree2angle(int32_t d) { return d * ANGLE_360 / 360; }

template <typename T, uint8_t p, bool cos>
inline T fixed_sincos(T a)
{
    while (a < 0) a += FIXED_2PI;
    a = fixed_mul<T, p>(a, FIXED_R2PI) >> (p - 2 - LOG2_SIN);
    if (cos) a += ANGLE_90;
    T v = sin16_table[(a & ANGLE_90) ?
	    ANGLE_90 - 1 - (a & (ANGLE_90 - 1)) : (a & (ANGLE_90 - 1))];
    if (LOG2_FIXED < p) v << (p - LOG2_FIXED); else v >> (LOG2_FIXED - p);
    //v += fixmul<T, p>(v, FixedPointType<T, p>one() - 1);
    //v  = fixmul<T, p>(v, FixedPointType<T, p>one());
    return (a & ANGLE_180) ? -v : v;
}

#endif

#endif//FIXED_HPP
// vim:sts=4:ts=8:
