#ifndef ETK_TYPES_H
#define ETK_TYPES_H


/*
 *
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 */

// 32bit PC
/*
typedef unsigned char byte ;
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long int64;
typedef unsigned long uint64;

typedef double real_t;
*/

//g++ v6.1.0 on windows 10 / 64bit arch linux

#ifdef __AVR__

typedef unsigned char byte ;
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef long int int32;
typedef unsigned long int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef double real_t;


#else

typedef unsigned char byte ;
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef double real_t;

#endif

/*
// 32bit ARM
typedef unsigned char byte ;
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long int64;
typedef unsigned long uint64;

typedef float real_t;
*/

// 8bit AVR

static_assert(sizeof(int8) == 1, "Size of int8 is not correct. Check int8 typedef in types.h");
static_assert(sizeof(uint8) == 1, "Size of uint8 is not correct. Check uint8 typedef in types.h");
static_assert(sizeof(int16) == 2, "Size of int16 is not correct. Check int16 typedef in types.h");
static_assert(sizeof(uint16) == 2, "Size of uint16 is not correct. Check uint16 typedef in types.h");
static_assert(sizeof(int32) == 4, "Size of int32 is not correct. Check int32 typedef in types.h");
static_assert(sizeof(uint32) == 4, "Size of uint32 is not correct. Check uint32 typedef in types.h");
static_assert(sizeof(int64) == 8, "Size of int64 is not correct. Check int64 typedef in types.h");
static_assert(sizeof(uint64) == 8, "Size of uint64 is not correct. Check uint64 typedef in types.h");

#endif
