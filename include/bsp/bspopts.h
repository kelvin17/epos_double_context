/* BSP dependent options file */
/* automatically generated -- DO NOT EDIT!! */

#ifndef __BSP_OPTIONS_H
#define __BSP_OPTIONS_H

/* include/bspopts.tmp.  Generated from bspopts.h.in by configure.  */
/* include/bspopts.h.in.  Generated from configure.ac by autoheader.  */

/* If defined, then the BSP Framework will put a non-zero pattern into the
   RTEMS Workspace and C program heap. This should assist in finding code that
   assumes memory starts set to zero. */
#define BSP_DIRTY_MEMORY 0

/* If defined, print a message and wait until pressed before resetting board
   when application exits. */
#define BSP_PRESS_KEY_FOR_RESET 0

/* If defined, reset the board when the application exits. */
#define BSP_RESET_BOARD_AT_EXIT 1

/* If defined, set the VGA display to 80x50. */
#define BSP_VIDEO_80x50 0

/* If enabled, the clock driver will use the good old 8254 chip to report
   microsecond-accuracy clock times. Enable it, if: - you have nanosecond
   timing enabled (you do NOT have USE_TICKS_FOR_CPU_USAGE_STATISTICS enabled)
   - you do NOT have CLOCK_DRIVER_USE_TSC enabled (use one, the other, or
   neither) - you do not mind adding roughly 5 microseconds to each context
   switch. */
#define CLOCK_DRIVER_USE_8254 0

/* If enabled, the clock driver will use the TSC register available with
   Pentium-class CPUs to report close to nanosecond-accuracy clock times.
   Enable it, if: - you have nanosecond timing enabled (you do NOT have
   USE_TICKS_FOR_CPU_USAGE_STATISTICS enabled) - you do NOT have
   CLOCK_DRIVER_USE_8254 enabled (use one, the other, or neither) - you have a
   Pentium which supports TSC (all Intels, and probably all or most clones) -
   you do not have a variable-speed CPU clock. Note that some motherboard BIOS
   will automatically vary clock speed for thermal control. Note also,
   however, that really new Pentium-class chips from Intel and AMD will
   maintain a constant-rate TSC regardless. */
#define CLOCK_DRIVER_USE_TSC 0

/* Determines, whether RTEMS will try to use the primary IDE interface.
   Disable it, if: - you have no primary IDE interface - or you have no disk
   attached to this interface - or you do not want to access disks attached to
   this interface */
#define IDE_USE_PRIMARY_INTERFACE 1

/* Determines, whether RTEMS will try to use the secondary IDE interface.
   Enable it, if: - you have a secondary IDE interface - and you have at least
   one disk attached to this interface - and you do want to access disks
   attached to this interface */
#define IDE_USE_SECONDARY_INTERFACE 0

/* if using 16 bit mode assembler support */
#define NEW_GAS 1







/* Determines, whether the console will be associated with the standard VGA
   display or with the COM1 serial port. Currently only the VGA display and
   COM1 support printk. */
#define USE_COM1_AS_CONSOLE 0

#endif /* __BSP_OPTIONS_H */
