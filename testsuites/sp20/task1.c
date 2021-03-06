/*  Task_1_through_5
 *
 *  This routine serves as a test task for the period capabilities of the
 *  Rate Monotonic Manager.
 *
 *  Input parameters:
 *    argument - task argument
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "system.h"

/*
 runtime of TA6 should be shorter than TA5
 */
#define TA6_ITERATIONS 10
#define TA6_PERIOD_FACTOR 10

uint32_t      Periods[7]    = { 0,   2,   2,   2,   2, 100, 0 };
uint32_t      Iterations[7] = { 0,  50,  50,  50,  50,   1, TA6_ITERATIONS };
epos_task_priority Priorities[7] = { 0,   1,   1,   3,   4,   5, 1 };

epos_task Task_1_through_6(
  epos_task_argument argument
)
{
  epos_id          rmid;
  epos_id          test_rmid;
  uint32_t    index;
  uint32_t    pass;
  uint32_t    failed;
  epos_status_code status;

  status = epos_rate_monotonic_create( argument, &rmid );
  directive_failed( status, "epos_rate_monotonic_create" );
  put_name( Task_name[ argument ], FALSE );
  printk( "- epos_rate_monotonic_create id = 0x%08x\n", rmid );

  status = epos_rate_monotonic_ident( argument, &test_rmid );
  directive_failed( status, "epos_rate_monotonic_ident" );
  put_name( Task_name[ argument ], FALSE );
  printk( "- epos_rate_monotonic_ident id = 0x%08x\n", test_rmid );

  if ( rmid != test_rmid ) {
     printk( "RMID's DO NOT MATCH (0x%x and 0x%x)\n", rmid, test_rmid );
     epos_test_exit( 0 );
  }

  put_name( Task_name[ argument ], FALSE );
  printk( "- (0x%08x) period %d\n", rmid, Periods[ argument ] );

  status = epos_task_wake_after( 2 );
  directive_failed( status, "epos_task_wake_after" );

  switch ( argument ) {
    case 1:
    case 2:
    case 3:
    case 4:
      while ( FOREVER ) {
        status = epos_rate_monotonic_period( rmid, Periods[ argument ] );
        directive_failed( status, "epos_rate_monotonic_period" );

        Count.count[ argument ]++;
      }
      break;
    case 5:
      pass   = 0;
      failed = 0;

      status = epos_rate_monotonic_period( rmid, Periods[ argument ] );
      directive_failed( status, "epos_rate_monotonic_period 1 of TA5" );

      Get_all_counters();

      while ( FOREVER ) {

        status = epos_rate_monotonic_period( rmid, Periods[ argument ] );
        directive_failed( status, "epos_rate_monotonic_period 2 of TA5" );

        Get_all_counters();

        for( index = 1 ; index <= 4 ; index++ ) {
          if ( Temporary_count.count[ index ] != Iterations[ index ] ) {
            puts_nocr( "FAIL -- " );
            put_name ( Task_name[ index ], FALSE );
            printk   ( " Actual=%d, Expected=%d\n",
                       Temporary_count.count[ index ],
                       Iterations[ index ]
                     );
            failed += 1;
          }
        }

        if ( failed == 5 )
          epos_test_exit( 0 );

        pass += 1;

        printk( "TA5 - PERIODS CHECK OK (%d)\n", pass );

        fflush( stdout );

        if ( pass == 10 ) {
          printk( "*** END OF TEST 20 ***" );
          epos_test_exit( 0 );
        }

      }
      break;
    case 6:
      /* test changing periods */
      {
        uint32_t   time[TA6_ITERATIONS+1];
        epos_interval period;

        period = 1*TA6_PERIOD_FACTOR;
        status = epos_rate_monotonic_period( rmid, period);
        directive_failed( status, "epos_rate_monotonic_period of TA6" );
        time[0] = _Watchdog_Ticks_since_boot; /* timestamp */
        /*printk("%d - %d\n", period, time[0]);*/

        for (index = 1; index <= TA6_ITERATIONS; index++)
        {
          period = (index+1)*TA6_PERIOD_FACTOR;
          status = epos_rate_monotonic_period( rmid,  period);
          directive_failed( status, "epos_rate_monotonic_period of TA6" );
          time[index] = _Watchdog_Ticks_since_boot; /* timestamp */
          /*printk("%d - %d\n", period, time[index]);*/
        }

        for (index = 1; index <= TA6_ITERATIONS; index++)
        {
          epos_interval meas = time[index] - time[index-1];
          period = index*TA6_PERIOD_FACTOR;
          printk("TA6 - Actual: %d  Expected: %d", meas, period);
          if (period == meas) printk(" - OK\n");
          else printk(" - FAILED\n");
        }
      }
      epos_task_suspend(RTEMS_SELF);
      break;
  }
}
