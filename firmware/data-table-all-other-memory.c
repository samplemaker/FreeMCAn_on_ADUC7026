/** \file firmware/data-table-all-other-memory.c
 * \brief Data table occupying all the rest of the SRAM
 *
 * \author Copyright (C) 2010 samplemaker
 * \author Copyright (C) 2010 Hans Ulrich Niedermann <hun@n-dimensional.de>
 * \author Copyright (C) 1998, 1999, 2000, 2007, 2008, 2009 Free Software Foundation, Inc.
 *         (for the assembly code in ts_init() to clear data_table)
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
 * \defgroup data_table_all_other_memory Data table occupying all the rest of the SRAM
 * \ingroup firmware_personality_groups
 *
 * Data table occupying all the rest of the SRAM (in conjunction with
 * linker script data-table-all-other-memory.x).
 *
 * @{
 */

#include "init.h"

/* import the symbols in data type inspecific way */
extern char data_table[];
extern char data_table_end[];
extern char data_table[];

/** Setup, needs to be called once on startup */
void __init data_table_init(void)
{
  /** As the table is outside of the memory area with the normal data,
   * its content will NOT be cleared by the default avr-libc startup
   * code.  So we clear the table memory ourselves.
   */
  asm volatile("\n\t"  
               "mov  r0, #0 \n\t"
               "ldr  r1, =data_table \n\t"
               "ldr  r2, =data_table_end \n\t"

               "zero_table: \n\t"

               "cmp  r1, r2 \n\t"
               /* 
               "stmltia r1!,{r0} \n\t"
               "blt zero_table \n\t"
                */
               /* if r1<r2: *(r1)=r0 then r1+=4 */
               "strlo r0,[r1], #4 \n\t"
               /* branch if condition lower */
               "blo  zero_table \n\t"

               /* output operands */
               :
               /* input operands */
               :
               /* clobbers, change condition code flag, 
                * store all cached values before and reload them after */
               : "r0", "r1", "r2", "cc", "memory"
               );
}

/** Put function into init section, register function pointer and
 *  execute function at start up
 */
register_init5(data_table_init);


/** @} */


/*
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */
