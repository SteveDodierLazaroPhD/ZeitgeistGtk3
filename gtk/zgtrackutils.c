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

#include "gtkwidget.h"
#include "zgtrackutils.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <gdk/gdk.h>
#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

/**
 * SECTION:zgtrackutils
 * @Short_description: Utilities for logging events into the Zeitgeist daemon
 * @Title: ZGTrackUtils
 * @See_also: #GtkFileChooser
 *
 * #ZGTrackUtils is a set of utility functions used across parts of
 * the GTK+ framework that were modified to send events to the Zeitgeist
 * daemon. These utilities are not responsible for sending events as this
 * will ultimately be the responsibility of separately-run widgets that
 * interact with GTK+ applications through IPC, but they provide a common
 * codebase between modified classes to determine event actors, MIME types,
 * etc.
 *
 * <note>
 * The code in this section should never be directly called by GTK+ functions.
 * </note>
 *
 */

/**
 * _get_actor_name_from_pid:
 * @pid: a process id, 0 or -1 to use self id.
 *
 * Returns a Zeitgeist Actor string for the process indicated by the pid
 * parameter or for the calling process. The string is generated by retrieving
 * the executable name, prefixing it by 'app://' and suffexing it by
 * '.desktop'. The idea behind this is that there is no current consensus on
 * how to name an application in FreeDesktop.org and that Zeitgeist does not
 * actually make much use of the Actor string.
 *
 * Return value: (type URI): The process' corresponding Actor URI, or %NULL
 *  if an error occurred. Free with g_free().
 *
 * Copyright: Steve Dodier-Lazaro, 2015, UCL
 *            <s.dodier-lazaro@cs.ucl.ac.uk>
 *            Licensed under GPLv3
 *
 * Since: 3.10.6
 **/
gchar *
_get_actor_name_from_pid (pid_t pid)
{
    char         *link_file     = NULL;
    char         *link_target   = NULL;
    char        **split_target  = NULL;
    char         *actor_name    = NULL;
    ssize_t       read_len      = PATH_MAX; // 4096, so it's unlikely link_len ever overflows :)
    ssize_t       link_len      = 1;

    if (pid <= 0)
    {
        return NULL;
    }

    link_file = g_strdup_printf ("/proc/%d/exe", pid);
    if (link_file == NULL)
    {
        return NULL;
    }

    // It is impossible to obtain the size of /proc link targets as /proc is
    // not POSIX compliant. Hence, we start from the NAME_MAX limit and increase
    // it all the way up to readlink failing. readlink will fail upon reaching
    // the PATH_MAX limit on Linux implementations. read_len will be strictly
    // inferior to link_len as soon as the latter is big enough to contain the
    // path to the executable and a trailing null character.
    while (read_len >= link_len)
    {
        link_len += NAME_MAX;

        g_free(link_target);
        link_target = g_malloc(link_len * sizeof (char));

        if (link_target == NULL)
        {
            g_free (link_file);
            g_free (link_target);
            return NULL;
        }

        read_len= readlink (link_file, link_target, link_len);

        if (read_len < 0)
        {
            g_free (link_file);
            g_free (link_target);
            return NULL;
        }
    }

    g_free (link_file);

    // readlink does not null-terminate the string
    link_target[read_len] = '\0';

    split_target = g_strsplit (link_target, "/", -1);
    g_free (link_target);

    if(split_target == NULL)
    {
        return NULL;
    }

    // Iterate to the last element which is the executable name
    for (read_len = 0; split_target[read_len]; read_len++);

    // Turn it into an arbitrary actor name
    actor_name = g_strdup_printf ("application://%s.desktop", split_target[--read_len]);
    g_strfreev (split_target);

    // Better safe than sorry
    if (!actor_name)
      actor_name = g_strdup ("application://unknown.desktop");

    return actor_name;
}

/**
 * _get_window_id_from_widget:
 * @widget: a #GtkWidget.
 *
 * Returns a string representation of the X11 window id of the window that
 * contains the given widget, or the string "n/a" if one could not be found.
 *
 * Return value: (type URI): The window id as a string, or "n/a".
 * Free with g_free().
 *
 * Copyright: Steve Dodier-Lazaro, 2015, UCL
 *            <s.dodier-lazaro@cs.ucl.ac.uk>
 *            Licensed under GPLv3
 *
 * Since: 3.10.6
 **/
char *
_get_window_id_from_widget (GtkWidget *widget)
{
  char *window_id = NULL;
  #ifdef GDK_WINDOWING_X11
  GdkWindow *gwin = gtk_widget_get_window (GTK_WIDGET (widget));

  if (gwin) {
    GdkDisplay *dsp = gdk_window_get_display (gwin);
    if (dsp && GDK_IS_X11_DISPLAY (dsp))
      window_id = g_strdup_printf ("%lu", gdk_x11_window_get_xid (gwin));
  }

  if (!window_id)
  #endif
    window_id = g_strdup ("n/a");
  return window_id;
}