/* CMPSC 311, Spring 2012, Project 1
 *
 * Author: Brian Golden
 * Email: bwg5079@psu.edu
 *
 * version 2, <date>
 *   <additional types>
 *   <additional information>
 * version 1, 9 Jan. 2012
 *   starter kit, replicate and extend CS:APP Fig. 2.3
 *   supply additional memory alignment info
 *
 * This program uses the GCC __alignof__ extension to C99.
 * __alignof__ is non-standard in C99, but it has been added to C11 as alignof.
 * So, compile the program with   gcc -std=c99 -Wall -Wextra -o pr1 pr1.1.c
 *
 * See also, the accompanying Makefile.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <complex.h>
#include <time.h>
#include <wchar.h>
#include <limits.h>

// See the project description for an explanation of this macro.
#define SHOW(type) printf("%-32s %3zd %3zd\n", #type, sizeof(type), __alignof__(type))

int main(int argc, char *argv[])
{
  printf("sizeof(type), alignof(type)\n");

  printf("\n");
  printf("Boolean type, from <stdbool.h>\n");
  SHOW(bool);	// SHOW(_Bool);

  printf("\n");
  printf("The usual integer types\n");
  SHOW(char);
  SHOW(signed char);
  SHOW(unsigned char);

  SHOW(short int);
  SHOW(int);
  SHOW(long int);
  SHOW(long long int);

  SHOW(unsigned short int);
  SHOW(unsigned int);
  SHOW(unsigned long int);
  SHOW(unsigned long long int);

  printf("\n");
  printf("The usual floating-point types\n");
  SHOW(float);
  SHOW(double);
  SHOW(long double);
  //  In C11, <math.h> defines float_t and double_t

  SHOW(complex);
  SHOW(float complex);
  SHOW(double complex);
  SHOW(long double complex);

  printf("\n");
  printf("The generic pointer type\n");
  SHOW(void *);

  printf("\n");
  printf("Exact-width integer types, from <stdint.h>\n");
  SHOW(int8_t);
  SHOW(int16_t);
  SHOW(int32_t);
  SHOW(int64_t);
  SHOW(uint8_t);
  SHOW(uint16_t);
  SHOW(uint32_t);
  SHOW(uint64_t);

  printf("\n");
  printf("Minimum-width integer types, from <stdint.h>\n");
  SHOW(int_least8_t);
  SHOW(int_least16_t);
  SHOW(int_least32_t);
  SHOW(int_least64_t);
  SHOW(uint_least8_t);
  SHOW(uint_least16_t);
  SHOW(uint_least32_t);
  SHOW(uint_least64_t);

  printf("\n");
  printf("Fastest minimum-width integer types, from <stdint.h>\n");
  SHOW(int_fast8_t);
  SHOW(int_fast16_t);
  SHOW(int_fast32_t);
  SHOW(int_fast64_t);
  SHOW(uint_fast8_t);
  SHOW(uint_fast16_t);
  SHOW(uint_fast32_t);
  SHOW(uint_fast64_t);

  printf("\n");
  printf("Pointer-width integer types, from <stdint.h>\n");
  SHOW(intptr_t);
  SHOW(uintptr_t);

  printf("\n");
  printf("Greatest-width integer types, from <stdint.h>\n");
  SHOW(intmax_t);
  SHOW(uintmax_t);

  printf("\n");
  printf("Miscellaneous integer types, from <stddef.h>\n");
  SHOW(ptrdiff_t);
  SHOW(size_t);
  // SHOW(max_align_t);	// C11 only
  SHOW(wchar_t);

  printf("\n");
  printf("Miscellaneous integer types, from <time.h>\n");
  SHOW(clock_t);
  SHOW(time_t);

  printf("\n");
  printf("Miscellaneous integer types, from <wchar.h>\n");
  SHOW(wint_t);

  printf("\n");
  printf("Limits from <limits.h>, decimal\n");
  printf("%-24s %hhd %hhd\n", "char",            CHAR_MIN,  CHAR_MAX);
  printf("%-24s %hhd %hhd\n", "signed char",    SCHAR_MIN, SCHAR_MAX);
  printf("%-24s %hhu %hhu\n", "unsigned char",          0, UCHAR_MAX);

  printf("%-24s %hd %hd\n",   "short int",       SHRT_MIN,  SHRT_MAX);
  printf("%-24s %d %d\n",     "int",              INT_MIN,   INT_MAX);
  printf("%-24s %ld %ld\n",   "long int",        LONG_MIN,  LONG_MAX);
  printf("%-24s %lld %lld\n", "long long int",  LLONG_MIN, LLONG_MAX);

  printf("%-24s %hu %hu\n",   "unsigned short int",     0,     USHRT_MAX);
  printf("%-24s %u %u\n",     "unsigned int",           0,      UINT_MAX);
  printf("%-24s %lu %lu\n",   "unsigned long int",      0UL,   ULONG_MAX);
  printf("%-24s %llu %llu\n", "unsigned long long int", 0ULL, ULLONG_MAX);

  printf("\n");
  printf("Limits from <limits.h>, hexadecimal\n");
  printf("%-24s %hhx %hhx\n", "char",            CHAR_MIN,  CHAR_MAX);
  printf("%-24s %hhx %hhx\n", "signed char",    SCHAR_MIN, SCHAR_MAX);
  printf("%-24s %hhx %hhx\n", "unsigned char",          0, UCHAR_MAX);

  printf("%-24s %hx %hx\n",   "short int",       SHRT_MIN,  SHRT_MAX);
  printf("%-24s %x %x\n",     "int",              INT_MIN,   INT_MAX);
  printf("%-24s %lx %lx\n",   "long int",        LONG_MIN,  LONG_MAX);
  printf("%-24s %llx %llx\n", "long long int",  LLONG_MIN, LLONG_MAX);

  printf("%-24s %hx %hx\n",   "unsigned short int",     0,     USHRT_MAX);
  printf("%-24s %x %x\n",     "unsigned int",           0,      UINT_MAX);
  printf("%-24s %lx %lx\n",   "unsigned long int",      0UL,   ULONG_MAX);
  printf("%-24s %llx %llx\n", "unsigned long long int", 0ULL, ULLONG_MAX);

  printf("\n");
  printf("Miscellaneous\n");
  printf("the number of bits in a char is %d\n", CHAR_BIT);
  printf("MB_LEN_MAX = %d\n", MB_LEN_MAX);	// multibyte characters

  printf("\n");
  printf("Additional struct types\n");
  SHOW(struct { char a; char b; });
  SHOW(struct { char a; int b; });
  SHOW(struct { int a; char b; });
  SHOW(struct { int a; int b; });
  SHOW(struct { bool a; bool b; });
  SHOW(struct { bool a; int b; });
  SHOW(struct { int a; bool b; });
  SHOW(struct { float a; float b; });
  SHOW(struct { float a; double b; });
  SHOW(struct { double a; float b; });
  SHOW(struct { double a; double b; });

  printf("\n");
  printf("Additional union types\n");
  SHOW(union { char a; char b; });
  SHOW(union { char a; int b; });
  SHOW(union { int a; char b; });
  SHOW(union { int a; int b; });
  SHOW(union { bool a; bool b; });
  SHOW(union { bool a; int b; });
  SHOW(union { int a; bool b; });
  SHOW(union { float a; float b; });
  SHOW(union { float a; double b; });
  SHOW(union { double a; float b; });
  SHOW(union { double a; double b; });

  return 0;
}

