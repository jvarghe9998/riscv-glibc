/* Get system-specific information at run-time.  Linux/powerpc version.
   Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <unistd.h>
#include <sys/auxv.h>

static long linux_sysconf (int name);

static long
auxv2sysconf (unsigned long type)
{
  long rc;
  rc = __getauxval (type);
  if (rc == 0)
  {
    __set_errno (EINVAL);
    rc = -1;
  }
  return rc;
}

static long
auxv2sysconf_cache_associativity (unsigned long type)
{
  long rc;
  rc = auxv2sysconf (type);
  if (rc != -1)
    rc = (rc & 0xffff0000) >> 16;
  return rc;
}

static long
auxv2sysconf_cache_linesize (unsigned long type)
{
  long rc;
  rc = auxv2sysconf (type);
  if (rc != -1)
    rc = rc & 0xffff;
  return rc;
}

/* Get the value of the system variable NAME.  */
long int
__sysconf (int name)
{
  switch (name)
    {
      case _SC_LEVEL1_ICACHE_SIZE:
	return auxv2sysconf (AT_L1I_CACHESIZE);
      case _SC_LEVEL1_ICACHE_ASSOC:
	return auxv2sysconf_cache_associativity (AT_L1I_CACHEGEOMETRY);
      case _SC_LEVEL1_ICACHE_LINESIZE:
	return auxv2sysconf_cache_linesize (AT_L1I_CACHEGEOMETRY);
      case _SC_LEVEL1_DCACHE_SIZE:
	return auxv2sysconf (AT_L1D_CACHESIZE);
      case _SC_LEVEL1_DCACHE_ASSOC:
	return auxv2sysconf_cache_associativity (AT_L1D_CACHEGEOMETRY);
      case _SC_LEVEL1_DCACHE_LINESIZE:
	return auxv2sysconf_cache_linesize (AT_L1D_CACHEGEOMETRY);
      case _SC_LEVEL2_CACHE_SIZE:
	return auxv2sysconf (AT_L2_CACHESIZE);
      case _SC_LEVEL2_CACHE_ASSOC:
	return auxv2sysconf_cache_associativity (AT_L2_CACHEGEOMETRY);
      case _SC_LEVEL2_CACHE_LINESIZE:
	return auxv2sysconf_cache_linesize (AT_L2_CACHEGEOMETRY);
      case _SC_LEVEL3_CACHE_SIZE:
	return auxv2sysconf (AT_L3_CACHESIZE);
      case _SC_LEVEL3_CACHE_ASSOC:
	return auxv2sysconf_cache_associativity (AT_L3_CACHEGEOMETRY);
      case _SC_LEVEL3_CACHE_LINESIZE:
	return auxv2sysconf_cache_linesize (AT_L3_CACHEGEOMETRY);
      default:
	return linux_sysconf (name);
    }
}

/* Now the generic Linux version.  */
#undef __sysconf
#define __sysconf static linux_sysconf
#include "../sysconf.c"
