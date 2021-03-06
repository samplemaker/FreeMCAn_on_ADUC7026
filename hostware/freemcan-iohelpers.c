/** \file freemcan-iohelpers.c
 * \brief select(2) helper functions (implementation)
 *
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
 * \defgroup freemcan_iohelpers Generic IO helper functions
 * \ingroup hostware_generic
 *
 * @{
 */


#include <sys/ioctl.h>

#include "freemcan-log.h"
#include "freemcan-iohelpers.h"


int read_size(const int in_fd)
{
  int bytes_to_read;
  int r = ioctl(in_fd, FIONREAD, &bytes_to_read);
  if (r < 0) {
    fmlog_error("cannot determine number of characters to read from stdin");
    abort();
  }
  return bytes_to_read;
}

/** @} */


/*
 * Local Variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */
