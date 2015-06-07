/*
 * Copyright © 2014 Canonical Ltd
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GDK_PRIVATE_MIR_H__
#define __GDK_PRIVATE_MIR_H__

#include "gdkmir.h"
#include "gdkdisplay.h"
#include "gdkscreen.h"
#include "gdkdevicemanager.h"
#include "gdkkeys.h"
#include "gdkwindowimpl.h"

typedef struct _GdkMirWindowImpl GdkMirWindowImpl;
typedef struct _GdkMirWindowReference GdkMirWindowReference;
typedef struct _GdkMirEventSource GdkMirEventSource;

#define GDK_TYPE_MIR_WINDOW_IMPL              (gdk_mir_window_impl_get_type ())
#define GDK_MIR_WINDOW_IMPL(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_MIR_WINDOW_IMPL, GdkMirWindowImpl))
#define GDK_IS_WINDOW_IMPL_MIR(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_MIR_WINDOW_IMPL))

GType gdk_mir_window_impl_get_type (void);

GdkDisplay *_gdk_mir_display_open (const gchar *display_name);

GdkScreen *_gdk_mir_screen_new (GdkDisplay *display);

GdkDeviceManager *_gdk_mir_device_manager_new (GdkDisplay *display);

GdkDevice *_gdk_mir_device_manager_get_keyboard (GdkDeviceManager *device_manager);

GdkKeymap *_gdk_mir_keymap_new (void);

gboolean _gdk_mir_keymap_key_is_modifier (GdkKeymap *keymap, guint keycode);

GdkDevice *_gdk_mir_keyboard_new (GdkDeviceManager *device_manager, const gchar *name);

GdkDevice *_gdk_mir_pointer_new (GdkDeviceManager *device_manager, const gchar *name);

void _gdk_mir_pointer_set_location (GdkDevice *pointer, gdouble x, gdouble y, GdkWindow *window, GdkModifierType mask);

GdkCursor *_gdk_mir_cursor_new_for_type (GdkDisplay *display, GdkCursorType type);

GdkCursor *_gdk_mir_cursor_new_for_name (GdkDisplay *display, const gchar *name);

const gchar *_gdk_mir_cursor_get_name (GdkCursor *cursor);

GdkWindowImpl *_gdk_mir_window_impl_new (void);

void _gdk_mir_window_impl_set_surface_state (GdkMirWindowImpl *impl, MirSurfaceState state);

void _gdk_mir_window_impl_set_surface_type (GdkMirWindowImpl *impl, MirSurfaceType type);

void _gdk_mir_window_impl_set_cursor_state (GdkMirWindowImpl *impl, gdouble x, gdouble y, gboolean cursor_inside, MirMotionButton button_state);

void _gdk_mir_window_impl_get_cursor_state (GdkMirWindowImpl *impl, gdouble *x, gdouble *y, gboolean *cursor_inside, MirMotionButton *button_state);

GdkMirEventSource *_gdk_mir_display_get_event_source (GdkDisplay *display);

GdkMirEventSource *_gdk_mir_event_source_new (GdkDisplay *display);

GdkMirWindowReference *_gdk_mir_event_source_get_window_reference (GdkWindow *window);

void _gdk_mir_window_reference_unref (GdkMirWindowReference *ref);

void _gdk_mir_event_source_queue (GdkMirWindowReference *window_ref, const MirEvent *event);

void _gdk_mir_print_modifiers (unsigned int modifiers);

void _gdk_mir_print_key_event (const MirKeyEvent *event);

void _gdk_mir_print_motion_event (const MirMotionEvent *event);

void _gdk_mir_print_surface_event (const MirSurfaceEvent *event);

void _gdk_mir_print_resize_event (const MirResizeEvent *event);

void _gdk_mir_print_event (const MirEvent *event);

/* TODO: Remove once we have proper transient window support. */
GdkWindow * _gdk_mir_window_get_visible_transient_child (GdkWindow *window,
                                                         gdouble    x,
                                                         gdouble    y,
                                                         gdouble   *out_x,
                                                         gdouble   *out_y);

/* TODO: Remove once we have proper transient window support. */
void _gdk_mir_window_transient_children_foreach (GdkWindow  *window,
                                                 void      (*func) (GdkWindow *, gpointer),
                                                 gpointer    user_data);

#endif /* __GDK_PRIVATE_MIR_H__ */
