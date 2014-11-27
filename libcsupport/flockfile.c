/**
 * @file
 *
 * @ingroup libcsupport
 *
 * @brief Flockfile()
 *
 * Copyright (c) 2009 by
 * Ralf Corsépius, Ulm, Germany. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */
#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems.h>
#include <stdio.h>

#if defined(RTEMS_NEWLIB) && !defined(HAVE_FLOCKFILE) && defined(HAVE_DECL_FLOCKFILE)


/* This is a non-functional stub */
void flockfile(FILE* file)
{
}

#endif
