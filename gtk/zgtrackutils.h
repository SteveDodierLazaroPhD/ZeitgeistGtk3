/* Activity Finder
 * zgtrackutils.c: Utilities for Zeitgeist event logging
 * Copyright (C) 2013-2016, Steve Dodier-Lazaro, UCL.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 *  the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ZG_TRACK_UTILS_H__
#define __ZG_TRACK_UTILS_H__

#include <glib.h>
#include <unistd.h>
#include <zeitgeist.h>

gchar *
_get_actor_name_from_pid (pid_t pid);

gchar *
_get_window_id_from_widget (GtkWidget *widget);

#endif /* __ZG_TRACK_UTILS_H__ */

