#include <gdk/gdk.h>

GMainLoop* main_loop;
GdkWindow* window;
guint timer_clock;

void on_end_app()
{
	g_source_remove(timer_clock);
	gdk_window_destroy(window);
	g_main_loop_quit(main_loop);
}

void on_expose(GdkEventExpose* ev)
{
	if (ev->region == NULL)
		return;
	GdkDrawingContext* dc = gdk_window_begin_draw_frame(
		ev->window, ev->region);
	cairo_t* cr = gdk_drawing_context_get_cairo_context(dc);
	cairo_set_source_rgba(cr, 1.0, 1.0, 0.8, 1.0);
	cairo_paint(cr);
	cairo_set_source_rgba(cr, 0.5, 0.0, 0.0, 1.0);
	cairo_set_line_width(cr, 5.0);
	cairo_move_to(cr, 0, 0);
	cairo_line_to(cr, ev->area.width, ev->area.height);
	cairo_move_to(cr, ev->area.width, 0);
	cairo_line_to(cr, 0, ev->area.height);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.8, 1.0);
	cairo_select_font_face(cr, "Sans",
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 18);
	cairo_move_to(cr, 50, 50);
	cairo_show_text(cr, "Nějaký český text v utf8 ...");
	time_t timenow;
	time(&timenow);
	static char buff[64];
	strftime(buff, sizeof(buff), "%d.%m.%Y %H:%M:%S",
		localtime((const time_t*)&timenow));
	cairo_move_to(cr, 50, ev->area.height - 40);
	cairo_show_text(cr, buff);
	gdk_window_end_draw_frame(ev->window, dc);
}

void on_gdk_event(GdkEvent* event, gpointer data)
{
	if (event->type == GDK_EXPOSE)
	{
		on_expose((GdkEventExpose*)event);
	}
	else if (event->type == GDK_CONFIGURE)
	{
		gdk_window_invalidate_rect(window, NULL, TRUE);
	
	}
	else if (event->type == GDK_KEY_PRESS)
	{
		if (((GdkEventKey*)event)->keyval == GDK_KEY_Escape)
			on_end_app();
	}
	else if (event->type == GDK_DELETE)
	{
		on_end_app();
	}
}

gboolean on_timer(gpointer data)
{
	gdk_window_invalidate_rect(window, NULL, TRUE);
	return G_SOURCE_CONTINUE;
}

int main(int argc, char** argv)
{
	GdkWindowAttr attributes;
	gint attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_TITLE | GDK_WA_WMCLASS;
	if (!gdk_init_check(&argc, &argv))
		return EXIT_FAILURE;
	memset(&attributes, 0, sizeof(attributes));
	attributes.window_type = GDK_WINDOW_TOPLEVEL;
	attributes.x = 100;
	attributes.y = 50;
	attributes.event_mask = GDK_ALL_EVENTS_MASK;
	attributes.width = 440;
	attributes.height = 480;
	attributes.title = "GDK - Cairo";
	attributes.wclass = GDK_INPUT_OUTPUT;
	gdk_event_handler_set(on_gdk_event, NULL, NULL);
	window = gdk_window_new(NULL, &attributes, attr_mask);
	gdk_window_show(window);
	timer_clock = g_timeout_add(1000, on_timer, NULL);

	main_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(main_loop);
	g_main_loop_unref(main_loop);
	return EXIT_SUCCESS;
}
