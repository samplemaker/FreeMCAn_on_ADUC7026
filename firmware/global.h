/** \file firmware/global.h
 * \brief Global adjustments for freemcan firmware
 *
 * \author Copyright (C) 2011 samplemaker
 * \author Copyright (C) 2010 Hans Ulrich Niedermann <hun@n-dimensional.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301 USA
 *
 * \defgroup global_constants Global Constants And Definitions
 * \ingroup firmware
 * @{
 */

#ifndef GLOBAL_H
#define GLOBAL_H


#ifndef __ASSEMBLER__

#include <stdint.h>


/** ADC resolution in bit
 *
 *  Put in here resonable value:
 *  i.e. 10bit +-1LSB accuracy
 */
#define ADC_RESOLUTION (11)

/** ADC trigger on rising or falling edge
 *
 */
#define ADC_TRIGGER_ON_RISING_EDGE 1

/** Histogram element size in bytes
 *
 */
#define ELEMENT_SIZE_IN_BYTES 3


#if (ELEMENT_SIZE_IN_BYTES == 3)

/** Unsigned 24bit integer type
 *
 * This could be called a uint24_t, but we do not want to intrude on
 * that namespace.
 */
typedef uint8_t freemcan_uint24_t[3];

#endif


/** Histogram element type */
typedef
#if (ELEMENT_SIZE_IN_BYTES == 2)
  uint16_t
#elif (ELEMENT_SIZE_IN_BYTES == 3)
  freemcan_uint24_t
#elif (ELEMENT_SIZE_IN_BYTES == 4)
  uint32_t
#else
# error Unsupported ELEMENT_SIZE_IN_BYTES
#endif
  histogram_element_t;


#if (ELEMENT_SIZE_IN_BYTES == 3)
/** Increment 24bit unsigned integer */
inline static
void histogram_element_inc(volatile freemcan_uint24_t *element)
{
  register uint32_t accu;
  register uint32_t tmp_reg;
  asm volatile("\n\t"
               /* load three bytes with respect to endianess */
               "eor   %[tmp_reg], %[tmp_reg]          \n\t"
               "ldrb  %[accu],    [%[elem], #2]       \n\t"
               "orr   %[tmp_reg], %[accu] ,LSL #16    \n\t"
               "ldrb  %[accu],    [%[elem], #1]       \n\t"
               "orr   %[tmp_reg], %[accu] ,LSL #8     \n\t"
               "ldrb  %[accu],    [%[elem]]           \n\t"
               "orr   %[tmp_reg], %[accu]             \n\t"
               /* increase by one */
               "add   %[tmp_reg], %[tmp_reg], #1      \n\t"
               /* store three bytes with respect to endianess */
               "strb  %[tmp_reg], [%[elem]]           \n\t"
               "mov   %[tmp_reg], %[tmp_reg] ,LSR #8  \n\t"
               "strb  %[tmp_reg], [%[elem], #1]       \n\t"
               "mov   %[tmp_reg], %[tmp_reg] ,LSR #8  \n\t"
               "strb  %[tmp_reg], [%[elem], #2]       \n\t"
               : /* output operands */
                 /* let compiler decide which registers to clobber */
                 [accu] "=&r" (accu),
                 /* temporary register */
                 [tmp_reg] "=&r" (tmp_reg)
               : /* input operands */
                 [elem] "r" (element)
                 /* inform that we change the condition code flag */
                 /* : "cc"*/
                 /* store all cached values before and reload them after */
                    : "memory"
  );
}
#else
/** Increment 8bit, 16bit, or 32bit unsigned integer */
inline static
void histogram_element_inc(volatile histogram_element_t *element)
{
  (*element)++;
}
#endif


/** @} */

#else /* ifdef __ASSEMBLER__ */

#endif /* __ASSEMBLER__ */

#endif /* !GLOBAL_H */


/*
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */