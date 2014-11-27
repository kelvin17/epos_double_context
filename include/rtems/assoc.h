/**
 * @file rtems/assoc.h
 */

/*
 *
 *  Rtems associativity routines.  Mainly used to convert a value from
 *  one space to another (eg: our errno's to host errno's and v.v)
 *
 *
 *  $Id: assoc.h,v 1.11 2009/06/03 03:38:38 ralf Exp $
 */

#ifndef __RTEMS_ASSOC_h
#define __RTEMS_ASSOC_h

#include <stdint.h> /* uint32_t */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char  *name;
    uint32_t     local_value;
    uint32_t     remote_value;
} epos_assoc_t;

/*
 * Flag/marker for optional default value in each table
 */

#define RTEMS_ASSOC_DEFAULT_NAME "(default)"

const epos_assoc_t *epos_assoc_ptr_by_name(
  const epos_assoc_t *,
  const char *
);

const epos_assoc_t *epos_assoc_ptr_by_remote(
  const epos_assoc_t *,
  uint32_t
);

uint32_t epos_assoc_remote_by_local(
  const epos_assoc_t *,
  uint32_t
);

uint32_t epos_assoc_local_by_remote(
  const epos_assoc_t *,
  uint32_t
);

uint32_t epos_assoc_remote_by_name(
  const epos_assoc_t *,
  const char *
);
uint32_t epos_assoc_local_by_name(
  const epos_assoc_t *,
  const char *
);

const char *epos_assoc_name_by_local(
  const epos_assoc_t *,
  uint32_t
);

const char *epos_assoc_name_by_remote(
  const epos_assoc_t *,
  uint32_t
);

uint32_t epos_assoc_remote_by_local_bitfield(
  const epos_assoc_t *,
  uint32_t
);

char *epos_assoc_name_by_local_bitfield(
  const epos_assoc_t *,
  uint32_t  ,
  char *
);

char *epos_assoc_name_by_remote_bitfield(
  const epos_assoc_t *,
  uint32_t  ,
  char *
);

uint32_t     epos_assoc_local_by_remote_bitfield(
  const epos_assoc_t *,
  uint32_t
);

const epos_assoc_t *epos_assoc_ptr_by_local(
  const epos_assoc_t *ap,
  uint32_t             local_value
);

#if defined(INSIDE_ASSOC)

#define epos_assoc_is_default(_ap) \
  ((_ap)->name && !strcmp((_ap)->name, RTEMS_ASSOC_DEFAULT_NAME))

/*
 * what to return if a value is not found
 * this is not reentrant, but it really shouldn't be invoked anyway
 */

const char *epos_assoc_name_bad(
  uint32_t   bad_value
);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ! _RTEMS_RTEMS_ASSOC_H */
