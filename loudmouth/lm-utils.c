/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2003 Mikael Hallendal <micke@imendio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "lm-internals.h"
#include "lm-utils.h"

LmCallback *
_lm_utils_new_callback (gpointer func, 
			gpointer user_data,
			GDestroyNotify notify)
{
	LmCallback *cb;
	
	cb = g_new0 (LmCallback, 1);
	cb->func = func;
	cb->user_data = user_data;
	cb->notify = notify;

	return cb;
}

void
_lm_utils_free_callback (LmCallback *cb)
{
	if (!cb) {
		return;
	}

	if (cb->notify) {
		(* cb->notify) (cb->user_data);
	}
	g_free (cb);
}

gchar *
_lm_utils_generate_id (void)
{
	static gint  number = 0;

	return g_strdup_printf ("msg_%d", ++number);
}

struct tm *
lm_utils_get_localtime (const gchar *stamp)
{
	struct tm tm;
	time_t    t;
	gint      year, month;
	
	/* 20021209T23:51:30 */

	sscanf (stamp, "%4d%2d%2dT%2d:%2d:%2d", 
		&year, &month, &tm.tm_mday, &tm.tm_hour,
		&tm.tm_min, &tm.tm_sec);

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_isdst = -1;

 	t = mktime (&tm);

#if defined(HAVE_TM_GMTOFF)
	t += tm.tm_gmtoff;
#elif defined(HAVE_TIMEZONE)
	t -= timezone;
	if (tm.tm_isdst > 0) {
		t += 3600;
	}
#endif	

	return localtime (&t);
}

