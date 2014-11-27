/* spmonotonic -- sanity check the rate monotonic manager
 *
 * license and distribution terms for this file may be found in the file
 * LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE .
 *
 * $Id$
 */

#define CONFIGURE_INIT
#include <tmacros.h>  /* includes bsp.h, stdio, etc... */

/* prototype */
epos_task Init (epos_task_argument ignored);

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS        1
#define CONFIGURE_MAXIMUM_PERIODS      1

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

epos_task Init(
    epos_task_argument ignored
) {
  epos_status_code  status;
  epos_interval     timestamps[6],
                     wantintervals[5] = { 1, 50, 200, 25, 3 };
  epos_name         period_name = epos_build_name('P','E','R','a');
  epos_id           period_id;
  int                loopy;

  printk("\n\n*** TEST 32 ***\n");

  /* create period */
  status = epos_rate_monotonic_create(
      period_name,
      &period_id
  );
  directive_failed(status, "rate_monotonic_create");

  /* start period with initial value */
  status = epos_rate_monotonic_period(
      period_id,
      wantintervals[0]
  );
  directive_failed(status, "rate_monotonic_period");

  /* get our first timestamp */
  status = epos_clock_get(
      RTEMS_CLOCK_GET_TICKS_SINCE_BOOT,
      &timestamps[0]
  );
  directive_failed(status, "clock_get");

  /* loop through and gather more timestamps */
  for (loopy = 1; loopy < 5; loopy++) {

    status = epos_rate_monotonic_period(
        period_id,
        wantintervals[loopy]
    );
    directive_failed(status, "rate_monotonic_period");

    status = epos_clock_get(
        RTEMS_CLOCK_GET_TICKS_SINCE_BOOT,
        &timestamps[loopy]
    );
    directive_failed(status, "clock_get");
  }

  /* block one last time */
  status = epos_rate_monotonic_period(
      period_id,
      1
  );
  directive_failed(status, "rate_monotonic_period");

  /* get one last timestamp */
  status = epos_clock_get(
      RTEMS_CLOCK_GET_TICKS_SINCE_BOOT,
      &timestamps[loopy]
  );
  directive_failed(status, "clock_get");

  /* cancel the period */
  status = epos_rate_monotonic_cancel(period_id);
  directive_failed(status, "rate_monotonic_cancel");

  /* delete it */
  status = epos_rate_monotonic_delete(period_id);
  directive_failed(status, "rate_monotonic_delete");

  /* tabulate and print results */
  for (loopy = 0; loopy < 5; loopy++) {
    printk(
        "period %d: measured %d tick(s), wanted %d\n",
        loopy, timestamps[loopy+1] - timestamps[loopy],
        wantintervals[loopy]
    );
  }

  /* the end */
  printk("*** END OF TEST 32 ***");
  epos_test_exit(0);
}
